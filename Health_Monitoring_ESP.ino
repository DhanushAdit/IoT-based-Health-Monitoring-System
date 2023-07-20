// ArduinoMqttClient - Version: Latest 
//Setup your ESP module in Arduino cloud
//Install Arduino Agent

#include "thingProperties.h"

void setup() {
  Serial.begin(9600);
  initProperties();
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
}

void loop() {
  ArduinoCloud.update();
  if(Serial.available()){
    String receivedData = Serial.readStringUntil('\n');
    String values[3];
    int count = 0;
    int startpos=0;
    int endpos;
    
    while((endpos=receivedData.indexOf(',',startpos))>=0){
      values[count] = receivedData.substring(startpos,endpos);
      startpos=endpos+1;
      count++;
    }
    if(count==3){
      ecg = values[0].toInt();
      Mybpm = values[1].toInt();
      temp = values[2].toInt();
      
    }
    //Serial.println(ecg);
    Serial.println(Mybpm);
    //Serial.println(temp);
    
  }
  
  
}

/*
  Since Mybpm is READ_WRITE variable, onMybpmChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onMybpmChange()  {
  // Add your code here to act upon Mybpm change
}
/*
  Since Ecg is READ_WRITE variable, onEcgChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onEcgChange()  {
  // Add your code here to act upon Ecg change
}
/*
  Since Temp is READ_WRITE variable, onTempChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onTempChange()  {
  // Add your code here to act upon Temp change
}
