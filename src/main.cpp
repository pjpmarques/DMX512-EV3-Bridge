
#include <Arduino.h>
#include <HardwareSerial.h>
#include <FastLED.h>
#include <dmx.h>

//---------------------------------

#define BAUD_RATE             115200        // Of the serial port
#define INTERNAL_LED_PIN      13            // For signaling what is being sent to the EV3
#define EV3_PIN               19            // This pin is connected to the analog pin of EV3 for signaling if the clock should be advanced
#define CLOCK_CHANNEL         1             // Channel that the DMX controller will use to control the clock -- This is PIN 13 on the ESP32 and DATA0 on the shield
#define THRESHOLD             50            // Threshold of the value received to either decide to send HIGH or LOW to the EV3 brick

#define RGB_LED_PIN           2
#define COLOR_ORDER           GRB
#define BRIGHTNESS            25
#define CHIPSET               WS2812

//---------------------------------

DMX dmx;
CRGB rgb_led[1];

void setup()
{
  Serial.begin(BAUD_RATE);
  pinMode(EV3_PIN, OUTPUT);
  pinMode(INTERNAL_LED_PIN, OUTPUT);
  digitalWrite(EV3_PIN, LOW);
  digitalWrite(INTERNAL_LED_PIN, LOW);

  delay(2000);

  FastLED.addLeds<WS2812, RGB_LED_PIN, GRB>(rgb_led, 1).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);

  Serial.printf("\n\n\n");
  Serial.printf("DMX512 to EV3 Bridge\n");
  Serial.print("=====================\n");
  Serial.printf("DMX Configured. Channel for clock is %d.\n", CLOCK_CHANNEL);
  Serial.printf("EV3 pin (%d) configured to LOW.\n\n", EV3_PIN);

  rgb_led[0] = CRGB::Red;
  FastLED.show();
  delay(100);
}

int current_dmx = 0;

void loop()
{
  int channels = 0;

  if ((channels = dmx.update()) > 0)
  {
    rgb_led[0] = CRGB::Green;
    FastLED.show();
    delay(100);
    rgb_led[0] = CRGB::Blue;
    FastLED.show();
    delay(100);
    rgb_led[0] = CRGB::Red;
    FastLED.show();

    Serial.printf("\n");
    Serial.printf("Read %d channels from DMX512\n", channels);
    int from_dmx = dmx.read(CLOCK_CHANNEL);
    Serial.printf("Value of CLOCK channel (%d) = %d\n", CLOCK_CHANNEL, from_dmx);

    if (from_dmx != current_dmx)
    {
      current_dmx = from_dmx;
      if (current_dmx < THRESHOLD)
      {
        digitalWrite(EV3_PIN, LOW);
        digitalWrite(INTERNAL_LED_PIN, LOW);
        Serial.printf("Read new data from DMX (%d) -- Sending to EV3: LOW\n", current_dmx);
      }
      else
      {
        digitalWrite(EV3_PIN, HIGH);
        digitalWrite(INTERNAL_LED_PIN, HIGH);
        Serial.printf("Read new data from DMX (%d) -- Sending to EV3: HIGH\n", current_dmx);
      }
    }

    Serial.printf("Current values of all channels:\n");
    for (int i = 0; i < channels; i++)
    {
      if (i % 32 == 0)
      {
        Serial.printf("\n(%3i)  ", i);
      }
      Serial.printf("%4d", dmx.read(i));
    }
  }
  else
  {
    Serial.printf("Couldn't get data from DMX512.\n");
  }

  Serial.printf("\n");
}
