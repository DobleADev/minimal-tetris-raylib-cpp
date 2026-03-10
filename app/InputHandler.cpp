// InputHandler.cpp
#include "InputHandler.h"
#include "raylib.h"

InputAction InputHandler::GetAction() const {
    if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) return InputAction::MoveLeft;
    if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) return InputAction::MoveRight;
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) return InputAction::Rotate;
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER)) return InputAction::Restart;
    // Aquí podrías añadir más teclas para hold, etc.
    return InputAction::None;
}

bool InputHandler::IsSoftDropHeld() const {
    return IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S);
}