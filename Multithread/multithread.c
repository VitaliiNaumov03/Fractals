#include "multithread.h"

#define TRIANGLE_SCALE 0.8f //Make triangle smaller to fit in the screen
#define TRIANGLE_SIDE_LENGTH (TRIANGLE_SCALE * ((2 * WINDOW_SIZE) / SQRT3))
#define TRIANGLE_HEIGHT ((SQRT3 / 2) * TRIANGLE_SIDE_LENGTH)
#define TRIANGLE_START_POINT (Vector2){WINDOW_SIZE / 2.0f, WINDOW_SIZE / 2.0f}

#define FERN_SCALE_X (0.17f * WINDOW_SIZE)
#define FERN_SCALE_Y (0.09f * WINDOW_SIZE)
#define FERN_OFFSET_X (WINDOW_SIZE / 2)
#define FERN_OFFSET_Y (WINDOW_SIZE - 0.05 * WINDOW_SIZE)

const Vector2 verticesABC[3] = {{WINDOW_SIZE / 2.0f, (WINDOW_SIZE - TRIANGLE_HEIGHT) / 2.0f},
                               {(WINDOW_SIZE - TRIANGLE_SIDE_LENGTH) / 2.0f, (WINDOW_SIZE + TRIANGLE_HEIGHT) / 2.0f},
                               {(WINDOW_SIZE + TRIANGLE_SIDE_LENGTH) / 2.0f, (WINDOW_SIZE + TRIANGLE_HEIGHT) / 2.0f}};

typedef struct WorkerData{
    Vector2 pointToDraw; //For triangle
    uint bufferWidth;
    uint randomState;
    volatile bool *isRunning;
    const Vector2 *verticesABC; //For triangle
    _Atomic(Color*) *writeBuffer;
    _Atomic int *pointBudget;
} WorkerData;

void* DrawPointTriangle(void *arg){
    WorkerData *data = (WorkerData*)arg;

    while(*(data->isRunning)){
        const int pointsLeft = atomic_fetch_sub(data->pointBudget, 1);
        if (pointsLeft <= 0){
            atomic_fetch_add(data->pointBudget, 1);
            continue;
        }

        const Vector2 vertex = verticesABC[GetRandomUint(2, &data->randomState)];
        data->pointToDraw.x = (data->pointToDraw.x + vertex.x) / 2.0f;
        data->pointToDraw.y = (data->pointToDraw.y + vertex.y) / 2.0f;

        Color *buffer = atomic_load(data->writeBuffer);
        const int x = (int)data->pointToDraw.x;
        const int y = (int)data->pointToDraw.y;
        if (x >=0 && y >= 0) buffer[y * data->bufferWidth + x] = COLOR_TRIANGLE;
    }

    return NULL;
}

void* DrawPointFern(void *arg){
    WorkerData *data = (WorkerData*)arg;

    float currX = 0, currY = 0; //Start point
    float nextX, nextY;    
    uint randomValue;

    while (*(data->isRunning)){
        const int pointsLeft = atomic_fetch_sub(data->pointBudget, 1);
        if (pointsLeft <= 0){
            atomic_fetch_add(data->pointBudget, 1);
            continue;
        }

        randomValue = GetRandomUint(100, &data->randomState);

        if (randomValue < 1){
            nextX = 0.0f;
            nextY = 0.16f * currY;
        }
        else if (randomValue < 86){
            nextX = 0.85f * currX + 0.04f * currY;
            nextY = -0.04f * currX + 0.85f * currY + 1.6f;
        }
        else if (randomValue < 93){
            nextX = 0.2f * currX - 0.26f * currY;
            nextY = 0.23f * currX + 0.22f * currY + 1.6f;
        }
        else{
            nextX = -0.15f * currX + 0.28f * currY;
            nextY = 0.26f * currX + 0.24f * currY + 0.44f;
        }

        currX = nextX;
        currY = nextY;

        Color *buffer = atomic_load(data->writeBuffer);
        const int x = (int)(FERN_SCALE_X * currX + FERN_OFFSET_X);
        const int y = (int)(-FERN_SCALE_Y * currY + FERN_OFFSET_Y);
        if (x >=0 && y >= 0) buffer[y * data->bufferWidth + x] = COLOR_FERN;
    }

    return NULL;
}

void DrawFractalMultithread(void *(*function)(void *)){
    if (WindowShouldClose()) return;
    Image image1 = GenImageColor(WINDOW_SIZE, WINDOW_SIZE, COLOR_BACKGROUND);
    Image image2 = GenImageColor(WINDOW_SIZE, WINDOW_SIZE, COLOR_BACKGROUND);
    Texture2D texture = LoadTextureFromImage(image1);

    _Atomic(Color*) writeBuffer = image1.data;
    Color *readBuffer = image2.data;

    pthread_t worker;
    volatile bool isRunning = true;
    
    WorkerData workerData;
    _Atomic int pointBudget = 0;
    
    workerData.pointToDraw = TRIANGLE_START_POINT;
    workerData.bufferWidth = image1.width;
    workerData.randomState = GetRandomValue(1000000, 2000000);
    workerData.isRunning = &isRunning;
    workerData.verticesABC = verticesABC;
    workerData.writeBuffer = &writeBuffer;
    workerData.pointBudget = &pointBudget;

    pthread_create(&worker, NULL, function, &workerData);

    float accumulator = 0.0f;
    uint pointsToAdd = 0;

    while (!WindowShouldClose()){
        accumulator += GetFrameTime() * POINTS_PER_SECOND;
        pointsToAdd = (uint)accumulator;
        accumulator -= pointsToAdd; //Saving fractionnal part of float
        atomic_fetch_add(&pointBudget, pointsToAdd);

        readBuffer = atomic_exchange(&writeBuffer, readBuffer);
        UpdateTexture(texture, readBuffer);
        BeginDrawing();
            DrawTexture(texture, 0, 0, WHITE);
        EndDrawing();

        if (IsKeyPressed(KEY_CLEAR)){
            atomic_store(&pointBudget, 0); //Stop worker
            Color* wb = atomic_load(&writeBuffer);
            for (int i = 0; i < image1.width * image1.height; i++){
                readBuffer[i] = COLOR_BACKGROUND;
                wb[i] = COLOR_BACKGROUND;
            }
        }
        if (IsKeyPressed(KEY_CHANGE_MODE)) break;
    }

    isRunning = false;
    pthread_join(worker, NULL);
    
    UnloadTexture(texture);
    UnloadImage(image2);
    UnloadImage(image1);
}