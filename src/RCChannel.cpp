#include "RCChannel.h"
#include "Config.h"

// Static member initialization
RCChannel *RCChannel::externalInstances[3] = {nullptr, nullptr, nullptr};
RCChannel *RCChannel::pcintInstances[8] = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};
uint8_t RCChannel::pcintInstanceCount = 0;

RCChannel::RCChannel(uint8_t pin) : pin(pin) {}

void RCChannel::begin()
{
    pinMode(pin, INPUT);
    
    if (isExternalInterruptPin()) {
        setupExternalInterrupt();
    } else if (isPinChangeInterruptPin()) {
        setupPinChangeInterrupt();
    } else {
        // Fallback for pins that don't support interrupts
        Serial.print("Warning: Pin ");
        Serial.print(pin);
        Serial.println(" does not support interrupts. Using polling method.");
    }
}

bool RCChannel::isExternalInterruptPin() const
{
    return (pin == 2) || (pin == 3) || (pin == 21);
}

bool RCChannel::isPinChangeInterruptPin() const
{
    // PCINT2 group covers pins 0-7 (PCINT16-23)
    return (pin >= 0 && pin <= 7);
}

void RCChannel::setupExternalInterrupt()
{
    if (pin == 2)
    {
        externalInstances[0] = this;
        attachInterrupt(digitalPinToInterrupt(pin), handleInterrupt0, CHANGE);
    }
    else if (pin == 3)
    {
        externalInstances[1] = this;
        attachInterrupt(digitalPinToInterrupt(pin), handleInterrupt1, CHANGE);
    }
    else if (pin == 21)
    {
        externalInstances[2] = this;
        attachInterrupt(digitalPinToInterrupt(pin), handleInterrupt2, CHANGE);
    }
}

void RCChannel::setupPinChangeInterrupt()
{
    registerPinChangeInstance();
    
    // Enable PCINT2 interrupt for D0–D7 (PCINT16–23)
    PCICR |= (1 << PCIE2);
    PCMSK2 |= (1 << pin);
}

void RCChannel::registerPinChangeInstance()
{
    if (pcintInstanceCount < 8)
    {
        pcintInstances[pcintInstanceCount++] = this;
    }
}

uint16_t RCChannel::getPulseWidth() const
{
    return pulseWidth;
}

bool RCChannel::isSignalValid() const
{
    return signalValid && (pulseWidth >= RC_PULSE_VALID_MIN && pulseWidth <= RC_PULSE_VALID_MAX);
}

void RCChannel::handleInterrupt0()
{
    if (externalInstances[0])
        externalInstances[0]->handleInterrupt();
}

void RCChannel::handleInterrupt1()
{
    if (externalInstances[1])
        externalInstances[1]->handleInterrupt();
}

void RCChannel::handleInterrupt2()
{
    if (externalInstances[2])
        externalInstances[2]->handleInterrupt();
}

void RCChannel::handlePinChange()
{
    handleInterrupt();
}

void RCChannel::handleInterrupt()
{
    if (digitalRead(pin) == HIGH)
    {
        pulseStart = micros();
    }
    else
    {
        uint32_t now = micros();
        pulseWidth = now - pulseStart;
        
        // Validate signal
        if (pulseWidth >= RC_PULSE_VALID_MIN && pulseWidth <= RC_PULSE_VALID_MAX) {
            signalValid = true;
        } else {
            signalValid = false;
        }
    }
}

// Global Pin Change Interrupt handler
void handlePCINT2_vect()
{
    static uint8_t lastPinState = 0;
    uint8_t currentPinState = PIND; // D0–D7 mapped to PIND
    uint8_t changedPins = currentPinState ^ lastPinState;
    lastPinState = currentPinState;

    for (uint8_t i = 0; i < RCChannel::pcintInstanceCount; ++i)
    {
        if (RCChannel::pcintInstances[i])
        {
            uint8_t pin = RCChannel::pcintInstances[i]->getPin();
            if (changedPins & (1 << pin))
            {
                RCChannel::pcintInstances[i]->handlePinChange();
            }
        }
    }
}

// Global ISR
ISR(PCINT2_vect)
{
    handlePCINT2_vect();
}