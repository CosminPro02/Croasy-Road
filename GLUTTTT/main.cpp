#define _USE_MATH_DEFINES
#include <cmath>
#include <GL/freeglut.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>
#include <sstream>
#include <algorithm>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

const float PLAYER_SIZE = 0.05f;
const float CAR_SIZE = 0.08f;
const float LANE_HEIGHT = 0.25f;
const float PLAYER_SPEED = 0.1f;
const float INITIAL_CAR_SPEED = 0.003f;
const float CAR_SPEED_INCREMENT = 0.0005f;
const int MAX_CARS_PER_LANE = 3;
const int TARGET_SCORE = 25;
const float MIN_CAR_DISTANCE = 0.3f;

struct Car {
    float x, y, speed;
    bool direction;
};

std::vector<Car> cars;
float playerX = 0.0f, playerY = -0.9f;
bool gameOver = false;
bool gameWin = false;
int score = 0;
int level = 1;
float gameSpeed = 1.0f;
float animationTime = 0.0f;

void drawSquare(float x, float y, float size, float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
    glVertex2f(x - size, y - size);
    glVertex2f(x + size, y - size);
    glVertex2f(x + size, y + size);
    glVertex2f(x - size, y + size);
    glEnd();
}

void drawCircle(float x, float y, float radius, float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    for (int i = 0; i <= 20; i++) {
        float angle = 2.0f * M_PI * i / 20;
        glVertex2f(x + cos(angle) * radius, y + sin(angle) * radius);
    }
    glEnd();
}

void drawPlayer(float x, float y, float size) {
    float bounce = sin(animationTime * 8.0f) * 0.01f;
    y += bounce;

    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 8; i++) {
        float angle = 2.0f * M_PI * i / 8;
        glVertex2f(x + cos(angle) * size * 0.8f, playerY - 0.9f - size);
    }
    glEnd();

    drawCircle(x, y, size, 0.95f, 0.95f, 0.85f);

    drawCircle(x - size * 0.6f, y + size * 0.2f, size * 0.4f, 0.9f, 0.9f, 0.8f);
    drawCircle(x + size * 0.6f, y + size * 0.2f, size * 0.4f, 0.9f, 0.9f, 0.8f);

    drawCircle(x, y + size * 0.8f, size * 0.7f, 0.98f, 0.98f, 0.9f);

    glColor3f(1.0f, 0.6f, 0.0f);
    glBegin(GL_TRIANGLES);
    glVertex2f(x - size * 0.2f, y + size * 0.8f);
    glVertex2f(x + size * 0.2f, y + size * 0.8f);
    glVertex2f(x, y + size * 1.1f);
    glEnd();

    drawCircle(x - size * 0.25f, y + size * 0.9f, size * 0.15f, 0.0f, 0.0f, 0.0f);
    drawCircle(x + size * 0.25f, y + size * 0.9f, size * 0.15f, 0.0f, 0.0f, 0.0f);

    drawCircle(x - size * 0.2f, y + size * 0.95f, size * 0.08f, 1.0f, 1.0f, 1.0f);
    drawCircle(x + size * 0.3f, y + size * 0.95f, size * 0.08f, 1.0f, 1.0f, 1.0f);

    glColor3f(1.0f, 0.6f, 0.0f);
    glBegin(GL_TRIANGLES);
    glVertex2f(x - size * 0.3f, y - size);
    glVertex2f(x - size * 0.1f, y - size);
    glVertex2f(x - size * 0.2f, y - size * 1.3f);

    glVertex2f(x + size * 0.1f, y - size);
    glVertex2f(x + size * 0.3f, y - size);
    glVertex2f(x + size * 0.2f, y - size * 1.3f);
    glEnd();
}

