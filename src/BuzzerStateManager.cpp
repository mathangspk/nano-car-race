#include "BuzzerStateManager.h"

#define EEPROM_MELODY_ADDR 0

BuzzerStateManager::BuzzerStateManager(BuzzerController& bz) : buzzer(bz) {}

void BuzzerStateManager::handleBuzzerMode(uint16_t onTune, uint16_t selectTune) {
    // Check for error condition first
    if (onTune < Config::RC_ERROR_MIN || onTune > Config::RC_ERROR_MAX) {
        setErrorMode();
        return;
    }

    // Handle different buzzer modes
    if (onTune > Config::ON_TUNE_OFF_MIN && onTune < Config::ON_TUNE_OFF_MAX) {
        setOffMode();
    } else if (onTune > Config::ON_TUNE_DEFAULT_MIN && onTune < Config::ON_TUNE_DEFAULT_MAX) {
        handleDefaultMelodyMode();
    } else if (onTune > Config::ON_TUNE_SELECT_MIN && onTune < Config::ON_TUNE_SELECT_MAX) {
        handleSelectionMode(selectTune);
    } else {
        handleNoMusicMode();
    }

    // Update buzzer if mode changed
    if (currentMode != previousMode) {
        buzzer.setMode(currentMode);
        previousMode = currentMode;
    }

    buzzer.update();
}

bool BuzzerStateManager::isRCConnected(uint16_t throttle) {
    return throttle >= Config::RC_ERROR_MIN && throttle <= Config::RC_ERROR_MAX;
}

void BuzzerStateManager::setErrorMode() {
    currentMode = BuzzerMode::ERROR;
    buzzer.setMode(currentMode);
    buzzer.update();
    Serial.println("Error: RC signal out of range!");
}

void BuzzerStateManager::setOffMode() {
    currentMode = BuzzerMode::OFF;
    buzzer.setMode(currentMode);
    buzzer.update();
}

void BuzzerStateManager::handleDefaultMelodyMode() {
    if (!melodyActive) {
        Serial.println(">> Play default melody");
        currentMelodyIndex = 1;
        buzzer.setCustomMelody(melodies[currentMelodyIndex]);
        currentMode = BuzzerMode::CUSTOM;
        melodyActive = true;
    }
    // Reset selection mode flags
    melodyInitialized = false;
}

void BuzzerStateManager::handleSelectionMode(uint16_t selectTune) {
    melodyActive = false; // Turn off default melody mode

    // Initialize when first entering selection mode
    if (!melodyInitialized) {
        initializeSelectionMode();
    }

    // Handle melody selection
    handleMelodySelection(selectTune);

    currentMode = BuzzerMode::CUSTOM;
}

void BuzzerStateManager::initializeSelectionMode() {
    melodyInitialized = true;
    currentMelodyIndex = EEPROM.read(EEPROM_MELODY_ADDR);
    if (currentMelodyIndex >= melodyCount) {
        currentMelodyIndex = 0;
    }

    Serial.print(">> Enter selection mode, playing saved melody index: ");
    Serial.println(currentMelodyIndex);
    buzzer.setCustomMelody(melodies[currentMelodyIndex]);
    buzzer.setMode(BuzzerMode::CUSTOM);
}

void BuzzerStateManager::handleMelodySelection(uint16_t selectTune) {
    // When selectTune > threshold: allow selection but don't execute yet
    if (selectTune > Config::SELECT_TUNE_THRESHOLD && prevSelectPulse <= Config::SELECT_TUNE_THRESHOLD) {
        readyToSelectNext = true;
    }

    // When returning to center: execute increment
    if (selectTune >= Config::SELECT_TUNE_EXECUTE_MIN && selectTune <= Config::SELECT_TUNE_EXECUTE_MAX && readyToSelectNext) {
        readyToSelectNext = false;

        // Select next melody
        currentMelodyIndex++;
        if (currentMelodyIndex >= melodyCount) {
            currentMelodyIndex = 0;
        }

        EEPROM.write(EEPROM_MELODY_ADDR, currentMelodyIndex);
        buzzer.setCustomMelody(melodies[currentMelodyIndex]);
        buzzer.setMode(BuzzerMode::CUSTOM);

        Serial.print("Selected melody index: ");
        Serial.println(currentMelodyIndex);
    }

    prevSelectPulse = selectTune;
}

void BuzzerStateManager::handleNoMusicMode() {
    if (melodyActive || melodyInitialized) {
        Serial.println(">> Stop playing melody");
        melodyActive = false;
        melodyInitialized = false;
        currentMode = BuzzerMode::OFF;
    }
} 