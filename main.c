#include "raylib.h"
#include "icon.h"
#include "constants.h"
#include "welcome.h"
#include "multithread.h"
#include "tree.h"

int main(void){
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(WINDOW_SIZE, WINDOW_SIZE, WINDOW_NAME);
    SetWindowIcon((Image){ICON_DATA, ICON_WIDTH, ICON_HEIGHT, 1, ICON_FORMAT});
    SetTargetFPS(60);
    
    ShowWelcomeScreen();
    
    while (!WindowShouldClose()){
        DrawFractalMultithread(DrawPointTriangle);
        DrawFractalMultithread(DrawPointFern);
        DrawPythagorasTree();
    }
    
    CloseWindow();
    return 0;
}