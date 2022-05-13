#include <Arduino.h>
#include "PinDefinitionsAndMore.h"
#include <IRremote.hpp>
#include <Arduino_LSM9DS1.h>


uint16_t sAddress = 0x0;
uint8_t sCommand = 0xC;
uint8_t sRepeats = 2;

int _seconds = 0;
int _minutes = 0;

float x, y, z;
int degreesX = 0;
int degreesY = 0;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);

  Serial.println(F("START " __FILE__ " from " __DATE__ "\r\nUsing library version " VERSION_IRREMOTE));

  /*
     The IR library setup. That's all!
  */
  IrSender.begin(); // Start with IR_SEND_PIN as send pin and if NO_LED_FEEDBACK_CODE is NOT defined, enable feedback LED at default feedback LED pin

  Serial.print(F("Ready to send IR signals at pin "));
  Serial.println(IR_SEND_PIN);

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  Serial.print("Accelerometer sample rate = ");
  Serial.print(IMU.accelerationSampleRate());
  Serial.println("Hz");
}

void loop() {

  delay(1000);
  Serial.println(IR_SEND_PIN);
  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(x, y, z);
  }

  if (ft_check(x) && ft_check(y) && ft_check(z)) {
    _seconds++;

    if (_seconds >= 60) {
      _seconds = 0;
      _minutes++;
    }

    if (_minutes >= 15 ) {

      Serial.println(F("Send"));
      Serial.flush();

      IrSender.sendRC5(sAddress, sCommand, sRepeats);

      _minutes = 0;
      _seconds = 0;
    }

  }
  else
  {
    _minutes = 0;
    _seconds = 0;
  }
}

int ft_check(float v) {
  if (v <= 0.4 && v >= -0.4 )
  {
    return 1;
  }
  return 0;
}
