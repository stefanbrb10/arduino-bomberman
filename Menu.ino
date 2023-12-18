#include <LiquidCrystal.h>
#include "Menu.h"


void menu() {
    if(!horizontalArrowShowed){
        displayArrow(horizontalArrows);
        horizontalArrowShowed = true;
        Serial.println("aici");
    }
    if (inMainMenu)
        displayMenu();
    if (!inMainMenu && inSubMenu && !toStartGame) {
        handleMenuOption();  // act accordingly to what the player has chosen through joystick
    }
    if (inSubSubMenu) {
        handleSubMenuOption();
    }
    if (millis() - startToningTime > toneDuration && toning) {
        noTone(buzzerPin);
        toning = false;
    }
}

void displayDoodleCharacters() {
    lcd.createChar(0, doodleCharacter);
    lcd.setCursor(0, 0);
    lcd.write((uint8_t)0);
    lcd.setCursor(lcdCols - 1, 0);
    lcd.write((uint8_t)0);
}


void handleMenuOption() {
    if (currentMenuOption == 0) {
        toStartGame = true;
        inMainMenu = false;
        inSubMenu = false;
    }
    if (currentMenuOption == 1) {
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
    if(!verticalArrowShowed){
        displayArrow(verticalArrows);
        verticalArrowShowed = true;
    }
    if (currentMenuOption == 1) {
        if (currentSubmenuOption == 0) {
            readNumber("Matrix brightness", matrixBrightness, 0, 4);  // get the user values
        } else if (currentSubmenuOption == 1)
            readNumber("LCD brightness", lcdBrightness, 0, 4);
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
                    exitGame();  // resfresh game
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
    matrixBrightness /= 4;
    eepromOffset = 2;
    int newStr1AddrOffset = readStringFromEEPROM(eepromOffset, &playerName);
}

void readString(String toDisplay, String &word) {
    displayCenteredText(toDisplay, 0);
    joystickPosition.x = analogRead(pinX);
    joystickPosition.y = analogRead(pinY);
    if (!joyMoved) {
        if (joystickPosition.y > maxJoyThreshold && cursorStringPosition > 0) {
            cursorStringPosition--;
        }
        if (joystickPosition.y < minJoyThreshold && cursorStringPosition < 15) {
            cursorStringPosition++;
            if (cursorStringPosition > readingWord.length() - 1) {
                readingWord += 'a';
            }
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
    //scrollText(aboutInfo, 1);
    PlaySong();
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
        String textToDisplay = "Time: ";
        textToDisplay += millisToMinutes(millis() - startGameTime);
        displayCenteredText(textToDisplay, 0);
        lastTimeShow = millis();
    }
}

void displayRoom() {
    String textToDisplay = "Room: ";
    textToDisplay += String(room);
    displayCenteredText(textToDisplay, 1);
}

void exitGame() {
    inGame = false;
    inMainMenu = true;
    lost = false;
    won = false;
    killedByEnemy = false;
    clearMatrix();
}
