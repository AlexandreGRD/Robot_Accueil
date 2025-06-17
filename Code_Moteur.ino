#include <Servo.h>

#define LineTurnLeft 2 //Capteur de suivi de ligne 1
#define LineTurnRight 3 //Capteur de suivi de ligne 2
#define LineFollow 6 //Capteur de suivi de ligne 5
#define Motion 7 //Capteur de mouvement
#define Obstacle_Trigger1 8 //Capteur d'obstacle gauche trigger
#define Obstacle_Echo1 9 //Capteur d'obstacle gauche reponse
#define Obstacle_Trigger2 A0 //Capteur d'obstacle droit trigger
#define Obstacle_Echo2 A1 //Capteur d'obstacle droit reponse
#define Obstacle_Trigger3 A2 //Capteur d'obstacle tête trigger
#define Obstacle_Echo3 A3 //Capteur d'obstacle tête reponse
int MesureMaxi=300,MesureMini=3; //limite des capteurs d'obstacle
float Cm=1.715; //formule distance
long DureeG,DistanceG,DureeD,DistanceD,DureeT,DistanceT; //resultat des capteurs d'obsacle
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
  pinMode(Obstacle_Trigger3,OUTPUT);
  pinMode(Obstacle_Echo1,INPUT);
  pinMode(Obstacle_Echo2,INPUT);
  pinMode(Obstacle_Echo3,INPUT); //definition des pins 
  delay(3000);
  Serial2.println(0);
}

void loop() {
  if(Serial.available()>0){
    switch(Serial.parseInt()){
      case 48:
        PartyMode=0;
        break;
      case 49:
        PartyMode=1;
        break;
      case 50:
        Serial2.println(2);
        while(Serial2.available()==0){}
        Serial.println(Serial.parseInt());
        break;
    }
  }
  if(Serial2.available()>0){
    switch(Serial2.read()){
      case 49:
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
        if(cd-millis()<=30000){
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
      SensCase=SensCase+4;
      break;
    case 1:
      SensCase=SensCase-4;
      break;
  }
}
void TurnRight(){
  switch(digitalRead(LineTurnRight)){
    case 0:
      SensCase=SensCase+2;
      break;
    case 1:
      SensCase=SensCase-2;
      break;
  }
}
void Detect(){
  Serial2.println(3);
  Serial1.println(1);
  delay(6000);
  Drive();
  Serial1.println(3);
  Serial2.println(1);
  delay(12000);
  Backward();
}
void Drive(){
  vitesse(0,180);
  delay(500);
  while(digitalRead(LineTurnRight)==1 && digitalRead(LineTurnLeft)==1 && digitalRead(LineFollow)==0){
    if(digitalRead(Motion)==1){
      vitesse(45,135);
      DureeG = pulseIn(Obstacle_Echo1, HIGH);
      DistanceG=DureeG*Cm;
      DureeD = pulseIn(Obstacle_Echo2, HIGH);
      DistanceD=DureeD*Cm;
      if ((DistanceD<100)||(DistanceG<100)){
        vitesse(90,90);
        Serial1.println(2);
        delay(3000);
      }
    }
    switch(SensCase){
      case 0:
        vitesse(0,180);
        break;
      case 2:
        vitesse(0,165);
        break;
      case 4:
        vitesse(15,180);
        break;
    }
  }
}
void Backward(){
  vitesse(180,0);
  delay(500);
  while(digitalRead(LineTurnRight)==1 && digitalRead(LineTurnLeft)==1 && digitalRead(LineFollow)==0){
    if(digitalRead(Motion)==1){
      vitesse(135,45);
      DureeT = pulseIn(Obstacle_Echo3, HIGH);
      DistanceT=DureeT*Cm;
      if (DistanceT<100){
        vitesse(90,90);
        Serial1.println(2);
        delay(3000);
      }
    }
      switch(SensCase){
      case 0:
        vitesse(180,0);
        break;
      case 2:
        vitesse(165,0);
        break;
      case 4:
        vitesse(0,165);
        break;
    }
  }
}