void drawCar(float x, float y, float size, bool direction) {
    glColor3f(0.1f, 0.1f, 0.1f);
    glBegin(GL_QUADS);
    glVertex2f(x - size + 0.02f, y - size / 2 - 0.02f);
    glVertex2f(x + size + 0.02f, y - size / 2 - 0.02f);
    glVertex2f(x + size + 0.02f, y + size / 2 - 0.02f);
    glVertex2f(x - size + 0.02f, y + size / 2 - 0.02f);
    glEnd();

    if (direction) {
        for (int i = 0; i < 10; i++) {
            float offset = (i / 10.0f) * size * 2;
            float intensity = 0.6f + (i / 10.0f) * 0.4f;
            glColor3f(intensity, 0.1f, 0.1f);
            glBegin(GL_QUADS);
            glVertex2f(x - size + offset, y - size / 2);
            glVertex2f(x - size + offset + size * 0.2f, y - size / 2);
            glVertex2f(x - size + offset + size * 0.2f, y + size / 2);
            glVertex2f(x - size + offset, y + size / 2);
            glEnd();
        }
    }
    else {
        for (int i = 0; i < 10; i++) {
            float offset = (i / 10.0f) * size * 2;
            float intensity = 0.6f + (i / 10.0f) * 0.4f;
            glColor3f(0.1f, 0.1f, intensity);
            glBegin(GL_QUADS);
            glVertex2f(x - size + offset, y - size / 2);
            glVertex2f(x - size + offset + size * 0.2f, y - size / 2);
            glVertex2f(x - size + offset + size * 0.2f, y + size / 2);
            glVertex2f(x - size + offset, y + size / 2);
            glEnd();
        }
    }

    glColor3f(0.2f, 0.2f, 0.2f);
    glBegin(GL_QUADS);
    glVertex2f(x - size / 2, y + size / 6);
    glVertex2f(x + size / 2, y + size / 6);
    glVertex2f(x + size / 3, y + size / 2);
    glVertex2f(x - size / 3, y + size / 2);
    glEnd();

    glColor3f(0.6f, 0.8f, 1.0f);
    glBegin(GL_QUADS);
    glVertex2f(x - size / 3 + 0.01f, y + size / 6 + 0.01f);
    glVertex2f(x, y + size / 6 + 0.01f);
    glVertex2f(x - size / 6, y + size / 2 - 0.01f);
    glVertex2f(x - size / 3 + 0.01f, y + size / 2 - 0.01f);
    glEnd();

    glBegin(GL_QUADS);
    glVertex2f(x, y + size / 6 + 0.01f);
    glVertex2f(x + size / 3 - 0.01f, y + size / 6 + 0.01f);
    glVertex2f(x + size / 3 - 0.01f, y + size / 2 - 0.01f);
    glVertex2f(x + size / 6, y + size / 2 - 0.01f);
    glEnd();

    glColor3f(0.1f, 0.1f, 0.1f);
    float wheelY = y - size / 2 + 0.02f;
    float frontWheelX = x + size * 0.6f;
    float rearWheelX = x - size * 0.6f;

    drawCircle(frontWheelX, wheelY, size * 0.15f, 0.1f, 0.1f, 0.1f);
    drawCircle(frontWheelX, wheelY, size * 0.1f, 0.4f, 0.4f, 0.4f);
    drawCircle(frontWheelX, wheelY, size * 0.05f, 0.7f, 0.7f, 0.7f);

    drawCircle(rearWheelX, wheelY, size * 0.15f, 0.1f, 0.1f, 0.1f);
    drawCircle(rearWheelX, wheelY, size * 0.1f, 0.4f, 0.4f, 0.4f);
    drawCircle(rearWheelX, wheelY, size * 0.05f, 0.7f, 0.7f, 0.7f);

    if (direction) {
        glColor3f(0.1f, 0.1f, 0.1f);
        glBegin(GL_QUADS);
        glVertex2f(x + size - 0.01f, y - size / 4);
        glVertex2f(x + size, y - size / 4);
        glVertex2f(x + size, y + size / 4);
        glVertex2f(x + size - 0.01f, y + size / 4);
        glEnd();

        glColor3f(1.0f, 1.0f, 0.9f);
        drawCircle(x + size - 0.02f, y + size / 6, size * 0.08f, 1.0f, 1.0f, 0.9f);
        drawCircle(x + size - 0.02f, y - size / 6, size * 0.08f, 1.0f, 1.0f, 0.9f);

        glColor3f(0.8f, 0.1f, 0.1f);
        drawCircle(x - size + 0.02f, y + size / 8, size * 0.05f, 0.8f, 0.1f, 0.1f);
        drawCircle(x - size + 0.02f, y - size / 8, size * 0.05f, 0.8f, 0.1f, 0.1f);
    }
    else {
        glColor3f(0.1f, 0.1f, 0.1f);
        glBegin(GL_QUADS);
        glVertex2f(x - size, y - size / 4);
        glVertex2f(x - size + 0.01f, y - size / 4);
        glVertex2f(x - size + 0.01f, y + size / 4);
        glVertex2f(x - size, y + size / 4);
        glEnd();

        glColor3f(1.0f, 1.0f, 0.9f);
        drawCircle(x - size + 0.02f, y + size / 6, size * 0.08f, 1.0f, 1.0f, 0.9f);
        drawCircle(x - size + 0.02f, y - size / 6, size * 0.08f, 1.0f, 1.0f, 0.9f);

        glColor3f(0.8f, 0.1f, 0.1f);
        drawCircle(x + size - 0.02f, y + size / 8, size * 0.05f, 0.8f, 0.1f, 0.1f);
        drawCircle(x + size - 0.02f, y - size / 8, size * 0.05f, 0.8f, 0.1f, 0.1f);
    }

    glColor3f(0.9f, 0.9f, 0.9f);
    if (direction) {
        glBegin(GL_QUADS);
        glVertex2f(x + size - 0.05f, y - size / 2 + 0.01f);
        glVertex2f(x + size - 0.02f, y - size / 2 + 0.01f);
        glVertex2f(x + size - 0.02f, y - size / 2 + 0.04f);
        glVertex2f(x + size - 0.05f, y - size / 2 + 0.04f);
        glEnd();
    }
    else {
        glBegin(GL_QUADS);
        glVertex2f(x - size + 0.02f, y - size / 2 + 0.01f);
        glVertex2f(x - size + 0.05f, y - size / 2 + 0.01f);
        glVertex2f(x - size + 0.05f, y - size / 2 + 0.04f);
        glVertex2f(x - size + 0.02f, y - size / 2 + 0.04f);
        glEnd();
    }
}

