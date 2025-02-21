const int pingPin = 5; //trig
int inPin = 18; //echo
long duration, cm; 

void setup() {
Serial.begin(9600);
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
delay(1000);
}
 
long microsecondsToCentimeters(long microseconds)
{
return microseconds / 29 / 2;
}