#include <Arduino.h>
#include <SoftwareSerial.h>
#include "MotorController.h"
#include "RCChannel.h"
#include "RCChannelPCINT.h"
#include "BuzzerController.h"
#include "Melodies.h"
#include <EEPROM.h>

#define EEPROM_MELODY_ADDR 0
// Pin definitions for motor control (Arduino Nano)

const uint8_t LEFT_MOTOR_PWM = 9;
const uint8_t LEFT_MOTOR_DIR1 = 7;
const uint8_t LEFT_MOTOR_DIR2 = 8;
const uint8_t RIGHT_MOTOR_PWM = 10;
const uint8_t RIGHT_MOTOR_DIR1 = 11;
const uint8_t RIGHT_MOTOR_DIR2 = 12;
// Create instances of our classes
MotorController motorController(
    LEFT_MOTOR_PWM, LEFT_MOTOR_DIR1, LEFT_MOTOR_DIR2,
    RIGHT_MOTOR_PWM, RIGHT_MOTOR_DIR1, RIGHT_MOTOR_DIR2);

// Create RCChannel instances
RCChannel turnChannel(2);
RCChannel moveChannel(3);
RCChannelPCINT throttleChannel(4); // Using PCINT for pin 21 (INT2 on Nano)
RCChannelPCINT onTuneChannel(5);
RCChannelPCINT selectTuneChannel(6);

// Create BuzzerController instance
BuzzerController buzzerController(A0);

// Helper: Chuyển RC pulse (1000-2000) → tốc độ motor (-255 đến 255)
int mapRCToSpeed(uint16_t pulse)
{
    pulse = constrain(pulse, 1000, 2000);
    return map(pulse, 1000, 2000, -255, 255);
}
// Chuyển xung throttle RC (1000–2000) → hệ số tốc độ 0.0–1.0
float mapThrottleToScale(uint16_t pulse)
{
    pulse = constrain(pulse, 1000, 2000);
    return 0.35f + (pulse - 1000) * (0.65f / 1000.0f); // map 1000→0.35, 2000→1.0
}

// Thêm biến global
BuzzerMode currentMode = BuzzerMode::OFF;
BuzzerMode previousMode = BuzzerMode::OFF;

static uint8_t currentMelodyIndex = 0;
static bool selectPressed = false;
uint16_t prevSelectPulse = 1500; // Khởi đầu là mức trung bình
bool readyToSelectNext = false;  // Cờ cho biết đã sẵn sàng chọn tiếp

static bool inSelectMode = false;
static bool melodyInitialized = false;
static bool melodyActive = false;

volatile bool throttleSignalReceived = false;

void onThrottleSignalChange()
{
    // ... đọc pulse width vào _throttle
    throttleSignalReceived = true;
}
void setup()
{
    //  Initialize debug serial port
    Serial.begin(115200);
    Serial.println("\nNano RC Car Controller Starting...");
    // Initialize RC channels
    turnChannel.begin();
    moveChannel.begin();
    throttleChannel.begin();
    onTuneChannel.begin();
    selectTuneChannel.begin();
    delay(1000); // Wait for channels to stabilize
    Serial.println("RC channels initialized.");
    // Initialize components
    motorController.begin();
    /*
        uint8_t savedIndex = EEPROM.read(EEPROM_MELODY_ADDR);
        if (savedIndex < melodyCount)
        {
            buzzerController.setCustomMelody(melodies[savedIndex]);
            buzzerController.setMode(BuzzerMode::CUSTOM);
        }
        else
        {
            buzzerController.setCustomMelody(melodies[0]);
            buzzerController.setMode(BuzzerMode::CUSTOM);
        }
        // simpleMelody.setLoop(true);
        // buzzerController.setCustomMelody(&simpleMelody);
        buzzerController.setMode(BuzzerMode::CUSTOM);
    */
    // buzzerController.update();
    Serial.println("System initialized and ready!");
}

