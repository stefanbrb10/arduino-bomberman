// Bomberman game on 8x8 matrix arduino
// The player moves around the matrix and has to destroy walls using bombs. The purpose is to clean the map ASAP

#include "LedControl.h"
#include <LiquidCrystal.h>
#include <EEPROM.h>
#include "Menu.h"
#include "Matrix.h"
#include "Game.h"
#include "Enemy.h"


// SW pins
const int pinSW = 2;
const int pinX = A0;
const int pinY = A1;
const int buzzerPin = A5;


bool firstImpulse = true; // to skip the first unnecesary "press" on SW

// for debouncing
unsigned long lastDebounceTime = 0;
int lastSwState = LOW;
int swState = LOW;
const int debounceDelay = 50;

bool joyMoved = false;
const int minJoyThreshold = 300;
const int maxJoyThreshold = 700;



void setup() {
    Serial.begin(9600);
    pinMode(pinSW, INPUT_PULLUP);
    pinMode(pinX, INPUT);
    pinMode(pinY, INPUT);
    pinMode(brightnessPin, OUTPUT);
    randomSeed(analogRead(0));
    initEEPROM();
    analogWrite(brightnessPin, 250);
    lcd.begin(lcdCols, lcdRows);
    lc.shutdown(0, false);
    lc.setIntensity(0, matrixBrightness);
    lc.clearDisplay(0);
    displayGreetingMessage();
}

void loop() {
    menu();
    readSw();
    if (toStartGame) {
        startGame(); // start the game (init)
    }
    if (inGame) {
        playGame();
    }
}