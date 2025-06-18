// -------------------- Librerías --------------------
#include <DHT.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <SoftwareSerial.h>

// -------------------- Definiciones --------------------
#define DHTPIN 2
#define DHTTYPE DHT11
#define BUZZER 3
#define RX 10
#define TX 11

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);
SoftwareSerial mySerial(RX, TX); // RX ← Receptor, TX → Receptor

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);

  dht.begin();
  lcd.init();
  lcd.backlight();

  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, LOW);

  Serial.println("Emisor listo");
}

void loop() {
  float temp = dht.readTemperature();

  if (isnan(temp)) {
    Serial.println("Error al leer el sensor DHT");
    return;
  }

  Serial.print("Temperatura medida: ");
  Serial.println(temp);
  mySerial.println(temp);

  // Esperar respuesta (1s máx)
  unsigned long startTime = millis();
  while (!mySerial.available() && millis() - startTime < 1000) {}

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temp);
  lcd.print(" C");

  lcd.setCursor(0, 1);
  if (mySerial.available()) {
    String respuesta = mySerial.readStringUntil('\n');
    respuesta.trim();
    Serial.print("Respuesta: ");
    Serial.println(respuesta);

    if (respuesta == "ALERTA") {
      lcd.print("Estado: ALERTA");
      for (int i = 0; i < 3; i++) {
        digitalWrite(BUZZER, HIGH);
        delay(200);
        digitalWrite(BUZZER, LOW);
        delay(200);
      }
    } else {
      lcd.print("Estado: OK");
      digitalWrite(BUZZER, LOW);
    }
  } else {
    Serial.println("No se recibió respuesta");
    lcd.print("Sin respuesta");
  }

  delay(2000);
}

