// GameRenderer.h
#pragma once
#include "raylib.h"
#include "GameState.h"
#include "Transform2D.h"

class GameRenderer {
public:
    GameRenderer(Font font);
    void Draw(const GameState& state, double fallProgress); 
    void DrawGrid(const Grid& grid);
    void DrawPiece(const Tetromino& piece, int offsetX, int offsetY, double fallProgress);
    void DrawPiece(const Tetromino& piece, const Transform2D& transform, Texture2D blockTex);
    void DrawUI(const GameState& state);
private:
    Font font;
    std::vector<Color> colors; // los colores de las piezas
    
};