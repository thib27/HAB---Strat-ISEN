//========================================================
//                        INCLUDE
//========================================================
//RTC
#include <Wire.h>
#include "RTClib.h"
//CCS811
#include "Adafruit_CCS811.h"
// SD Card
#include <SD.h>
#include <SPI.h>




//========================================================
//                       CENSOR VAR
//========================================================
Adafruit_CCS811 ccs; // air quality
RTC_DS1307 rtc;      //rtc
#define sdCard 8;    //SD Card



//========================================================
//                         SETUP
//========================================================
void setup()
{
  Serial.begin(9600);

  //---------------------------------------------------------
  // Test for RTC DS1307 MR514
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  //---------------------------------------------------------
  //SD Card 
  if (!SD.begin(chipSelect)) {
    Serial.println("uSD Card failed, or not present");
    // don't do anything more:
    while (1); // debeug : fais pas une crise de panique alexis
  }
  Serial.println("Card initialized.");

  //---------------------------------------------------------
  //CCS811
  if(!ccs.begin())
  {
    Serial.println("Failed to start CCS811! Please check your wiring.");
    while(1);
  }
  while(!ccs.available());
  float temp = ccs.calculateTemperature();
  ccs.setTempOffset(temp - 25.0);
  Serial.println("CCS811 initialized.");

}



//========================================================
//                         LOOP
//========================================================
void loop()
{
  sensorRequest();
  delay(500);
}



//========================================================
//                    SENSOR REQUEST
/* 
Function wich call sensors functions and record their data into a 
array. Then, give this array to file_Wrtiting for the file save.
*/
//========================================================
double[][] sensorRequest()
{
  double sensorValues[8];

  // call each censor functions
  sensorValues[0] = Adafruit_CCS811_air_quality(0);
  sensorValues[1] = Adafruit_CCS811_air_quality(1);
  sensorValues[2] = Adafruit_CCS811_air_quality(2);
  sensorValues[3] = yes();
  sensorValues[4] = yes();
  sensorValues[5] = yes();
  sensorValues[6] = yes();
  sensorValues[7] = yes();

  // get information from each functions
  // stocks them in an array
  // return the array
}


//========================================================
//                    TO COMPLETE : ALERT
//========================================================

void led_alert(){
  // turn on a RED led to alert there is a problem.
}

//========================================================
//                    FILE WRITING
//========================================================
void file_Wrtiting(int Tab){
  
  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  if(dataFile){
    for(int i(0); i< Tab.size(); i++){
        dataFile.println(Tab[i] + ";");
    }
  }
  else{
    alert();
  }
    dataFile.close();

}


//========================================================
//              Adafruit_CCS811_air_quality
//========================================================
float Adafruit_CCS811_air_quality(int value)
{
  if (ccs.available() && !ccs.readData())
  {
    switch (value)
    {
    case 0: // CO2 (ppm)
      return ccs.geteCO2();
    case 1: // TVOC (ppb)
      return ccs.getTVOC();
    case 2: // temp (°C)
      return ccs.calculateTemperature();
    default:
      return -1;
    }
  }
  else
    return -1;
}
