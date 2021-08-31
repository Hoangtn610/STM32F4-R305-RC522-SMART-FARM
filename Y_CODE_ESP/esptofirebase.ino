#include "FirebaseESP8266.h"
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <EEPROM.h>

#include <string.h>
#define FIREBASE_HOST "https://espfirebase-7cc13-default-rtdb.firebaseio.com/" // SINH
#define FIREBASE_AUTH "o02QCqU1tZLgLKom5sT6W7oofc0vOi2wp5yCLdwb"  
#define FIREBASE_HOST1 "https://testmoi-dd56e-default-rtdb.firebaseio.com/" // SINH
#define FIREBASE_AUTH1 "9gy7vBgUFgYsiK7WmJejJm0LWWwbTVdfqMz08KX1"  

#define WIFI_SSID "MeoCun"   
#define WIFI_PASSWORD "ngahieu1998"     
#define Rx 13             
#define Tx 15             
SoftwareSerial mySerial(Rx,Tx);        
int i=0, ID=0, n = 0;

String control,den,quat,maybom;
String RxBuffer="";
String RxBuffer1="";
String path2="cacgiatri";
String path1="moi";
String path3="dht11";
String ten, tg, tg_new, ngayS;
String cbs,cbd,nd,da;
byte moc;
char GIO_PHUT_GIAY[20], THU_NGAY_THANG[20], THOI_GIAN[20], THOI_GIAN_NEW[20], NGAY[3];
FirebaseData firebaseData;
void setup() {
  // put your setup code here, to run once:
  pinMode(16,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(0,OUTPUT);
  Serial.begin(57600); 
  mySerial.begin(57600);
  EEPROM.begin(512);
  EEPROM.commit();
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  Firebase.begin(FIREBASE_HOST1, FIREBASE_AUTH1);
  Firebase.reconnectWiFi(true);
  // XOA EEPROM
//  for (int j = 0; j < 60; j++) 
//  {
//      EEPROM.write(j, 0);
//  }
//  EEPROM.end();
}

void loop() {
delay(1000);
  NHAN_UART();
  // put your main code here, to run repeatedly:
Firebase.setString(firebaseData, path2+"/cambienanhsang", cbs);
Firebase.setString(firebaseData, path2+"/cambiendat", cbd);
Firebase.setString(firebaseData, path3+"/nhietdo", nd);
Firebase.setString(firebaseData, path3+"/doam", da);
 NHAN_UART1();
if(ten != "")
{Firebase.setString(firebaseData, path1+"/1",ten);
}
Firebase.getString(firebaseData,path1+"/control",control);
Firebase.getString(firebaseData,path1+"/maybom",maybom);
Firebase.getString(firebaseData,path1+"/den",den);
Firebase.getString(firebaseData,path1+"/quat",quat);
if(control=="0")
  digitalWrite(16,LOW);
 else digitalWrite(16,HIGH);
 
if(maybom=="1")
  digitalWrite(5,HIGH);
 else digitalWrite(5,LOW);
 
if(den=="1")
  digitalWrite(4,HIGH);
 else digitalWrite(4,LOW);
 
if(quat=="1")
  digitalWrite(0,HIGH);
 else digitalWrite(0,LOW);
}
void NHAN_UART()
{
  if(Serial.available())
  {
    while(Serial.available())
    {
      RxBuffer = Serial.readString();
    }
    cbs=RxBuffer;
    cbd=RxBuffer;
    nd=RxBuffer;
    da=RxBuffer;
    cbs.remove(2);
    cbd.remove(5);
    cbd.remove(0,3);
    nd.remove(8);
    nd.remove(0,6);
    da.remove(11);
    da.remove(0,9);
  }
}
void NHAN_UART1()
{if(mySerial.available())
  {
    while(mySerial.available())
    {
      RxBuffer1 = mySerial.readString();
    }
  ten=RxBuffer1;
  ten.remove(26);

   }
}
