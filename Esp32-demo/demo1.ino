// Load Wi-Fi library
#include <WiFi.h>

const char* ssid = "Esp111";
const char* password = "11223344";

// Set web server port 
WiFiServer server(80);

// HTTP request
String header;

// Biến để lưu trạng thái hiện tại của các chân đầu ra
String output26State = "off";
String output27State = "off";
String output25State = "off";
String output12State = "off";
String output13State = "off";
String output14State = "off";


// Gán các biến đầu ra vào các chân GPIO
const int output12 = 12;
const int output13 = 13;
const int output14 = 14;
const int output25 = 25;
const int output26 = 26;
const int output27 = 27;


// Set thời gian hiệnt tại và trước đó
unsigned long currentTime = millis();
unsigned long previousTime = 0; 
// Xác định thời gian chờ trong mili-giây ( 1300ms)
const long timeoutTime = 1300;


// SETup gpio
void setup() {
  Serial.begin(115200);
  
  pinMode(output12, OUTPUT);
  pinMode(output13, OUTPUT);
  pinMode(output14, OUTPUT);
  pinMode(output25, OUTPUT);
  pinMode(output26, OUTPUT);
  pinMode(output27, OUTPUT);

 // Đặt trạng thái ban đầu của các chân đầu ra là LOW
  digitalWrite(output12, LOW);
  digitalWrite(output13, LOW);
  digitalWrite(output14, LOW);
  digitalWrite(output25, LOW);
  digitalWrite(output26, LOW);
  digitalWrite(output27, LOW);
  
 
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
  server.begin();
}

void loop(){
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
            
            // Điều khiển bật/tắt các chân GPIO
            if (header.indexOf("GET /12/on") >= 0) {
              Serial.println("GPIO 12 on");
              output12State = "on";
              digitalWrite(output12, HIGH);
            } else if (header.indexOf("GET /12/off") >= 0) {
              Serial.println("GPIO 12 off");
              output12State = "off";
              digitalWrite(output12, LOW);
            }
            else if (header.indexOf("GET /13/on") >= 0) {
              Serial.println("GPIO 13 on");
              output13State = "on";
              digitalWrite(output13, HIGH);
            } else if (header.indexOf("GET /13/off") >= 0) {
              Serial.println("GPIO 13 off");
              output13State = "off";
              digitalWrite(output13, LOW);
            }
            else if (header.indexOf("GET /14/on") >= 0) {
              Serial.println("GPIO 14 on");
              output14State = "on";
              digitalWrite(output14, HIGH);
            } else if (header.indexOf("GET /14/off") >= 0) {
              Serial.println("GPIO 14 off");
              output14State = "off";
              digitalWrite(output14, LOW);
            }

            else if (header.indexOf("GET /25/on") >= 0) {
              Serial.println("GPIO 25 on");
              output25State = "on";
              digitalWrite(output25, HIGH);
            } else if (header.indexOf("GET /25/off") >= 0) {
              Serial.println("GPIO 25 off");
              output25State = "off";
              digitalWrite(output25, LOW);
            }else if (header.indexOf("GET /26/on") >= 0) {
              Serial.println("GPIO 26 on");
              output26State = "on";
              digitalWrite(output26, HIGH);
            } else if (header.indexOf("GET /26/off") >= 0) {
              Serial.println("GPIO 26 off");
              output26State = "off";
              digitalWrite(output26, LOW);
            }
             else if (header.indexOf("GET /27/on") >= 0) {
              Serial.println("GPIO 27 on");
              output27State = "on";
              digitalWrite(output27, HIGH);
            } else if (header.indexOf("GET /27/off") >= 0) {
              Serial.println("GPIO 27 off");
              output27State = "off";
              digitalWrite(output27, LOW);
            }
            
            
            
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
            client.println("<body><h1>Demo GR1 by do hieu</h1>");
            client.println("<div>");
               
            client.println("<p>GPIO 12 - State " + output12State + "</p>");
                  
            if (output12State=="off") {
              client.println("<p><a href=\"/12/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/12/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
              
            client.println("<p>GPIO 13 - State " + output13State + "</p>");
                   
            if (output13State=="off") {
              client.println("<p><a href=\"/13/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/13/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
             
            client.println("<p>GPIO 14 - State " + output14State + "</p>");
                   
            if (output14State=="off") {
              client.println("<p><a href=\"/14/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/14/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
            client.println("</div>");
            client.println("<div>");
              
                  
            if (output25State=="off") {
              client.println("<p><a href=\"/25/on\"><button class=\"button\">Red ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/25/off\"><button class=\"button button2\">Red OFF</button></a></p>");
            }
            
               
            if (output26State=="off") {
              client.println("<p><a href=\"/26/on\"><button class=\"button\">Yerllow ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/26/off\"><button class=\"button button2\">Yerllow OFF</button></a></p>");
            } 
               
            
             
            if (output27State=="off") {
              client.println("<p><a href=\"/27/on\"><button class=\"button\">Green ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/27/off\"><button class=\"button button2\">Green OFF</button></a></p>");
            }
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
