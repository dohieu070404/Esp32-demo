#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(115200);
  
  // Khởi tạo I2C (SDA = D2, SCL = D1)
  Wire.begin(D2, D1);
  
  // Kiểm tra màn hình OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("Không tìm thấy màn hình OLED!"));
    while (true); // Dừng chương trình nếu không kết nối được
  }

  Serial.println(F("OLED đã kết nối!"));

  display.clearDisplay();
  
  // Hiển thị văn bản kiểm tra
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(10, 20);
  display.println("OK!");
  display.display();
}

void loop() {
}
