#ifndef RC_CHANNEL_H
#define RC_CHANNEL_H

#include <Arduino.h>

class RCChannel
{
public:
    RCChannel(uint8_t pin);
    void begin();
    uint16_t getPulseWidth() const;
    bool isSignalValid() const;
    uint8_t getPin() const { return pin; }
    void handlePinChange();

    static RCChannel *externalInstances[2];
    static RCChannel *pcintInstances[8];
    static uint8_t pcintInstanceCount;

private:
    void setupExternalInterrupt();
    static void handleInterrupt0();
    static void handleInterrupt1();
    void setupPinChangeInterrupt();
    void handleInterrupt();
    
    bool isExternalInterruptPin() const { return pin == 2 || pin == 3; }
    bool isPinChangeInterruptPin() const { return pin <= 7; }

    uint8_t pin;
    volatile uint32_t pulseStart = 0;
    volatile uint16_t pulseWidth = 1500;
    volatile bool signalValid = false;
    volatile unsigned long lastPulseTime = 0;
};

#endif
