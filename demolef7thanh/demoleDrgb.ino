#include <ESP8266WiFi.h>

// Thông tin WiFi
const char* ssid = "P502_vtv";         // 🔹 Nhập tên WiFi
const char* password = "123456789"; // 🔹 Nhập mật khẩu WiFi

// Tạo server web trên cổng 80
WiFiServer server(80);

// Chân điều khiển LED RGB
const int pinRed = 12;   // Chân R (Đỏ)
const int pinGreen = 13; // Chân G (Xanh lá)
const int pinBlue = 14;  // Chân B (Xanh dương)

// Trạng thái màu
String redState = "OFF";
String greenState = "OFF";
String blueState = "OFF";

void setup() {
    Serial.begin(115200);
    
    // Cấu hình các chân RGB là OUTPUT
    pinMode(pinRed, OUTPUT);
    pinMode(pinGreen, OUTPUT);
    pinMode(pinBlue, OUTPUT);

    // Tắt LED ban đầu (Common Anode: HIGH = Tắt)
    digitalWrite(pinRed, HIGH);
    digitalWrite(pinGreen, HIGH);
    digitalWrite(pinBlue, HIGH);

    // Kết nối WiFi
    Serial.print("Đang kết nối WiFi...");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    // Kết nối thành công
    Serial.println("\n✅ WiFi đã kết nối!");
    Serial.print("📡 Địa chỉ IP: ");
    Serial.println(WiFi.localIP()); // Lấy địa chỉ IP

    server.begin(); // Khởi động server
}

void loop() {
    WiFiClient client = server.available(); // Chờ kết nối từ client

    if (client) { 
        Serial.println("🔗 Client kết nối!");
        String header = "";
        String currentLine = "";

        while (client.connected()) {  
            if (client.available()) {  
                char c = client.read();
                header += c;
                if (c == '\n') { 
                    if (currentLine.length() == 0) {  
                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-type:text/html");
                        client.println("Connection: close");
                        client.println();

                        // Xử lý điều khiển LED
                        if (header.indexOf("GET /red/on") >= 0) {
                            Serial.println("🔴 Red ON");
                            redState = "ON";
                            digitalWrite(pinRed, LOW);
                        } else if (header.indexOf("GET /red/off") >= 0) {
                            Serial.println("🔴 Red OFF");
                            redState = "OFF";
                            digitalWrite(pinRed, HIGH);
                        }
                        if (header.indexOf("GET /green/on") >= 0) {
                            Serial.println("🟢 Green ON");
                            greenState = "ON";
                            digitalWrite(pinGreen, LOW);
                        } else if (header.indexOf("GET /green/off") >= 0) {
                            Serial.println("🟢 Green OFF");
                            greenState = "OFF";
                            digitalWrite(pinGreen, HIGH);
                        }
                        if (header.indexOf("GET /blue/on") >= 0) {
                            Serial.println("🔵 Blue ON");
                            blueState = "ON";
                            digitalWrite(pinBlue, LOW);
                        } else if (header.indexOf("GET /blue/off") >= 0) {
                            Serial.println("🔵 Blue OFF");
                            blueState = "OFF";
                            digitalWrite(pinBlue, HIGH);
                        }

                        // Giao diện web
                        client.println("<!DOCTYPE html><html>");
                        client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
                        client.println("<link rel=\"icon\" href=\"data:,\">");
                        client.println("<style>");
                        client.println("html { font-family: Arial; text-align: center; }");
                        client.println(".button { padding: 15px 30px; font-size: 18px; margin: 10px; cursor: pointer; }");
                        client.println(".red { background-color: red; color: white; }");
                        client.println(".green { background-color: green; color: white; }");
                        client.println(".blue { background-color: blue; color: white; }");
                        client.println(".off { background-color: gray; color: white; }");
                        client.println("</style></head>");

                        client.println("<body>");
                        client.println("<h1>🚀 Điều khiển LED RGB</h1>");

                        // Nút điều khiển LED Red
                        client.println("<p>🔴 LED ĐỎ - Trạng thái: " + redState + "</p>");
                        if (redState == "OFF") {
                            client.println("<a href=\"/red/on\"><button class=\"button red\">BẬT</button></a>");
                        } else {
                            client.println("<a href=\"/red/off\"><button class=\"button off\">TẮT</button></a>");
                        }

                        // Nút điều khiển LED Green
                        client.println("<p>🟢 LED XANH LÁ - Trạng thái: " + greenState + "</p>");
                        if (greenState == "OFF") {
                            client.println("<a href=\"/green/on\"><button class=\"button green\">BẬT</button></a>");
                        } else {
                            client.println("<a href=\"/green/off\"><button class=\"button off\">TẮT</button></a>");
                        }

                        // Nút điều khiển LED Blue
                        client.println("<p>🔵 LED XANH DƯƠNG - Trạng thái: " + blueState + "</p>");
                        if (blueState == "OFF") {
                            client.println("<a href=\"/blue/on\"><button class=\"button blue\">BẬT</button></a>");
                        } else {
                            client.println("<a href=\"/blue/off\"><button class=\"button off\">TẮT</button></a>");
                        }

                        client.println("</body></html>");
                        client.println();
                        break;
                    } else {  
                        currentLine = "";
                    }
                } else if (c != '\r') { 
                    currentLine += c;
                }
            }
        }
        
        header = "";
        client.stop();
        Serial.println("❌ Client đã ngắt kết nối.");
    }
}
