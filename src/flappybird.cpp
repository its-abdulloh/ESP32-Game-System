#include "display.h"
#include "esp32-hal-gpio.h"
#include "input.h"

#define BIRD_SIZE 16
#define OLED_WIDTH 128
#define OLED_HEIGHT 64

#define FRAME_DELAY 50

#define STATE_START 0
#define STATE_PLAYING 1
#define STATE_GAMEOVER 2

#define GAP_SIZE 30
#define WALL_THICKNESS 10


// 'flappy1', 16x16px
const uint8_t flappy_bird_sprite[] = {
	0x00, 0x00, 0x00, 0x00, 0xe0, 0x01, 0x18, 0x06, 0x04, 0x04, 0x82, 0x3d, 0x82, 0x41, 0x02, 0x40, 
	0x02, 0x40, 0x02, 0x38, 0x06, 0x08, 0x1c, 0x04, 0xf0, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

namespace FlappyBird {

    // All game-specific variables
    int score = 0;
    int gravity = 0;
    int current_state = STATE_START;

    int birdX = OLED_WIDTH / 4;
    int birdY = 0;
    int bestScore = 0;

    int wallX[2];
    int gapY[2];

    // Internal helper functions
    void wipeScreen() {
        display.clearBuffer();
        display.sendBuffer();
    }

    void updateGame() {
        display.clearBuffer();

        if (a_pressed()) gravity = -4;

        gravity++;
        birdY += gravity;

        birdY = constrain(birdY, 0, OLED_HEIGHT - BIRD_SIZE);

        if (birdY == OLED_HEIGHT - BIRD_SIZE)
            gravity = -2;

        display.drawXBMP(birdX, birdY, BIRD_SIZE, BIRD_SIZE, flappy_bird_sprite);

        for (int i = 0; i < 2; i++) {
            display.drawBox(wallX[i], 0, WALL_THICKNESS, gapY[i]);

            display.drawBox(
                wallX[i],
                gapY[i] + GAP_SIZE,
                WALL_THICKNESS,
                OLED_HEIGHT - gapY[i] + GAP_SIZE
            );

            wallX[i] -= 4;

            if (wallX[i] < -WALL_THICKNESS) {
                wallX[i] = OLED_WIDTH;
                gapY[i] = random(10, OLED_HEIGHT - GAP_SIZE - 10);
            }

            if (wallX[i] == birdX) {
                score++;
                bestScore = max(bestScore, score);
            }

            if (
                (birdX + BIRD_SIZE > wallX[i] &&
                 birdX < wallX[i] + WALL_THICKNESS) &&
                (birdY < gapY[i] ||
                 birdY + BIRD_SIZE > gapY[i] + GAP_SIZE)
            ) {
                current_state = STATE_GAMEOVER;
            }
        }

        display.setCursor(OLED_WIDTH / 2 - 6, 5);
        display.print(score);
        display.sendBuffer();
    }

    void showGameOver() {
        wipeScreen();

        display.setCursor(OLED_WIDTH / 2 - 30, OLED_HEIGHT / 2 - 20);
        display.print(F("GAME OVER"));

        display.setCursor(OLED_WIDTH / 2 - 3, OLED_HEIGHT / 2 - 10);
        display.print(score);

        display.setCursor(OLED_WIDTH / 2 - 30, OLED_HEIGHT - 26);
        display.print(F("BEST SCORE"));

        display.setCursor(OLED_WIDTH / 2 - 3, OLED_HEIGHT - 18);
        display.print(bestScore);

        display.sendBuffer();

        while (a_pressed());

        birdY = OLED_HEIGHT / 2;
        gravity = -4;
        score = 0;

        wallX[0] = OLED_WIDTH;
        gapY[0] = OLED_HEIGHT / 2 - GAP_SIZE / 2;

        wallX[1] = OLED_WIDTH + OLED_WIDTH / 2;
        gapY[1] = random(10, OLED_HEIGHT - GAP_SIZE - 10);

        while (!a_pressed());

        wipeScreen();
        current_state = STATE_PLAYING;
    }

    void showStartScreen() {
        display.clearBuffer();

        display.setCursor(OLED_WIDTH / 2 - 30, OLED_HEIGHT / 2 - 8);
        display.print(F("ESP BIRD"));

        display.setCursor(OLED_WIDTH / 2 - 42, OLED_HEIGHT / 2 + 8);
        display.print(F("Press to Start"));

        display.sendBuffer();

        if (a_pressed()) {
            wipeScreen();

            birdY = OLED_HEIGHT / 2;
            gravity = -4;
            score = 0;

            wallX[0] = OLED_WIDTH;
            gapY[0] = OLED_HEIGHT / 2 - GAP_SIZE / 2;

            wallX[1] = OLED_WIDTH + OLED_WIDTH / 2;
            gapY[1] = random(10, OLED_HEIGHT - GAP_SIZE - 10);

            current_state = STATE_PLAYING;
        }
    }

    // Public game function
    void play() {
      if (current_state == STATE_PLAYING) {
          updateGame();
      }
      else if (current_state == STATE_GAMEOVER) {
          showGameOver();
      }
      else {
          showStartScreen();
      }

      delay(FRAME_DELAY);
    }
}
