#include <WiFi.h>
#include <WebServer.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <PubSubClient.h>
const char* mqttServer = "10.0.110.115";
const int mqttPort = 1883; 


 #define oled 32
 Adafruit_SSD1306 display = Adafruit_SSD1306(128, oled, &Wire);

 

IPAddress apIP(192, 168, 0, 1);
const char* ssid = "ESP32-Red-Unida";
const char* password = "password";
WebServer server(80);
int sensorPin = 22;

void setup() {
  Serial.begin(9600);
   display.begin(SSD1306_SWITCHCAPVCC, 0x3C); 
   display.display();
   display.clearDisplay();
   display.display();
   display.setTextSize(1);
   display.setTextColor(WHITE,BLACK);
  Serial.begin(115200);
  delay(10);

  Serial.println();
  Serial.print("[WiFi] Configurando punto de acceso: ");
  display.clearDisplay();
  display.display();
  display.clearDisplay();
  Serial.println(ssid);
  display.setCursor(0, 0);
  display.print("AP: ");
  display.println(ssid);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0)); 
  WiFi.softAP(ssid, password);

  Serial.println("[WiFi] Punto de acceso configurado");
  Serial.print("[WiFi] Direccion IP del punto de acceso: ");
  display.print("IP: ");
  display.println(apIP);
  Serial.println(apIP);
  display.display();

 server.on("/", HTTP_GET, [](){
  String html = "<!DOCTYPE html><html><head>";
  html += "<meta charset='UTF-8'>";
  html += "<style>";
  html += "body {";
  html += "  font-family: Arial, sans-serif;";
  html += "  background-color: #f4f4f4;";
  html += "}";
  html += "form {";
  html += "  max-width: 300px;";
  html += "  margin: 0 auto;";
  html += "  padding: 20px;";
  html += "  background-color: #fff;";
  html += "  box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);";
  html += "}";
  html += "input[type='text'], input[type='password'], input[type='submit'] {";
  html += "  width: 100%;";
  html += "  padding: 12px;";
  html += "  margin: 8px 0;";
  html += "  display: inline-block;";
  html += "  border: 1px solid #ccc;";
  html += "  border-radius: 4px;";
  html += "  box-sizing: border-box;";
  html += "}";
  html += "input[type='submit'] {";
  html += "  background-color: #4CAF50;";
  html += "  color: white;";
  html += "  border: none;";
  html += "  cursor: pointer;";
  html += "}";
  html += "input[type='submit']:hover {";
  html += "  background-color: #45a049;";
  html += "}";
  html += "</style>";
  html += "</head><body>";
  html += "<h2 style='text-align:center'>Punto de acceso WiFi</h2>";
  html += "<form action='/submit' method='post'>";
  html += "  <label for='ssid'>SSID a conectar:</label><br>";
  html += "  <input type='text' id='ssid' name='ssid'><br>";
  html += "  <label for='password'>Contraseña:</label><br>";
  html += "  <input type='password' id='password' name='password'><br><br>";
  html += "  <input type='submit' value='Conectar a red WiFi'>";
  html += "<input type='button' value='Probar buzzer' onclick='executeFunction()'>";
  html += "<script>";
  html += "function executeFunction() {";
  html += "  var xhttp = new XMLHttpRequest();";
  html += "  xhttp.open('GET', '/execute-function', true);";
  html += "  xhttp.send();";
  html += "}";
  html += "</script>";
  html += "</form>";
  html += "</body></html>";
  server.send(200, "text/html", html);
});

server.on("/execute-function", HTTP_GET, [](){
  const int buzzerPin = 13;
  tone(buzzerPin, 1000, 1000);  
  delay(1000);  
  noTone(buzzerPin);  
  Serial.println("Function executed");
  server.send(200, "text/plain", "Function executed");
});


  server.on("/submit", HTTP_POST, [](){
  String newSSID = server.arg("ssid");
  String newPassword = server.arg("password");
  
  if (newSSID != "" && newPassword != "") {
    // Intentar conexión a la nueva red WiFi
    WiFi.begin(newSSID.c_str(), newPassword.c_str());
    unsigned long startTime = millis();  
    
    Serial.println("Intentando conectar a la red WiFi: " + newSSID);
    
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
      
      if ((millis() - startTime) > 10000) {  
        Serial.println("No se ha podido conectar a la red " + newSSID + " en 10 segundos.");
        break;
      }
    }
    
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println();
      Serial.print("Conectado a la red WiFi:   ");
      Serial.println(newSSID);
      Serial.print("Dirección IP asignada:          ");
      Serial.println(WiFi.localIP());
      display.clearDisplay();
      display.setCursor(0, 0);
      display.print("WiFi: ");
      display.println(newSSID);
      display.print("IP: ");
      display.println(WiFi.localIP());
      display.display();
      
      
    } else {
      Serial.println("Error al conectar a la red " + newSSID);
    }
  } else {
    Serial.println("Por favor ingrese un nombre de red y una contraseña válidos.");
  }
  server.send(200, "text/html", "<!DOCTYPE html><html><head><style>body{font-family: Arial, sans-serif;font-size: 16px;color: #333;background-color: #f4f4f4;padding: 10px;border: 1px solid #ccc;}</style></head><body><p>Proceso de conexión completo. Por favor, revisa la consola para más detalles.</p></body></html>");


});

  Serial.begin(115200); 
  pinMode(sensorPin, INPUT);
  server.begin();
}

void loop() {
  server.handleClient();
  int movimiento = digitalRead(sensorPin); 

  if (movimiento == HIGH) {
    Serial.println("Movimiento detectado");
    
  } else {
    Serial.println("Sin movimiento");
  }
  delay(500);
}






