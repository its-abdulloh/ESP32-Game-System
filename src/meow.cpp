#include "meow.h"
#include "display.h"
#include "input.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

namespace Meow {

    // Game settings
    const int baseline = 44;

    int spawnX = 150;
    int nextSpawnX = 150;
    int minGap = 80;
    int maxGap = 120;

    float gravity = 0.8;
    float speed = 2.0;

    unsigned long score = 0;
    unsigned long lastScoreTime = 0;

    int nextDifficulty = 100;

    bool playing = false;
    bool gameover = false;

    // Cat bitmap
    const uint8_t epd_bitmap_cat[] = {
        0x00, 0x00, 0x12, 0x01, 0xf2, 0x01,
        0x56, 0x01, 0xfc, 0x01, 0xfc, 0x00,
        0xfc, 0x00, 0xa4, 0x00, 0xa4, 0x00
    };

    const uint8_t* cat_bitmap() {
        return epd_bitmap_cat;
    }

    // Obstacle class
    class Obstacle {
    public:
        int x;
        int y;
        int width;
        int height;
        bool spawned = false;

        Obstacle() {
            width = random(3, 8);
            height = random(4, 10);
        }

        void spawn() {
            x = nextSpawnX;
            y = baseline - height;
            spawned = true;
        }

        void update() {
            if (spawned) {
                x -= speed;
            }
        }

        void draw() {
            if (spawned && x < SCREEN_WIDTH) {
                display.drawBox(x, y, width, height);
            }
        }
    };

    // Player class
    class Player {
    public:
        int width;
        int height;
        int x;
        float y;
        int defaultY;
        float vely = 0;
        float jumpStrength = -6;

        Player() {
            width = 10;
            height = 10;
            x = 10;
            defaultY = baseline - height;
            y = baseline - height;
        }

        void draw() {
            display.drawXBMP(x, (int)y, 10, 9, cat_bitmap());
        }
    };

    // Game objects
    Obstacle obstacles[5];
    Obstacle* available[5];
    Obstacle* active[5];

    Player cat;

    // Internal function declarations
    void spawn(int availableLength);
    int getUnspawned();
    int getSpawned();
    int detectCollision(int activeLength);
    void reset();
    void showStartScreen();
    void update_game();

    // Main game function
    void play() {

        if (gameover) {
            if (a_pressed()) {
                reset();
                delay(200);
            }

            display.clearBuffer();

            int length = getSpawned();

            for (int i = 0; i < length; i++) {
                active[i]->draw();
            }

            display.drawHLine(0, baseline, SCREEN_WIDTH);
            cat.draw();

            display.setCursor(40, 20);
            display.print("GAMEOVER");

            display.sendBuffer();
            delay(10);
        }

        else if (playing) {
            update_game();
        }

        else {
            showStartScreen();
        }
    }

    // Game update
    void update_game() {

        display.clearBuffer();

        // Jumping and landing
        if (a_pressed()) {
            if (cat.y == cat.defaultY) {
                cat.vely = cat.jumpStrength;
            }
        }

        cat.vely += gravity;
        cat.y += cat.vely;

        if (cat.y >= cat.defaultY) {
            cat.y = cat.defaultY;
            cat.vely = 0;
        }

        // Score
        if (millis() - lastScoreTime >= 100) {
            score++;
            lastScoreTime += 100;
        }

        // Difficulty
        if (score >= nextDifficulty && nextDifficulty <= 10000) {
            speed *= 1.1;
            nextDifficulty += 100;
        }

        // Obstacles
        int availableLength = getUnspawned();

        spawn(availableLength);

        nextSpawnX -= speed;

        int activeLength = getSpawned();

        int isCollided = detectCollision(activeLength);

        if (isCollided) {
            gameover = true;
        }

        for (int i = 0; i < activeLength; i++) {

            active[i]->update();

            if (active[i]->x < 0) {
                active[i]->spawned = false;
            }
            else {
                active[i]->draw();
            }
        }

        display.drawHLine(0, baseline, SCREEN_WIDTH);

        display.setCursor(80, 10);
        display.print(score);

        cat.draw();

        display.sendBuffer();

        delay(16);
    }

    // Start screen
    void showStartScreen() {

        display.clearBuffer();

        display.setCursor(30, 34);
        display.print("Meow Jump");

        display.setCursor(30,54);
        display.print("START");

        display.sendBuffer();

        if (a_pressed()) {
            playing = true;
        }
        
        delay(10);
    }

    // Reset game
    void reset() {

        playing = true;
        gameover = false;

        // Score
        score = 0;
        lastScoreTime = 0;
        nextDifficulty = 100;

        // Speed
        speed = 2.0;

        // Spawning
        nextSpawnX = spawnX;

        // Cat
        cat.x = 10;
        cat.y = cat.defaultY;
        cat.vely = 0;

        // Obstacles
        for (int i = 0; i < 5; i++) {
            obstacles[i].spawned = false;
            obstacles[i].x = 0;
        }
    }

    // Spawn obstacle
    void spawn(int availableLength) {

        if (availableLength == 0) {
            return;
        }

        if (nextSpawnX <= spawnX) {

            int n = random(0, availableLength);

            available[n]->spawn();

            int gap = random(minGap, maxGap);

            nextSpawnX += gap;
        }
    }

    // Find unspawned obstacles
    int getUnspawned() {

        int availableCount = 0;

        for (int i = 0; i < 5; i++) {

            if (!obstacles[i].spawned) {

                available[availableCount] = &obstacles[i];

                availableCount++;
            }
        }

        return availableCount;
    }

    // Find active obstacles
    int getSpawned() {

        int activeCount = 0;

        for (int i = 0; i < 5; i++) {

            if (obstacles[i].spawned) {

                active[activeCount] = &obstacles[i];

                activeCount++;
            }
        }

        return activeCount;
    }

    // Collision detection
    int detectCollision(int length) {

        for (int i = 0; i < length; i++) {

            if (
                cat.x < active[i]->x + active[i]->width &&
                cat.x + cat.width > active[i]->x &&
                cat.y < active[i]->y + active[i]->height &&
                cat.y + cat.height > active[i]->y
            ) {
                return 1;
            }
        }

        return 0;
    }

}
