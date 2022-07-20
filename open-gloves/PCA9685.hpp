#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm_board_0 = Adafruit_PWMServoDriver(PWM_Board_0_I2C_ADDRESS, Wire);

int Initialize_PCA9685_Board()
{
  pwm_board_0.begin();
  pwm_board_0.setOscillatorFrequency(25000000);
  pwm_board_0.setPWMFreq(PWM_Board_0_PWM_FREQUENCY);  // Analog servos usually run at ~50 Hz updates
      Serial.println("PCA9685 Board Initialized ");
}
