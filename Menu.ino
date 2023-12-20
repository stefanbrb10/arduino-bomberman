#include <LiquidCrystal.h>
#include "Menu.h"


void menu() {
    // help the user see what he has to do with the joystick
    if (!horizontalArrowShowed) {
        displayArrow(horizontalArrows);
        horizontalArrowShowed = true; // show it only once
    }
    if (inMainMenu)
        displayMenu(); // display menu oprions (main)
    if (!inMainMenu && inSubMenu && !toStartGame) {
        handleMenuOption();  // act accordingly to what the player has chosen through joystick
    }
    if (inSubSubMenu) {
        handleSubMenuOption(); // move around options
    }
    if (millis() - startToningTime > toneDuration && toning) {
        noTone(buzzerPin);
        toning = false;
    }
}

// custom char that looks like a doodle
void displayDoodleCharacters() {
    lcd.createChar(0, doodleCharacter);
    lcd.setCursor(0, 0);
    lcd.write((uint8_t)0);
    lcd.setCursor(lcdCols - 1, 0);
    lcd.write((uint8_t)0);
}


void handleMenuOption() {
    if (currentMenuOption == 0) {
        // start the game and exit the menu
        toStartGame = true;
        inMainMenu = false;
        inSubMenu = false;
    }
    if (currentMenuOption == 1) { // display the number of options and the current option
        String settingsText = "Settings [";
        settingsText += String(currentSubmenuOption + 1);
        settingsText += "/";
        settingsText += String(settingsCount);
        settingsText += "]";
        move(currentSubmenuOption, settingsOptions, settingsCount, settingsText);  // scroll though the options
    }
    if (currentMenuOption == 2)
        displayAbout();
}

void handleSubMenuOption() {
    if (!verticalArrowShowed) {
        displayArrow(verticalArrows); // the user should move the joystick up/down
        verticalArrowShowed = true;
    }
    if (currentMenuOption == 1) {
        if (currentSubmenuOption == 0) {
            readNumber("Matrix brightness", matrixBrightness, 0, brightnessStep);  // get the user values
        } else if (currentSubmenuOption == 1)
            readNumber("LCD brightness", lcdBrightness, 0, brightnessStep);
        else if (currentSubmenuOption == 2) {
            String toShow = "Before: " + playerName;
            readString(toShow, playerName);
        }
    }
}

void displayCenteredText(String text, int line) {
    lcd.setCursor(0, line);
    int textLen = text.length();
    int centerIndex = (lcdCols - textLen) / 2;  // center of lcd
    for (int i = 0; i < centerIndex; i++)
        lcd.print(' ');  // the actual text is surrounded by whitespace
    lcd.print(text);
    for (int i = 0; i < centerIndex; i++)
        lcd.print(' ');
}

void displayGreetingMessage() {
    displayCenteredText("Welcome to", 0);
    displayCenteredText("Bomberman", 1);
    displayDoodleCharacters();
    delay(greetingTime);
    lcd.clear();
}

void displayMenu() {
    String mainMenuText = "Main menu [";
    mainMenuText += String(currentMenuOption + 1);
    mainMenuText += "/";
    mainMenuText += String(menuCount);
    mainMenuText += "]";
    move(currentMenuOption, menuOptions, menuCount, mainMenuText);  // scroll through menu options
}


// function to scroll 2 texts at the same time in a function (i know it is not ok but i didn't have time)
void scrollText(String text, String text2, int line) {
    // show the scrolling text if it is bigger than the lcd
    if (millis() - lastScroll > scrollingSpeed) {
        if (text != "") {
            lcd.setCursor(0, line);
            lcd.print(text.substring(currentCharFirstLine, currentCharFirstLine + lcdCols));
            currentCharFirstLine++;
            if (currentCharFirstLine >= text.length()) {
                currentCharFirstLine = 0;
            }
        }
        if (text2 != "") {
            lcd.setCursor(1, line + 1);
            //lcd.print(text2);
            lcd.print(text2.substring(currentCharSecondLine, currentCharSecondLine + lcdCols));
            currentCharSecondLine++;
            if (currentCharSecondLine >= text2.length()) {
                currentCharSecondLine = 0;
            }
        }
        lastScroll = millis();
    }
}

void move(int &option, String optionList[], int len, String title) {
    displayCenteredText(title, 0);
    joystickPosition.y = analogRead(pinY);
    displayCenteredText(optionList[option], 1);  // option

    if (!joyMoved) {
        if (joystickPosition.y < minJoyThreshold && option < len - 1) {
            option++;
            if (soundOn)
                tone(buzzerPin, 587);
            buzzerStartTime = millis();
        } else if (joystickPosition.y > maxJoyThreshold && option > 0) {
            option--;
            if (soundOn)
                tone(buzzerPin, 330);
            buzzerStartTime = millis();
        }
        joyMoved = true;
    }
    if (millis() - buzzerStartTime > buzzerDuration)
        noTone(buzzerPin);

    if (joystickPosition.y >= minJoyThreshold && joystickPosition.y <= maxJoyThreshold)
        joyMoved = false;
}

