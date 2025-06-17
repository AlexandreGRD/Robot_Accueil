#include <SD.h>
#include <SPI.h>
#include <AudioZero.h>
#include <Servo.h>
int i=1;
Servo cervi;

void setup(){
  randomSeed(analogRead(A6));
  Serial1.begin(9600);        //Setup la connexion série avec l'arduino de contrôle
  if (!SD.begin()) {
    return;
  }
  cervi.attach(2);
  cervi.write(103);
}

          // Déclaration des différentes pistes audio//

void Personnel(){
  File myFile = SD.open("Person.wav");         // Ouvrir le fichier de la carte SD
  AudioZero.begin(44100); // fichier .wav mono ou stereo 44100Hz (si stereo begin(2*44100))
  AudioZero.play(myFile);             // et ne seront pas dans le code final
  AudioZero.end(); 
}

void Agreable(){
  File myFile = SD.open("BjrAgr.wav");              // Ouvrir le fichier de la carte SD
  AudioZero.begin(44100); // fichier .wav mono ou stereo 44100Hz (si stereo begin(2*44100))
  AudioZero.play(myFile);
  AudioZero.end();
  i=2;
}

void Obstacle(){
  if (random(1,200)==42){
  File myFile = SD.open("Whilelm.wav");         // Ouvrir le fichier de la carte SD
  AudioZero.begin(44100); // fichier .wav mono ou stereo 44100Hz (si stereo begin(2*44100))
  AudioZero.play(myFile);
  AudioZero.end();
  }
  else{
  File myFile = SD.open("Obstacle.wav");         // Ouvrir le fichier de la carte SD
  AudioZero.begin(44100); // fichier .wav mono ou stereo 44100Hz (si stereo begin(2*44100))
  AudioZero.play(myFile);
  AudioZero.end();}
}

void Service(){
  File myFile = SD.open("BjrSer.wav");         // Ouvrir le fichier de la carte SD
  AudioZero.begin(44100); // fichier .wav mono ou stereo 44100Hz (si stereo begin(2*44100))
  AudioZero.play(myFile);
  AudioZero.end();
  i=3;
}

void Assist(){
  File myFile = SD.open("BjrAss.wav");         // Ouvrir le fichier de la carte SD
  AudioZero.begin(44100); // fichier .wav mono ou stereo 44100Hz (si stereo begin(2*44100))
  AudioZero.play(myFile);
  AudioZero.end(); 
  i=4;
}

void Batterie(){
  File myFile = SD.open("BatCri.wav");         // Ouvrir le fichier de la carte SD
  AudioZero.begin(44100); // fichier .wav mono ou stereo 44100Hz (si stereo begin(2*44100))
  AudioZero.play(myFile);
  AudioZero.end(); 
}

void Aider(){
  File myFile = SD.open("BjrAider.wav");         // Ouvrir le fichier de la carte SD
  AudioZero.begin(44100); // fichier .wav mono ou stereo 44100Hz (si stereo begin(2*44100))
  AudioZero.play(myFile);
  AudioZero.end();
  i=1;
}

void loop(){ 
  switch(Serial1.read()){
    case '1':
      cervi.write(90);
      switch(i){
        case 1:
          Agreable();
          break;
        case 2:
          Service();
          break;
        case 3:
          Assist();
          break;
        case 4:
          Aider();
          break;
      }
      break;
    case '2':
      Obstacle();
      break;
    case '3':
      Personnel();
      delay(500);
      cervi.write(130);
      break;
    case '5':
      Batterie();
      break;
    case '0':
      cervi.write(103);
      break;
  }
}
