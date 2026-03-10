#include "TetrominoTypes.h"
#include "Position.h"
#include <map>
#include <vector>

std::vector<Tetromino> GetAllTetrominos() {
    std::vector<Tetromino> result;

    // L (id 1)
    {
        std::map<int, std::vector<Position>> rotations;
        rotations[0] = {Position(0,2), Position(1,0), Position(1,1), Position(1,2)};
        rotations[1] = {Position(0,1), Position(1,1), Position(2,1), Position(2,2)};
        rotations[2] = {Position(1,0), Position(1,1), Position(1,2), Position(2,0)};
        rotations[3] = {Position(0,0), Position(0,1), Position(1,1), Position(2,1)};
        Tetromino L(1, rotations);
        L.SetPosition(0, 3);
        result.push_back(L);
    }

    // J (id 2)
    {
        std::map<int, std::vector<Position>> rotations;
        rotations[0] = {Position(0,0), Position(1,0), Position(1,1), Position(1,2)};
        rotations[1] = {Position(0,1), Position(0,2), Position(1,1), Position(2,1)};
        rotations[2] = {Position(1,0), Position(1,1), Position(1,2), Position(2,2)};
        rotations[3] = {Position(0,1), Position(1,1), Position(2,0), Position(2,1)};
        Tetromino J(2, rotations);
        J.SetPosition(0, 3);
        result.push_back(J);
    }

    // I (id 3)
    {
        std::map<int, std::vector<Position>> rotations;
        rotations[0] = {Position(1,0), Position(1,1), Position(1,2), Position(1,3)};
        rotations[1] = {Position(0,2), Position(1,2), Position(2,2), Position(3,2)};
        rotations[2] = {Position(2,0), Position(2,1), Position(2,2), Position(2,3)};
        rotations[3] = {Position(0,1), Position(1,1), Position(2,1), Position(3,1)};
        Tetromino I(3, rotations);
        I.SetPosition(-1, 3); // Ajuste de I: fila -1, columna 3
        result.push_back(I);
    }

    // O (id 4)
    {
        std::map<int, std::vector<Position>> rotations;
        std::vector<Position> cells = {Position(0,0), Position(0,1), Position(1,0), Position(1,1)};
        rotations[0] = cells;
        rotations[1] = cells;
        rotations[2] = cells;
        rotations[3] = cells;
        Tetromino O(4, rotations);
        O.SetPosition(0, 4); // Ajuste de O: columna 4
        result.push_back(O);
    }

    // S (id 5)
    {
        std::map<int, std::vector<Position>> rotations;
        rotations[0] = {Position(0,1), Position(0,2), Position(1,0), Position(1,1)};
        rotations[1] = {Position(0,1), Position(1,1), Position(1,2), Position(2,2)};
        rotations[2] = {Position(1,1), Position(1,2), Position(2,0), Position(2,1)};
        rotations[3] = {Position(0,0), Position(1,0), Position(1,1), Position(2,1)};
        Tetromino S(5, rotations);
        S.SetPosition(0, 3);
        result.push_back(S);
    }

    // T (id 6)
    {
        std::map<int, std::vector<Position>> rotations;
        rotations[0] = {Position(0,1), Position(1,0), Position(1,1), Position(1,2)};
        rotations[1] = {Position(0,1), Position(1,1), Position(1,2), Position(2,1)};
        rotations[2] = {Position(1,0), Position(1,1), Position(1,2), Position(2,1)};
        rotations[3] = {Position(0,1), Position(1,0), Position(1,1), Position(2,1)};
        Tetromino T(6, rotations);
        T.SetPosition(0, 3);
        result.push_back(T);
    }

    // Z (id 7)
    {
        std::map<int, std::vector<Position>> rotations;
        rotations[0] = {Position(0,0), Position(0,1), Position(1,1), Position(1,2)};
        rotations[1] = {Position(0,2), Position(1,1), Position(1,2), Position(2,1)};
        rotations[2] = {Position(1,0), Position(1,1), Position(2,1), Position(2,2)};
        rotations[3] = {Position(0,1), Position(1,0), Position(1,1), Position(2,0)};
        Tetromino Z(7, rotations);
        Z.SetPosition(0, 3);
        result.push_back(Z);
    }

    return result;
}