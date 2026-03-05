#include "raylib.h"
#include "game.h"
#include "colors.h"
#include <math.h>

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#define CURRENT_PLATFORM "WEB"
#else
#define CURRENT_PLATFORM "DESKTOP"
#endif
#include <cstdio>

Font font;
Game game;
const char *startLabel = "Press [ENTER] to start a new game";
int startLabelSize = 16;
float startLabelBlinkRate = 4;

double blockFallSpeedRate = 0.4;
double blockFallSpeedMultiplier = 5;
int screenWidth;
int screenHeight;

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

void DrawStartup()
{
    if (game.gameOver)
    {
        DrawRectangle(0, 0, screenWidth, screenHeight, Color({0, 0, 0, 127}));
        int textWidth = MeasureText(startLabel, startLabelSize);
        unsigned char alpha = static_cast<unsigned char>(round(abs(sin(GetTime() * startLabelBlinkRate)) * 255));
        Color whiteBlink = {255, 255, 255, static_cast<unsigned char>(15 + alpha * 0.9)};
        DrawTextEx(font, startLabel, {(float)((screenWidth / 2) - (textWidth)), (float)(screenHeight / 2)}, startLabelSize, 2, whiteBlink);

        DrawRectangleGradientV(0, 0, screenWidth, screenHeight, Color({64, 0, 255, 0}), Color({0, 127, 255, static_cast<unsigned char>(97 + alpha * 0.3)}));
    }
}

void DrawGameUI()
{
    float rightMargin = 200;
    float verticalGap = 16;
    float nextVerticalPosition = 0;

    // SCORE LABEL
    const char *scoreTextLabel = "Score";
    int scoreTextSize = 24;
    int scoreTextWidth = MeasureText(scoreTextLabel, scoreTextSize);
    DrawTextEx(font, scoreTextLabel, {(float)((screenWidth - (scoreTextWidth) - rightMargin)), (float)scoreTextSize}, scoreTextSize, 2, WHITE);
    nextVerticalPosition += (scoreTextSize * 2) + verticalGap;

    // SCORE VALUE BACK
    float scoreRectangleWidth = 170;
    float scoreRectangleHeight = 60;
    float scoreRectangleXPosition = (float)((screenWidth - (scoreRectangleWidth * 0.5f) - rightMargin));
    float scoreRectangleYPosition = nextVerticalPosition;
    DrawRectangleRounded({scoreRectangleXPosition, scoreRectangleYPosition, scoreRectangleWidth, scoreRectangleHeight}, 0.3, 6, lightBlue);
    nextVerticalPosition += (scoreRectangleHeight) + verticalGap;

    // SCORE VALUE LABEL
    char scoreValueLabel[10];
    sprintf(scoreValueLabel, "%d", game.score);
    int scoreValueSize = 24;
    int scoreValueWidth = MeasureText(scoreValueLabel, scoreValueSize);
    DrawTextEx(
        font, 
        scoreValueLabel, 
        {(float)((scoreRectangleXPosition + (scoreRectangleWidth * 0.5f) - scoreValueWidth)), 
            (float)scoreRectangleYPosition + (scoreRectangleHeight * 0.5f) - (scoreValueSize * 0.5f)}, 
            scoreValueSize, 2, WHITE);

    // NEXT BLOCK LABEL
    const char *nextBlockLabel = "Next";
    int nextBlockTextSize = 24;
    int nextBlockTextWidth = MeasureText(nextBlockLabel, nextBlockTextSize);
    DrawTextEx(font, nextBlockLabel, {(float)((screenWidth - (nextBlockTextWidth) - rightMargin)), nextVerticalPosition + (float)nextBlockTextSize}, nextBlockTextSize, 2, WHITE);
    nextVerticalPosition += (nextBlockTextSize * 2) + verticalGap;
    
    // NEXT BLOCK BACK
    float nextBlockRectangleWidth = 170;
    float nextBlockRectangleHeight = 170;
    float nextBlockRectangleXPosition = (float)((screenWidth - (nextBlockRectangleWidth * 0.5f) - rightMargin));
    float nextBlockRectangleYPosition = nextVerticalPosition;
    DrawRectangleRounded({nextBlockRectangleXPosition, nextBlockRectangleYPosition, nextBlockRectangleWidth, nextBlockRectangleHeight}, 0.3, 6, lightBlue);
    nextVerticalPosition += nextBlockRectangleHeight + verticalGap;
}

void update(void)
{
    UpdateMusicStream(game.music);
    game.HandleInput();

    screenWidth = GetScreenWidth();
    screenHeight = GetScreenHeight();

    double currentBlockFallSpeedMultiplier = 1;
    if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S))
    {
        currentBlockFallSpeedMultiplier /= blockFallSpeedMultiplier;
    }

    if (EventTriggered(blockFallSpeedRate * currentBlockFallSpeedMultiplier))
    {
        game.MoveBlockDown();
    }

    BeginDrawing();
    ClearBackground(darkBlue);
    DrawRectangleGradientV(0, 0, screenWidth, screenHeight, Color({15, 10, 127, 255}), Color({0, 95, 201, 255}));
    DrawGameUI();
    game.Draw();
    DrawStartup();
    DrawFPS(8, 8);
    EndDrawing();
}

int main(void)
{
    InitWindow(800, 600, "Minimal Tetris");
    font = LoadFontEx("../resources/PressStart2P-Regular.ttf", 24, 0, 0);

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(update, 0, 1);
#else
    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        update();
    }
#endif

    CloseWindow();
    return 0;
}