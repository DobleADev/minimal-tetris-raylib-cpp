#include "raylib.h"
#include "GameState.h"
#include "GameRenderer.h"
#include "InputHandler.h"
#include "AudioManager.h"
#include "Colors.h"

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif
#include "PieceVisual.h"
#include "Transform2D.h"
#include "Button.h"

GameState* gameState;
GameRenderer* renderer;
InputHandler* input;
AudioManager* audio;
Button* startButton = nullptr;
Button* pauseButton = nullptr;
Button* resumeButton = nullptr;
Button* restartButton = nullptr;
static PieceVisual pieceVisual;
Texture2D blockTexture;

void UpdateFrame() {
    double deltaTime = GetFrameTime(); // en lugar de nuestro propio cálculo
    if ((IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT))
    && (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER))) 
    {
        ToggleFullscreen();
    }
    Vector2 mouse = GetMousePosition();
    if (gameState->IsGameOver())
    {
        startButton->Update(mouse);
    }
    else if (gameState->IsPaused())
    {
        resumeButton->Update(mouse);
        restartButton->Update(mouse);
    }
    else
    {
        pauseButton->Update(mouse);
        input->Update();
    }
    audio->UpdateMusic();
    

    InputAction action = input->GetAction();
    gameState->HandleInput(action);
    gameState->SetSoftDropHeld(input->IsSoftDropHeld());


    gameState->Update(deltaTime);

    if (!gameState->IsGameOver() && !gameState->IsPaused()) {
        pieceVisual.Update(deltaTime, gameState->GetCurrentPiece(), gameState->GetFallProgress());
    }

    BeginDrawing();
    ClearBackground(darkBlue);
    renderer->DrawGrid(gameState->GetGrid());
    if (!gameState->IsGameOver())
    {
        // Draw current piece using pieceVisual
        Transform2D pieceTransform;
        pieceTransform.position = { 
            241 + pieceVisual.visualCol * 30, 
            1 + pieceVisual.visualRow * 30 
        };
        pieceTransform.rotation = pieceVisual.visualRotation;
        pieceTransform.scale = {1, 1};
        renderer->DrawPiece(gameState->GetCurrentPiece(), pieceTransform, blockTexture);
    }
    
    renderer->DrawUI(*gameState);
    pauseButton->Draw();
    renderer->DrawPause(*gameState);
    if (gameState->IsPaused())
    {
        resumeButton->Draw();
        restartButton->Draw();
    }
    renderer->DrawMenu(*gameState);
    // Dibujar overlay de game over si es necesario
    if (gameState->IsGameOver())
    {
        startButton->Draw();
    }
    EndDrawing();
}

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_ALWAYS_RUN);
    InitWindow(800, 600, "Minimal Tetris");
    SetExitKey(KEY_NULL); 
    InitAudioDevice();
    SetTargetFPS(60);
    // SetGesturesEnabled(GESTURE_DRAG | GESTURE_SWIPE_DOWN | GESTURE_TAP | GESTURE_HOLD);
    Texture2D startButtonTex = LoadTexture("resources/sprites/start-btn.png");   // más directo que LoadTextureFromImage
    Texture2D pauseButtonTex = LoadTexture("resources/sprites/pause-btn.png");   // más directo que LoadTextureFromImage
    Texture2D resumeButtonTex = LoadTexture("resources/sprites/resume-btn.png");   // más directo que LoadTextureFromImage
    Texture2D restartButtonTex = LoadTexture("resources/sprites/restart-btn.png");   // más directo que LoadTextureFromImage
    Sound btnSound = LoadSound("resources/sounds/rotate-block.wav");
    startButton = new Button(startButtonTex, { 400 - 100, 300 }, { 4, 4}, 3, btnSound);
    startButton->SetOnClick([]() {
        gameState->HandleInput(InputAction::Start);
    });

    pauseButton = new Button(pauseButtonTex, { 8, 8 }, { 4, 4}, 3, btnSound);
    pauseButton->SetOnClick([]() {
        gameState->HandleInput(InputAction::Pause);
    });

    resumeButton = new Button(resumeButtonTex, { 370 - 100, 300 }, { 4, 4}, 3, btnSound);
    resumeButton->SetOnClick([]() {
        gameState->HandleInput(InputAction::Pause);
    });

    restartButton = new Button(restartButtonTex, { 490 - 100, 300 }, { 4, 4}, 3, btnSound);
    restartButton->SetOnClick([]() {
        gameState->Reset();
    });

    blockTexture = LoadTextureFromImage(LoadImage("resources/sprites/block.png"));
    blockTexture.format = PIXELFORMAT_COMPRESSED_DXT1_RGB;
    Font font = LoadFontEx("resources/fonts/PressStart2P-Regular.ttf", 24, 0, 0);
    // Si falla, usar fuente por defecto
    if (font.texture.id == 0) font = GetFontDefault();

    renderer = new GameRenderer(font);
    input = new InputHandler();
    audio = new AudioManager();

    gameState = new GameState();

    gameState->onRotate = []() {
        audio->PlayRotateSound();
    // PlaySound(lineClearSound);
    // Podrías también llamar a un método del renderizador
    // renderer->StartLineClearAnimation(lines);
    };

    gameState->onLinesCleared = [](int lines) {
        audio->PlayClearSound();
    // PlaySound(lineClearSound);
    // Podrías también llamar a un método del renderizador
    // renderer->StartLineClearAnimation(lines);
    };
    gameState->onGameOver = []() {
        audio->PlayClearSound();
        // PlaySound(gameOverSound);
        // renderer->StartGameOverAnimation();
    };
    

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateFrame, 0, 1);
#else
    while (!WindowShouldClose()) {
        UpdateFrame();
    }
#endif
    delete startButton; 
    delete pauseButton; 
    delete resumeButton; 
    delete restartButton; 
    delete audio;
    delete input;
    delete renderer;
    delete gameState;

    CloseAudioDevice();
    CloseWindow();
    return 0;
}