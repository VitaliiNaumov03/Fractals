#include "tree.h"

#define MAX_DEPTH 13
#define SCROLL_SPEED_RAD 0.07f
#define MIN_ANGLE (DEG2RAD * 12) //12°
#define MAX_ANGLE (DEG2RAD * 90) //90°
#define START_LENGTH (WINDOW_SIZE / 4.0f)
#define TRIANGLE_START_POINT (Vector2){WINDOW_SIZE / 2.0f, WINDOW_SIZE - (WINDOW_SIZE - 3 * START_LENGTH) / 2.0f}

void DrawFrame(Vector2 start, float length, float angle, int depth, float angleOffset){
    if (depth <= 0) return;

    const Vector2 endPoint = {
        start.x + length * cosf(angle),
        start.y - length * sinf(angle)
    };

    DrawLineV(start, endPoint, COLOR_TREE);

    DrawFrame(endPoint, length * 0.7f, angle + angleOffset, depth - 1, angleOffset);
    DrawFrame(endPoint, length * 0.7f, angle - angleOffset, depth - 1, angleOffset);
}

void DrawPythagorasTree(void){
    float angleOffset = QUARTER_PI; //Start angle (45°)

    while (!WindowShouldClose()){
        const float wheelMove = GetMouseWheelMove();

        if (wheelMove != 0){
            angleOffset -= wheelMove * SCROLL_SPEED_RAD;
            if (angleOffset < 0.05f) angleOffset = 0.05f;
            if (angleOffset > MAX_ANGLE) angleOffset = MAX_ANGLE;
        }

        BeginDrawing();
            ClearBackground(COLOR_BACKGROUND);
            DrawFrame(TRIANGLE_START_POINT, START_LENGTH, HALF_PI, MAX_DEPTH, angleOffset);
        EndDrawing();

        if (IsKeyPressed(KEY_CHANGE_MODE)) break;
    }
}