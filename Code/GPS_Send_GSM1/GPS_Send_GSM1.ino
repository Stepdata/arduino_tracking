#include <SoftwareSerial.h>
#include <floatToString.h>
#include <TinyGPS.h>
TinyGPS gps;
SoftwareSerial ss(9, 8); //GPSConnect RX=8  TX=9
int Mode = 0 ;
char* var1 = "Not";
int couter = 0;
int couter1 = 0;
 int Sumq = 0;
#include "SIM900.h"   //TX - 2 /  RX - 3 /VCC - 3.4-4.4V หรือไฟ 5V จากบอร์ด Arduino ผ่านไดโอด / Gnd - Gnd
#include "sms.h"
SMSGSM sms;

void setup() {
  Serial.begin(115200);
}
void(*resetFunc)(void) = 0;

void loop() {

  if (Mode == 0) {
   
    int sensorValue = analogRead(A1);
    if (sensorValue <= 200) {
      Sumq=Sumq+1;
      Serial.print("Mode0 = ");
      Serial.println(sensorValue);
    } else if (sensorValue > 200) {
      Serial.print("Mode0 = ");
      Serial.println(sensorValue);
      Sumq = 0;
    }
    
    if(Sumq==10){ Mode = 1;}
    delay(1000);
  }
  if (Mode == 1) {
    Serial.println("Mode1");

    ss.begin(9600);
    bool newData = false;
    unsigned long chars;
    unsigned short sentences, failed;

    for (unsigned long start = millis(); millis() - start < 1000;) {
       while (ss.available()) {
        char c = ss.read();
        if (gps.encode(c))newData = true;
      }
    }
    couter1++;
    if (newData) {
      float flat, flon;
      char flat1[50], flon1[50];
      unsigned long age;
      gps.f_get_position(&flat, &flon, &age);
      floatToString(flat1, flat, 6);
      floatToString(flon1, flon, 6);
      char* msg = strcat(flat1, ",");
      msg = strcat(msg, flon1);
      Serial.println(msg);
      var1 = msg;
      couter++;
      if (couter == 2)Mode = 2;

    }
    if (couter1 == 11)Mode = 3;
  }
  if (Mode == 2) {
    Serial.println("Mode2");
    gsm.begin(9600);
    Serial.println("Send");
    char local[100] = "https://www.google.co.th/maps/search/";   //ข้อความที่จะขึ้นก่อนตำแหน่ง

    var1 = strcat(local, var1);
    sms.SendSMS("0828202426", var1); //สามารถเปลี่ยนเบอร์ที่จะส่ง SMS ได้ หรือเพิ่มเบอร์ที่จะส่งได้โดยการ Copy ทั้งบรรทัดวางใต้บรรทัดนี้แล้วเปลี่ยนหมายเลขโทสับ ถ้าต้องการเพิ่มอีกก็ทำเหมือนเดิมแล้วอัพโหลด
    //sms.SendSMS("0862977832",var1);
    Mode = 0;
    resetFunc();
  }
  if (Mode == 3) {
    Serial.println("Mode3");
    gsm.begin(9600);
    Serial.println("Send");
    sms.SendSMS("0828202426", "https://www.google.co.th/maps/search/13.823716,100.518313"); //สามารถเปลี่ยนเบอร์ที่จะส่ง SMS ได้ หรือเพิ่มเบอร์ที่จะส่งได้โดยการ Copy ทั้งบรรทัดวางใต้บรรทัดนี้แล้วเปลี่ยนหมายเลขโทสับ ถ้าต้องการเพิ่มอีกก็ทำเหมือนเดิมแล้วอัพโหลด
    sms.SendSMS("0856407084", "https://www.google.co.th/maps/search/13.823716,100.518313");
    //sms.SendSMS("0993598833", "https://www.google.co.th/maps/search/13.823716,100.518313");
    //sms.SendSMS("0862977832",var1);
    Mode = 0;
    resetFunc();
  }
}
