#include "RCChannel.h"
#include "Config.h"

RCChannel *RCChannel::externalInstances[2] = {nullptr, nullptr};
RCChannel *RCChannel::pcintInstances[8] = {nullptr};
uint8_t RCChannel::pcintInstanceCount = 0;

RCChannel::RCChannel(uint8_t pin) : pin(pin) {}

void RCChannel::begin() {
    pinMode(pin, INPUT);
    if (isExternalInterruptPin()) {
        setupExternalInterrupt();
    } else if (isPinChangeInterruptPin()) {
        setupPinChangeInterrupt();
    }
}

void RCChannel::setupExternalInterrupt() {
    if (pin == 2) {
        externalInstances[0] = this;
        attachInterrupt(0, handleInterrupt0, CHANGE);
    } else if (pin == 3) {
        externalInstances[1] = this;
        attachInterrupt(1, handleInterrupt1, CHANGE);
    }
}

void RCChannel::setupPinChangeInterrupt() {
    if (pcintInstanceCount < 8) {
        pcintInstances[pcintInstanceCount++] = this;
    }
    PCICR |= (1 << PCIE2);
    PCMSK2 |= (1 << pin);
}

uint16_t RCChannel::getPulseWidth() const { return pulseWidth; }

bool RCChannel::isSignalValid() const {
    return signalValid && 
           (pulseWidth >= RC_PULSE_VALID_MIN && pulseWidth <= RC_PULSE_VALID_MAX) &&
           (millis() - lastPulseTime < SIGNAL_TIMEOUT_MS);
}

void RCChannel::handleInterrupt0() { if (externalInstances[0]) externalInstances[0]->handleInterrupt(); }
void RCChannel::handleInterrupt1() { if (externalInstances[1]) externalInstances[1]->handleInterrupt(); }
void RCChannel::handlePinChange() { handleInterrupt(); }

void RCChannel::handleInterrupt() {
    if (digitalRead(pin) == HIGH) {
        pulseStart = micros();
    } else {
        uint32_t now = micros();
        pulseWidth = now - pulseStart;
        if (pulseWidth >= RC_PULSE_VALID_MIN && pulseWidth <= RC_PULSE_VALID_MAX) {
            signalValid = true;
            lastPulseTime = millis();
        } else {
            signalValid = false;
        }
    }
}

void handlePCINT2_vect() {
    static uint8_t lastPinState = 0;
    static bool firstRun = true;
    if (firstRun) {
        lastPinState = PIND;
        firstRun = false;
    }
    uint8_t currentPinState = PIND;
    uint8_t changedPins = currentPinState ^ lastPinState;
    lastPinState = currentPinState;

    for (uint8_t i = 0; i < RCChannel::pcintInstanceCount; ++i) {
        if (RCChannel::pcintInstances[i]) {
            uint8_t pin = RCChannel::pcintInstances[i]->getPin();
            if (changedPins & (1 << pin)) {
                RCChannel::pcintInstances[i]->handlePinChange();
            }
        }
    }
}

ISR(PCINT2_vect) { handlePCINT2_vect(); }