#include <WiFi.h>
#include <time.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include "ThingSpeak.h"

#include <WiFiClient.h>

//buzzer
#define BUZZER_PIN 27


const char* ssid = "name";
const char* password = "password";

// Telegram Bot Token
const char* botToken = "------------------------------------";
const char* chat_id ="-----------------------------------";
//HC501 sensor
int HC501_digitalPin = 33;
int HC501_val = 0;

// สร้าง Client และ Bot Instance
WiFiClientSecure client;
UniversalTelegramBot bot(botToken, client);

//Bool
bool Start_Notify = true; //เตือนเมื่อถึงเวลา
bool Warning_Notify = true;//เตือนเมื่อพบผู้บุกรุก
bool Duty = false; //การทำงาน
bool Auto = true; //อัตโนมัติ
bool hasBeeped = false; //เช็คการส่งเสียง
bool doit = false; //ทดสอบทำงานตรวจจับ

//Thingspeak
int count_intrusions = 0;
// กำหนด API Key และ URL ของ ThingSpeak
const char* server = "------------------------------------";
String apiKey = "------------------------------------"; // ใส่ API Key ของคุณ

//Time
char ntp_server1[20] = "pool.ntp.org";
char ntp_server2[20] = "time.nist.gov";
char ntp_server3[20] = "time.uni.net.th";
int timezone = 7;
int dst = 0;
int currentHour;
int currentMin;
int currentSec;

//TimeSet
int time_start_hour = 1;
int time_start_minute = 30;
int time_end_hour = 6;
int time_end_minute = 30;

//ultrasonic
const int pingPin = 5; //trig
int inPin = 18; //echo
long duration, cm; 

void setup() {
  pinMode(HC501_digitalPin, INPUT); // sets the pin as input
  pinMode(BUZZER_PIN, OUTPUT); 
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(BUZZER_PIN, HIGH); 
    Serial.begin(115200);
    Serial.printf("Connecting to %s ", ssid);
    
    WiFi.begin(ssid, password);
    int retry_count = 0;
    while (WiFi.status() != WL_CONNECTED && retry_count < 20) {
        delay(500);
        Serial.print(".");
        retry_count++;
    }
    
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println(" CONNECTED");
        configTime(timezone * 3600, dst, ntp_server1, ntp_server2, ntp_server3);
        digitalWrite(LED_BUILTIN, HIGH);
        delay(10000);
        digitalWrite(LED_BUILTIN, LOW);
    } else {
        Serial.println(" Connection Failed!");
        digitalWrite(LED_BUILTIN, HIGH);
        delay(3000);
        digitalWrite(LED_BUILTIN, LOW);
        delay(3000);
        digitalWrite(LED_BUILTIN, HIGH);
        delay(3000);
        digitalWrite(LED_BUILTIN, LOW);
    }
  client.setInsecure();  
}

void loop() {
    checkTime();
    telebot();
    detect();
    ultrasonic();
    delay(1000);  // เช็คทุก 1 วินาที
}

void checkTime() {
    time_t now = time(nullptr);
    if (now == 0) {
        Serial.println("Waiting for NTP...");
        return;
    }
    
    struct tm newtime;
    localtime_r(&now, &newtime);
    //ประกาศตัวแปร ชม น วิ
    currentHour = newtime.tm_hour;
    currentMin = newtime.tm_min;
    currentSec = newtime.tm_sec;

    // แสดงเวลาปัจจุบัน
    Serial.printf("Current Time: %02d:%02d:%02d\n", currentHour, currentMin, currentSec);

    // ตรวจสอบเวลา 
    if (Auto == true){
      if ((currentHour > time_start_hour || 
    (currentHour == time_start_hour && currentMin >= time_start_minute)) && 
    (currentHour < time_end_hour || 
    (currentHour == time_end_hour && currentMin <= time_end_minute))) 
{
    Duty = true;
    if (Start_Notify) {
        bot.sendMessage(chat_id, "ถึงเวลาที่ตั้งค่าไว้แล้ว :)", "");
        Start_Notify = false;
    }
} 
else if (currentHour == time_end_hour && currentMin > time_end_minute) {
    Start_Notify = true;
    Duty = false;
}
    }
    }
    

    //ตั้งค่าในช่วงเวลาสั้นๆ
    /*if (Auto == true){
    if (currentHour == time_start_hour && currentMin == time_start_minute ){ //&& currentSec <= 59
      Duty = true;
      if(Start_Notify == true){
        bot.sendMessage(chat_id, "ถึงเวลาที่ตั้งค่าไว้แล้ว :)", "");
        Start_Notify = false;
        }
        }
    else{
       Start_Notify = true;
       Duty = false;
    }
      if(currentHour < time_end_hour){
          Start_Notify = true;
          Duty = false;
      }
    }
    }*/

