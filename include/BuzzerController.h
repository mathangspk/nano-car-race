#pragma once
#include <Arduino.h>
#include "Melody.h"

enum class BuzzerMode
{
    OFF,
    READY,
    ERROR,
    REVERSING,
    CUSTOM
};

class BuzzerController
{
public:
    BuzzerController(uint8_t pin);
    void begin();
    void setMode(BuzzerMode mode);
    void update();

    void setCustomMelody(const Melody *melody); // Nhận melody để phát
    BuzzerMode getMode() const { return _mode; }

private:
    void playReadyTone();
    void playErrorTone();
    void playReversingTone();
    void playCustomMelody();

    uint8_t _pin;
    BuzzerMode _mode, _lastMode;
    unsigned long _lastToneTime;
    bool _toneState;

    const Melody *_customMelody;
    size_t _melodyIndex;
};
