#pragma once
#include "Melody.h"
#include "Note.h"
// Melody 1
const Note melody1[] = {
    {440, 200}, {0, 50}, {494, 200}, {0, 50}, {523, 200}, {0, 50}, {440, 400}, {0, 300}};
extern const Melody simpleMelody;

// Melody 2
const Note melody2[] = {
    {659, 125}, {494, 125}, {523, 125}, {587, 125}, {659, 125}, {587, 125}, {523, 125}, {494, 125}, {440, 125}, {440, 125}, {523, 125}, {659, 125}, {587, 125}, {523, 125}, {494, 125}};
extern const Melody nokiaTune;

// Melody 3
const Note melody3[] = {
    {659, 125}, {587, 125}, {523, 125}, {587, 125}, {659, 125}, {587, 125}, {523, 125}, {587, 125}, {659, 125}, {784, 125}, {880, 125}, {698, 125}, {784, 250}, {659, 125}, {587, 125}, {523, 125}, {587, 125}, {659, 125}, {784, 125}, {880, 125}, {698, 125}, {784, 250}};
extern const Melody nokiaOldTune;

// Melody 4
const Note marimbaTune[] = {
    {1046, 200}, {1174, 200}, {1396, 200}, {1046, 200}, {1046, 200}, {1174, 200}, {1396, 200}, {1046, 200}, {1046, 200}, {1174, 200}, {1396, 200}, {1046, 200}, {1318, 400}, {1046, 200}, {1318, 400}, {1046, 200}, {1396, 600}, {0, 100}, {1046, 200}, {1174, 200}, {1396, 200}, {1567, 200}, {1396, 400}, {0, 100}, {1396, 200}, {1567, 200}, {1760, 200}, {1396, 200}};
extern const Melody iphoneMarimba;

// Melody 5
const Note sunnyStartExtended[] = {
    {659, 150}, {784, 150}, {880, 150}, {988, 150}, {1046, 300}, {880, 200}, {988, 200}, {1174, 400}, {1318, 200}, {1396, 200}, {1567, 300}, {1174, 150}, {1046, 150}, {988, 200}, {880, 200}, {784, 250}, {659, 150}, {784, 150}, {880, 150}, {784, 150}, {659, 400}, {0, 100}};
extern const Melody sunnyMelodyExtended;

// Melody 6
const Note chauLenBa[] = {
    {392, 300}, {440, 300}, {392, 300}, {392, 300}, {392, 300}, {440, 300}, {392, 300}, {349, 300}, {392, 300}, {392, 300}, {440, 300}, {392, 300}, {392, 300}, {440, 300}, {392, 300}, {349, 300}, {392, 300}, {440, 300}, {392, 300}, {392, 300}, {392, 300}, {440, 300}, {392, 300}, {349, 600}, {0, 500}};
extern const Melody chauLenBaMelody;

// Melody 7
const Note happyBirthday[] = {
    // Happy birthday to you
    {NOTE_C4, 250},
    {NOTE_C4, 250},
    {NOTE_D4, 500},
    {NOTE_C4, 500},
    {NOTE_F4, 500},
    {NOTE_E4, 1000},

    // Happy birthday to you
    {NOTE_C4, 250},
    {NOTE_C4, 250},
    {NOTE_D4, 500},
    {NOTE_C4, 500},
    {NOTE_G4, 500},
    {NOTE_F4, 1000},

    // Happy birthday, happy birthday
    {NOTE_C4, 250},
    {NOTE_C4, 250},
    {NOTE_C5, 500},
    {NOTE_A4, 500},
    {NOTE_F4, 500},
    {NOTE_E4, 500},
    {NOTE_D4, 1000},

    // Happy birthday to you
    {NOTE_AS4, 250},
    {NOTE_AS4, 250},
    {NOTE_A4, 500},
    {NOTE_F4, 500},
    {NOTE_G4, 500},
    {NOTE_F4, 1000}};

extern const Melody happyBirthdayMelody;

// Array of melodies
extern const Melody *melodies[];
extern const size_t melodyCount;
