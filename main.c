#include <stdint.h>
#include "raylib.h"

uint32_t xorshift32State = 2463534242;
uint32_t XORShift32(uint32_t *state){
	uint32_t x = *state;
	x ^= x << 13;
	x ^= x >> 17;
	x ^= x << 5;
	return *state = x;
}

Vector2 GenerateRandomPoint(const uint32_t windowSize){
    Vector2 point;
    point.x = XORShift32(&xorshift32State) % (windowSize - 1);
    point.y = XORShift32(&xorshift32State) % (windowSize - 1);
    return point;
}

uint32_t ThrowDice(void){
    return XORShift32(&xorshift32State) % 5;
}

void DrawSierpinskiTriangles(const uint32_t windowSize, const Color *backgroundColor){
    const Color pointColor = {216, 162, 94};
    const float pointRadiusPixels = 3.0;
    const Vector2 pointA = {(int)windowSize / 2, (int)(0.15 * windowSize)};
    const Vector2 pointB = {(int)(0.15 * windowSize), (int)(windowSize - 0.15 * windowSize)};
    const Vector2 pointC = {(int)(windowSize - 0.15 * windowSize), (int)(windowSize - 0.15 * windowSize)};

    while (!WindowShouldClose && !IsKeyPressed(KEY_SPACE)){
        const Vector2 generatedPoint = GenerateRandomPoint(windowSize);
        const uint32_t numberOnDice = ThrowDice();
        Vector2 centerPoint;

        switch (numberOnDice){
        case 1:
        case 2:
            centerPoint.x = (int)(generatedPoint.x + pointA.x) / 2;
            centerPoint.y = (int)(generatedPoint.y + pointA.y) / 2;
            break;
        case 3:
        case 4:
            centerPoint.x = (int)(generatedPoint.x + pointB.x) / 2;
            centerPoint.y = (int)(generatedPoint.y + pointB.y) / 2;
            break;
        case 5:
        case 6:
            centerPoint.x = (int)(generatedPoint.x + pointC.x) / 2;
            centerPoint.y = (int)(generatedPoint.y + pointC.y) / 2;
            break;
        }

        DrawCircle(centerPoint.x, centerPoint.y, pointRadiusPixels, pointColor);
        if (IsKeyPressed(KEY_R)) ClearBackground(*backgroundColor);
    }
    
}

int main(void){
    const uint32_t windowSize = 0.7 * GetScreenWidth() <= GetScreenHeight() ? 0.7 * GetScreenWidth() : 0.8 * GetScreenHeight();
    const Color backgroundColor = {52, 49, 49};
    InitWindow(windowSize, windowSize, "Fractals");
    SetTargetFPS(60);
    
    while (!WindowShouldClose()){
        DrawSierpinskiTriangles(windowSize, &backgroundColor);
    }
    
    CloseWindow();
    return 0;
}