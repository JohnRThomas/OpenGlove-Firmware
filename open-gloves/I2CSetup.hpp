#include <Wire.h>


#define Is_I2C_Initialized false

int InitializeI2CInterface()
{
  #if (Is_I2C_Initialized == false)
    Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL);
    Serial.println("I2C interface Initialized on following pins: ");
    Serial.print("I2C SDA Pin: GPIO ");
    Serial.println(PIN_I2C_SDA);
    Serial.print("I2C SCL Pin: GPIO ");
    Serial.println(PIN_I2C_SCL);
    #define Is_I2C_Initialized true
  #else
    Serial.println("I2C interface already initialized");
  #endif
}
