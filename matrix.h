
#include "LedControl.h"
#ifndef MATRIX_H
#define MATRIX_H


// MAX7219 pins
const int dinPin = 12;
const int clockPin = 11;
const int loadPin = 10;
LedControl lc = LedControl(dinPin, clockPin, loadPin, 1);

int matrixBrightness;
const int matrixBrightnessAddress = 0;
const int minBrightness = 0;
const int maxBrightness = 16;

const int playerBlinkRate = 600;
const int bombBlinkRate = 200;

const int matrixSize = 8;
byte matrix[matrixSize][matrixSize] = { false };
byte room1[matrixSize][matrixSize] = { false };
byte room2[matrixSize][matrixSize] = { false };
byte room3[matrixSize][matrixSize] = { false };
byte room4[matrixSize][matrixSize] = { false };

int initialIndexLine = 0;
int finalIndexLine = matrixSize - 1;
int initialIndexColumn = 0;
int finalIndexColumn = matrixSize - 1;

byte wPattern[matrixSize][matrixSize] = {
        { 1, 0, 0, 0, 0, 0, 0, 1 },
        { 1, 0, 0, 0, 0, 0, 0, 1 },
        { 1, 0, 0, 0, 0, 0, 0, 1 },
        { 1, 0, 0, 0, 0, 0, 0, 1 },
        { 1, 0, 0, 1, 0, 0, 0, 1 },
        { 1, 0, 1, 0, 1, 0, 0, 1 },
        { 1, 1, 0, 0, 0, 1, 1, 1 },
        { 1, 0, 0, 0, 0, 0, 0, 1 }
};


const uint64_t downArrow = 0x00000040e0400000;
const uint64_t upArrow = 0x0000020702000000;
const uint64_t rightArrow = 0x0000000000081c08;
const uint64_t leftArrow = 0x081c080000000000;
const uint64_t allArrow = 0x081c0842e74a1c08;
const uint64_t verticalArrows = 0x00000042e7420000;
const uint64_t horizontalArrows = 0x081c080000081c08;

bool horizontalArrowShowed = false;
bool verticalArrowShowed = false;


void displayLoseAnimation();
void displayWinAnimation();
void displayMatrix();
#endif