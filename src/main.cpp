#include "raylib.h"

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
    #define CURRENT_PLATFORM "WEB"
#else
    #define CURRENT_PLATFORM "DESKTOP"
#endif

void UpdateDrawFrame(void) {
    
    
    BeginDrawing();
        ClearBackground(BLACK);
        
        DrawFPS(20, 50);
        
    EndDrawing();
}

int main(void) {
    InitWindow(800, 450, "Minimal Tetris");

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
#else
    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        UpdateDrawFrame();
    }
#endif

    CloseWindow();
    return 0;
}