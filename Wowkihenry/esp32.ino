 
#include <WiFi.h>
#include <WebServer.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "PubSubClient.h"

// Configuración de la red WiFi
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// Servidor MQTT
const char* mqttServer = "broker.emqx.io";
const int mqttPort = 1883;
const char* mqttUser = "redunida";
const char* mqttPassword = "redunida";

// Cliente MQTT
WiFiClient espClient;
PubSubClient client(espClient);

// Pantalla OLED
#define OLED_PIN 32
Adafruit_SSD1306 display(128, 64, &Wire, OLED_PIN);

// Pin del sensor de movimiento, LED y buzzer
int sensorPin = 14;
int ledPin = 13;
int buzzerPin = 12;

void callback(char* topic, byte* payload, unsigned int length) {
  // Lógica para manejar los mensajes MQTT recibidos
  // Puedes agregar tu propia lógica aquí
}

void reconnect() {
  // Reconectar al servidor MQTT solo si no está conectado
  if (!client.connected()) {
    Serial.println("Conectando a MQTT...");
    if (client.connect("ESP32Client")) {
      Serial.println("Conectado a MQTT");
      client.subscribe("tu_topic"); // Reemplazar "tu_topic" por tu topico
    } else {
      Serial.print("Error al conectar a MQTT, rc=");
      Serial.print(client.state());
      Serial.println(" Intentando de nuevo en 5 segundos...");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(9600);
  Serial.println("Conectando a WiFi...");

  // Conexión a la red WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a WiFi...");
  }
  Serial.println("Conectado a WiFi");

  // Inicialización de la pantalla OLED
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Hola, MQTT!");
  display.display();

  // Configuración del cliente MQTT
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);

  // Configuración del pin del sensor de movimiento, LED y buzzer
  pinMode(sensorPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  if (!client.connected()) {
    Serial.println("Conexión perdida con el servidor MQTT. Intentando reconectar...");
    reconnect();
  }

  // Lectura del sensor de movimiento
  int movimiento = digitalRead(sensorPin);

  // Control del LED en función del estado del sensor
  if (movimiento == HIGH) {
    digitalWrite(ledPin, HIGH); // Encender el LED
    digitalWrite(buzzerPin, HIGH); // Encender el buzzer
  } else {
    digitalWrite(ledPin, LOW); // Apagar el LED
    digitalWrite(buzzerPin, LOW); // Apagar el buzzer
  }

  // Publicar mensaje MQTT dependiendo del estado del sensor
  if (movimiento == HIGH) {
    client.publish("tu_topic", "Movimiento detectado"); // Reemplazar "tu_topic" por tu topico
  } else {
    client.publish("tu_topic", "Sin movimiento"); // Reemplazar "tu_topic" por tu topico
  }

  // Actualizar la pantalla OLED
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Sensor: ");

  if (movimiento == HIGH) {
    display.println("ON");
    display.println("Hay movimiento");
  } else {
    display.println("OFF");
    display.println("No hay movimiento");
  }

  display.display();

  delay(500);
}
