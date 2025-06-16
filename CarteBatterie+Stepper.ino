#define LED75 A8 //supposant que la rouge soit A11, sinon inverse (pas encore branché)
#define LED50 A9
#define LED25 A10
#define LED0 A11
#define CREF A5 //Pin théorique, à mettre en place ou vérifier
float CurrentSensor; // Valeur de 0 à 1024 sortie capteur courant
float VoltCS; // Valeur tension capteur
float AmpCS; // intensité
float UsedCapacity=0; //capacité consommée
unsigned long TotTime=0; //Total Time in miliseconds
unsigned long CompTime=0; //Temps pour comparer
unsigned long LEDTime=0; //Comparateur, permet de tester l'état des LEDs une fois par minute
float VBAT;
float ReadBat;
float CapaMAX=5.2;
float CapaMIN=CapaMAX*0.25;
float CapaRANGE=CapaMAX-CapaMIN;
float BatPerc=100;


const int phase1 = 10;          // Déclaration des broches 
const int phase2 = 11;
int angle=0;                    // Angle de départ de la tête du robot
unsigned long temp_millis = millis();


void AllumLED(float CAPA){
  if (CAPA<CapaRANGE*0.25){
    digitalWrite(LED75,HIGH);
    digitalWrite(LED50,HIGH);
    digitalWrite(LED25,HIGH);
    digitalWrite(LED0,HIGH);
}
  else if (CAPA<CapaRANGE*0.5){
    digitalWrite(LED75,LOW);
    digitalWrite(LED50,HIGH);
    digitalWrite(LED25,HIGH);
    digitalWrite(LED0,HIGH);
  }
  else if (CAPA<CapaRANGE*0.75){
    digitalWrite(LED75,LOW);
    digitalWrite(LED50,LOW);
    digitalWrite(LED25,HIGH);
    digitalWrite(LED0,HIGH);
  }
  else if (CAPA<CapaRANGE){
    digitalWrite(LED75,LOW);
    digitalWrite(LED50,LOW);
    digitalWrite(LED25,LOW);
    digitalWrite(LED0,HIGH);
  }
  else if (CAPA>CapaRANGE){
    pinMode(CREF,OUTPUT);
    digitalWrite(CREF,LOW);    
  }
}

void setup() {
  Serial.begin(9600);       //Communication avec l'arduino de contrôle

  pinMode(A1,INPUT);        //Setup batterie
  pinMode(A0,INPUT);
  pinMode(LED75,OUTPUT);
  pinMode(LED50,OUTPUT);
  pinMode(LED25,OUTPUT);
  pinMode(LED0,OUTPUT);
  pinMode(CREF,INPUT);
  digitalWrite(LED75,HIGH);
  digitalWrite(LED50,HIGH);
  digitalWrite(LED25,HIGH);
  digitalWrite(LED0,HIGH);
  
  pinMode(phase1, OUTPUT);      // Initialiser les broches Pour le Steper
  pinMode(phase2, OUTPUT);
  digitalWrite(phase1, LOW);
  digitalWrite(phase2, LOW);   
}

