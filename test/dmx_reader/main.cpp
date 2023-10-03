
#include <Arduino.h>
#include <HardwareSerial.h>
#include "dmx.h"

DMX dmx;

void setup() {  
  Serial.begin(115200);

  pinMode(16, INPUT);
  pinMode(4, OUTPUT);
  digitalWrite(4, LOW);

  Serial.printf("\n\n\n\n\n");
  Serial.printf("Let's start reading this:\n\n"); 
}

void loop()
{
  int channelsRead = dmx.update();
  if (channelsRead == -1) {
    Serial.printf("Problem reading DMX!\n");
  } else {
    Serial.printf("Read %d DMX channels!\n", channelsRead);
    for (int i=0; i<channelsRead; i++) {
      if (i%32 == 0) {
        Serial.printf("\n(%3i)  ", i);
      }
      Serial.printf("%4d", dmx.read(i));
    }
  }

  Serial.printf("\n");
  delay(2000);
}
