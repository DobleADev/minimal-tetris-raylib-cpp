// Grid.h
#pragma once
#include <vector>

class Grid {
public:
    Grid();
    void Initialize();
    bool IsCellOutside(int row, int col) const;
    bool IsCellEmpty(int row, int col) const;
    int GetCell(int row, int col) const;
    void SetCell(int row, int col, int value);
    int ClearFullRows();
    int GetNumRows() const { return numRows; }
    int GetNumCols() const { return numCols; }
private:
    bool IsRowFull(int row) const;
    void ClearRow(int row);
    void MoveRowDown(int row, int numRows);
    static const int numRows = 20;
    static const int numCols = 10;
    int grid[numRows][numCols];
};