void loop() {
  ReadBat=analogRead(A0);
  VBAT=ReadBat/1023.0*15.0;
  CurrentSensor=analogRead(A1);
  VoltCS=5*(CurrentSensor/1023);
  AmpCS=(2.495-VoltCS)*10; //À 0A : 2.495V
  TotTime=millis();
  UsedCapacity=UsedCapacity+(AmpCS*(TotTime-CompTime)/1000); 
  CompTime=TotTime;
  BatPerc=(CapaRANGE-UsedCapacity)/CapaRANGE*100;
  if (TotTime>=LEDTime){
    AllumLED(UsedCapacity);
    LEDTime+=60000;
  }
  if (Serial1.available()>0){
    switch(Serial1.read()){
      case 50:
        Serial1.print(BatPerc);
    }
  }
  
  switch (Serial.read()){                         //Information envoyée par l'arduino de contrôle
    case '1':                                 // Tête à 180° 
      temp_millis = millis();
      if (angle == 0){                          //Si tête est à 0°
        if (millis() - temp_millis < 12000) {
        digitalWrite(phase1, LOW);
        delay(10);
        digitalWrite(phase2, LOW);
        delay(10);
        digitalWrite(phase1, HIGH);
        delay(10);
        digitalWrite(phase2, HIGH);
        delay(10);
      }}
      else if(angle==90){                       //Si tête est à 90°
        if (millis() - temp_millis < 6000) {
        digitalWrite(phase1, LOW);
        delay(10);
        digitalWrite(phase2, LOW);
        delay(10);
        digitalWrite(phase1, HIGH);
        delay(10);
        digitalWrite(phase2, HIGH);
        delay(10);                                                    // NECESSITE DE "BREAK" (ou autre ?) POUR BIEN SORTIR DU IF ?
      }}
      angle=180;               //Changement de la valeur de l'angle pour futures rotations
      break;

    case '3':                                 // Tête à 0°
      temp_millis = millis();
      if (angle == 180){                        //Si tête est à 180°
        if (millis() - temp_millis < 12000){
        digitalWrite(phase2, LOW);
        delay(10);
        digitalWrite(phase1, LOW);
        delay(10);
        digitalWrite(phase2, HIGH);
        delay(10);
        digitalWrite(phase1, HIGH);
        delay(10);
      }}
      else if(angle==90){                        //Si tête est à 90°
        if (millis() - temp_millis < 6000){
        digitalWrite(phase2, LOW);
        delay(10);
        digitalWrite(phase1, LOW);
        delay(10);
        digitalWrite(phase2, HIGH);
        delay(10);
        digitalWrite(phase1, HIGH);
        delay(10);
      }}
      angle=0;                //Changement de la valeur de l'angle pour futures rotations
      break;

    case '0':                                   // Tête à 90°
      temp_millis = millis();
      if (angle == 180){                            //Si tête est à 180°
        if (millis() - temp_millis < 6000){
        digitalWrite(phase2, LOW);
        delay(10);
        digitalWrite(phase1, LOW);
        delay(10);
        digitalWrite(phase2, HIGH);
        delay(10);
        digitalWrite(phase1, HIGH);
        delay(10);
      }}
      else if(angle==0){                            //Si tête est à 0°
        if (millis() - temp_millis < 6000) {
        digitalWrite(phase1, LOW);
        delay(10);
        digitalWrite(phase2, LOW);
        delay(10);
        digitalWrite(phase1, HIGH);
        delay(10);
        digitalWrite(phase2, HIGH);
        delay(10);
      }}
      angle=90;                //Changement de la valeur de l'angle pour futures rotations
      break;

  }
}

/*
  Serial.print("Temps : "); // À enlever sur le programme final d'ici
  Serial.print(TotTime/1000);
  Serial.println(" Secondes");
  Serial.print("Tension batterie : ");
  Serial.print(VBAT);
  Serial.println(" Volts"); 
  Serial.print("Courant : ");
  Serial.print(AmpCS);
  Serial.println(" Ampères");
  Serial.print("Tension capteur : ");
  Serial.print(VoltCS);
  Serial.println(" Volts"); 
  Serial.print("Valeur AnalogPin : ");
  Serial.println(CurrentSensor);
  Serial.print("Capacité consomée: ");
  Serial.print(UsedCapacity);
  Serial.println(" Coulombs");
  delay(2000); // à ici
*/

// DANS L'IMMEDIAT J'AIMERAIS BIEN GARDER CE CODE EN DESSOUS, C'EST LA BASE DE L'ANNEE DERNIERE STP.

/*  if (millis() - temp_millis < 3000) {
    digitalWrite(phase1, LOW);
    delay(10);
    digitalWrite(phase2, LOW);
    delay(10);
    digitalWrite(phase1, HIGH);
    delay(10);
    digitalWrite(phase2, HIGH);
    delay(10);
  } else if (millis() - temp_millis < 6000){
    digitalWrite(phase2, LOW);
    delay(10);
    digitalWrite(phase1, LOW);
    delay(10);
    digitalWrite(phase2, HIGH);
    delay(10);
    digitalWrite(phase1, HIGH);
    delay(10);
  } else {
    temp_millis = millis();
  }
*/
