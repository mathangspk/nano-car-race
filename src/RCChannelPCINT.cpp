#include "RCChannelPCINT.h"

#define MAX_PCINT_CHANNELS 8

static RCChannelPCINT *pcintInstances[MAX_PCINT_CHANNELS];
static uint8_t pcintInstanceCount = 0;

RCChannelPCINT::RCChannelPCINT(uint8_t pin) : pin(pin) {}

void RCChannelPCINT::begin()
{
    pinMode(pin, INPUT);
    registerPCINTInstance(this);

    // Enable PCINT2 interrupt for D0–D7 (corresponds to PCINT16–23)
    PCICR |= (1 << PCIE2);      // Enable PCINT2 group
    PCMSK2 |= (1 << (pin - 0)); // Enable specific pin in PCINT2
}

uint16_t RCChannelPCINT::getPulseWidth() const
{
    return pulseWidth;
}

uint8_t RCChannelPCINT::getPin() const
{
    return pin;
}

void RCChannelPCINT::handlePinChange()
{
    if (digitalRead(pin) == HIGH)
    {
        pulseStart = micros();
    }
    else
    {
        pulseWidth = micros() - pulseStart;
    }
}

void registerPCINTInstance(RCChannelPCINT *instance)
{
    if (pcintInstanceCount < MAX_PCINT_CHANNELS)
    {
        pcintInstances[pcintInstanceCount++] = instance;
    }
}

void handlePCINT2_vect()
{
    static uint8_t lastPinState = 0;
    uint8_t currentPinState = PIND; // D0–D7 mapped to PIND
    uint8_t changedPins = currentPinState ^ lastPinState;
    lastPinState = currentPinState;

    for (uint8_t i = 0; i < pcintInstanceCount; ++i)
    {
        uint8_t pin = pcintInstances[i]->getPin();
        if (changedPins & (1 << pin))
        {
            pcintInstances[i]->handlePinChange();
        }
    }
}

// Global ISR
ISR(PCINT2_vect)
{
    handlePCINT2_vect();
}
