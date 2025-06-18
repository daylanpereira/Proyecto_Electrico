// === Código para Arduino Maestro (Emisor) ===
#include <Wire.h>

const int ledVerde = 3;
const int ledRojo = 4;
const int pinPot = A0;
const byte direccionEsclavo = 0x08;
byte datoAEnviar = 0b00100101;
unsigned long tiempoAnterior = 0;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  pinMode(ledVerde, OUTPUT);
  pinMode(ledRojo, OUTPUT);
  pinMode(pinPot, INPUT);
  digitalWrite(ledRojo, LOW);
  digitalWrite(ledVerde, LOW);
  Serial.println("Iniciado el Arduino Maestro");
}

void loop() {
  int valorPot = analogRead(pinPot);
  int intervaloEnvio = map(valorPot, 0, 1023, 100, 5000);
  unsigned long tiempoActual = millis();

  if (tiempoActual - tiempoAnterior >= intervaloEnvio) {
    tiempoAnterior = tiempoActual;

    Serial.println("Enviando dato automaticamente");
    long velocidadI2C = map(valorPot, 0, 1023, 100000, 500000);
    Wire.setClock(velocidadI2C);

    Serial.print("Velocidad I2C configurada: ");
    Serial.println(velocidadI2C);
    Serial.print("Intervalo de envío: ");
    Serial.println(intervaloEnvio);

    digitalWrite(ledRojo, LOW);
    digitalWrite(ledVerde, HIGH);

    Wire.beginTransmission(direccionEsclavo);
    Wire.write(datoAEnviar);
    Wire.endTransmission();

    Serial.print("Dato enviado: ");
    Serial.println(datoAEnviar, BIN);

    delay(100);
    digitalWrite(ledVerde, LOW);
    digitalWrite(ledRojo, HIGH);

    Wire.requestFrom(direccionEsclavo, 1);
    if (Wire.available()) {
      byte recibido = Wire.read();
      Serial.print("Respuesta recibida: ");
      Serial.println(recibido, BIN);
    }

    delay(100);
    digitalWrite(ledRojo, LOW);
  }
}

