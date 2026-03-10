// Grid.cpp
#include "Grid.h"

Grid::Grid() { Initialize(); }

void Grid::Initialize() {
    for (int r = 0; r < numRows; ++r)
        for (int c = 0; c < numCols; ++c)
            grid[r][c] = 0;
}

bool Grid::IsCellOutside(int row, int col) const {
    return row < 0 || row >= numRows || col < 0 || col >= numCols;
}

bool Grid::IsCellEmpty(int row, int col) const {
    return grid[row][col] == 0;
}

int Grid::GetCell(int row, int col) const {
    return grid[row][col];
}

void Grid::SetCell(int row, int col, int value) {
    grid[row][col] = value;
}

int Grid::ClearFullRows() {
    int cleared = 0;
    for (int r = numRows - 1; r >= 0; --r) {
        if (IsRowFull(r)) {
            ClearRow(r);
            ++cleared;
        } else if (cleared > 0) {
            MoveRowDown(r, cleared);
        }
    }
    return cleared;
}

bool Grid::IsRowFull(int row) const {
    for (int c = 0; c < numCols; ++c)
        if (grid[row][c] == 0) return false;
    return true;
}

void Grid::ClearRow(int row) {
    for (int c = 0; c < numCols; ++c)
        grid[row][c] = 0;
}

void Grid::MoveRowDown(int row, int numRowsDown) {
    for (int c = 0; c < numCols; ++c) {
        grid[row + numRowsDown][c] = grid[row][c];
        grid[row][c] = 0;
    }
}