void detect(){
if(Duty == true){
HC501_val = digitalRead(HC501_digitalPin); //อ่านค่าสัญญาณ digital ขา33 ที่ต่อกับเซ็นเซอร์

Serial.print("HC501_val = "); // HC501 พิมพ์ข้อมความส่งเข้าคอมพิวเตอร์ "val = "
Serial.println(HC501_val); // HC501 พิมพ์ค่าของตัวแปร val

//Ultra
Serial.print(cm);
Serial.print("cm");
Serial.println();

//HC501
if((cm < 70) || (doit == true)){  // ค่า เป็น 0 ตรวจจับเจอวัตถุ if((cm < 70) || (doit == true)){  , if ((HC501_val == 1 && cm <= 70) || doit == true) { 
Serial.println("Jone500 Detected !!!");
if(Warning_Notify == true){
digitalWrite(LED_BUILTIN, HIGH);
bot.sendMessage(chat_id, "🔴 ตรวจพบการเคลื่อนไหว! โปรดตรวจสอบ!", "");
Serial.println("Sended Message");
if (!hasBeeped) {  // ตรวจจับผู้บุกรุกและยังไม่เคยส่งเสียง
  beep(8, 100); // ดัง 3 ครั้ง, ครั้งละ 300ms, ห่างกัน 500ms
  hasBeeped = true;   // ตั้งค่าให้ไม่ส่งเสียงซ้ำ
}
Warning_Notify = false;
count_intrusions++; // เพิ่มจำนวนครั้งที่ตรวจพบ

  // 🔥 ส่งค่าขึ้น ThingSpeak
  sendDataToThingSpeak(count_intrusions, currentHour, currentMin, Duty);
}
doit = false;
}
else {
Serial.println("Cant'Dectect :(");
Warning_Notify = true;
hasBeeped = false;
digitalWrite(LED_BUILTIN, LOW);
}
}
}

