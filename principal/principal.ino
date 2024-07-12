
// Incluímos la librería
#include <QMC5883LCompass.h>


QMC5883LCompass magnetometro;


// Constantes
const int LEDPIN = 7; // Pin al que está conectado el LED
const int UMBRALPISO = 25; // UMBRAL a superar por las lecturas
const int UMBRALTECHO = 30; // UMBRAL que no deben superar las lecturas
const int BUZZERPIN = 12; // Pin al que está conectado el buzzer
const int BAUD = 9600; // bits por segundo para la comunicación serial


// Variables
int xOffset = 0;
int yOffset = 0;
int zOffset = 0;
bool estaCalibrado = false;


// Funciones
void controlarSalida(int x, int y, int z);


void setup() {
  // Inicializamos el pin del LED como salida
  pinMode(LEDPIN, OUTPUT);
  // Inicializamos el pin del buzzer como salida
  pinMode(BUZZERPIN, OUTPUT);
  // Inicializamos la comunicación serie
  Serial.begin(BAUD);
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


  // Aplicamos el UMBRAL de calibración para eliminar ruido
  x = (abs(x) < UMBRALPISO) ? 0 : x;
  y = (abs(y) < UMBRALPISO) ? 0 : y;
  z = (abs(z) < UMBRALPISO) ? 0 : z;


  // Mostramos los valores de la lectura por el puerto serial
  Serial.print("X: ");
  Serial.print(x);
  Serial.print(" Y: ");
  Serial.print(y);
  Serial.print(" Z: ");
  Serial.print(z);
  Serial.println();


  controlarSalida(x, y, z);


  // Vuelve a calibrar si se mueve
  if(abs(x) > UMBRALTECHO || abs(y) > UMBRALTECHO || abs(z) > UMBRALTECHO){
   estaCalibrado = false;    
  }


  delay(250);
}


// Definir funciones
void controlarSalida(int x, int y, int z){
  // Si el valor absoluto de z es mayor que el UMBRAL, encendemos el LED
  if (((abs(x) > UMBRALPISO) && (abs(x) < UMBRALTECHO)) || ((abs(y) > UMBRALPISO) && (abs(y) < UMBRALTECHO)) || ((abs(z) > UMBRALPISO) && (abs(z) < UMBRALTECHO))) {
    digitalWrite(LEDPIN, HIGH);
    digitalWrite(BUZZERPIN, HIGH);
  } else {
    digitalWrite(LEDPIN, LOW);
    digitalWrite(BUZZERPIN, LOW);
  }
}