void drawText(float x, float y, const std::string& text, void* font = GLUT_BITMAP_HELVETICA_18) {
    glRasterPos2f(x, y);
    for (char c : text) {
        glutBitmapCharacter(font, c);
    }
}

void drawCenteredText(float y, const std::string& text, void* font = GLUT_BITMAP_HELVETICA_18) {
    float textWidth = 0;
    for (char c : text) {
        textWidth += glutBitmapWidth(font, c);
    }
    float x = -textWidth / 2 / 800.0f;
    drawText(x, y, text, font);
}

bool isPositionSafe(float x, float y, const std::vector<Car>& existingCars) {
    for (const auto& car : existingCars) {
        if (fabs(car.y - y) < 0.1f) {
            if (fabs(car.x - x) < MIN_CAR_DISTANCE) {
                return false;
            }
        }
    }
    return true;
}

void generateCars() {
    cars.clear();
    for (int i = 0; i < 6; ++i) {
        float laneY = -0.6f + i * LANE_HEIGHT;
        bool laneDirection = rand() % 2;
        int carsInLane = 1 + rand() % MAX_CARS_PER_LANE;
        float minSpacing = 0.4f;

        for (int j = 0; j < carsInLane; ++j) {
            Car car;
            car.y = laneY;
            car.direction = laneDirection;
            car.speed = (INITIAL_CAR_SPEED + (rand() % 8) / 4000.0f) * gameSpeed;

            int attempts = 0;
            bool positionOK;
            do {
                positionOK = true;
                car.x = (rand() % 2000 / 1000.0f) - 1.0f;

                for (const auto& other : cars) {
                    if (other.y == car.y && fabs(other.x - car.x) < minSpacing) {
                        positionOK = false;
                        break;
                    }
                }
                attempts++;
            } while (!positionOK && attempts < 20);

            if (attempts < 20) {
                cars.push_back(car);
            }
        }
    }
}

void resetGame() {
    playerX = 0.0f;
    playerY = -0.9f;
    score = 1;
    level = 1;
    gameSpeed = 1.0f;
    gameOver = false;
    gameWin = false;
    animationTime = 0.0f;
    generateCars();
}

