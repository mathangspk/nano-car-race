#ifndef BUZZER_STATE_MANAGER_H
#define BUZZER_STATE_MANAGER_H

#include <Arduino.h>
#include <EEPROM.h>
#include "Config.h"
#include "BuzzerController.h"
#include "Melodies.h"

class BuzzerStateManager {
private:
    BuzzerMode currentMode = BuzzerMode::OFF;
    BuzzerMode previousMode = BuzzerMode::OFF;
    uint8_t currentMelodyIndex = 0;
    uint16_t prevSelectPulse = Config::RC_CENTER_PULSE;
    bool readyToSelectNext = false;
    bool melodyInitialized = false;
    bool melodyActive = false;
    BuzzerController& buzzer;

    void setErrorMode();
    void setOffMode();
    void handleDefaultMelodyMode();
    void handleSelectionMode(uint16_t selectTune);
    void initializeSelectionMode();
    void handleMelodySelection(uint16_t selectTune);
    void handleNoMusicMode();

public:
    BuzzerStateManager(BuzzerController& bz);
    void handleBuzzerMode(uint16_t onTune, uint16_t selectTune);
    bool isRCConnected(uint16_t throttle);
};

#endif 