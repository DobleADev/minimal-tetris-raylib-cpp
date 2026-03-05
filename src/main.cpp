#include "raylib.h"
#include "game.h"
#include <math.h>

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
const char *startLabel = "Press any key to start a new game";
int startLabelSize = 24;
float startLabelBlinkRate = 4;

double blockFallSpeedRate = 0.4;
double blockFallSpeedMultiplier = 5;

void update(void) {
    game.HandleInput();

    double currentBlockFallSpeedMultiplier = 1;
    if (IsKeyDown(KEY_DOWN))
    {
        currentBlockFallSpeedMultiplier /= blockFallSpeedMultiplier;
    }

    if (EventTriggered(blockFallSpeedRate * currentBlockFallSpeedMultiplier))
    {
        game.MoveBlockDown();
    }
    
    BeginDrawing();
        int screenWidth = GetScreenWidth();
        int screenHeight = GetScreenHeight();
        ClearBackground(BLACK);
        game.Draw();
        if (game.gameOver)
        {
            DrawRectangle(0, 0, screenWidth, screenHeight, Color({ 0, 0, 0, 127 }));
            
            int textWidth = MeasureText(startLabel, startLabelSize);
            
            unsigned char alpha = static_cast<unsigned char>(round(abs(sin(GetTime() * startLabelBlinkRate)) * 255));
            Color whiteBlink = { 255, 255, 255, alpha };
            DrawText(
                startLabel, 
                (screenWidth / 2) - (textWidth / 2), 
                (screenHeight / 2), 
                24, whiteBlink
            );
        }
        DrawFPS(8, 8);        
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