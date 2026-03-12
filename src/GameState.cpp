// GameState.cpp
#include "GameState.h"
#include "TetrominoTypes.h"
#include <algorithm>
#include <random>

const double GameState::fallInterval = 0.8; // 0.4 segundos por celda

GameState::GameState()
    : grid(),
      piecesBag(GetAllTetrominos()),
      gameOver(true),
      score(0),
      fallTimer(0),
      softDropMoves(0),
      nextBagIndex(0),
      fallProgress(0.0),
      softDropHeld(false),
      visualRotation(0.0f),
      targetVisualRotation(0.0f)
{
    // Mezclar la bolsa aleatoriamente
    static std::random_device rd;
    static std::mt19937 g(rd());
    std::shuffle(piecesBag.begin(), piecesBag.end(), g);
    // Inicializar nextPiece y currentPiece? Se hará en Reset()
    Reset();
    gameOver = true;
}

void GameState::Reset()
{
    grid.Initialize();
    gameOver = false;
    score = 0;
    softDropMoves = 0;
    fallTimer = 0;
    fallProgress = 0.0;
    softDropHeld = false;
    // Rebarajar bolsa
    static std::random_device rd;
    static std::mt19937 g(rd());
    std::shuffle(piecesBag.begin(), piecesBag.end(), g);
    nextBagIndex = 0;
    currentPiece = GetNextPieceFromBag();
    nextPiece = GetNextPieceFromBag();
}

void GameState::HandleInput(InputAction action)
{
    if (gameOver)
    {
        if (action == InputAction::Restart)
        {
            Reset();
        }
        return;
    }

    switch (action)
    {
    case InputAction::MoveLeft:
        currentPiece.Move(0, -1);
        if (!PieceFits(currentPiece) || IsPieceOutside(currentPiece))
            currentPiece.Move(0, 1); // deshacer
        break;
    case InputAction::MoveRight:
        currentPiece.Move(0, 1);
        if (!PieceFits(currentPiece) || IsPieceOutside(currentPiece))
            currentPiece.Move(0, -1);
        break;
    case InputAction::Rotate:
    {
        int oldRot = currentPiece.GetRotation(); // Necesitamos añadir getter/setter de rotación en Tetromino
        // Por ahora, asumimos que tenemos métodos para rotar
        // Para simplificar, añadiremos Rotate() en Tetromino
        currentPiece.Rotate();
        if (!PieceFits(currentPiece) || IsPieceOutside(currentPiece))
        {
            currentPiece.UndoRotate();
        }
        else 
        {
            targetVisualRotation += 90.0f;
        }
            
    }
    break;
    default:
        break;
    }
}

void GameState::Update(double deltaTime)
{
    if (gameOver)
        return;
    // Velocidad efectiva: si soft drop está activo, se acelera
    double effectiveInterval = softDropHeld ? fallInterval / 5.0 : fallInterval;
    fallProgress += deltaTime / effectiveInterval;

    while (fallProgress >= 0.5)
    {
        fallProgress -= 1.0;

        // Intentar mover hacia abajo
        currentPiece.Move(1, 0);
        if (!PieceFits(currentPiece) || IsPieceOutside(currentPiece))
        {
            // No se puede mover, se bloquea
            currentPiece.Move(-1, 0);
            LockPiece();
            fallProgress = 0.0; // nueva pieza empieza con progreso 0
            break;
        }
        else
        {
            // Movimiento exitoso: si es soft drop, sumar punto
            if (softDropHeld)
            {
                softDropMoves++;
            }
        }
    }

    // Animación de rotación
    const float ROTATION_SPEED = 720.0f; // grados por segundo (90° en 0.125s)
    if (visualRotation != targetVisualRotation) {
        float diff = targetVisualRotation - visualRotation;
        float step = ROTATION_SPEED * deltaTime;
        if (fabs(diff) <= step) {
            visualRotation = targetVisualRotation;
        } else {
            visualRotation += (diff > 0 ? step : -step);
        }
    }
}

void GameState::LockPiece()
{
    // Fijar la pieza actual en la cuadrícula
    auto cells = currentPiece.GetCurrentCells(); // (necesitas este método)
    int r0 = currentPiece.GetRow();
    int c0 = currentPiece.GetCol();
    for (const auto &cell : cells)
    {
        int r = r0 + cell.row;
        int c = c0 + cell.column;
        if (!grid.IsCellOutside(r, c))
            grid.SetCell(r, c, currentPiece.GetId());
    }

    int lines = grid.ClearFullRows();
    UpdateScore(lines, softDropMoves);
    softDropMoves = 0;

    // Avanzar a la siguiente pieza
    currentPiece = nextPiece;
    nextPiece = GetNextPieceFromBag();

    // Comprobar si la nueva pieza cabe; si no, game over
    if (!PieceFits(currentPiece) || IsPieceOutside(currentPiece))
    {
        gameOver = true;
    }
}

bool GameState::PieceFits(const Tetromino &piece) const
{
    auto cells = piece.GetCells(piece.GetRotation());
    int r0 = piece.GetRow();
    int c0 = piece.GetCol();
    for (const auto &cell : cells)
    {
        int r = r0 + cell.row;
        int c = c0 + cell.column;
        if (grid.IsCellOutside(r, c))
            continue; // las celdas fuera se comprueban aparte
        if (!grid.IsCellEmpty(r, c))
            return false;
    }
    return true;
}

bool GameState::IsPieceOutside(const Tetromino &piece) const
{
    auto cells = piece.GetCells(piece.GetRotation());
    int r0 = piece.GetRow();
    int c0 = piece.GetCol();
    for (const auto &cell : cells)
    {
        int r = r0 + cell.row;
        int c = c0 + cell.column;
        if (grid.IsCellOutside(r, c))
            return true;
    }
    return false;
}

void GameState::UpdateScore(int linesCleared, int softDropMoves)
{
    switch (linesCleared)
    {
    case 1:
        score += 100;
        break;
    case 2:
        score += 300;
        break;
    case 3:
        score += 500;
        break;
    case 4:
        score += 800;
        break; // opcional
    }
    score += softDropMoves; // 1 punto por cada celda acelerada
}

Tetromino GameState::GetNextPieceFromBag()
{
    Tetromino piece = piecesBag[nextBagIndex];
    nextBagIndex = (nextBagIndex + 1) % piecesBag.size();
    if (nextBagIndex == 0)
    {
        // Se ha dado la vuelta, rebarajar para más variedad
        static std::random_device rd;
        static std::mt19937 g(rd());
        std::shuffle(piecesBag.begin(), piecesBag.end(), g);
    }
    return piece;
}