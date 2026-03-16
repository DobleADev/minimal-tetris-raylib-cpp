// InputHandler.cpp
#include "InputHandler.h"
#include <cmath>
#include <raymath.h>

InputHandler::InputHandler()
    : mousePressed(false)
    , mousePressPos{0,0}
    , dragAccumX(0.0f)
    , clickDetected(false)
    , swipeDownDetected(false)
    , isHorizontalDrag(false)
{}

void InputHandler::Update() {
    ProcessMouse();
}

void InputHandler::ProcessMouse() {
    bool leftDown = IsMouseButtonDown(MOUSE_BUTTON_LEFT);
    Vector2 currentMouse = GetMousePosition();

    if (leftDown && !mousePressed) {
        // Inicio de presión
        mousePressed = true;
        mousePressPos = currentMouse;
        dragAccumX = 0.0f;
        clickDetected = false;
        swipeDownDetected = false;
        isHorizontalDrag = false;     // aún no sabemos
    }
    else if (leftDown && mousePressed) {
        // Arrastrando: actualizar acumulador y evaluar dirección
        float deltaX = currentMouse.x - mousePressPos.x;
        float deltaY = currentMouse.y - mousePressPos.y;
        dragAccumX = deltaX;   // mantenemos para movimiento

        // Determinar si el arrastre es predominantemente horizontal
        // Solo si llevamos al menos 10 píxeles en alguna dirección para evitar ruido
        if (std::abs(deltaX) > 10 || std::abs(deltaY) > 10) {
            if (std::abs(deltaY) * HORIZONTAL_RATIO < std::abs(deltaX)) {
                isHorizontalDrag = true;
            } else {
                // Si es muy vertical, desactivamos el arrastre horizontal
                isHorizontalDrag = false;
            }
        }
    }
    else if (!leftDown && mousePressed) {
        // Se soltó el botón: detectar swipe o clic
        float dist = Vector2Distance(mousePressPos, currentMouse);
        float deltaY = currentMouse.y - mousePressPos.y;
        float deltaX = std::abs(currentMouse.x - mousePressPos.x);

        if (deltaY > SWIPE_THRESHOLD && deltaX < SWIPE_THRESHOLD / 2) {
            swipeDownDetected = true;
        }
        else if (dist < CLICK_THRESHOLD) {
            clickDetected = true;
        }

        mousePressed = false;
        dragAccumX = 0.0f;
        isHorizontalDrag = false;
    }
}

InputAction InputHandler::GetAction() {
    // 1. Swipe hacia abajo (hard drop)
    if (swipeDownDetected) {
        swipeDownDetected = false;
        return InputAction::HardDrop;
    }

    // 2. Clic (rotar)
    if (clickDetected) {
        clickDetected = false;
        return InputAction::Rotate;
    }

    // 3. Movimiento horizontal por arrastre (solo si es predominantemente horizontal)
    if (mousePressed && isHorizontalDrag && std::abs(dragAccumX) >= DRAG_THRESHOLD) {
        InputAction action = (dragAccumX > 0) ? InputAction::MoveRight : InputAction::MoveLeft;
        if (dragAccumX > 0) {
            mousePressPos.x += DRAG_THRESHOLD;
        } else {
            mousePressPos.x -= DRAG_THRESHOLD;
        }
        dragAccumX = GetMousePosition().x - mousePressPos.x;
        return action;
    }
    // 4. Teclado
    if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) return InputAction::MoveLeft;
    if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) return InputAction::MoveRight;
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) return InputAction::Rotate;
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER)) return InputAction::Start;
    if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_P)) return InputAction::Pause;
    if (IsKeyPressed(KEY_SPACE)) return InputAction::HardDrop;

    return InputAction::None;
}

float mouseHeldTime;   // variable global (podría ser miembro mutable, pero se mantiene así por simplicidad)

bool InputHandler::IsSoftDropHeld() const {
    bool keyHeld = IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S);

    bool mouseHeld = false;
    if (mousePressed) {
        // Se usa mouseHeldTime para evitar activación instantánea
        mouseHeldTime += GetFrameTime();
        mouseHeld = (mouseHeldTime > SOFT_DROP_HOLD_TIME);
    } else {
        mouseHeldTime = 0;
    }

    return keyHeld || mouseHeld;
}