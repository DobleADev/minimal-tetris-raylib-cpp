// GameRenderer.cpp
#include "GameRenderer.h"
#include "Colors.h" // tus colores definidos
#include <cstdio>
#include <cmath>

GameRenderer::GameRenderer(Font font) : font(font)
{
    colors = GetCellColors(); // de colors.h
}

void GameRenderer::Draw(const GameState& state, double fallProgress) 
{
    DrawGrid(state.GetGrid());
    if (!state.IsGameOver())
    {
        DrawPiece(state.GetCurrentPiece(), 91, -29, fallProgress);
    }
    
    DrawUI(state);

    if (!state.IsGameOver())
    {
        DrawPiece(state.GetNextPiece(), 464, 260, 0.0); // ajusta según necesites
    }
}

void GameRenderer::DrawGrid(const Grid &grid)
{
    for (int r = 0; r < grid.GetNumRows(); ++r)
    {
        for (int c = 0; c < grid.GetNumCols(); ++c)
        {
            int cellValue = grid.GetCell(r, c);
            DrawRectangle(c * 30 + 91, r * 30 + 1, 29, 29, colors[cellValue]);
        }
    }
}

void GameRenderer::DrawPiece(const Tetromino &piece, int offsetX, int offsetY, double fallProgress)
{
    auto cells = piece.GetCells(piece.GetRotation());
    int r0 = piece.GetRow();
    int c0 = piece.GetCol();
    for (const auto &cell : cells)
    {
        float x = (c0 + cell.column) * 30 + offsetX;
        float y = (r0 + cell.row + fallProgress) * 30 + offsetY; // interpolación vertical
        DrawRectangle(x, y, 29, 29, colors[piece.GetId()]);
    }
}

const char *startLabel = "Press [ENTER] to start a new game";
int startLabelSize = 16;
float startLabelBlinkRate = 4;

double blockFallSpeedRate = 0.4;
double blockFallSpeedMultiplier = 5;
int screenWidth;
int screenHeight;

void GameRenderer::DrawUI(const GameState &state)
{
    screenWidth = GetScreenWidth();
    screenHeight = GetScreenHeight();
    float rightMargin = 200;
    float verticalGap = 16;
    float nextVerticalPosition = 0;

    // SCORE LABEL
    const char *scoreTextLabel = "Score";
    int scoreTextSize = 24;
    int scoreTextWidth = MeasureText(scoreTextLabel, scoreTextSize);
    DrawTextEx(font, scoreTextLabel, {(float)((screenWidth - (scoreTextWidth)-rightMargin)), (float)scoreTextSize}, scoreTextSize, 2, WHITE);
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
    sprintf(scoreValueLabel, "%d", state.GetScore());
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
    DrawTextEx(font, nextBlockLabel, {(float)((screenWidth - (nextBlockTextWidth)-rightMargin)), nextVerticalPosition + (float)nextBlockTextSize}, nextBlockTextSize, 2, WHITE);
    nextVerticalPosition += (nextBlockTextSize * 2) + verticalGap;

    // NEXT BLOCK BACK
    float nextBlockRectangleWidth = 170;
    float nextBlockRectangleHeight = 170;
    float nextBlockRectangleXPosition = (float)((screenWidth - (nextBlockRectangleWidth * 0.5f) - rightMargin));
    float nextBlockRectangleYPosition = nextVerticalPosition;
    DrawRectangleRounded({nextBlockRectangleXPosition, nextBlockRectangleYPosition, nextBlockRectangleWidth, nextBlockRectangleHeight}, 0.3, 6, lightBlue);
    nextVerticalPosition += nextBlockRectangleHeight + verticalGap;

    // STARTUP
    if (state.IsGameOver())
    {
        DrawRectangle(0, 0, screenWidth, screenHeight, Color({0, 0, 0, 127}));
        int textWidth = MeasureText(startLabel, startLabelSize);
        float t = GetTime() * startLabelBlinkRate;
        int alpha = (int)abs(sin(t) * 255);
        Color whiteBlink = {255, 255, 255, static_cast<unsigned char>(15 + alpha * 0.9)};
        DrawTextEx(font, startLabel, {(float)((screenWidth / 2) - (textWidth)), (float)(screenHeight / 2)}, startLabelSize, 2, whiteBlink);

        DrawRectangleGradientV(0, 0, screenWidth, screenHeight, Color({64, 0, 255, 0}), Color({0, 127, 255, static_cast<unsigned char>(97 + alpha * 0.3)}));
    }
}
