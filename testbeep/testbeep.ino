#define BUZZER_PIN 27  // กำหนดให้ใช้ขา 27

const int pingPin = 5; //trig
int inPin = 18; //echo
long duration, cm; 
bool hasbeep = false;

void setup() {
Serial.begin(9600);
pinMode(BUZZER_PIN, OUTPUT);
digitalWrite(BUZZER_PIN, HIGH); // ปิดเสียงเริ่มต้น
}
 
void loop()
{

 
pinMode(pingPin, OUTPUT);
 
digitalWrite(pingPin, LOW);
delayMicroseconds(2);
digitalWrite(pingPin, HIGH);
delayMicroseconds(5);
digitalWrite(pingPin, LOW);
pinMode(inPin, INPUT);
duration = pulseIn(inPin, HIGH);
 
cm = microsecondsToCentimeters(duration);
 
Serial.print(cm);
Serial.print("cm");
Serial.println();

if(cm > 80){
  if(!hasbeep){
  beep(8,100);
  hasbeep = true;
  }
}
else{
  hasbeep = false;
}

delay(1000);


}
 
long microsecondsToCentimeters(long microseconds)
{
return microseconds / 29 / 2;
}

void beep(int times, int duration) {
    for (int i = 0; i < times; i++) {
        digitalWrite(BUZZER_PIN, LOW);  // เปิดเสียง
        delay(duration);
        digitalWrite(BUZZER_PIN, HIGH);   // ปิดเสียง
        delay(duration);
    }
}