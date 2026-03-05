#include "raylib.h"
#include "game.h"

double lastUpdateTime = 0;

bool EventTriggered(double interval)
{
    double currentTime = GetTime();
    if (currentTime - lastUpdateTime >= interval)
    {
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
    #define CURRENT_PLATFORM "WEB"
#else
    #define CURRENT_PLATFORM "DESKTOP"
#endif

Game game;

void update(void) {
    game.HandleInput();
    if (EventTriggered(0.16666))
    {
        game.MoveBlockDown();
    }
    
    BeginDrawing();
        ClearBackground(BLACK);
        game.Draw();
        // DrawFPS(20, 50);        
    EndDrawing();
}

int main(void) {
    InitWindow(800, 600, "Minimal Tetris");

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(update, 0, 1);
#else
    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        update();
    }
#endif

    CloseWindow();
    return 0;
}