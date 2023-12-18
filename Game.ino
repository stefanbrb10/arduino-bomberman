#include "Matrix.h"
#include "LedControl.h"
#include "Game.h"
#include "Menu.h"

void startGame() {
    killedByEnemy = false;
    killedByBomb = false;
    currentPosition.x = initialPosition.x;
    currentPosition.y = initialPosition.y;
    enemyPosition.x = matrixSize / 2 - 1;
    enemyPosition.y = matrixSize / 2 - 1;
    generateWalls(levelProbability[currentLevel - 1], room1, 1);
    generateWalls(levelProbability[currentLevel - 1], room2, 2);
    generateWalls(levelProbability[currentLevel - 1], room3, 3);
    generateWalls(levelProbability[currentLevel - 1], room4, 4);

    passRoom(room1);
    toStartGame = false;  // the game starts only once
    inGame = true;
    startGameTime = millis();
    for (int row = 0; row < matrixSize; row++)
        for (int column = 0; column < matrixSize; column++)
            lc.setLed(0, row, column, 1);
}


void generateWalls(const int probability, byte array[][matrixSize], int roomIndex) {  // now the probability is accordingly to the level
    for (int row = 0; row < matrixSize; row++)
        for (int column = 0; column < matrixSize; column++) {
            if (row - currentPosition.x < explodingZone && column - currentPosition.y < explodingZone)
                array[row][column] = false;  // to ensure the player can move at the beginning
            else
                array[row][column] = (random(matrixSize * matrixSize) < probability);
            if (array[row][column])
                wallCount[roomIndex - 1]++;
        }
}

void passRoom(byte room[][matrixSize]) {
    tone(buzzerPin, 578, 250);
    for (int row = 0; row < matrixSize; row++)
        for (int column = 0; column < matrixSize; column++)
            matrix[row][column] = room[row][column];
}

void movePlayer() {
    joystickPosition.x = analogRead(pinX);
    joystickPosition.y = analogRead(pinY);

    if (!joyMoved) {
        // the joystick is moved accordingly and we dont have wall and we are in the map
        if (joystickPosition.y < minJoyThreshold && canMove("right", currentPosition, false)) {
            currentPosition.y--;
        } else if (joystickPosition.y > maxJoyThreshold && canMove("left", currentPosition, false))
            currentPosition.y++;
        else if (joystickPosition.x < minJoyThreshold && canMove("up", currentPosition, false))
            currentPosition.x--;
        else if (joystickPosition.x > maxJoyThreshold && canMove("down", currentPosition, false))
            currentPosition.x++;

        joyMoved = true;
    }


    if (joystickPosition.y >= minJoyThreshold && joystickPosition.y <= maxJoyThreshold && joystickPosition.x >= minJoyThreshold && joystickPosition.x <= maxJoyThreshold)
        joyMoved = false;  // to get the joystick position only once
}

void moveEnemy() {
    if (millis() - lastEnemyMove > enemySpeed) {
        int direction = random(4);
        if (canMove("left", enemyPosition, true) && direction == 0)
            enemyPosition.y++;
        else if (canMove("up", enemyPosition, true) && direction == 1)
            enemyPosition.x--;
        else if (canMove("right", enemyPosition, true) && direction == 2)
            enemyPosition.y--;
        else if (canMove("down", enemyPosition, true) && direction == 3)
            enemyPosition.x++;
        lastEnemyMove = millis();
    }
}



bool canMove(const char *direction, Position position, bool enemy) {
    if (direction == "right") {
        // no wall and in the map
        if (position.y == 0 && !enemy) {
            if (room == 1) {
                if (currentLevel > 1) {
                    currentPosition.y = matrixSize - 1;
                    room = 2;
                    passRoom(room2);
                } else tone(buzzerPin, 100, 200);
            }
            if (room == 4 && !enemy) {
                currentPosition.y = matrixSize - 1;
                room = 3;
                passRoom(room3);
            }
            //return false;
        }
        return !matrix[position.x][position.y - 1] && position.y > 0;
    }
    if (direction == "left") {
        if (position.y == matrixSize - 1 && !enemy) {
            if (room == 2 && !enemy) {
                currentPosition.y = 0;
                room = 1;
                passRoom(room1);
            }
            if (room == 3 && !enemy) {
                if (currentLevel > 3) {
                    currentPosition.y = 0;
                    room = 4;
                    passRoom(room4);
                } else tone(buzzerPin, 100, 200);
            }
        }
        return !matrix[position.x][position.y + 1] && position.y < matrixSize - 1;
    }
    if (direction == "down") {
        if (position.x == matrixSize - 1 && !enemy) {
            if (room == 2 && !enemy) {
                if (currentLevel > 2) {
                    currentPosition.x = 0;
                    room = 3;
                    passRoom(room3);
                } else tone(buzzerPin, 100, 200);
            }
            if (room == 1 && !enemy) {
                if (currentLevel == levelCount) {
                    currentPosition.x = 0;
                    room = 4;
                    passRoom(room4);
                } else tone(buzzerPin, 100, 200);
            }
        }
        return !matrix[position.x + 1][position.y] && position.x < matrixSize - 1;
    }
    if (direction == "up") {
        if (position.x == 0 && !enemy) {
            if (room == 4) {
                currentPosition.x = matrixSize - 1;
                room = 1;
                passRoom(room1);
            }
            if (room == 3 && !enemy) {
                currentPosition.x = matrixSize - 1;
                room = 2;
                passRoom(room2);
            }
        }
        return !matrix[position.x - 1][position.y] && position.x > 0;
    }
    //return false;
}

