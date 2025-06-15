#include "Melodies.h"

// Định nghĩa melody
const Melody simpleMelody(melody1, sizeof(melody1) / sizeof(melody1[0]), true);
const Melody nokiaTune(melody2, sizeof(melody2) / sizeof(melody2[0]), true);
const Melody nokiaOldTune(melody3, sizeof(melody3) / sizeof(melody3[0]), true);
const Melody iphoneMarimba(marimbaTune, sizeof(marimbaTune) / sizeof(marimbaTune[0]), true);
const Melody sunnyMelodyExtended(sunnyStartExtended, sizeof(sunnyStartExtended) / sizeof(sunnyStartExtended[0]), true);
const Melody chauLenBaMelody(chauLenBa, sizeof(chauLenBa) / sizeof(chauLenBa[0]), true);
const Melody happyBirthdayMelody(happyBirthday, sizeof(happyBirthday) / sizeof(happyBirthday[0]), true);

// Danh sách
const Melody *melodies[] = {
    &simpleMelody,
    &nokiaTune,
    &nokiaOldTune,
    &iphoneMarimba,
    &sunnyMelodyExtended,
    &chauLenBaMelody,
    &happyBirthdayMelody};

const size_t melodyCount = sizeof(melodies) / sizeof(melodies[0]);
