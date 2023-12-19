#include <LiquidCrystal.h>
#ifndef MENU_H
#define MENU_H



const byte rs = 9;
const byte en = 8;
const byte d4 = 7;
const byte d5 = 6;
const byte d6 = 5;
const byte d7 = 4;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

String menuOptions[] = { "Start game", "Settings", "About" }; // move through those
int menuCount = 3;
String aboutInfo = "Creator: Stefan Barbu  Github: stefanbrb10; @2023unibuc  press SW to go back";
String settingsOptions[] = { "Matrix brightness", "LCD brightness", "Change name", "Sound ON/OFF", "Back" };
int settingsCount = 5;

const int lcdCols = 16; // to divide the LCD
const int lcdRows = 2;

const int brightnessPin = 3; // LCD brightness pin

const int greetingTime = 1000; // the greeting message is displayed this period
const int scrollingSpeed = 500; // each 0.5s the text that is too large refreshes
unsigned long lastScroll = 0;
int currentCharFirstLine = 0;
int currentCharSecondLine = 0;

int currentMenuOption = 0; // first layer
int currentSubmenuOption = 0; // second layer
// variables to indicate the level we're on
bool inSubSubMenu = false;
bool inMainMenu = true;
bool inSubMenu = false;

unsigned long lastTimeShow = 0;
const int showRate = 1;

const int lcdBrightnessAddress = 1;
int lcdBrightness;

int cursorStringPosition = 0;
unsigned long lastCursorShow = 0;
unsigned long lastCharChange = 0;
String playerName;
int eepromOffset = 2;
String readingWord = "a";

unsigned long buzzerStartTime = 0;
const int buzzerDuration = 50;
const int eepromSoundAddress = 255;
bool soundOn = true;
unsigned long startToningTime = 0;
int toneDuration = 300;
bool toning = false;
int space = 0;


byte doodleCharacter[] = {
        0b01110,
        0b01001,
        0b01011,
        0b11010,
        0b10001,
        0b11111,
        0b10101,
        0b10101
};


void menu();
void displayDoodleCharacters();
void handleMenuOption();
void handleSubMenuOption();
void displayCenteredText(String text, int line);
void displayGreetingMessage();
void displayMenu();
void scrollText(String text, int line);
void move(int &option, String optionList[], int len, String title);
void readSw();
void handleSubMenuLogic();
void handleSubSubMenuLogic();
void readNumber(String numToDisplay, int &number);
void displayAbout();
String millisToMinutes(unsigned long value);
void clock();
void displayRoom();
void exitGame() ;
void moveMenu(bool &state1, bool &state2, char *direction);
#endif