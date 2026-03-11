// Tetromino.h
#pragma once
#include <vector>
#include <map>
#include "Position.h"

class Tetromino {
public:
    Tetromino();
    Tetromino(int id, const std::map<int, std::vector<Position>>& rotations);
    int GetId() const;
    const std::vector<Position>& GetCells(int rotation) const;
    int GetNumRotations() const;
    void SetPosition(int row, int col);
    void SetCenter(float x, float y);
    void Move(int dRow, int dCol);
    int GetRow() const;
    int GetCol() const;
    float GetXCenter() const;
    float GetYCenter() const;
    int GetRotation() const { return rotation; }
    void Rotate() { rotation = (rotation + 1) % cells.size(); }
    void UndoRotate() { rotation = (rotation + cells.size() - 1) % cells.size(); }
    const std::vector<Position>& GetCurrentCells() const { return cells.at(rotation); }
    
private:
    int id;
    std::map<int, std::vector<Position>> cells; // rotación -> lista de celdas relativas
    int rowOffset;
    int colOffset;
    int rotation; // rotación actual
    float xCenter;
    float yCenter;
};