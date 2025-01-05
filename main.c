#include "raylib.h"

#define SQRT3 1.73205080757
#define KEY_CLEAR KEY_C
#define KEY_CHANGE_MODE KEY_SPACE
#define KEY_FPS KEY_F

typedef unsigned int uint;

uint xorshift32State = 2463534242;
uint XORShift32(uint *state){ //Randomizer
	uint x = *state;
	x ^= x << 13;
	x ^= x >> 17;
	x ^= x << 5;
	return *state = x;
}

uint RandomIntFrom0To(const uint maxValue){
    return XORShift32(&xorshift32State) % (maxValue + 1);
}

void DrawSierpinskiTriangles(const uint windowSize, const Color *backgroundColor){
    //Point parameters
    const Color pointColor = {216, 162, 94, 255};
    Vector2 pointToDraw = {windowSize / 2.0f, windowSize / 2.0f};

    //Triangle parameters
    const float scale = 0.8f; //Make triangle smaller to fit in screen
    const uint sideLength = scale * ((2 * windowSize) / SQRT3);
    const uint height = (SQRT3 / 2) * sideLength;
    const Vector2 verticesABC[3] = {{windowSize / 2.0f, (windowSize - height) / 2.0f},
                                   {(windowSize - sideLength) / 2.0f, (windowSize + height) / 2.0f},
                                   {(windowSize + sideLength) / 2.0f, (windowSize + height) / 2.0f}};
    
    bool showFPS = false;

    //Create blank canvas
    RenderTexture2D canvas = LoadRenderTexture(windowSize, windowSize);
    BeginTextureMode(canvas);
    ClearBackground(*backgroundColor);
    EndTextureMode();

    while (!WindowShouldClose()){
        const Vector2 vertex = verticesABC[RandomIntFrom0To(2)]; //Choose 1 of 3 triangle vertices

        //Calculate middle point between our start point and chosen vertex
        pointToDraw.x = (pointToDraw.x + vertex.x) / 2.0f;
        pointToDraw.y = (pointToDraw.y + vertex.y) / 2.0f;

        //Draw on canvas
        BeginTextureMode(canvas);
            DrawPixel(pointToDraw.x, pointToDraw.y, pointColor);
            if (IsKeyPressed(KEY_CLEAR)) ClearBackground(*backgroundColor);
        EndTextureMode();

        //Display canvas on screen
        BeginDrawing();
            ClearBackground(*backgroundColor);
            DrawTextureRec(canvas.texture, (Rectangle) { 0, 0, canvas.texture.width, -canvas.texture.height }, (Vector2) { 0, 0 }, WHITE);
            if (showFPS) DrawFPS(0, 0);
        EndDrawing();

        if (IsKeyPressed(KEY_CHANGE_MODE)) break;
        if (IsKeyPressed(KEY_FPS)) showFPS = !showFPS;
    }
    
    UnloadRenderTexture(canvas);
}

void DrawBarnsleyFern(const uint windowSize, const Color *backgroundColor){
    //Point parameters
    const Color pointColor = GREEN;
    float x = 0, y = 0; //Start point
    float nextX, nextY;

    //Fern parameters
    const float scaleX = 0.17f * windowSize;
    const float scaleY = 0.09f * windowSize;
    const uint offsetX = windowSize / 2;
    const uint offsetY = windowSize - 0.05 * windowSize;
    
    uint randomValue;
    bool showFPS = false;

    //Create blank canvas
    RenderTexture2D canvas = LoadRenderTexture(windowSize, windowSize);
    BeginTextureMode(canvas);
    ClearBackground(*backgroundColor);
    EndTextureMode();

    while (!WindowShouldClose()){
        randomValue = RandomIntFrom0To(100);

        if (randomValue < 1){
            nextX = 0.0f;
            nextY = 0.16f * y;
        }
        else if (randomValue < 86){
            nextX = 0.85f * x + 0.04f * y;
            nextY = -0.04f * x + 0.85f * y + 1.6f;
        }
        else if (randomValue < 93){
            nextX = 0.2f * x - 0.26f * y;
            nextY = 0.23f * x + 0.22f * y + 1.6f;
        }
        else{
            nextX = -0.15f * x + 0.28f * y;
            nextY = 0.26f * x + 0.24f * y + 0.44f;
        }

        x = nextX;
        y = nextY;

        BeginTextureMode(canvas);
            DrawPixel(scaleX * x + offsetX, -scaleY * y + offsetY, pointColor);
            if (IsKeyPressed(KEY_CLEAR)) ClearBackground(*backgroundColor);
        EndTextureMode();

        BeginDrawing();
            ClearBackground(*backgroundColor);
            DrawTextureRec(canvas.texture, (Rectangle) { 0, 0, canvas.texture.width, -canvas.texture.height }, (Vector2) { 0, 0 }, WHITE);
            if (showFPS) DrawFPS(0, 0);
        EndDrawing();

        if (IsKeyPressed(KEY_CHANGE_MODE)) break;
        if (IsKeyPressed(KEY_FPS)) showFPS = !showFPS;
    }

    UnloadRenderTexture(canvas);
}

void ResizeWindow(const float widthCoefficient, const float heightCoefficient){
    const uint currentSize = GetScreenWidth();
    const uint monitorWidth = GetMonitorWidth(GetCurrentMonitor());
    const uint monitorHeight = GetMonitorHeight(GetCurrentMonitor());
    const uint newSize = widthCoefficient * monitorWidth <= monitorHeight ? widthCoefficient * monitorWidth : heightCoefficient * monitorHeight;

    SetWindowSize(newSize, newSize);
    SetWindowPosition((monitorWidth - newSize) / 2, (monitorHeight - newSize) / 2);
}

int main(void){
    const uint windowSize = 650;
    const Color backgroundColor = {52, 49, 49, 255};
    
    InitWindow(windowSize, windowSize, "Fractals");
    ResizeWindow(0.48, 0.85);
    
    while (!WindowShouldClose()){
        DrawSierpinskiTriangles(windowSize, &backgroundColor);
        DrawBarnsleyFern(windowSize, &backgroundColor);
    }
    
    CloseWindow();
    return 0;
}