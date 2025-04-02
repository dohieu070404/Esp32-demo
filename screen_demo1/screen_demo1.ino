#include <SPI.h>
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_SH110X.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

const char* ssid = "P502_vtv";
const char* password = "123456789";

// HTTP request
String header;
// Set web server port 
WiFiServer server(80);

Adafruit_SH1106G display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


// Set thời gian hiệnt tại và trước đó
unsigned long currentTime = millis();
unsigned long previousTime = 0; 
// Xác định thời gian chờ trong mili-giây ( 1300ms)
const long timeoutTime = 1300;
 

void setup() {
  Serial.begin(115200);
  Wire.begin(D1, D2);  // SDA = D1, SCL = D2 trên ESP8266

  // Khởi động màn hình
  if (!display.begin(0x3C, true)) { // 0x3C là địa chỉ I2C
    Serial.println(F("SH1106 allocation failed"));
    for (;;);
  }

  display.clearDisplay();
  display.display();
  // setup wwifi
    // Kết nối Wi-Fi với SSID và mật khẩu
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
    display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0, 0);
  display.println("🔄 Dang ket noi WiFi...");
  display.display();
  // In địa chỉ IP và bắt đầu sever
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  display.println(WiFi.localIP());
  server.begin();

    display.clearDisplay();
  display.setCursor(0, 0);
  display.println("✅ Da ket noi WiFi!");
  display.println("SSID: " + String(WiFi.SSID()));
  display.println("IP: " + WiFi.localIP().toString());
  display.display();
}

void loop() {
  // // display.clearDisplay();
  
  // // display.setTextSize(2);
  // // display.setTextColor(SH110X_WHITE);
  // // display.setCursor(10, 10);
  // // display.println("Hello SH1106!");

  // // display.display();
  // // delay(2000);
  //  display.clearDisplay();
  // // display.setCursor(0,0);
  // display.setTextSize(1);
  // display.setTextColor(SH110X_WHITE);
  // // display.println("Start");
  // // display.display();
  // // delay(2000);
  
  // display.setCursor(10,27);
  // display.println("WiFi.localIP()");
  // display.display();
  // delay(2000);
  
  // display.setCursor(100,55);
  // display.println("End");
  // display.display();
  // delay(2000);
  
  // display.setCursor(0,55);
  // display.println("bottom left");
  // display.display();
  // delay(2000);
  
  // display.setCursor(72,0);
  // display.println("top right");
  // display.display();
}

// #define SCREEN_WIDTH 128 // OLED display width, in pixels
// #define SCREEN_HEIGHT 80 // OLED display height, in pixels

// // Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// #define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
// Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// void setup() {
//   Serial.begin(9600);

//   // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
//   if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
//     Serial.println(F("SSD1306 allocation failed"));
//     for(;;); // Don't proceed, loop forever
//   }

//   // Clear the buffer
//   display.clearDisplay();
//   display.display();

//   testdrawstyles();    // Draw 'stylized' characters


// }

// void loop() {
// }


