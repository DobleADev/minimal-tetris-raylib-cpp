#pragma once
#include "grid.h"
#include "blocks.cpp"

class Game
{
    public:
    Game();
    ~Game();
    void HandleInput();
    void MoveBlockDown();
    void Draw();
    bool gameOver;
    int score;
    Music music;
    
    private:
    Block GetRandomBlock();
    std::vector<Block> GetAllBlocks();
    void MoveBlockLeft();
    void MoveBlockRight();
    bool IsBlockOutside();
    void RotateBlock();
    void LockBlock();
    bool BlockFits();
    void Reset();
    void UpdateScore(int linesCleared, int moveDownPoints);
    Grid grid;
    std::vector<Block> blocks;
    Block currentBlock;
    Block nextBlock;
    Sound rotateSound;
    Sound clearSound;
};