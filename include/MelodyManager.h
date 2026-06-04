#ifndef MELODY_MANAGER_H
#define MELODY_MANAGER_H

#include <Arduino.h>
#include "BuzzerController.h"

class MelodyManager {
public:
    MelodyManager(BuzzerController& buzzer);
    void begin();
    void update(uint16_t onTunePulse, uint16_t selectTunePulse, bool signalValid);

private:
    BuzzerController& _buzzer;
    BuzzerMode _currentMode;
    uint8_t _currentMelodyIndex;
    bool _melodyActive;
    bool _melodyInitialized;
    bool _readyToSelectNext;
    uint16_t _prevSelectPulse;
};

#endif
