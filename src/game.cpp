#include "game.h"
#include <random>

Game::Game()
{
    grid = Grid();
    gameOver = true;
    // Reset();
    InitAudioDevice();
    music = LoadMusicStream("resources/Minimal Tetris A-Type Music.ogg");
    SetMusicVolume(music, 0.75f);
    PlayMusicStream(music);
    rotateSound = LoadSound("resources/rotate-block.wav");
    clearSound = LoadSound("resources/line-clear.wav");
    
}

Game::~Game()
{
    UnloadSound(rotateSound);
    UnloadSound(clearSound);
    UnloadMusicStream(music);
    CloseAudioDevice();
}

Block Game::GetRandomBlock()
{
    if (blocks.empty())
    {
        blocks = GetAllBlocks();
    }
    static std::random_device rd;  
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, blocks.size() - 1);
    int randomIndex = dis(gen);
    // int randomIndex = rand() % blocks.size();
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
    if (gameOver && (keyPressed == KEY_ENTER || keyPressed == KEY_KP_ENTER))
    {
        Reset();
    }
    switch (keyPressed)
    {
    case KEY_LEFT:
    case KEY_A:
        MoveBlockLeft();
        break;
    case KEY_RIGHT: 
    case KEY_D:
        MoveBlockRight();
        break;
    // case KEY_DOWN:
    //     MoveBlockDown();
    //     break;
    case KEY_UP:
    case KEY_W:
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
    currentBlock.Draw(91, 1);

    Vector2 nextBlockDraw = {464, 260};
    switch (nextBlock.id)
    {
    case 3:
        nextBlockDraw.x += -15;
        nextBlockDraw.y += 20;
        break;
    
    case 4:
        nextBlockDraw.x += -15;
        nextBlockDraw.y += 0;
        break;
    
    default:
        break;
    }
    nextBlock.Draw(nextBlockDraw.x, nextBlockDraw.y);
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
    else
    {
        PlaySound(rotateSound);
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
    int rowsCleared = grid.ClearFullRows();
    if (rowsCleared > 0)
    {
        PlaySound(clearSound);
        UpdateScore(rowsCleared, 0);
    }
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
    score = 0;
    gameOver = false;
    grid.Initialize();
    blocks = GetAllBlocks();
    currentBlock = GetRandomBlock();
    nextBlock = GetRandomBlock();
}

void Game::UpdateScore(int linesCleared, int moveDownPoints)
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
    default:
        break;
    }

    score += moveDownPoints;
}