
#include <Arduino.h>
#include <HardwareSerial.h>
#include "dmx.h"

DMX::DMX(int uart_number, int rx, int tx, int en) : uart(uart_number), rx_pin(rx), tx_pin(tx), en_pin(en)
{
}

DMX::DMX() : DMX(DMX_DEFAULT_UART, DMX_RX_PIN, DMX_TX_PIN, DMX_EN_PIN)
{
  uart.setRxBufferSize(DMX_MAX_CHANNEL + 1);
  pinMode(en_pin, OUTPUT);
  pinMode(rx_pin, INPUT);
}

int DMX::update()
{
  uart.begin(DMX_BAUD, SERIAL_8N2, rx_pin, tx_pin);
  uart.setTimeout(DMX_TIMEOUT);
  digitalWrite(en_pin, LOW);

  bool good_start = false;
  while (!good_start)
  {
    // Wait while RX is HIGH (idle)
    while (digitalRead(rx_pin) == HIGH)
      ;

    // We've detected RX became low, start counting time
    unsigned long t1 = micros();

    // Now the signal needs to be low for at least 88 usec
    while (true)
    {
      // If the signal becomes high before 88usec, it was a false start, if it continues low after that, it's a good start
      if (digitalRead(rx_pin) == HIGH)
      {
        break;
      }

      unsigned long t2 = micros();
      if (t2 - t1 >= DMX_START_DELAY)
      {
        good_start = true;
        break;
      }
    }
  }

  // Ok, we have a good start with RX still low. Let's continue to monitor until it becomes HIGH
  while (digitalRead(rx_pin) == LOW)
    ;

  // Ok, RX is now HIGH, we can activate the UART for the rest of the decoding
  uart.flush(false);

  // Read at most the 512+1 channels within the available timeout (at most 25msec)
  int channelsRead = uart.readBytes(channel, DMX_MAX_CHANNEL);
  uart.end();

  // Sanity check. Channel zero always needs to be zero.
  if (channel[0] != 0x00)
  {
    return -1;
  }

  // All ok, returns channels read (which it total bytes read minus start code)
  return channelsRead - 1;
}

uint8_t DMX::read(int channel_num)
{
  return channel[channel_num];
}
