#include "RCChannel.h"

RCChannel *RCChannel::instances[3] = {nullptr, nullptr, nullptr};

RCChannel::RCChannel(uint8_t pin) : pin(pin) {}

void RCChannel::begin()
{
    pinMode(pin, INPUT);

    // Gán instance vào mảng static theo interrupt số
    if (pin == 2)
    {
        instances[0] = this;
        attachInterrupt(digitalPinToInterrupt(pin), handleInterrupt0, CHANGE);
    }
    else if (pin == 3)
    {
        instances[1] = this;
        attachInterrupt(digitalPinToInterrupt(pin), handleInterrupt1, CHANGE);
    }
#if defined(EICRA) // Uno/Nano only có INT0, INT1, INT2
    else if (pin == 21)
    {
        instances[2] = this;
        attachInterrupt(digitalPinToInterrupt(pin), handleInterrupt2, CHANGE);
    }
#endif
}

uint16_t RCChannel::getPulseWidth() const
{
    return pulseWidth;
}

void RCChannel::handleInterrupt0()
{
    if (instances[0])
        instances[0]->handleInterrupt();
}

void RCChannel::handleInterrupt1()
{
    if (instances[1])
        instances[1]->handleInterrupt();
}

void RCChannel::handleInterrupt2()
{
    if (instances[2])
        instances[2]->handleInterrupt();
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
    }
}