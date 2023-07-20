#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
#include "dht.h"
dht DHT;
#include <Wire.h>
#include "MAX30105.h"           //MAX3010x library
#include "heartRate.h"          //Heart rate  calculating algorithm
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs,en,d4,d5,d6,d7);
SoftwareSerial esp(0,8);
const int pulsein = A1;
const int ecgin = A0;
int bpm;
int ecg;
float temp;
int Mybpm;
#define temp_pin A3
MAX30105 particleSensor;

const byte RATE_SIZE  = 4; //Increase this for more averaging. 4 is good.
byte rates[RATE_SIZE]; //Array  of heart rates
byte rateSpot = 0;
long lastBeat = 0; //Time at which the last  beat occurred
float beatsPerMinute;
int beatAvg;


void setup() {
  pinMode(6,OUTPUT);
  analogWrite(6,50);
  pinMode(0,INPUT); //the Serial TX port
  pinMode(8,OUTPUT); // the Serial RX port
  pinMode(temp_pin,INPUT);
  esp.begin(9600);
  particleSensor.begin(Wire, I2C_SPEED_FAST); //Use default  I2C port, 400kHz speed
  particleSensor.setup(); //Configure sensor with default  settings
  particleSensor.setPulseAmplitudeRed(0x0A); //Turn Red LED to low to  indicate sensor is running
  Serial.begin(9600);
}

void loop() {
  //For ECG

  ecg = analogRead(ecgin);

  //temperature
  //DHT.read11(temp_pin);
  

  //Pulse rate of the patient
  long irValue = particleSensor.getIR();
  if(irValue > 7000){
    if(checkForBeat(irValue)==true){
      long delta = millis()-lastBeat;
      lastBeat = millis();
      beatsPerMinute = 60/ (delta/1000.0);
      if(beatsPerMinute < 255 && beatsPerMinute >20){
        rates[rateSpot++]=(byte)beatsPerMinute;
        rateSpot %= RATE_SIZE;
        beatAvg =0;
        for(byte x=0;x<RATE_SIZE;x++){
          beatAvg += rates[x];
          beatAvg /= RATE_SIZE;
        }
      }
    }
  }
  else{
    beatAvg = 0;
  }
  temp=27;

  
 

  String myData = String(ecg)+","+String(beatAvg*3)+","+String(temp)+",";
  esp.println(myData);
  
  if(Serial.available()){
    String dat = Serial.readStringUntil('\n');
    esp.println(dat);
    
    }
}
