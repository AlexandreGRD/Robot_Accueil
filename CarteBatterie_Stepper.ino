#define LED75 A9
#define LED50 A10
#define LED25 A11
#define LED0 A12
#define CREF A7
#define CMOT A4
#define VMOT A0 
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
float CapaMIN=CapaMAX*0.25; // 0.2C Limite basse à ne pas dépasser
float CapaRANGE=CapaMAX-CapaMIN;
float BatPerc=100;
bool BatWarn=1; //Vérifie si le message d'avertissement batterie faible a déjà été envoyé

const int phase1 = 32;          // Déclaration des broches 
const int phase2 = 34;
int angle=0;                    // Angle de départ de la tête du robot
unsigned long temp_millis = millis();

void AllumLED(float CAPA){
  if (CAPA<CapaRANGE*0.25){ //Batterie entre 100% et 76%
    digitalWrite(LED75,HIGH);
    digitalWrite(LED50,HIGH);
    digitalWrite(LED25,HIGH);
    digitalWrite(LED0,HIGH);
}
  else if (CAPA<CapaRANGE*0.5){ //Batterie entre 75% et 51%
    digitalWrite(LED75,LOW);
    digitalWrite(LED50,HIGH);
    digitalWrite(LED25,HIGH);
    digitalWrite(LED0,HIGH);
  }
  else if (CAPA<CapaRANGE*0.75){ //Batterie entre 50% et 26%
    digitalWrite(LED75,LOW);
    digitalWrite(LED50,LOW);
    digitalWrite(LED25,HIGH);
    digitalWrite(LED0,HIGH);
  }
  else if (CAPA<CapaRANGE){ //Batterie entre 25% et 1%
    digitalWrite(LED75,LOW);
    digitalWrite(LED50,LOW);
    digitalWrite(LED25,LOW);
    digitalWrite(LED0,HIGH);
    if (BatWarn){
      Serial1.print(1);
    }
  }
  else if (CAPA>CapaRANGE){ //Batterie à 0%
    pinMode(CREF,OUTPUT);
    digitalWrite(CREF,HIGH); // Force le relai à s'ouvrir
  }
}

void setup() {
  Serial1.begin(9600);       //Communication avec l'arduino de contrôle
  pinMode(CMOT,INPUT);        //Setup batterie
  pinMode(VMOT,INPUT);
  pinMode(LED75,OUTPUT);
  pinMode(LED50,OUTPUT);
  pinMode(LED25,OUTPUT);
  pinMode(LED0,OUTPUT);
  pinMode(CREF,INPUT); // Haute impédance
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
  ReadBat=analogRead(VMOT);
  VBAT=ReadBat/1023.0*15.0; // Tension batterie
  CurrentSensor=analogRead(CMOT);
  VoltCS=5*(CurrentSensor/1023); // Tension de sortie du capteur
  AmpCS=(2.546-VoltCS)*10; //Courant moteurs ; Tention capteur moteurs eteints : 2.546V 
  TotTime=millis();
  UsedCapacity=UsedCapacity+((AmpCS+1.4)*(TotTime-CompTime)/3600000); //Intégration par parties ; 1.4A, consommation robot moteurs eteints
  CompTime=TotTime;
  BatPerc=(CapaRANGE-UsedCapacity)/CapaRANGE*100;
  if (TotTime>=LEDTime){ // Ajuste l'état des LEDs une fois par minute
    AllumLED(UsedCapacity);
    LEDTime+=60000;
  }
  if (Serial1.available()>0){
    switch(Serial1.read()){
      case 50:
        Serial1.print(BatPerc); //Envoie valeur batterie
        break;
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