void drawRoad() {
    glColor3f(0.2f, 0.7f, 0.2f);
    glBegin(GL_QUADS);
    glVertex2f(-1, 0.8f);
    glVertex2f(1, 0.8f);
    glVertex2f(1, 1);
    glVertex2f(-1, 1);
    glEnd();

    glBegin(GL_QUADS);
    glVertex2f(-1, -1);
    glVertex2f(1, -1);
    glVertex2f(1, -0.8f);
    glVertex2f(-1, -0.8f);
    glEnd();

    glColor3f(0.15f, 0.6f, 0.15f);
    for (int i = 0; i < 50; i++) {
        float x = -1.0f + (i % 10) * 0.2f + (rand() % 20 - 10) / 100.0f;
        float y1 = 0.8f + (rand() % 20) / 100.0f;
        float y2 = -0.8f - (rand() % 20) / 100.0f;

        glBegin(GL_LINES);
        glVertex2f(x, y1);
        glVertex2f(x + 0.01f, y1 + 0.03f);
        glEnd();

        glBegin(GL_LINES);
        glVertex2f(x, y2);
        glVertex2f(x + 0.01f, y2 - 0.03f);
        glEnd();
    }

    for (float y = -0.8f; y <= 0.8f; y += 0.01f) {
        float intensity = 0.15f + 0.05f * sin((y + 0.8f) * 10);
        glColor3f(intensity, intensity, intensity);
        glBegin(GL_QUADS);
        glVertex2f(-1, y);
        glVertex2f(1, y);
        glVertex2f(1, y + 0.01f);
        glVertex2f(-1, y + 0.01f);
        glEnd();
    }

    glColor3f(1.0f, 1.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(-1, -0.8f);
    glVertex2f(1, -0.8f);
    glVertex2f(1, -0.78f);
    glVertex2f(-1, -0.78f);
    glEnd();

    glBegin(GL_QUADS);
    glVertex2f(-1, 0.78f);
    glVertex2f(1, 0.78f);
    glVertex2f(1, 0.8f);
    glVertex2f(-1, 0.8f);
    glEnd();

    glColor3f(1.0f, 1.0f, 0.8f);
    for (float y = -0.6f; y <= 0.6f; y += LANE_HEIGHT) {
        for (float x = -1.0f; x <= 1.0f; x += 0.1f) {
            if ((int)(x * 20) % 3 != 0) continue;

            glBegin(GL_QUADS);
            glVertex2f(x - 0.03f, y - 0.008f);
            glVertex2f(x + 0.03f, y - 0.008f);
            glVertex2f(x + 0.03f, y + 0.008f);
            glVertex2f(x - 0.03f, y + 0.008f);
            glEnd();
        }
    }

    glColor3f(0.1f, 0.1f, 0.1f);
    for (int i = 0; i < 20; i++) {
        float x = -0.8f + (rand() % 160) / 100.0f;
        float y = -0.7f + (rand() % 140) / 100.0f;
        glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x + 0.02f, y);
        glVertex2f(x + 0.02f, y + 0.005f);
        glVertex2f(x, y + 0.005f);
        glEnd();
    }
}

