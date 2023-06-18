#include <SPI.h>
#include <MFRC522.h>
constexpr uint8_t RST_PIN = D3;     
constexpr uint8_t SS_PIN = D4;

// importing libraries for connecting the nodemcu with wifi network
#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
const char *ssid="55-J";
const char *password="5445845555";

// making a client for sending and receiving time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP,"pool.ntp.org",19800);

MFRC522 rfid(SS_PIN, RST_PIN); // making Object of the class
// String checktime(void); //function that will take care of the current time
  
bool flag=1;
String tag;
void setup() {
  Serial.begin(115200); //to initiate the communication between controller board and the computer to which it is connected
  SPI.begin(); // Init SPI bus  (to initialize the spi communication)
  rfid.PCD_Init(); // Init MFRC522 (it initializes proximity coupling device i.e rfid module)
  pinMode(D8, OUTPUT);


  //For editing the excel sheet
  Serial.println("CLEARDATA");
  Serial.println("LABEL,Name,Roll_no,branch,Date,Time,RFID UID,Status");

  //Connecting the WiFi
  WiFi.begin(ssid,password);
  while(WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
    
    timeClient.begin();    
  }
  
  

}
void loop() {
  //fetching and checking current time
  String curr_time=checktime();
  if(curr_time>"121700") flag=0;
  else flag=1;

  if ( ! rfid.PICC_IsNewCardPresent())
    return;
  if (rfid.PICC_ReadCardSerial())
  {
        for (byte i = 0; i < 4; i++) {
          tag += rfid.uid.uidByte[i];
        }
        if (tag == "1158689166" || tag=="21123847173")
        {
          digitalWrite(D8, HIGH);
          delay(100);
          digitalWrite(D8, LOW);
          delay(100);
          digitalWrite(D8, HIGH);
          delay(100);
          digitalWrite(D8, LOW);
          delay(100);
          digitalWrite(D8, HIGH);
          delay(100);
          digitalWrite(D8, LOW);
          delay(100);
        
          if(tag=="1158689166")
          {
            // Serial.println("LABEL,Name,Roll_no,branch,Date,Time,RFID UID,Status");
            String name="Jainam_jain";
            String roll="20bec043";
            Serial.print("DATA,");
            Serial.print(name+",");
            Serial.print(roll+","+"ECE,");
            Serial.print("DATE,TIME,"+tag);
            Serial.print(",");
            
            if(flag) Serial.println("ON_TIME");
            else  Serial.println("NOT_ON_TIME");
            
          }
          if(tag=="21123847173")
          {
            String name="Maharshi_patel";
            String roll="20bec061";
            Serial.print("DATA,");
            Serial.print(name+",");
            Serial.print(roll+","+"ECE,");
            Serial.print("DATE,TIME,"+tag);
            Serial.print(",");

            
            if(flag) Serial.println("ON_TIME");
            else Serial.println("NOT_ON_TIME");

          }

    } 
    else
    {
      Serial.println("Access Denied!");
      digitalWrite(D8, HIGH);
      delay(2000);
      digitalWrite(D8, LOW);
    }
    
    tag = "";
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
    Serial.println("SAVEWORKBOOKAS,Automatic_Attendence_sheet");
  }
}

String checktime(void)
{
  timeClient.update();

  String time="";
  time=time+ ((String)timeClient.getHours())+((String)timeClient.getMinutes())+((String)timeClient.getSeconds());
  return time;
}