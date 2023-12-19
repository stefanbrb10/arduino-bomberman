#include "Matrix.h"
#include "LedControl.h"

void displayLoseAnimation() {
    for (int row = 0; row < matrixSize; row++) {
        for (int column = 0; column < matrixSize; column++) {
            if (column == matrixSize - 1 || row == matrixSize - 2)
                matrix[row][column] = 1;
            else matrix[row][column] = 0;
        }
    }
}

void displayWinAnimation() {
    for (int row = 0; row < matrixSize; row++) {
        for (int column = 0; column < matrixSize; column++) {
            matrix[row][column] = wPattern[row][column];
        }
    }
}

void clearMatrix() {
    for (int i = 0; i < matrixSize; i++)
        for (int j = 0; j < matrixSize; j++) {
            matrix[i][j] = false;
            lc.setLed(0, i, j, matrix[i][j]);
        }
}

void displayArrow(uint64_t image) {
    //  byte operation from https://xantorohara.github.io/led-matrix-editor
    for (int i = 0; i < 8; i++) {
        byte row = (image >> i * 8) & 0xFF;
        for (int j = 0; j < 8; j++) {
            lc.setLed(0, i, j, bitRead(row, j));
        }
    }
}