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
static PieceVisual pieceVisual;
Texture2D blockTexture;

void UpdateFrame() {
    double deltaTime = GetFrameTime(); // en lugar de nuestro propio cálculo
    Vector2 mouse = GetMousePosition();
    if (gameState->IsGameOver())
    {
        startButton->Update(mouse);
    }
    audio->UpdateMusic();
    // input->UpdateTouch();
    input->Update();

    InputAction action = input->GetAction();
    gameState->HandleInput(action);
    gameState->SetSoftDropHeld(input->IsSoftDropHeld());


    gameState->Update(deltaTime);

    if (!gameState->IsGameOver()) {
        pieceVisual.Update(deltaTime, gameState->GetCurrentPiece(), gameState->GetFallProgress());
    }

    BeginDrawing();
    ClearBackground(darkBlue);
    // renderer->Draw(*gameState, gameState->GetFallProgress());
    renderer->DrawGrid(gameState->GetGrid());
    if (!gameState->IsGameOver())
    {
        // renderer->DrawPiece(gameState->GetCurrentPiece(), 241, -14, gameState->GetFallProgress());
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
    // Dibujar overlay de game over si es necesario
    if (gameState->IsGameOver())
    {
        startButton->Draw();
    }
    EndDrawing();
}

int main() {
    InitWindow(800, 600, "Minimal Tetris");
    InitAudioDevice();
    SetTargetFPS(60);
    SetGesturesEnabled(GESTURE_DRAG | GESTURE_SWIPE_DOWN | GESTURE_TAP | GESTURE_HOLD);
    Texture2D btnTex = LoadTexture("resources/start-btn.png");   // más directo que LoadTextureFromImage
    Sound btnSound = LoadSound("resources/rotate-block.wav");
    startButton = new Button(btnTex, { 400 - 100, 300 }, { 4, 4}, 3, btnSound);
    startButton->SetOnClick([]() {
        gameState->HandleInput(InputAction::Restart);
    });
    blockTexture = LoadTextureFromImage(LoadImage("resources/block.png"));
    blockTexture.format = PIXELFORMAT_COMPRESSED_DXT1_RGB;
    Font font = LoadFontEx("resources/PressStart2P-Regular.ttf", 24, 0, 0);
    // Si falla, usar fuente por defecto
    if (font.texture.id == 0) font = GetFontDefault();

    gameState = new GameState();
    renderer = new GameRenderer(font);
    input = new InputHandler();
    audio = new AudioManager();

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateFrame, 0, 1);
#else
    while (!WindowShouldClose()) {
        UpdateFrame();
    }
#endif
    delete startButton; 
    delete audio;
    delete input;
    delete renderer;
    delete gameState;

    CloseAudioDevice();
    CloseWindow();
    return 0;
}