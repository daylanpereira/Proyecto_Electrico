#include <SPI.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const byte dato = 0b00001100;
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Pines de LEDs
const int ledVerdeEmisor = 3;
const int ledRojoEmisor = 4;

void imprimirBinario8(byte val) {
  for (int i = 7; i >= 0; i--) {
    lcd.print(bitRead(val, i));
    Serial.print(bitRead(val, i));
  }
}

byte invertirOrdenBits(byte b) {
  byte rev = 0;
  for (int i = 0; i < 8; i++) {
    rev <<= 1;
    rev |= (b & 1);
    b >>= 1;
  }
  return rev;
}

void setup() {
  Serial.begin(9600);
  SPI.begin();
  pinMode(SS, OUTPUT);
  digitalWrite(SS, HIGH);

  pinMode(ledVerdeEmisor, OUTPUT);
  pinMode(ledRojoEmisor, OUTPUT);
  digitalWrite(ledVerdeEmisor, LOW);
  digitalWrite(ledRojoEmisor, LOW);

  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SPI Emisor listo");

  delay(2000);
  lcd.clear();
}

void loop() {
  Serial.println("=== Ciclo nuevo ===");

  // --- INICIO DE ENVÍO ---
  digitalWrite(ledRojoEmisor, LOW);
  digitalWrite(ledVerdeEmisor, HIGH);

  Serial.print("Emisor envió: ");
  lcd.setCursor(0, 0);
  lcd.print("Enviado: ");
  imprimirBinario8(dato);
  Serial.println();

  digitalWrite(SS, LOW);
  SPI.transfer(dato);
  digitalWrite(SS, HIGH);

  digitalWrite(ledVerdeEmisor, LOW);
  digitalWrite(ledRojoEmisor, HIGH);
  delay(500); // breve indicación visible
  digitalWrite(ledRojoEmisor, LOW); // apagado inmediato

  delay(500);

  // --- INICIO DE RECEPCIÓN ---
  digitalWrite(ledVerdeEmisor, HIGH);

  digitalWrite(SS, LOW);
  byte recibido = SPI.transfer(0x00);
  digitalWrite(SS, HIGH);

  digitalWrite(ledVerdeEmisor, LOW);
  digitalWrite(ledRojoEmisor, HIGH);
  delay(500);
  digitalWrite(ledRojoEmisor, LOW);

  byte invertido = invertirOrdenBits(recibido);
  Serial.print("Emisor recibió: ");
  lcd.setCursor(0, 1);
  lcd.print("Recibido: ");
  imprimirBinario8(invertido);
  Serial.println();

  delay(10000);
  lcd.clear();
}


