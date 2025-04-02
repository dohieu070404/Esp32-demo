#include <SPI.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_SH110X.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

const char* ssid = "P502_vtv";
const char* password = "123456789";

// API thời tiết (OpenWeatherMap)
const char* apiKey = "your-API-key";  
const char* city = "Hanoi";
const char* countryCode = "VN";

// Cài đặt NTP
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 7 * 3600, 60000); // GMT+7 (Việt Nam)

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
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0, 0);
  display.println("Dang ket noi WiFi...");
  display.display();
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

  // In địa chỉ IP và bắt đầu sever
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  display.println(WiFi.localIP());
  server.begin();

    display.clearDisplay();
  display.setCursor(0, 0);
  display.println(" Da ket noi WiFi!");
  display.println("SSID: " + String(WiFi.SSID()));
  display.println("IP: " + WiFi.localIP().toString());
  delay(2000);
  display.display();
}
void loop() {
 timeClient.update();
  String timeNow = timeClient.getFormattedTime();

  // Hiển thị trên OLED
  display.clearDisplay();
  display.setTextSize(4,5);
  display.setCursor(0, 20);
  display.println(timeNow);
  display.display();

  Serial.println("いまは: " + timeNow);
  delay(1000);
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   WiFiClient client = server.available();   // Lắng nghe kết nối từ client


  if (client) {                             // Lắng nghe kết nối client mới
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          
    String currentLine = "";                
    while (client.connected() && currentTime - previousTime <= timeoutTime) {  // Duy trì kết nối
      currentTime = millis();
      if (client.available()) {             
        char c = client.read();             // Đọc một byte dữ liệu
        Serial.write(c);                    // In ra Serial Monitor
        header += c;
        if (c == '\n') {                    

          if (currentLine.length() == 0) {
            
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
          
            // Tạo giao diện web
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS cho on/off buttons 
            // CSS cho div và page
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;background-color: #fbf8f2}");

            client.println(" div {display: block; background-color: #F5F5DC; ; margin: auto;border: 2px solid;padding: 20px; width: 300px;resize: horizontal;overflow: auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println("span {display: block; background-color: #F5F5DC; ; margin: auto;border: 0.1px solid;padding: 20px; width: 300px;resize: horizontal;overflow: auto; text-align: center;}");
            client.println(".button2 {background-color: #530000;}</style></head>");
            
            // Tiêu đề trang web
            client.println("<body><h1>Demo ESP8266 by do hieu</h1>");
            client.println("<div>");
               
            
            client.println("</div>");
          client.println("</body></html>");
            
            
            client.println();
            
            break;
          } else { 
            currentLine = "";
          }
        } else if (c != '\r') {  // nếu bạn có bất cứ thứ gì khác ngoài ký tự trả về đầu dòng,
          currentLine += c;      // thêm nó vào cuối currentLine
        }
      }
    }
    
    header = "";
    // Đóng kết nối
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}



// void loop() {
//   // // display.clearDisplay();
//   // // display.setTextSize(2);
//   // // display.setTextColor(SH110X_WHITE);
//   // // display.setCursor(10, 10);
//   // // display.println("Hello SH1106!"); 
//    WiFiClient client = server.available();   // Lắng nghe kết nối từ client


//   if (client) {                             // Lắng nghe kết nối client mới
//     currentTime = millis();
//     previousTime = currentTime;
//     Serial.println("New Client.");          
//     String currentLine = "";                
//     while (client.connected() && currentTime - previousTime <= timeoutTime) {  // Duy trì kết nối
//       currentTime = millis();
//       if (client.available()) {             
//         char c = client.read();             // Đọc một byte dữ liệu
//         Serial.write(c);                    // In ra Serial Monitor
//         header += c;
//         if (c == '\n') {                    

//           if (currentLine.length() == 0) {
            
//             client.println("HTTP/1.1 200 OK");
//             client.println("Content-type:text/html");
//             client.println("Connection: close");
//             client.println();
            
          
//             // Tạo giao diện web
//             client.println("<!DOCTYPE html><html>");
//             client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
//             client.println("<link rel=\"icon\" href=\"data:,\">");
//             // CSS cho on/off buttons 
//             // CSS cho div và page
//             client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;background-color: #fbf8f2}");

//             client.println(" div {display: block; background-color: #F5F5DC; ; margin: auto;border: 2px solid;padding: 20px; width: 300px;resize: horizontal;overflow: auto; text-align: center;}");
//             client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
//             client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
//             client.println("span {display: block; background-color: #F5F5DC; ; margin: auto;border: 0.1px solid;padding: 20px; width: 300px;resize: horizontal;overflow: auto; text-align: center;}");
//             client.println(".button2 {background-color: #530000;}</style></head>");
            
//             // Tiêu đề trang web
//             client.println("<body><h1>Demo ESP8266 by do hieu</h1>");
//             client.println("<div>");
               
            
//             client.println("</div>");
//           client.println("</body></html>");
            
            
//             client.println();
            
//             break;
//           } else { 
//             currentLine = "";
//           }
//         } else if (c != '\r') {  // nếu bạn có bất cứ thứ gì khác ngoài ký tự trả về đầu dòng,
//           currentLine += c;      // thêm nó vào cuối currentLine
//         }
//       }
//     }
    
//     header = "";
//     // Đóng kết nối
//     client.stop();
//     Serial.println("Client disconnected.");
//     Serial.println("");
//   }
// }
