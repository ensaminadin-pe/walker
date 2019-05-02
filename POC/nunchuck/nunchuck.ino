
/*
 * Arduino Nano pins :
 * Wii nunchuck connector pins are listed top to bottom with the "U" shape of the connector facing up
 * Like so : https://cdn.instructables.com/F0N/RJQ3/IH90O28H/F0NRJQ3IH90O28H.LARGE.jpg
 * SDA = A4
 * SCL = A5
 * 
 * Nano       Nunchuck
 * A4(SDA)    Top left (SDA)
 * A5(SCL)    Bottom right (SCL)
 * 3.3V       Bottom left
 * GND        Top right
 */
 
#include <Wire.h>
#include "nunchuck.h"

void setup()
{
  Serial.begin(9600);
  Serial.print("B1\n");
  Wire.begin();
  Serial.print("B2\n");
  Wire.setClock(400000);
  Serial.print("B3\n");
  nunchuk_init();
  Serial.print("B4\n");
}

void loop()
{
  if (nunchuk_read()) {
      Serial.print(nunchuk_joystickX(), DEC);
      Serial.print(",");
      Serial.print(nunchuk_joystickY(), DEC);
      Serial.print(",");
      Serial.print(nunchuk_accelX(), DEC);
      Serial.print(",");
      Serial.print(nunchuk_accelY(), DEC);
      Serial.print(",");
      Serial.print(nunchuk_buttonZ(), DEC);
      Serial.print(",");
      Serial.print(nunchuk_buttonC(), DEC);
      Serial.print("\n");
  }
  delay(10);
}
