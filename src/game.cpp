#include "game.h"
#include <random>

Game::Game()
{
    grid = Grid();
    gameOver = true;
}

Block Game::GetRandomBlock()
{
    if (blocks.empty())
    {
        blocks = GetAllBlocks();
    }
    int randomIndex = rand() % blocks.size();
    Block block = blocks[randomIndex];
    blocks.erase(blocks.begin() + randomIndex);
    return block;
}

std::vector<Block> Game::GetAllBlocks()
{
    return { 
        IBlock(), 
        JBlock(), 
        LBlock(), 
        OBlock(), 
        SBlock(), 
        TBlock(), 
        ZBlock(),
    };
}

void Game::HandleInput()
{
    int keyPressed = GetKeyPressed();
    if (gameOver && keyPressed != 0)
    {
        Reset();
    }
    switch (keyPressed)
    {
    case KEY_LEFT:
        MoveBlockLeft();
        break;
    case KEY_RIGHT:
        MoveBlockRight();
        break;
    // case KEY_DOWN:
    //     MoveBlockDown();
    //     break;
    case KEY_UP:
        RotateBlock();
        break;
    
    default:
        break;
    }
}

void Game::MoveBlockLeft()
{
    if (gameOver) return;
    currentBlock.Move(0, -1);
    if (IsBlockOutside() || BlockFits() == false)
    {
        currentBlock.Move(0, 1);
    }
}

void Game::MoveBlockRight()
{
    if (gameOver) return;
    currentBlock.Move(0, 1);
    if (IsBlockOutside() || BlockFits() == false)
    {
        currentBlock.Move(0, -1);
    }
}

void Game::MoveBlockDown()
{
    if (gameOver) return;
    currentBlock.Move(1, 0);
    if (IsBlockOutside() || BlockFits() == false)
    {
        currentBlock.Move(-1, 0);
        LockBlock();
    }
}

void Game::Draw()
{
    grid.Draw();
    currentBlock.Draw();
}

bool Game::IsBlockOutside()
{
    std::vector<Position> tiles = currentBlock.GetCellPositions();
    for (Position item: tiles)
    {
        if (grid.IsCellOutside(item.row, item.column))
        {
            return true;
        }
    }
    
    return false;
}

void Game::RotateBlock()
{
    currentBlock.Rotate();
    if (IsBlockOutside() || BlockFits() == false)
    {
        currentBlock.UndoRotate();
    }
}

void Game::LockBlock()
{
    std::vector<Position> tiles = currentBlock.GetCellPositions();
    for(Position item: tiles)
    {
        grid.grid[item.row][item.column] = currentBlock.id;
    }
    currentBlock = nextBlock;
    if (BlockFits() == false)
    {
        gameOver = true;
        return;
    }
    nextBlock = GetRandomBlock();
    grid.ClearFullRows();
}

bool Game::BlockFits()
{
    std::vector<Position> tiles = currentBlock.GetCellPositions();
    for(Position item: tiles)
    {
        if (grid.IsCellEmpty(item.row, item.column) == false)
        {
            return false;
        }
    }
    return true;
}

void Game::Reset()
{
    gameOver = false;
    grid.Initialize();
    blocks = GetAllBlocks();
    currentBlock = GetRandomBlock();
    nextBlock = GetRandomBlock();
}
