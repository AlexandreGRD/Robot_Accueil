#include <Servo.h>

#define LineTurnLeft 2 //Capteur de suivi de ligne 1
#define LineTurnRight 3 //Capteur de suivi de ligne 2
#define LineFollow 6 //Capteur de suivi de ligne 5
#define Motion 7 //Capteur de mouvement
#define Obstacle_Trigger1 8 //Capteur d'obstacle gauche trigger
#define Obstacle_Echo1 9 //Capteur d'obstacle gauche reponse
#define Obstacle_Trigger2 A0 //Capteur d'obstacle tete trigger
#define Obstacle_Echo2 A1 //Capteur d'obstacle Tete reponse
int rotation=60;
int MesureMaxi=300,MesureMini=3; //limite des capteurs d'obstacle
float Cm=1.715; //formule distance
long DureeG,DistanceG,DureeT,DistanceT; //resultat des capteurs d'obsacle
volatile int SensCase=0;//To determine wich  case to use
bool PartyMode=1;
int Batterie,cd=0;
Servo moteurGauche;  // Crée un objet servo pour controler le servo
Servo moteurDroite;
int pos = 90;


void setup() {
  moteurDroite.attach(46,1000,2000);
  moteurGauche.attach(44,1000,2000);
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial2.begin(9600);
  pinMode(LineTurnLeft,INPUT);
  pinMode(LineTurnRight,INPUT);
  pinMode(LineFollow,INPUT);
  attachInterrupt(digitalPinToInterrupt(LineTurnLeft),TurnLeft,CHANGE); 
  attachInterrupt(digitalPinToInterrupt(LineTurnRight),TurnRight,CHANGE); 
  pinMode(Obstacle_Trigger1,OUTPUT);
  pinMode(Obstacle_Trigger2,OUTPUT);
  pinMode(Obstacle_Echo1,INPUT);
  pinMode(Obstacle_Echo2,INPUT);
  delay(1000);
  Serial2.println(0);
  delay(5000);
  calibrage();
  delay(2000);
}

void loop() {
  if(Serial.available()>0){
    switch(Serial.parseInt()){
      case 0:
        PartyMode=0;
        /*delay(300);
        Serial.println(0);*/
        break;
      case 1:
        PartyMode=1;
        /*delay(300);
        Serial.println(1);*/
        break;
      case 2:
        Serial2.print(2);
        delay(300);
        while(Serial2.available()==0){}
        Serial.println(Serial.parseInt());
        break;
    }
  }
  if(Serial2.available()>0){
    switch(Serial2.parseInt()){
      case 1:
        Serial1.print(5);
        break;
    }
  }
  if(digitalRead(Motion)==1){
    switch(PartyMode){
      case 0:
        Detect();
        break;
      case 1:
        if(millis()-cd<=90000){
          break;
        }
        else{
          Serial1.println(1);
          cd=millis();
          break;
        }
    }
  }
}
void vitesse(int vitesseG,int vitesseD) {
  moteurGauche.write(vitesseG);
  moteurDroite.write(vitesseD);
}
void TurnLeft(){
  switch(digitalRead(LineTurnLeft)){
    case 0:
      SensCase=SensCase-4;
      break;
    case 1:
      SensCase=SensCase+4;
      break;
  }
}
void TurnRight(){
  switch(digitalRead(LineTurnRight)){
    case 0:
      SensCase=SensCase-2;
      break;
    case 1:
      SensCase=SensCase+2;
      break;
  }
}
void Detect(){
  Serial2.print(3);
  delay(300);
  Serial1.println(1);
  delay(6000);
  Drive();
  Serial1.println(3);
  delay(300);
  Serial2.println(1);
  delay(6000);
  Backward();
}
void Drive(){
  vitesse(0,180);
  delay(500);
    while((digitalRead(LineTurnRight)==0 || digitalRead(LineTurnLeft)==0)){
    if(digitalRead(Motion)==1){
      vitesse(45,135);
      DureeG = pulseIn(Obstacle_Echo1, HIGH);
      DistanceG=DureeG*Cm;
      if ((DistanceG<100)){
        vitesse(90,90);
        Serial1.println(2);
        delay(8000);
      }
    }
    Serial.println(SensCase);
    switch(SensCase){
      case 0:
        vitesse(0,180);
        break;
      case 2:
        vitesse(0,180-rotation);
        break;
      case 4:
        vitesse(rotation,180);
        break;
    }
  }
  vitesse(90,90);
}
void Backward(){
  vitesse(180,0);
  delay(500);
  while((digitalRead(LineTurnRight)==0 || digitalRead(LineTurnLeft)==0)){
    /*if(digitalRead(Motion)==1){
      vitesse(135,45);
      DureeT = pulseIn(Obstacle_Echo2, HIGH);
      DistanceT=DureeT*Cm;
      if (DistanceT<100){
        vitesse(90,90);
        Serial1.println(2);
        delay(8000);
      }
    }*/
    Serial.println(SensCase);
    switch(SensCase){
      case 0:
        vitesse(180,0);
        break;
      case 2:
        vitesse(180-rotation,0);
        break;
      case 4:
        vitesse(180,rotation);
        break;
    }
  }
  vitesse(90,90);
}
void calibrage(){
  delay(500);
  vitesse(90,90);
  delay(500);
  vitesse(0,0);
  delay(500);
  vitesse(180,180);
  delay(500);
  vitesse(90,90);
  delay(500);
}
