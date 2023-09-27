
#include <Arduino.h>
#include <SparkFunDMX.h>

//---------------------------------

#define BAUD_RATE           115200      // Of the serial port
#define INTERNAL_LED_PIN    13          // For signaling what is being sent to the EV3
#define EV3_PIN             27          // This pin is connected to the analog pin of EV3 for signaling if the clock should be advanced
#define CLOCK_CHANNEL       127         // Channel that the DMX controller will use to control the clock
#define TOTAL_CHANNELS      512         // All possible channels of the DMX controller
#define THRESHOLD           127         // Threshold of the value received to either decide to send HIGH or LOW to the EV3 brick

//---------------------------------

SparkFunDMX dmx;

//---------------------------------

void setup() {
  Serial.begin(BAUD_RATE);
  pinMode(EV3_PIN, OUTPUT);
  pinMode(INTERNAL_LED_PIN, OUTPUT);
  digitalWrite(EV3_PIN, LOW);
  digitalWrite(INTERNAL_LED_PIN, LOW);
  dmx.initRead(TOTAL_CHANNELS);

  Serial.printf("DMX Configured. Channel for clock is %d.\n", CLOCK_CHANNEL);
  Serial.printf("EV3 pin (%d) configured to LOW.\n", EV3_PIN);
}

//---------------------------------

int current_dmx = 0;

void loop() {
  dmx.update();

  int from_dmx = dmx.read(CLOCK_CHANNEL);
  if (from_dmx != current_dmx) {
      current_dmx = from_dmx;

      if (current_dmx < THRESHOLD) {
        digitalWrite(EV3_PIN, LOW);
        digitalWrite(INTERNAL_LED_PIN, LOW);
        Serial.printf("Read new data from DMX (%d) -- Sending to EV3: LOW\n", current_dmx);
      } else {
        digitalWrite(EV3_PIN, HIGH);
        digitalWrite(INTERNAL_LED_PIN, HIGH);
        Serial.printf("Read new data from DMX (%d) -- Sending to EV3: HIGH\n", current_dmx);
      }
  }
}
