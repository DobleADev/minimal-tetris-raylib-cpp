// GameRenderer.h
#pragma once
#include "raylib.h"
#include "GameState.h"

class GameRenderer {
public:
    GameRenderer(Font font);
    void Draw(const GameState& state, double fallProgress); 
private:
    Font font;
    std::vector<Color> colors; // los colores de las piezas
    void DrawGrid(const Grid& grid);
    void DrawPiece(const Tetromino& piece, int offsetX, int offsetY, double fallProgress);
    void DrawUI(const GameState& state);
};