#include "raylib.h"
#include "GameState.h"
#include "GameRenderer.h"
#include "InputHandler.h"
#include "AudioManager.h"
#include "Colors.h"

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

GameState* gameState;
GameRenderer* renderer;
InputHandler* input;
AudioManager* audio;

void UpdateFrame() {
    double deltaTime = GetFrameTime(); // en lugar de nuestro propio cálculo
    
    audio->UpdateMusic();

    InputAction action = input->GetAction();
    gameState->HandleInput(action);
    gameState->SetSoftDropHeld(input->IsSoftDropHeld());


    gameState->Update(deltaTime);

    BeginDrawing();
    ClearBackground(darkBlue);
    renderer->Draw(*gameState, gameState->GetFallProgress());
    // Dibujar overlay de game over si es necesario
    EndDrawing();
}

int main() {
    InitWindow(800, 600, "Minimal Tetris");
    InitAudioDevice();
    SetTargetFPS(60);

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

    delete audio;
    delete input;
    delete renderer;
    delete gameState;

    CloseAudioDevice();
    CloseWindow();
    return 0;
}