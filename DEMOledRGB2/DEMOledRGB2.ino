#include <Arduino.h>

// Chân kết nối LED RGB (Common Anode)
const int redPin = 5;   // Chân R
const int greenPin = 4; // Chân G
const int bluePin = 0;  // Chân B

// Hàm để viết giá trị PWM cho LED RGB 
void setColor(int red, int green, int blue) {
    analogWrite(redPin, 255 - red);    
    analogWrite(greenPin, 255 - green);
    analogWrite(bluePin, 255 - blue);
}

void setup() {
    pinMode(redPin, OUTPUT);
    pinMode(greenPin, OUTPUT);
    pinMode(bluePin, OUTPUT);
}

void loop() {
  
    for (int i = 0; i <= 255; i++) {
        setColor(i, 0, 255 - i);  // Đỏ -> Tím -> Xanh dương
        delay(10);
    }
    for (int i = 0; i <= 255; i++) {
        setColor(255 - i, i, 0);  // Xanh dương -> Xanh lá
        delay(10);
    }
    for (int i = 0; i <= 255; i++) {
        setColor(0, 255 - i, i);  // Xanh lá -> Vàng -> Đỏ
        delay(10);
    }
}
