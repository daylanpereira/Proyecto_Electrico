// -------------------- UART Receptor --------------------
#define RX 10
#define TX 11
#define UMBRAL 25.0

#include <SoftwareSerial.h>
SoftwareSerial mySerial(RX, TX); // RX ← Emisor, TX → Emisor

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  Serial.println("Receptor listo");
}

void loop() {
  if (mySerial.available()) {
    String data = mySerial.readStringUntil('\n');
    data.trim();
    Serial.print("Temperatura recibida: ");
    Serial.println(data);

    float temp = data.toFloat();

    if (temp == 0.0 && data != "0" && data != "0.0") {
      Serial.println("Dato inválido");
      return;
    }

    String respuesta = (temp > UMBRAL) ? "ALERTA" : "OK";
    mySerial.println(respuesta);
    Serial.print("-> Enviando: ");
    Serial.println(respuesta);
  }
}
