//#include <ThingSpeak.h>

//#include <SPI.H>
//#include <LoRa.h>

#include <Adafruit_Sensor.h>
#include "DHT.h"
#include <stdlib.h>

#include <SPI.h>
#include <RH_RF95.h>

RH_RF95 rf95;

float frequency = 915.0;

const int MQ2 = 0;
const int MQ7 = 1;
const int MQ9 = 2;
const int MQ131 = 3;
const int MQ135 = 4;

float MQ2data=0,MQ7data=0, MQ9data=0, MQ131data=0, MQ135data=0, dhtHumidity=0, dhtTemp=0;


#define DHTPIN 9
#define DHTTYPE DHT11
DHT myDht(DHTPIN, DHTTYPE);

void setup() {
  myDht.begin();
  Serial.begin(9600);

 Serial.println("Start LoRa Client");
  if (!rf95.init())
    Serial.println("init failed");
  // Setup ISM frequency
  rf95.setFrequency(frequency);
  // Setup Power,dBm
  rf95.setTxPower(13);

  // Setup Spreading Factor (6 ~ 12)
  rf95.setSpreadingFactor(7);
  
  // Setup BandWidth, option: 7800,10400,15600,20800,31250,41700,62500,125000,250000,500000
  //Lower BandWidth for longer distance.
  rf95.setSignalBandwidth(125000);
  // Setup Coding Rate:5(4/5),6(4/6),7(4/7),8(4/8) 
  rf95.setCodingRate4(5);

}

void loop() {
Serial.print("serial started wait 3 minutes");
  initializeMQ();
  readData();

    Serial.print("CO2 conc: ");
    Serial.print (MQ2data);
    Serial.print("Air quality");
    Serial.print (MQ135data);
    Serial.print ("ozone conc");
    Serial.print (MQ131data);
    Serial.print("Humidity: ");
    Serial.print(dhtHumidity);
    Serial.println("%");
    Serial.print("Temperature: ");
    Serial.print(dhtTemp);
    Serial.println(" *C");
delay(100);

  Serial.println("Sending to LoRa Server");
  // Send a message to LoRa Server
  uint8_t data[] = {MQ2data, MQ135data, MQ131data, MQ7data, dhtHumidity,dhtTemp};
  rf95.send(data, sizeof(data));
  
  rf95.waitPacketSent();
  // Now wait for a reply
  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);

  if (rf95.waitAvailableTimeout(3000))
  { 
    // Should be a reply message for us now   
    if (rf95.recv(buf, &len))
   {
      Serial.print("got reply: ");
      Serial.println((char*)buf);
      Serial.print("RSSI: ");
      Serial.println(rf95.lastRssi(), DEC);    
    }
    else
    {
      Serial.println("recv failed");
    }
  }
  else
  {
    Serial.println("No reply, is LoRa server running?");
  }
  delay(5000);

}



//float h= myDht.readHumidity();
//
//float t= myDht.readTemperature();
void initializeMQ(){
analogWrite(MQ2,HIGH);
analogWrite(MQ7,HIGH);
analogWrite(MQ9,HIGH);
analogWrite(MQ131,HIGH);
analogWrite(MQ135,HIGH);

delay(60000);

analogWrite(MQ2,71.4);
analogWrite(MQ7,71.4);
analogWrite(MQ9,71.4);
analogWrite(MQ131,71.4);
analogWrite(MQ135,71.4);

delay(90000);

  
  }



void readData(){
analogWrite(MQ2,HIGH);
delay(50);
 MQ2data= analogRead(MQ2);

analogWrite(MQ7,HIGH);
delay(50);
 MQ7data= analogRead(MQ7);

analogWrite(MQ9,HIGH);
delay(50);
 MQ9data= analogRead(MQ9);

analogWrite(MQ131,HIGH);
delay(50); MQ131data= analogRead(MQ131);

analogWrite(MQ135,HIGH);
delay(50);
 MQ135data= analogRead(MQ135);


 dhtHumidity= myDht.readHumidity();
 dhtTemp= myDht.readTemperature();

  }

// float arrayList[]={MQ2data, MQ7data, MQ9data, MQ131data, MQ135data, dhtHumidity, dhtTemp};
// String charValues[8];
// char result[15];


 

//for(int i=0; i<6; i++){
// float value= arrayList[i];
//  dtostrf(value,8, 3, result);
//  charValue[i]= result;
//  }
//
//Serial.print("char value: " charValue);




 
