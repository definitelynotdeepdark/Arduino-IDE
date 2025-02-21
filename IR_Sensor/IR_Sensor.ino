
int digitalPin = 32;
int val = 0;
void setup() {
pinMode(digitalPin, INPUT); // sets the pin as input
Serial.begin(9600);
}
 
void loop() {
val = digitalRead(digitalPin); //อ่านค่าสัญญาณ digital ขา8 ที่ต่อกับ เซ็นเซอร์ตรวจจับวัตถุ IR Infrared
Serial.print("val = "); // พิมพ์ข้อมความส่งเข้าคอมพิวเตอร์ "val = "
Serial.println(val); // พิมพ์ค่าของตัวแปร val
if (val == 0) { // ค่า เป็น 0 ตรวจจับเจอวัตถุ สั่งให้ไฟ LED ติด
Serial.println("Found !!!");
}
else {
Serial.println("Not Found :("); // สั่งให้ LED ดับ
}
delay(1000);
}