void telebot(){
  // ตรวจสอบข้อความใหม่จาก Telegram
int New_Msg = bot.getUpdates(bot.last_message_received + 1);

while (New_Msg) {
Serial.println("New message to Telegram ");
for (int i = 0; i < New_Msg; i++) {
//String chat_id = bot.messages[i].chat_id;
String text = bot.messages[i].text;
Serial.println("Message: " + text);
// ส่งข้อความตอบกลับ
if (text == "/hello") {
bot.sendMessage(chat_id, "สวัสดี ฉันชื่องูจงอางมีหน้าที่แจ้งเตือนความปลอดภัย🐍", "");}
else if (text == "/chatid") {
bot.sendMessage(chat_id, "chat id =" + String(chat_id), "");}
else if (text == "/timeset") {
bot.sendMessage(chat_id, "🕐เวลาที่ตั้งไว้คือ "  + String(time_start_hour) + ":" + String(time_start_minute) + " จนถึงเวลา " + String(time_end_hour) + ":" + String(time_end_minute),""); }
else if (text == "/status") {
bot.sendMessage(chat_id, "🔧การทำงาน = " + String(Duty), "");}
else if (text == "/on") {
Auto = false;
Duty = true;
// 🔥 ส่งค่า Duty ไปยัง ThingSpeak
sendDutyToThingSpeak(1);
bot.sendMessage(chat_id, "🟢เปิดการทำงาน = " + String(Duty), "");}
else if (text == "/off") {
Auto = false;
Duty = false;
// 🔥 ส่งค่า Duty ไปยัง ThingSpeak
sendDutyToThingSpeak(0);
bot.sendMessage(chat_id, "🔴ปิดการทำงาน = " + String(Duty), "");}
else if (text == "/auto.on") {
Auto = true;
bot.sendMessage(chat_id, "🟢เปิดการทำงานAuto = " + String(Auto), "");}
else if (text == "/auto.off") {
Auto = false;
bot.sendMessage(chat_id, "🔴ปิดการทำงานAuto = " + String(Auto), "");}
else if (text == "/auto.check") {
bot.sendMessage(chat_id, "🦾สถานะการทำงานAuto = " + String(Auto), "");}
else if (text == "/whatstime") {
bot.sendMessage(chat_id, "🕐ตอนนี้เวลา "  + String(currentHour) + ":" + String(currentMin) + " น." ,""); }
else if (text == "/doit") {
doit = true;}
else if (text == "/dontdoit") {
doit = false;}
else if (text == "/dostatus") {
bot.sendMessage(chat_id, "🦾สถานะการทำงานDo = " + String(doit), "");}

else {
bot.sendMessage(chat_id, "🤯ฉันไม่เข้าใจ " + text, "");
}
}
New_Msg = bot.getUpdates(bot.last_message_received + 1);
}
}

void beep(int times, int duration) {
    for (int i = 0; i < times; i++) {
        digitalWrite(BUZZER_PIN, LOW);  // เปิดเสียง
        delay(duration);
        digitalWrite(BUZZER_PIN, HIGH);   // ปิดเสียง
        delay(duration);
    }
}


void sendDataToThingSpeak(int count, int hour, int minute, int status) {
    WiFiClient client;
    if (client.connect(server, 80)) { 
        String url = "/update?api_key=" + apiKey;
        url += "&field1=" + String(count);   // จำนวนผู้บุกรุก
        url += "&field2=" + String(hour);    // ชั่วโมงที่ตรวจพบ
        url += "&field3=" + String(minute);  // นาทีที่ตรวจพบ
        url += "&field4=" + String(status);  // สถานะระบบ

        client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                     "Host: " + server + "\r\n" +
                     "Connection: close\r\n\r\n");

        Serial.println("Data sent to ThingSpeak!");
        client.stop();
    } else {
        Serial.println("Connection to ThingSpeak failed.");
    }
}

void sendDutyToThingSpeak(int dutyStatus) {
    WiFiClient client;

    // เชื่อมต่อเซิร์ฟเวอร์
    if (client.connect(server, 80)) {
        String url = "/update?api_key=" + apiKey;
        url += "&field4=" + String(dutyStatus);
        
        // ส่ง HTTP Request
        client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                     "Host: " + server + "\r\n" +
                     "Connection: close\r\n\r\n");
        
        Serial.println("🔵 ส่งค่า Duty ไปยัง ThingSpeak แล้ว!");
    } else {
        Serial.println("🔴 ไม่สามารถเชื่อมต่อกับ ThingSpeak ได้!");
    }
    client.stop();
}

void ultrasonic(){
pinMode(pingPin, OUTPUT);
digitalWrite(pingPin, LOW);
delayMicroseconds(2);
digitalWrite(pingPin, HIGH);
delayMicroseconds(5);
digitalWrite(pingPin, LOW);
pinMode(inPin, INPUT);
duration = pulseIn(inPin, HIGH);
cm = microsecondsToCentimeters(duration);
}

long microsecondsToCentimeters(long microseconds)
{
return microseconds / 29 / 2;
}