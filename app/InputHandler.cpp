// InputHandler.cpp
#include "InputHandler.h"
#include <cmath>
#include <raymath.h>

InputHandler::InputHandler()
    : mousePressed(false)
    , mousePressPos{0,0}
    , dragAccumX(0.0f)
    , clickDetected(false)
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
        clickDetected = false;  // aún no sabemos si será clic
    }
    else if (leftDown && mousePressed) {
        // Arrastrando: calculamos desplazamiento horizontal desde el inicio
        float deltaX = currentMouse.x - mousePressPos.x;
        dragAccumX = deltaX;  // usamos la distancia total, no acumulamos frame a frame
        // Para soft drop, usamos el desplazamiento vertical actual
        // (se evaluará en IsSoftDropHeld)
    }
    else if (!leftDown && mousePressed) {
        // Se soltó el botón
        float dist = Vector2Distance(mousePressPos, currentMouse);
        if (dist < CLICK_THRESHOLD) {
            clickDetected = true;
        }
        mousePressed = false;
        dragAccumX = 0.0f;
    }
}

InputAction InputHandler::GetAction() {
    // 1. Clic detectado al soltar (rotar)
    if (clickDetected) {
        clickDetected = false;  // reset
        return InputAction::Rotate;
    }

    // 2. Movimiento horizontal por arrastre (usando la distancia total)
    if (mousePressed && std::abs(dragAccumX) >= DRAG_THRESHOLD) {
        InputAction action = (dragAccumX > 0) ? InputAction::MoveRight : InputAction::MoveLeft;
        // Consumimos el umbral para que no se repita infinitamente
        // Reiniciamos la posición de referencia para el próximo umbral
        // Una forma sencilla es resetear dragAccumX a 0 después de generar el movimiento,
        // pero eso perdería el arrastre restante. Mejor: restamos el umbral y mantenemos el resto.
        // Para ello, necesitamos actualizar mousePressPos.x sumando o restando el umbral.
        if (dragAccumX > 0) {
            mousePressPos.x += DRAG_THRESHOLD;
        } else {
            mousePressPos.x -= DRAG_THRESHOLD;
        }
        dragAccumX = GetMousePosition().x - mousePressPos.x;  // recalculamos
        return action;
    }

    // 3. Teclado (para PC)
    if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) return InputAction::MoveLeft;
    if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) return InputAction::MoveRight;
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) return InputAction::Rotate;
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER)) return InputAction::Restart;
    if (IsKeyPressed(KEY_SPACE)) return InputAction::HardDrop;

    return InputAction::None;
}

float mouseHeldTime;
bool InputHandler::IsSoftDropHeld() const {
    // Teclado
    bool keyHeld = IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S);

    // Ratón: arrastre hacia abajo durante la presión
    bool mouseHeld = false;
    if (mousePressed) {
        float deltaY = GetMousePosition().y - mousePressPos.y;
        mouseHeldTime += GetFrameTime();
        mouseHeld = (mouseHeldTime > SOFT_DROP_HOLD_TIME);
    }
    else
    {
        mouseHeldTime = 0;
    }

    return keyHeld || mouseHeld;
}