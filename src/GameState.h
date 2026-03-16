// GameState.h
#pragma once
#include "Grid.h"
#include "Tetromino.h"
#include <vector>
#include "InputAction.h"
#include <functional>

class GameState {
public:
    GameState();
    void Reset();
    void HandleInput(InputAction action);
    void Update(double deltaTime); // Para caída automática
    bool IsGameOver() const { return gameOver; }
    bool IsPaused() const { return paused; }
    int GetScore() const { return score; }
    int GetTotalPlacedPieces() const { return totalPlacedPieces; }
    int GetTotalClearedLines() const { return totalClearedLines; }
    const Grid& GetGrid() const { return grid; }
    const Tetromino& GetCurrentPiece() const { return currentPiece; }
    const Tetromino& GetNextPiece() const { return nextPiece; }
    void SetSoftDropHeld(bool held) { softDropHeld = held; }
    double GetFallProgress() const { return fallProgress; }
    float GetVisualRotation() const { return visualRotation; }
    std::function<void(int lines)> onLinesCleared;
    std::function<void()> onGameOver;
    std::function<void()> onRotate;
    void TogglePause();
    // Para pruebas, podemos exponer más métodos si es necesario
    private:
    bool TryHardDrop();
    void SpawnNewPiece();
    void LockPiece();
    bool PieceFits(const Tetromino& piece) const;
    bool IsPieceOutside(const Tetromino& piece) const;
    void UpdateScore(int linesCleared, int softDropMoves);
    Tetromino GetNextPieceFromBag();

    Grid grid;
    std::vector<Tetromino> piecesBag; // bolsa de piezas (para aleatoriedad)
    Tetromino currentPiece;
    Tetromino nextPiece;
    bool gameOver;
    bool paused;
    int score;
    int totalPlacedPieces;
    int totalClearedLines;
    double fallTimer; // tiempo acumulado para la caída automática
    static const double fallInterval; // intervalo de caída normal
    int softDropMoves; // contador para puntuación de aceleración
    int nextBagIndex;
    double fallProgress;      // progreso de caída entre 0 y 1
    bool softDropHeld;        // true si la tecla de abajo está presionada
    float visualRotation;      // ángulo visual actual (grados)
    float targetVisualRotation; // ángulo destino
};