void drawGameInfo() {
    std::stringstream info;
    info << "Level: " << score << "/" << TARGET_SCORE;
    glColor3f(1, 1, 1);
    drawText(-0.95f, 0.9f, info.str());

    glColor3f(0.8f, 0.8f, 0.8f);
    glBegin(GL_QUADS);
    glVertex2f(0.28f, 0.86f);
    glVertex2f(0.92f, 0.86f);
    glVertex2f(0.92f, 0.94f);
    glVertex2f(0.28f, 0.94f);
    glEnd();

    glColor3f(0.2f, 0.2f, 0.2f);
    glBegin(GL_QUADS);
    glVertex2f(0.3f, 0.88f);
    glVertex2f(0.9f, 0.88f);
    glVertex2f(0.9f, 0.92f);
    glVertex2f(0.3f, 0.92f);
    glEnd();

    float progress = (float)score / TARGET_SCORE;
    if (progress > 0) {
        for (int i = 0; i < (int)(progress * 60); i++) {
            float x = 0.3f + (i / 60.0f) * 0.6f;
            float r = 0.2f + (i / 60.0f) * 0.6f;
            float g = 0.8f - (i / 60.0f) * 0.3f;
            glColor3f(r, g, 0.2f);
            glBegin(GL_QUADS);
            glVertex2f(x, 0.88f);
            glVertex2f(x + 0.01f, 0.88f);
            glVertex2f(x + 0.01f, 0.92f);
            glVertex2f(x, 0.92f);
            glEnd();
        }
    }

    std::stringstream percent;
    percent << (int)(progress * 100) << "%";
    glColor3f(1.0f, 1.0f, 1.0f);
    drawText(0.55f, 0.89f, percent.str(), GLUT_BITMAP_HELVETICA_12);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    if (gameOver) {
        glColor4f(0.0f, 0.0f, 0.0f, 0.7f);
        glBegin(GL_QUADS);
        glVertex2f(-1, -1);
        glVertex2f(1, -1);
        glVertex2f(1, 1);
        glVertex2f(-1, 1);
        glEnd();

        glColor3f(1.0f, 0.3f, 0.3f);
        drawCenteredText(0.1f, "GAME OVER!", GLUT_BITMAP_TIMES_ROMAN_24);
        glColor3f(1.0f, 1.0f, 1.0f);
        drawCenteredText(-0.05f, "Score: " + std::to_string(score));
        drawCenteredText(-0.2f, "Press UP to Restart");
    }
    else if (gameWin) {
        glColor4f(1.0f, 1.0f, 0.0f, 0.3f);
        glBegin(GL_QUADS);
        glVertex2f(-1, -1);
        glVertex2f(1, -1);
        glVertex2f(1, 1);
        glVertex2f(-1, 1);
        glEnd();

        glColor3f(0.2f, 0.8f, 0.2f);
        drawCenteredText(0.1f, "YOU WIN!", GLUT_BITMAP_TIMES_ROMAN_24);
        glColor3f(1.0f, 1.0f, 1.0f);
        drawCenteredText(-0.05f, "Level: " + std::to_string(level));
        drawCenteredText(-0.2f, "Press UP to Continue");
    }
    else {
        drawRoad();
        drawGameInfo();

        drawPlayer(playerX, playerY, PLAYER_SIZE);

        for (auto& car : cars) {
            drawCar(car.x, car.y, CAR_SIZE, car.direction);
        }
    }

    glutSwapBuffers();
}

void update(int value) {
    animationTime += 0.016f;

    if (!gameOver && !gameWin) {
        for (auto& car : cars) {
            if (car.direction) {
                car.x += car.speed;
                if (car.x > 1.1f) car.x = -1.1f;
            }
            else {
                car.x -= car.speed;
                if (car.x < -1.1f) car.x = 1.1f;
            }
        }

        for (auto& car : cars) {
            if (fabs(playerX - car.x) < (PLAYER_SIZE + CAR_SIZE) &&
                fabs(playerY - car.y) < (PLAYER_SIZE + CAR_SIZE)) {
                gameOver = true;
            }
        }

        if (playerY > 0.9f) {
            score++;
            if (score >= TARGET_SCORE) {
                gameWin = true;
            }
            else {
                playerY = -0.9f;
                level++;
                gameSpeed += 0.1f;
                generateCars();
            }
        }

        playerX = std::max(-1.0f + PLAYER_SIZE, std::min(1.0f - PLAYER_SIZE, playerX));
    }

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void keyboard(int key, int, int) {
    switch (key) {
    case GLUT_KEY_UP:
        if (gameOver || gameWin) {
            resetGame();
        }
        else {
            playerY += PLAYER_SPEED;
        }
        break;
    case GLUT_KEY_DOWN:
        if (!gameOver && !gameWin) {
            playerY -= PLAYER_SPEED;
        }
        break;
    case GLUT_KEY_LEFT:
        if (!gameOver && !gameWin) {
            playerX -= PLAYER_SPEED;
        }
        break;
    case GLUT_KEY_RIGHT:
        if (!gameOver && !gameWin) {
            playerX += PLAYER_SPEED;
        }
        break;
    }
}

void init() {
    glClearColor(0.5f, 0.8f, 1.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

int main(int argc, char** argv) {
    srand(static_cast<unsigned>(time(0)));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Crossy Road ");

    init();
    resetGame();
    glutDisplayFunc(display);
    glutSpecialFunc(keyboard);
    glutTimerFunc(0, update, 0);

    glutMainLoop();
    return 0;
}

