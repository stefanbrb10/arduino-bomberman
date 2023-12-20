#include "MyPitches.h"

#define BUZZER_PIN A5

int melody[] = {
        NOTE_E5,
        NOTE_E5,
        REST,
        NOTE_E5,
        REST,
        NOTE_C5,
        NOTE_E5,
        NOTE_G5,
        REST,
        NOTE_G4,
        REST,
        NOTE_C5,
        NOTE_G4,
        REST,
        NOTE_E4,
        NOTE_A4,
        NOTE_B4,
        NOTE_AS4,
        NOTE_A4,
        NOTE_G4,
        NOTE_E5,
        NOTE_G5,
        NOTE_A5,
        NOTE_F5,
        NOTE_G5,
        REST,
        NOTE_E5,
        NOTE_C5,
        NOTE_D5,
        NOTE_B4,
        NOTE_C5,
        NOTE_G4,
        REST,
        NOTE_E4,
        NOTE_A4,
        NOTE_B4,
        NOTE_AS4,
        NOTE_A4,
        NOTE_G4,
        NOTE_E5,
        NOTE_G5,
        NOTE_A5,
        NOTE_F5,
        NOTE_G5,
        REST,
        NOTE_E5,
        NOTE_C5,
        NOTE_D5,
        NOTE_B4,
};

int durations[] = {
        8,
        8,
        8,
        8,
        8,
        8,
        8,
        4,
        4,
        8,
        4,
        4,
        8,
        4,
        4,
        4,
        4,
        8,
        4,
        8,
        8,
        8,
        4,
        8,
        8,
        8,
        4,
        8,
        8,
        4,
        4,
        8,
        4,
        4,
        4,
        4,
        8,
        4,
        8,
        8,
        8,
        4,
        8,
        8,
        8,
        4,
        8,
        8,
        4,
};

int noteIndex = 0;
unsigned long noteStartTime = 0;

void PlaySong() {
    scrollText("", aboutInfo,  0);
    int size = sizeof(durations) / sizeof(int);
    unsigned long currentTime = millis();
    displayDoodleCharacters();

    if (currentTime - noteStartTime >= (1000 / durations[noteIndex])) {
        // If enough time has passed for the current note, play the note
        tone(BUZZER_PIN, melody[noteIndex]);

        // Move to the next note
        noteIndex++;

        // Reset the start time for the next note
        noteStartTime = currentTime;

        // Check if the song is finished
        if (noteIndex >= size) {
            // Reset the note index to play the song again
            noteIndex = 0;
        }
    } else if (currentTime - noteStartTime >= durations[noteIndex] * 1.30) {
        // If it's time for the next note, but not enough time has passed for the current note,
        // stop the tone to create a gap between notes
        noTone(BUZZER_PIN);
    }
}