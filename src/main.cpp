#include "raylib.h"

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
    #define CURRENT_PLATFORM "WEB"
#else
    #define CURRENT_PLATFORM "DESKTOP"
#endif

void UpdateDrawFrame(void) {
    // --- LÓGICA ---
    // (Añade tu lógica aquí)

    // --- DIBUJO ---
    BeginDrawing();
        ClearBackground(RAYWHITE);
        
        DrawText(TextFormat("RUNNING ON: %s", CURRENT_PLATFORM), 20, 20, 20, MAROON);
        DrawFPS(20, 50);
        
        DrawText("Template: CMake + VS Code + MSYS2", 20, GetScreenHeight() - 30, 10, DARKGRAY);
    EndDrawing();
}

int main(void) {
    InitWindow(800, 450, "Raylib Project Template");

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
#else
    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        UpdateDrawFrame();
    }
#endif

    CloseWindow();
    return 0;
}