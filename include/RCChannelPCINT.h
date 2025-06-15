#ifndef RC_CHANNEL_PCINT_H
#define RC_CHANNEL_PCINT_H

#include <Arduino.h>

class RCChannelPCINT
{
public:
    RCChannelPCINT(uint8_t pin);
    void begin();
    void handlePinChange();
    uint16_t getPulseWidth() const;
    uint8_t getPin() const;

private:
    uint8_t pin;
    volatile uint32_t pulseStart = 0;
    volatile uint16_t pulseWidth = 0;
};

void registerPCINTInstance(RCChannelPCINT *instance);
void handlePCINT2_vect();

#endif
