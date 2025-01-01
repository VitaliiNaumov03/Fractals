#include <stdint.h>
#include "raylib.h"

uint32_t xorshift32State = 2463534242;
uint32_t XORShift32(uint32_t *state){ //Randomizer
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

uint32_t RandomVertex(void){
    return XORShift32(&xorshift32State) % 3;
}

void DrawSierpinskiTriangles(const uint32_t windowSize, const Color *backgroundColor){
    const float triangleScale = 0.1; //Gap between triangle vertices and window sides (%)
    const float pointRadiusPixels = 1.0;
    const Color pointColor = {216, 162, 94, 255};
    const Vector2 verticesABC[3] = {{(int)windowSize / 2, (int)(triangleScale * windowSize)},
                                   {(int)(triangleScale * windowSize), (int)(windowSize - triangleScale * windowSize)},
                                   {(int)(windowSize - triangleScale * windowSize), (int)(windowSize - triangleScale * windowSize)}};
    Vector2 pointToDraw = {(int)windowSize / 2, (int)windowSize / 2};

    RenderTexture2D canvas = LoadRenderTexture(windowSize, windowSize);
    BeginTextureMode(canvas);
    ClearBackground(*backgroundColor);
    EndTextureMode();

    while (!WindowShouldClose() && !IsKeyPressed(KEY_SPACE)){
        const Vector2 vertex = verticesABC[RandomVertex()];

        pointToDraw.x = (int)(pointToDraw.x + vertex.x) / 2;
        pointToDraw.y = (int)(pointToDraw.y + vertex.y) / 2;

        BeginTextureMode(canvas);
            DrawCircle(pointToDraw.x, pointToDraw.y, pointRadiusPixels, pointColor);
            if (IsKeyPressed(KEY_R)) ClearBackground(*backgroundColor);
        EndTextureMode();

        BeginDrawing();
            ClearBackground(*backgroundColor);
            DrawTextureRec(canvas.texture, (Rectangle) { 0, 0, (float)canvas.texture.width, (float)-canvas.texture.height }, (Vector2) { 0, 0 }, WHITE);
        EndDrawing();
    }
    
    UnloadRenderTexture(canvas);
}

int main(void){
    const uint32_t windowSize = 650/*0.7 * GetScreenWidth() <= GetScreenHeight() ? 0.7 * GetScreenWidth() : 0.8 * GetScreenHeight()*/;
    const Color backgroundColor = {52, 49, 49};
    InitWindow(windowSize, windowSize, "Fractals");
    SetTargetFPS(240);
    
    while (!WindowShouldClose()){
        DrawSierpinskiTriangles(windowSize, &backgroundColor);
    }
    
    CloseWindow();
    return 0;
}