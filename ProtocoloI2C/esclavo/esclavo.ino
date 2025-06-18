#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
const int ledVerde = 3;
const int ledRojo = 4;

volatile byte datoRecibido = 0;
volatile bool recibido = false;

byte invertirBits(byte dato) {
  byte resultado = 0;
  for (int i = 0; i < 8; i++) {
    resultado <<= 1;
    resultado |= (dato & 1);
    dato >>= 1;
  }
  return resultado;
}

void imprimirBinarioLCD(byte dato) {
  char binario[9];
  for (int i = 7; i >= 0; i--) {
    binario[7 - i] = bitRead(dato, i) ? '1' : '0';
  }
  binario[8] = '\0';
  lcd.print(binario);
}

void recibirDato(int numBytes) {
  if (Wire.available()) {
    byte recibidoOriginal = Wire.read();
    datoRecibido = invertirBits(recibidoOriginal);
    recibido = true;
  }
}

void setup() {
  Serial.begin(9600);
  Wire.begin(0x08);
  Wire.onReceive(recibirDato);
  Wire.onRequest([]() {
    Wire.write(datoRecibido);
  });

  lcd.init();
  lcd.backlight();
  lcd.clear();
  pinMode(ledVerde, OUTPUT);
  pinMode(ledRojo, OUTPUT);
  digitalWrite(ledRojo, LOW);
  digitalWrite(ledVerde, LOW);
  lcd.setCursor(0, 0);
  lcd.print("Esclavo listo");
  Serial.println("Iniciado Arduino Esclavo");
}

void loop() {
  if (recibido) {
    Serial.print("Dato recibido invertido: ");
    Serial.println(datoRecibido, BIN);

    digitalWrite(ledRojo, LOW);
    digitalWrite(ledVerde, HIGH);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Dato:");
    lcd.setCursor(0, 1);
    imprimirBinarioLCD(datoRecibido);

    delay(200);
    digitalWrite(ledVerde, LOW);
    digitalWrite(ledRojo, HIGH);
    delay(200);
    digitalWrite(ledRojo, LOW);
    recibido = false;
  }
}

