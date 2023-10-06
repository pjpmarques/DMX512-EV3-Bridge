#pragma once

#include <HardwareSerial.h>

#define DMX_MAX_CHANNEL                 512
#define DMX_BAUD                        250000
#define DMX_PROTOCOL                    SERIAL_8N2
#define DMX_START_DELAY                 88 // usec
#define DMX_TIMEOUT                     25 // msec
#define DMX_RX_PIN                      16
#define DMX_TX_PIN                      17
#define DMX_EN_PIN                      4
#define DMX_DEFAULT_UART                2

class DMX
{
public:
    DMX();
    DMX(int uart, int rx_pin, int tx_pin, int en_pin);

    int update();
    uint8_t read(int channel);

private:
    HardwareSerial uart;
    int rx_pin;
    int tx_pin;
    int en_pin;
    uint8_t channel[DMX_MAX_CHANNEL + 1] = {};
};