void readSw() {
    int reading = digitalRead(pinSW);
    if (reading != lastSwState) {
        lastDebounceTime = millis();
    }

    if ((millis() - lastDebounceTime) > debounceDelay) {
        if (reading != swState) {
            swState = reading;
            if (swState == HIGH) {
                if (firstImpulse)
                    firstImpulse = false;  // ignore first impulse
                else if (inMainMenu) {
                    lcd.clear();
                    inMainMenu = false;
                    inSubMenu = true;
                } else if (inSubMenu) {
                    verticalArrowShowed = false;
                    handleSubMenuLogic();
                } else if (inSubSubMenu) {
                    handleSubSubMenuLogic();  // for now, save values to eeprom
                    horizontalArrowShowed = false;
                }
                if (inGame && (lost || won)) {
                    exitGame();  // refresh game
                }
            }
        }
    }
    lastSwState = reading;
}

void moveMenu(bool &state1, bool &state2, char *direction) {
    lcd.clear();
    state1 = direction == "Back" ? true : false;
    //if(direction == "Back");
    state2 = !state1;
    clearMatrix();
}

// it is called when the user pressed SW while he was in settings
void handleSubMenuLogic() {
    clearMatrix();
    if ((currentMenuOption == 1 && currentSubmenuOption == settingsCount - 1) || currentMenuOption == 2) {
        moveMenu(inMainMenu, inSubMenu, "Back");
        horizontalArrowShowed = false;
    } else if (currentMenuOption == 1) {
        if (currentSubmenuOption == 3) {
            soundOn = !soundOn;
            horizontalArrowShowed = false;
            EEPROM.put(eepromSoundAddress, soundOn);
            if (soundOn) {
                tone(buzzerPin, 587);
                startToningTime = millis();
                toning = true;
            }
        } else moveMenu(inSubMenu, inSubSubMenu, "Forward");
    }
}

// the user has pressed SW while he was in the last menu level
void handleSubSubMenuLogic() {
    if (currentMenuOption == 1) {
        if (currentSubmenuOption == 0 && matrixBrightness >= minBrightness && matrixBrightness <= maxBrightness) {
            EEPROM.put(matrixBrightnessAddress, matrixBrightness * 4);
            lc.setIntensity(0, matrixBrightness);
        }
        if (currentSubmenuOption == 1 && lcdBrightness >= minBrightness && lcdBrightness <= maxBrightness) {
            EEPROM.put(lcdBrightnessAddress, lcdBrightness);
        }
        if (currentSubmenuOption == 2) {
            playerName = readingWord;
            writeStringToEEPROM(eepromOffset, playerName);
        }
        inSubSubMenu = false;
        inSubMenu = true;
    }
}

void readNumber(String numToDisplay, int &number, int lowerBound, int upperBound) {
    displayCenteredText(numToDisplay, 0);
    joystickPosition.x = analogRead(pinX);
    displayCenteredText(String(number), 1);

    if (!joyMoved) {
        if (joystickPosition.x < minJoyThreshold && number < upperBound)
            number++;
        else if (joystickPosition.x > maxJoyThreshold && number > lowerBound)
            number--;
        joyMoved = true;
    }

    if (joystickPosition.x >= minJoyThreshold && joystickPosition.x <= maxJoyThreshold)
        joyMoved = false;
}


// code from roboticsbackend.com/arduino-write-string-in-eeprom/
// each char is in an address
int writeStringToEEPROM(int addrOffset, const String &strToWrite) {
    byte len = strToWrite.length();
    EEPROM.put(addrOffset, len);
    for (int i = 0; i < len; i++) {
        EEPROM.put(addrOffset + 1 + i, strToWrite[i]);
    }
    return addrOffset + 1 + len;
}

int readStringFromEEPROM(int addrOffset, String *strToRead) {
    int newStrLen = EEPROM.read(addrOffset);
    char data[newStrLen + 1];
    for (int i = 0; i < newStrLen; i++) {
        data[i] = EEPROM.read(addrOffset + 1 + i);
    }
    data[newStrLen] = '\0';
    *strToRead = String(data);
    return addrOffset + 1 + newStrLen;
}

void initEEPROM() {
    lcdBrightness = EEPROM.read(lcdBrightnessAddress);  // use the EEPROM vlue
    soundOn = EEPROM.read(eepromSoundAddress);
    matrixBrightness = EEPROM.read(matrixBrightnessAddress);
    matrixBrightness /= brightnessStep;
    eepromOffset = 2;

    readStringFromEEPROM(eepromOffset, &playerName);
}

