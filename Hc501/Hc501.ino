int digitalPin = 33;
int val = 0;
void setup() {
pinMode(digitalPin, INPUT); // sets the pin as input
Serial.begin(9600);
}
 
void loop() {
val = digitalRead(digitalPin); //อ่านค่าสัญญาณ digital ขา8 ที่ต่อกับเซ็นเซอร์
Serial.print("val = "); // พิมพ์ข้อมความส่งเข้าคอมพิวเตอร์ "val = "
Serial.println(val); // พิมพ์ค่าของตัวแปร val
if (val == 1) { // ค่า เป็น 0 ตรวจจับเจอวัตถุ สั่งให้ไฟ LED ติด
Serial.println("Dectect Motion!!!");
}
else {
Serial.println("Cant'Dectect Any Motion :(");
}
delay(1000);
}