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

    // Static members for interrupt handling
    static RCChannel *externalInstances[3];
    static RCChannel *pcintInstances[8];
    static uint8_t pcintInstanceCount;

private:
    // External Interrupt methods
    void setupExternalInterrupt();
    static void handleInterrupt0();
    static void handleInterrupt1();
    static void handleInterrupt2();
    
    // Pin Change Interrupt methods
    void setupPinChangeInterrupt();
    
    // Common interrupt handler
    void handleInterrupt();
    
    // Helper methods
    bool isExternalInterruptPin() const;
    bool isPinChangeInterruptPin() const;
    void registerPinChangeInstance();

    uint8_t pin;
    volatile uint32_t pulseStart = 0;
    volatile uint16_t pulseWidth = 1500; // giá trị mặc định trung lập
    volatile bool signalValid = false;
};

// Global Pin Change Interrupt handler
void handlePCINT2_vect();

#endif
