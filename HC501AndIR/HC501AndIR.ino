//HC501 sensor
int HC501_digitalPin = 33;
int HC501_val = 0;
//IR sensor
int IR_digitalPin = 32;
int IR_val = 0;

void setup() {
pinMode(HC501_digitalPin, INPUT); // sets the pin as input
pinMode(IR_digitalPin, INPUT);
Serial.begin(115200);
}
 
void loop() {
  detect();
  delay(1000);
  }

void detect(){
HC501_val = digitalRead(HC501_digitalPin); //อ่านค่าสัญญาณ digital ขา33 ที่ต่อกับเซ็นเซอร์
IR_val = digitalRead(IR_digitalPin); //อ่านค่าสัญญาณ digital ขา32 ที่ต่อกับเซ็นเซอร์

Serial.print("HC501_val = "); // HC501 พิมพ์ข้อมความส่งเข้าคอมพิวเตอร์ "val = "
Serial.println(HC501_val); // HC501 พิมพ์ค่าของตัวแปร val

Serial.print("IR_val = "); // IR พิมพ์ข้อมความส่งเข้าคอมพิวเตอร์ "val = "
Serial.println(IR_val); // IR พิมพ์ค่าของตัวแปร val
//HC501
if (HC501_val == 1 && IR_val == 0) { // ค่า เป็น 0 ตรวจจับเจอวัตถุ
Serial.println("Found You 7 MAE!!!");
}
else {
Serial.println("Cant'Dectect :(");
}
}

