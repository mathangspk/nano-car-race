#ifndef CAR_APPLICATION_H
#define CAR_APPLICATION_H

#include <Arduino.h>
#include "MotorController.h"
#include "RCChannel.h"
#include "BuzzerController.h"
#include "MelodyManager.h"

class CarApplication {
public:
    CarApplication();
    void begin();
    void update();

private:
    bool validateAllSignals();
    int mapRCToSpeed(uint16_t pulse, uint16_t rcMin, uint16_t rcMax);
    float mapThrottleToScale(uint16_t pulse);
    bool isNeutral(uint16_t pulse);

    MotorController _motorController;
    RCChannel _turnChannel;
    RCChannel _moveChannel;
    RCChannel _throttleChannel;
    RCChannel _onTuneChannel;
    RCChannel _selectTuneChannel;
    BuzzerController _buzzerController;
    MelodyManager _melodyManager;

    bool _allSignalsValid;
};

#endif
