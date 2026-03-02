#include "welcome.h"

#define FONT_SIZE (0.042 * WINDOW_SIZE)
#define TEXT "Controls:\n\nC - clear screen\nSPACE - next fractal\nMOUSE WHEEL - change tree angle\n\nPress SPACE to start"
#define TEXT_WIDTH MeasureText(TEXT, FONT_SIZE)

void ShowWelcomeScreen(void){
    SetTargetFPS(60);

    while (!WindowShouldClose()){
        BeginDrawing();
            ClearBackground(COLOR_BACKGROUND);
            DrawText(TEXT, (WINDOW_SIZE - TEXT_WIDTH) / 2, (WINDOW_SIZE - FONT_SIZE * 7) / 2, FONT_SIZE, COLOR_TRIANGLE);
        EndDrawing();

        if (IsKeyPressed(KEY_CHANGE_MODE)) break;
    }

    SetTargetFPS(0);
}