// InputHandler.cpp
#include "InputHandler.h"
#include "raylib.h"
#include <cmath>

InputHandler::InputHandler() : dragAccumX(0.0f) {}

void InputHandler::UpdateTouch() {
    mobileInput.Update();
    ProcessDrag();
}

void InputHandler::ProcessDrag() {
    if (mobileInput.GetTouchCount() > 0) {
        Vector2 delta = mobileInput.GetDragDelta();
        dragAccumX += delta.x;
    } else {
        dragAccumX = 0.0f;
    }
}

InputAction InputHandler::GetAction() {
    // 1. Click táctil
    if (mobileInput.IsClickDetected()) {
        return InputAction::Rotate;
    }

    // 2. Swipe hacia abajo (gesto rápido)
    int gesture = mobileInput.GetCurrentGesture();
    if (gesture == GESTURE_SWIPE_DOWN) {
        return InputAction::HardDrop;
    }

    // 3. Movimiento horizontal por arrastre (acumulador por frame)
    if (std::abs(dragAccumX) >= DRAG_THRESHOLD) {
        InputAction action = (dragAccumX > 0) ? InputAction::MoveRight : InputAction::MoveLeft;
        dragAccumX -= (dragAccumX > 0) ? DRAG_THRESHOLD : -DRAG_THRESHOLD;
        return action;
    }

    // 4. Teclado
    if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) return InputAction::MoveLeft;
    if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) return InputAction::MoveRight;
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) return InputAction::Rotate;
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER)) return InputAction::Restart;

    return InputAction::None;
}

bool InputHandler::IsSoftDropHeld() const {
    // Teclado
    bool keyHeld = IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S);

    // Táctil: arrastre hacia abajo en el frame actual
    bool touchHeld = false;
    if (mobileInput.GetTouchCount() > 0) {
        Vector2 delta = mobileInput.GetDragDelta();
        touchHeld = (delta.y > SOFT_DROP_THRESHOLD);
    }

    return keyHeld || touchHeld;
}