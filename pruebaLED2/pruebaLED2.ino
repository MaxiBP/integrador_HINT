// Incluímos la librería
#include <QMC5883LCompass.h>

QMC5883LCompass magnetometro;

// Constantes
const int ledPin = 7; // Pin al que está conectado el LED 
const int umbral = 25; // Umbral de calibración para eliminar ruido

// Variables
int xOffset = 0;
int yOffset = 0;
int zOffset = 0;
bool estaCalibrado = false;

void setup() {
  // Inicializamos el pin del LED como salida
  pinMode(ledPin, OUTPUT);
  // Inicializamos la comunicación serie
  Serial.begin(9600);
  // Inicializamos el magnetómetro
  magnetometro.init();
}

void loop() {
  int x, y, z;
  
  // Obtenemos las lecturas del magnetómetro
  magnetometro.read();

  // Guardamos las lecturas correspondientes a cada eje en su respectiva variable
  x = magnetometro.getX();
  y = magnetometro.getY();
  z = magnetometro.getZ();

  // Calibramos si no está calibrado
  if (!estaCalibrado) {
    xOffset = x;
    yOffset = y;
    zOffset = z;
    estaCalibrado = true;
  }

  // Ajustamos las lecturas con los offsets
  x -= xOffset;
  y -= yOffset;
  z -= zOffset;

  // Aplicamos el umbral de calibración para eliminar ruido
  x = (abs(x) < umbral) ? 0 : x;
  y = (abs(y) < umbral) ? 0 : y;
  z = (abs(z) < umbral) ? 0 : z;

  // Mostramos los valores de la lectura por el puerto serial
  Serial.print("X: ");
  Serial.print(x);
  Serial.print(" Y: ");
  Serial.print(y);
  Serial.print(" Z: ");
  Serial.print(z);
  Serial.println();
  
  // Si el valor absoluto de z es mayor que el umbral, encendemos el LED
  if (abs(x) > umbral || abs(y) > umbral || abs(z) > umbral) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }

  delay(250);
}
