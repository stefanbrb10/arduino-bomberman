#include "Arduino.h"
#ifndef GAME_H
#define GAME_H


unsigned long startGameTime;

struct Position {
    int x, y;
};

const Position initialPosition = { 0, 0 };  // initial position of the player
Position currentPosition;
Position joystickPosition;  // raw joystick positiom
//Position bombPosition;

Position enemyPosition;


int wallCount[4] = { 0 };  // how many walls were generated
int wallsBlown[4] = { 0 };

// they change when the game has come to an end
bool lost = false;
bool won = false;

bool toStartGame = false;  // to know that the player selected the option "start game"
bool inGame = false;

//bool planted = false;             // indicates the bomb has been planted
//unsigned long plantBombTime = 0;  // the starting time of planting

unsigned long lastPlayerBlink = 0;  // to know when to blink
bool blinkStatePlayer = true;

//unsigned long lastBombBlink = 0;
//bool blinkStateBomb = true;   // indicates the state of the bomb LED
const int explodingZone = 2;  // all the blocks in a radius of 2 will explode
const int explodingTime = 2500;

bool playing = false;

const int levelProbability[] = { 7, 32 };  // the probability that a wall is on a cell
const int levelCount = 4;
int currentLevel = 1;

unsigned long lastEnemyMove = 0;
int enemySpeed = 1000;
bool enemyBlinkState = true;
unsigned long lastEnemyBlink = 0;
int room = 1;
unsigned long loseGameTime = 0;
bool killedByEnemy = false;
bool killedByBomb = false;
const int bombCount = 2;

struct Bomb {
    bool planted;
    int x, y;
    int bombLastSwState;
    int bombSwState;
    unsigned long plantBombTime;
    unsigned long bombLastDebounceTime;
    unsigned long lastBombBlink;
    bool blinkStateBomb;
    int plantedRoom;
    bool firstRead;

    Bomb() {
        blinkStateBomb = true;
        planted = false;
        bombLastSwState = LOW;
        bombSwState = LOW;
        bombLastDebounceTime = 0;
        lastBombBlink = 0;
        plantBombTime = 0;
        firstRead = true;
    }
};

Bomb bombs[2];

//int plantedRoom;

void startGame();
void generateWalls(const int probability);
void movePlayer();
bool canMove(const char *direction);
int getState(unsigned long &lastBlink, int rate, bool &blinkState);
//void checkForPlant();
// void plantBomb();
// void explode();
// void destroyWall(int x, int y) ;
// void plantBombLogic();
void playGame();

#endif