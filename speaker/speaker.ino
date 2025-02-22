#define BUZZER_PIN 27  // กำหนดให้ใช้ขา 27

void setup() {
    pinMode(BUZZER_PIN, OUTPUT);
    digitalWrite(BUZZER_PIN, HIGH); // ปิดเสียงเริ่มต้น
}

void beep(int times, int duration, int interval) {
    for (int i = 0; i < times; i++) {
        digitalWrite(BUZZER_PIN, HIGH);  // เปิดเสียง
        delay(duration);
        digitalWrite(BUZZER_PIN, LOW);   // ปิดเสียง
        delay(interval); // เว้นช่วงห่าง
    }
}

void loop() {

}
