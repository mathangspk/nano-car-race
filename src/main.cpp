#include <Arduino.h>
#include "CarApplication.h"
#include "Config.h"

CarApplication app;

void setup() {
    delay(3000); // Allow upload verification to complete before any Serial prints
    Serial.begin(SERIAL_BAUD_RATE);
    Serial.println("\nNano RC Car Controller Starting...");

    app.begin();

    Serial.println("System initialized and ready!");
}

void loop() {
    app.update();
    delay(LOOP_DELAY_MS);
}
