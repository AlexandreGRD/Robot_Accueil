 #define LED75 A9
#define LED50 A10
#define LED25 A11
#define LED0 A12
#define CREF A7
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

void Debug(){
  Serial.print("Temps : ");
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
  delay(2000);
}

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
  Serial.begin(9600);
  pinMode(A1,INPUT);
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

//Debug();
  
  
}
