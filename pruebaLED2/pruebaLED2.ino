#include <QMC5883LCompass.h>

QMC5883LCompass compass;

const int ledPin = 7; // Pin al que está conectado el LED integrado
const int threshold = 2000; // Umbral arbitrario
const int calibrationThreshold = 25; // Umbral de calibración para eliminar ruido

int xOffset = 0;
int yOffset = 0;
int zOffset = 0;
bool isCalibrated = false;

void setup() {
  // Inicializar el pin del LED como salida
  pinMode(ledPin, OUTPUT);
  // Inicializar la comunicación serie
  Serial.begin(9600);
  compass.init();
}

void loop() {
  int x, y, z;
  
  // Leer los valores de la brújula
  compass.read();

  // Obtener las lecturas XYZ
  x = compass.getX();
  y = compass.getY();
  z = compass.getZ();

  // Calibrar si no está calibrado
  if (!isCalibrated) {
    xOffset = x;
    yOffset = y;
    zOffset = z;
    isCalibrated = true;
  }

  // Ajustar las lecturas con los offsets
  x -= xOffset;
  y -= yOffset;
  z -= zOffset;

  // Aplicar el umbral de calibración para eliminar ruido
  if (abs(x) < calibrationThreshold) x = 0;
  if (abs(y) < calibrationThreshold) y = 0;
  if (abs(z) < calibrationThreshold) z = 0;

  Serial.print("X: ");
  Serial.print(x);
  Serial.print(" Y: ");
  Serial.print(y);
  Serial.print(" Z: ");
  Serial.print(z);
  Serial.println();
  
  // Si el valor absoluto de z es mayor que el umbral, encender el LED
  if (abs(z) > calibrationThreshold) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }

  delay(250);
}
