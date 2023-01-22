#include <Wire.h>


int InitializeI2CInterface()
{
      Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL);
    /*Serial.println("I2C interface Initialized on following pins: ");
    Serial.print("I2C SDA Pin: GPIO ");
    Serial.println(PIN_I2C_SDA);
    Serial.print("I2C SCL Pin: GPIO ");
    Serial.println(PIN_I2C_SCL);
    */
      Serial.println("I2C Initialized");
  }
