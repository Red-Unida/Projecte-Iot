// Incluyendo las librerías necesarias
#include <WiFi.h>
#include <WebServer.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <PubSubClient.h>

// Definiendo la dirección IP y el puerto del servidor MQTT
const char* mqttServer = "10.0.110.115";
const int mqttPort = 1883;

// Definiendo el pin al que está conectada la pantalla OLED
#define oled 32
Adafruit_SSD1306 display = Adafruit_SSD1306(128, oled, &Wire);

// Definiendo la dirección IP y las credenciales del punto de acceso WiFi
IPAddress apIP(192, 168, 0, 1);
const char* ssid = "ESP32-Red-Unida";
const char* password = "password";

// Creando un objeto WebServer para manejar las solicitudes HTTP en el puerto 80
WebServer server(80);

// Definiendo el pin al que está conectado el sensor de movimiento
int sensorPin = 22;

// Configuración inicial
void setup() {
  // Iniciando la comunicación serial a 9600 baudios
  Serial.begin(9600);

  // Iniciando la pantalla OLED
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.display();
  display.clearDisplay();
  display.display();
  display.setTextSize(1);
  display.setTextColor(WHITE,BLACK);

  // Iniciando la comunicación serial a 115200 baudios
  Serial.begin(115200);
  delay(10);

  // Configurando el punto de acceso WiFi
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(ssid, password);

  // Mostrando información sobre el punto de acceso WiFi en la pantalla OLED y la consola
  Serial.println("[WiFi] Configurando punto de acceso: ");
  display.clearDisplay();
  display.display();
  display.clearDisplay();
  Serial.println(ssid);
  display.setCursor(0, 0);
  display.print("AP: ");
  display.println(ssid);
  Serial.println("[WiFi] Punto de acceso configurado");
  Serial.print("[WiFi] Direccion IP del punto de acceso: ");
  display.print("IP: ");
  display.println(apIP);
  Serial.println(apIP);
  display.display();

  // Configurando las rutas del servidor web
  server.on("/", HTTP_GET, [](){
    // Creando y enviando la página HTML para la configuración de la red WiFi
    String html = "<!DOCTYPE html><html><head>";
    // Código HTML para la página
    // ...
    server.send(200, "text/html", html);
  });

  // Configurando una ruta para ejecutar una función específica
  server.on("/execute-function", HTTP_GET, [](){
    // Ejecutando una función y enviando una respuesta al cliente
    // ...
    server.send(200, "text/plain", "Function executed");
  });

  // Configurando una ruta para manejar el formulario de conexión a la red WiFi
  server.on("/submit", HTTP_POST, [](){
    // Manejando el formulario de conexión a la red WiFi y enviando una respuesta al cliente
    // ...
    server.send(200, "text/html", "<!DOCTYPE html><html><head><style>body{font-family: Arial, sans-serif;font-size: 16px;color: #333;background-color: #f4f4f4;padding: 10px;border: 1px solid #ccc;}</style></head><body><p>Proceso de conexión completo. Por favor, revisa la consola para más detalles.</p></body></html>");
  });

  // Iniciando la comunicación serial a 115200 baudios nuevamente
  Serial.begin(115200);
  
  // Configurando el pin del sensor de movimiento como entrada
  pinMode(sensorPin, INPUT);
  
  // Iniciando el servidor web
  server.begin();
}

// Loop principal
void loop() {
  // Manejando las solicitudes entrantes del cliente
  server.handleClient();

  // Leyendo el estado del sensor de movimiento
  int movimiento = digitalRead(sensorPin);

  // Mostrando el estado del sensor en la consola
  if (movimiento == HIGH) {
    Serial.println("Movimiento detectado");
  } else {
    Serial.println("Sin movimiento");
  }

  // Esperando medio segundo antes de volver a realizar el ciclo
  delay(500);
}
