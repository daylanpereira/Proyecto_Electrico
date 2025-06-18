// === RECEPTOR (ESCLAVO SPI) ===
#include <SPI.h>

volatile byte recibido = 0;
volatile byte respuesta = 0;
volatile bool nuevoDato = false;

const int ledVerdeReceptor = 3;
const int ledRojoReceptor = 4;

void imprimirBinario8(byte val) {
  for (int i = 7; i >= 0; i--) {
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

ISR(SPI_STC_vect) {
  byte entrante = SPDR;

  if (entrante != 0x00) {
    recibido = invertirOrdenBits(entrante);
    respuesta = recibido;
    SPDR = respuesta;
    nuevoDato = true;
  } else {
    SPDR = respuesta;
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(MISO, OUTPUT);
  SPCR |= _BV(SPE);
  SPI.attachInterrupt();

  pinMode(ledVerdeReceptor, OUTPUT);
  pinMode(ledRojoReceptor, OUTPUT);
  digitalWrite(ledVerdeReceptor, LOW);
  digitalWrite(ledRojoReceptor, LOW);

  Serial.println("Receptor SPI listo");
}

void loop() {
  if (nuevoDato) {
    digitalWrite(ledVerdeReceptor, HIGH);
    Serial.print("Receptor recibió: ");
    imprimirBinario8(recibido);
    Serial.println();

    Serial.print("Receptor respondió: ");
    imprimirBinario8(respuesta);
    Serial.println();

    delay(500);
    digitalWrite(ledVerdeReceptor, LOW);
    digitalWrite(ledRojoReceptor, HIGH);
    delay(500);
    digitalWrite(ledRojoReceptor, LOW);

    nuevoDato = false;
  }
}
