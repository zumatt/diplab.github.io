/* MIT License 2022 Matteo Subet <hi@zumat.ch>

   A simple example of how to use the I2C Grove Accellerometer with an ESP32

   28th December by Matteo Subet
*/

#include "LIS3DHTR.h"
#include <Wire.h>
LIS3DHTR<TwoWire> LIS; //IIC
#define WIRE Wire
 
void setup()
{
  Serial.begin(115200);
  LIS.begin(WIRE,0x19); //IIC init
  
  delay(100);
  
  LIS.setFullScaleRange(LIS3DHTR_RANGE_2G);
  LIS.setOutputDataRate(LIS3DHTR_DATARATE_50HZ);
}
void loop()
{
  if (!LIS)
  {
    Serial.println("LIS3DHTR didn't connect.");
    while (1)
      ;
    return;
  }
  //3 axis
    Serial.print("x:"); Serial.print(LIS.getAccelerationX()); Serial.print("  ");
    Serial.print("y:"); Serial.print(LIS.getAccelerationY()); Serial.print("  ");
    Serial.print("z:"); Serial.println(LIS.getAccelerationZ());

  delay(100);
}
