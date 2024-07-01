Conexionado
 * GY-271 --------------- Arduino Nano V3.0
 * SDA ------------------ A4
 * SCL ------------------ A5
 * GND ------------------ GND
 * VCC ------------------ 3V3
 * 
 * Valores extremos ejes en mi unidad
 * X -338 +423
 * Y -689 +353
 * Z -485 +459
 * Orientación: tal cual lo tengo montado en la protoboard, es decir,
 * con los circuitos mirando hacia arriba, la X sería el YAW,
 * la Y el cabeceo, y la Z el alabeo.
 */

#include <Wire.h>

/* Dirección I2C del módulo */
#define HMC5803L_Address 0x1E
 
/* Dirección de registro para los datos X, Y y Z*/
#define X 3
#define Y 7
#define Z 5
/* Valores mínimos y máximos de cada variable*/
int xmin, xmax, ymin, ymax, zmin, zmax;
/* Salidas digitales del altavoz y el diodo led/*
int speaker=9;
int led = 6;

/* Umbral a partir del cual queremos que se dispare el detector /*
int umbral=600;

void setup() 
{
  Serial.begin(9600); 
  Wire.begin();

  pinMode(led, OUTPUT);
  pinMode(speaker, OUTPUT);
    
  /* Inicializa el módulo */ 
  Init_HMC5803L();
}
 
void loop() 
{

  digitalWrite(led, LOW);   // Apaga el LED
  digitalWrite(speaker, LOW);    // Silencia el altavoz

  /* Lee el dato de cada eje del sensor y muestra su valor por el puerto serie */
  int xvalue = HMC5803L_Read(X);
  int yvalue = HMC5803L_Read(Y);
  int zvalue = HMC5803L_Read(Z);

  if (xvalue<xmin) xmin=xvalue;
  if (xvalue>xmax) xmax=xvalue;
  if (yvalue<ymin) ymin=yvalue;
  if (yvalue>ymax) ymax=yvalue;
  if (zvalue<zmin) zmin=zvalue;
  if (zvalue>zmax) zmax=zvalue;
  
  Serial.print(zvalue);
  Serial.print(" ");
  //Serial.print(yvalue);
  //Serial.print(" ");
  //Serial.print(zvalue);
  //Serial.print(" ");
  
 delay(10);

  if (abs(zvalue)>umbral) {
  digitalWrite(led, HIGH);   // Enciende el diodo LED
  digitalWrite(speaker, HIGH);    // Activa el altavoz
  delay(200);
  
  }
  
  Serial.print(zmin);
  Serial.print(" ");
  Serial.println(zmax);

} 
 
/* This function will initialise the module and only needs to be run once after the module is first powered up or reset */
void Init_HMC5803L(void)
{
  /* Set the module to 8x averaging and 15Hz measurement rate */
  Wire.beginTransmission(HMC5803L_Address);
  Wire.write(0x00);
  Wire.write(0x70);
          
  /* Set a gain of 5 */
  Wire.write(0x01);
  Wire.write(0xA0);
  Wire.endTransmission();
}
 
/* This function will read once from one of the 3 axis data registers
and return the 16 bit signed result. */
int HMC5803L_Read(byte Axis)
{
  int Result;
  
  /* Initiate a single measurement */
  Wire.beginTransmission(HMC5803L_Address);
  Wire.write(0x02);
  Wire.write(0x01);
  Wire.endTransmission();
  delay(6);
  
  /* Move modules the resiger pointer to one of the axis data registers */
  Wire.beginTransmission(HMC5803L_Address);
  Wire.write(Axis);
  Wire.endTransmission();
   
  /* Read the data from registers (there are two 8 bit registers for each axis) */  
  Wire.requestFrom(HMC5803L_Address, 2);
  Result = Wire.read() << 8;
  Result |= Wire.read();
 
  return Result;
}