void loop()
{
    /*
        Serial.print("D4: ");
        Serial.print(pulseIn(4, HIGH));
        Serial.print("  D5: ");
        Serial.print(pulseIn(5, HIGH));
        Serial.print("  D6: ");
        Serial.println(pulseIn(6, HIGH));
        delay(500);
    */

    // Read RC channel values
    uint16_t _turn = turnChannel.getPulseWidth();
    uint16_t _move = moveChannel.getPulseWidth();
    uint16_t _throttle = throttleChannel.getPulseWidth();
    uint16_t _onTune = onTuneChannel.getPulseWidth();
    uint16_t _selectTune = selectTuneChannel.getPulseWidth();

    // Print the pulse widths for debugging
    Serial.print("Channel 1 Pulse Width: ");
    Serial.println(_turn);
    Serial.print("Channel 2 Pulse Width: ");
    Serial.println(_move);
    Serial.print("Channel 3 Pulse Width: ");
    Serial.println(_throttle);
    Serial.print("Channel 4 Pulse Width: ");
    Serial.println(_onTune);
    Serial.print("Channel 5 Pulse Width: ");
    Serial.println(_selectTune);
    // delay(1000);

    // check connnect tx

    if (_throttle < 900 || _throttle > 2500)
    {
        currentMode = BuzzerMode::ERROR;
        buzzerController.setMode(currentMode);
        buzzerController.update();
        Serial.println("Error: Throttle pulse width out of range!");
        return;
    }
    else if (_onTune > 1200 && _onTune < 1700)
    {
        currentMode = BuzzerMode::OFF; // Không ở chế độ nhạc
        buzzerController.setMode(currentMode);
        buzzerController.update();
    }
    else if (_onTune > 950 && _onTune < 1200) // CHẾ ĐỘ PHÁT BÀI MẶC ĐỊNH
    {
        if (!melodyActive)
        {
            Serial.println(">> Play default melody");
            currentMelodyIndex = 1;
            buzzerController.setCustomMelody(melodies[currentMelodyIndex]);
            currentMode = BuzzerMode::CUSTOM;
            melodyActive = true;
        }
        // Ngăn chặn chạy chọn bài
        melodyInitialized = false;
        inSelectMode = false;
    }
    else if (_onTune > 1500 && _onTune < 2100) // CHẾ ĐỘ CHỌN BÀI
    {
        melodyActive = false; // Tắt chế độ phát mặc định

        // Khởi tạo khi mới vào chế độ chọn bài
        if (!melodyInitialized)
        {
            melodyInitialized = true;
            currentMelodyIndex = EEPROM.read(EEPROM_MELODY_ADDR);
            if (currentMelodyIndex >= melodyCount)
            {
                currentMelodyIndex = 0;
            }

            Serial.print(">> Enter selection mode, playing saved melody index: ");
            Serial.println(currentMelodyIndex);
            buzzerController.setCustomMelody(melodies[currentMelodyIndex]);
            buzzerController.setMode(BuzzerMode::CUSTOM); // Bắt đầu phát luôn
            inSelectMode = false;                         // Chưa vào chế độ chọn
        }

        // Bên trong khối if (_onTune > 1500 && _onTune < 2100)
        {
            // Khi selectTune > 1500: cho phép chọn, nhưng chưa thực hiện
            if (_selectTune > 1500 && prevSelectPulse <= 1500)
            {
                readyToSelectNext = true; // Đã gạt lên → sẵn sàng chọn
            }

            // Khi chuyển lại 1500: thực hiện tăng
            if (_selectTune >= 1400 && _selectTune <= 1600 && readyToSelectNext)
            {
                readyToSelectNext = false; // Reset lại cờ

                // Thực hiện chọn bài hát tiếp theo
                currentMelodyIndex++;
                if (currentMelodyIndex >= melodyCount)
                    currentMelodyIndex = 0;

                EEPROM.write(EEPROM_MELODY_ADDR, currentMelodyIndex);
                buzzerController.setCustomMelody(melodies[currentMelodyIndex]);
                buzzerController.setMode(BuzzerMode::CUSTOM);

                Serial.print("Selected melody index: ");
                Serial.println(currentMelodyIndex);
            }

            // Cập nhật trạng thái trước đó
            prevSelectPulse = _selectTune;
        }

        currentMode = BuzzerMode::CUSTOM; // Đảm bảo tiếp tục phát
    }

    else // KHÔNG Ở CHẾ ĐỘ NHẠC
    {
        if (melodyActive || melodyInitialized)
        {
            Serial.println(">> Stop playing melody");
            melodyActive = false;
            melodyInitialized = false;
            inSelectMode = false;
            currentMode = BuzzerMode::OFF;
        }
    }

    // Cập nhật nếu trạng thái mode thay đổi
    if (currentMode != previousMode)
    {
        buzzerController.setMode(currentMode);
        previousMode = currentMode;
    }

    buzzerController.update();

    int turnSpeed = mapRCToSpeed(_turn);
    if (_turn >= 1460 && _turn <= 1540)
    {
        turnSpeed = 0;
    }
    int moveSpeed = mapRCToSpeed(_move);
    if (_move >= 1460 && _move <= 1540)
    {
        moveSpeed = 0;
    }
    float throttleScale = mapThrottleToScale(_throttle); // 0.0 → 1.0

    turnSpeed = turnSpeed * throttleScale; // Scale turn speed by throttle
    moveSpeed = moveSpeed * throttleScale; // Scale move speed by throttle
    // Tính tốc độ từng bánh
    int leftSpeed = constrain(turnSpeed - moveSpeed, -255, 255);
    int rightSpeed = constrain(turnSpeed + moveSpeed, -255, 255);

    // Set the motor speeds
    motorController.setSpeed(leftSpeed, rightSpeed);

    // motorController.setSpeed(leftSpeed * throttleScale, rightSpeed * throttleScale);
    //  Add a small delay to avoid flooding the serial output
    /*
        Serial.print("Left speed: ");
        Serial.print(leftSpeed);
        Serial.print(" Right speed: ");
        Serial.print(rightSpeed);
        Serial.print(" Throttle: ");
        Serial.println(throttleScale);
*/
    // Delay to avoid flooding the serial output
    delay(10); // Adjust delay as needed
}