int getState(unsigned long &lastBlink, int rate, bool &blinkState) {
    if (millis() - lastBlink > rate) {
        blinkState = !blinkState;
        lastBlink = millis();
    }
    return blinkState;
}

void checkForPlant() {
    int reading = digitalRead(pinSW);
    if (reading != lastSwState) {
        lastDebounceTime = millis();
    }

    if ((millis() - lastDebounceTime) > debounceDelay) {
        if (reading != swState) {
            swState = reading;
            if (swState == HIGH && !planted && startGameTime - millis() > 2000) {
                plantBomb();
            }
        }
    }
    lastSwState = reading;
}

void plantBomb() {
    plantBombTime = millis();
    planted = true;
    bombPosition.x = currentPosition.x;
    bombPosition.y = currentPosition.y;
    plantedRoom = room;
}

void explode() {
    destroyWall(bombPosition.x + 1, bombPosition.y);
    destroyWall(bombPosition.x - 1, bombPosition.y);
    destroyWall(bombPosition.x, bombPosition.y + 1);
    destroyWall(bombPosition.x, bombPosition.y - 1);

    // check if the player is in the dangerous zone
    if (abs(currentPosition.x - bombPosition.x) < explodingZone && abs(currentPosition.y - bombPosition.y) < explodingZone) {
        loseGameTime = millis();
        killedByBomb = true;
        lose();
    } else if (wallsBlown[room - 1] >= wallCount[room - 1]) {
        if (room < levelCount) {
            currentLevel++;
        }
        if (currentLevel < levelCount) {
            //wallsBlown[room - 1] = 0;  // resfresh this to be able to count at the next playing
        } else {
            displayCenteredText("You finished the game", 0);
        }
    }
    planted = false;  // after a bomb has exploded, we can plant again
}

void destroyWall(int x, int y) {
    if (matrix[x][y]) {
        wallsBlown[plantedRoom - 1]++;
        if (room == plantedRoom)
            matrix[x][y] = 0;
    }
    if (plantedRoom == 1 && room1[x][y]) {
        room1[x][y] = 0;
    }
    if (plantedRoom == 2 && room2[x][y]) {
        room2[x][y] = 0;
    }
    if (plantedRoom == 3 && room3[x][y]) {
        room3[x][y] = 0;
    }
    if (plantedRoom == 4 && room4[x][y]) {
        room4[x][y] = 0;
    }
}


void plantBombLogic() {
    if ((millis() - plantBombTime < explodingTime) && plantedRoom == room) {
        lc.setLed(0, bombPosition.x, bombPosition.y, getState(lastBombBlink, bombBlinkRate, blinkStateBomb));
    } else {
        explode();  // if the time passed, it explodes
    }
}

void lose() {
    displayLoseAnimation();
    lost = true;
    //lcd.clear();
    String loseMessage = "   Next time, ";
    loseMessage += playerName;
    loseMessage += " ";
    String infoToDisplay = "   Time: ";
    infoToDisplay += String(millisToMinutes(loseGameTime - startGameTime));
    infoToDisplay += "  Killed by: ";
    if (killedByEnemy) {
        infoToDisplay += "Enemy ";
    } else if (killedByBomb) {
        infoToDisplay += "Bomb ";
    }
    for (int i = 0; i < 4; i++) {
        wallCount[i] = 0;
        wallsBlown[i] = 0;
    }
    currentLevel = 1;
    room = 1;
    scrollText(loseMessage, infoToDisplay, 0);
}

void displayMatrix() {
    for (int row = 0; row < matrixSize; row++)
        for (int column = 0; column < matrixSize; column++)
            if (row != currentPosition.x || column != currentPosition.y || lost)  // to make the blink look "smooth"
                lc.setLed(0, row, column, matrix[row][column]);
}

void playGame() {
    for (int i = 0; i < 4; i++) {
        Serial.print(wallsBlown[i]);
        Serial.print("-");
        Serial.print(wallCount[i]);
        Serial.print("    ");
        Serial.print(currentLevel);
        Serial.print("    ");
    }
    Serial.println();
    displayMatrix();
    if (!lost && !won && !killedByEnemy) {
        clock();
        displayRoom();
        lc.setLed(0, currentPosition.x, currentPosition.y, getState(lastPlayerBlink, playerBlinkRate, blinkStatePlayer));
        if (room != 1)
            lc.setLed(0, enemyPosition.x, enemyPosition.y, getState(lastEnemyBlink, enemySpeed / 4, enemyBlinkState));
        checkForPlant();  // get the input of the sw button, and plant the bomb when it is pressed
        if (planted) {    // if the bomb is planted then blink
            plantBombLogic();
        }
        movePlayer();  // read the joystick position and move around
        if (room != 1)
            moveEnemy();
        if ((enemyPosition.x == currentPosition.x && enemyPosition.y == currentPosition.y) && room != 1) {
            killedByEnemy = true;
            loseGameTime = millis();
            lcd.clear();
        }
    }
    if (killedByEnemy) {
        lose();
    }
    if (killedByBomb) {
        lose();
    }
}