void putLeaderBoard(int myScore, String myName) {
    int firstLeaderScoreOffset = readStringFromEEPROM(firstLeaderOffset, &leaderBoard[0].name);
    leaderBoard[0].score = EEPROM.read(firstLeaderScoreOffset);

    int secondLeaderScoreOffset = readStringFromEEPROM(secondLeaderOffset, &leaderBoard[1].name);
    leaderBoard[1].score = EEPROM.read(secondLeaderScoreOffset);

    int thirdLeaderScoreOffset = readStringFromEEPROM(thirdLeaderOffset, &leaderBoard[2].name);
    leaderBoard[2].score = EEPROM.read(thirdLeaderScoreOffset);

    String toDisplay = "";
    toDisplay += myName;
    toDisplay += ":";
    String scoreStr = String(myScore);
    toDisplay += scoreStr;

    if (myScore < leaderBoard[0].score) {
        leaderBoard[2] = leaderBoard[1];
        leaderBoard[1] = leaderBoard[0];
        leaderBoard[0].score = myScore;
        leaderBoard[0].name = myName;
        toDisplay += "(first)";

        writeStringToEEPROM(firstLeaderOffset, leaderBoard[0].name);
        EEPROM.put(firstLeaderScoreOffset, myScore);
    } else if (myScore < leaderBoard[1].score) {
        leaderBoard[2] = leaderBoard[1];
        leaderBoard[1].score = myScore;
        leaderBoard[1].name = myName;

        //toDisplay += "(second)";


        writeStringToEEPROM(secondLeaderOffset, leaderBoard[1].name);
        EEPROM.put(secondLeaderScoreOffset, myScore);
    } else if (myScore < leaderBoard[2].score) {
        leaderBoard[2].score = myScore;
        leaderBoard[2].name = myName;

        //toDisplay += "(third)";


        writeStringToEEPROM(thirdLeaderOffset, leaderBoard[2].name);
        EEPROM.put(thirdLeaderScoreOffset, myScore);
    }

    for (int i = 0; i < 3; i++) {
        Serial.print(leaderBoard[i].name);
        Serial.print("  ");
        Serial.print(leaderBoard[i].score);
        Serial.println();
    }
    Serial.println(toDisplay);
    displayCenteredText(toDisplay, 0);
}

void readString(String toDisplay, String &word) {
    displayCenteredText(toDisplay, 0);
    joystickPosition.x = analogRead(pinX);
    joystickPosition.y = analogRead(pinY);
    if (!joyMoved) {
        if (joystickPosition.y > maxJoyThreshold && cursorStringPosition > 0) {
            readingWord[cursorStringPosition] = ' '; // erase
            cursorStringPosition--;
        }
        if (joystickPosition.y < minJoyThreshold && cursorStringPosition < 15) {
            cursorStringPosition++;
            if (readingWord[cursorStringPosition] == ' ')
                readingWord[cursorStringPosition] = 'a';
            else
                readingWord += 'a';
        }

        if (joystickPosition.x > maxJoyThreshold) {
            readingWord[cursorStringPosition]++;
        }
        if (joystickPosition.x < minJoyThreshold) {
            readingWord[cursorStringPosition]--;
        }
        joyMoved = true;
    }

    if (joystickPosition.x >= minJoyThreshold && joystickPosition.x <= maxJoyThreshold && joystickPosition.y >= minJoyThreshold && joystickPosition.y <= maxJoyThreshold)
        joyMoved = false;

    lcd.setCursor(0, 1);
    lcd.print(readingWord);
    if (millis() - lastCursorShow > 20) {
        lcd.setCursor(cursorStringPosition, 1);
        lcd.print(" ");
        lastCursorShow = millis();
    }
}

void displayAbout() {
    scrollText("", aboutInfo, 0);
    //PlaySong();
}


String millisToMinutes(unsigned long value) {
    int seconds = value / 1000;
    int minutes = seconds / 60;
    seconds %= 60;
    String formattedTime = String(minutes) + ":" + (seconds < 10 ? "0" : "") + String(seconds);
    return formattedTime;
}

void clock() {  // count the time from the beginning of the game
    if (millis() - lastTimeShow > showRate) {
        String textToDisplay = "Time:";
        textToDisplay += millisToMinutes(millis() - startGameTime);

        if (currentLevel == 3) {
            unsigned long timeUntilRadiation = 30000 - (millis() - thirdLevelStartTime);
            // Serial.println(timeUntilRadiation);
            if (timeUntilRadiation < 2323232) {
                textToDisplay += "R:";
                textToDisplay += millisToMinutes(timeUntilRadiation);
            }
        }

        displayCenteredText(textToDisplay, 0);
        lastTimeShow = millis();
    }
}

void displayRoom() {
    String textToDisplay = "Room:";
    textToDisplay += String(room);
    textToDisplay += " ";
    textToDisplay += "Level:";
    textToDisplay += String(currentLevel);
    displayCenteredText(textToDisplay, 1);
}

void exitGame() {
    inGame = false;
    inMainMenu = true;
    lost = false;
    won = false;
    killedByEnemy = false;
    killedByRadiation = false;
    clearMatrix();
}