// Tetromino.cpp
#include "Tetromino.h"

Tetromino::Tetromino(int id, const std::map<int, std::vector<Position>>& rotations)
    : id(id), cells(rotations), rowOffset(0), colOffset(0), rotation(0), xCenter(0), yCenter(0) {}
Tetromino::Tetromino() : id(0), cells(), rowOffset(0), colOffset(0), rotation(0), xCenter(0), yCenter(0) {}
int Tetromino::GetId() const { return id; }
const std::vector<Position>& Tetromino::GetCells(int rotation) const { return cells.at(rotation); }
int Tetromino::GetNumRotations() const { return cells.size(); }
void Tetromino::SetPosition(int row, int col) { rowOffset = row; colOffset = col; }
void Tetromino::SetCenter(float x, float y) { xCenter = x; yCenter = y; }
void Tetromino::Move(int dRow, int dCol) { rowOffset += dRow; colOffset += dCol; }
int Tetromino::GetRow() const { return rowOffset; }
int Tetromino::GetCol() const { return colOffset; }
float Tetromino::GetXCenter() const { return xCenter; }
float Tetromino::GetYCenter() const { return yCenter; }