int sensorPin = 22;


void setup() {
  Serial.begin(115200);
  pinMode(sensorPin, INPUT);
}


void loop() {
  int movimiento = digitalRead(sensorPin);


  if (movimiento == HIGH) {
    Serial.println("Movimiento detectado");
  } else {
    Serial.println("Sin movimiento");
  }
  delay(500);
}