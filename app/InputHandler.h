// InputHandler.h
#pragma once
#include "InputAction.h"
#include "raylib.h"

class InputHandler {
public:
    InputHandler();
    
    // Debe llamarse una vez por frame para actualizar el estado del ratón
    void Update();

    // Devuelve la acción discreta detectada en este frame
    InputAction GetAction();

    // Indica si el soft drop está siendo sostenido (arrastre hacia abajo)
    bool IsSoftDropHeld() const;

private:
    // Estado del ratón para arrastre
    bool mousePressed;           // ¿Está presionado el botón izquierdo?
    Vector2 mousePressPos;       // Posición donde se presionó
    float dragAccumX;            // Acumulador de desplazamiento horizontal
    bool clickDetected;          // Se ha detectado un clic (para rotar)

    // Umbrales
    static constexpr float DRAG_THRESHOLD = 30.0f;      // píxeles para mover una casilla
    static constexpr float CLICK_THRESHOLD = 10.0f;     // píxeles máximos para considerar clic
    static constexpr float SOFT_DROP_THRESHOLD = 30.0f;  // píxeles/frame para activar soft drop
    static constexpr float SOFT_DROP_HOLD_TIME = 0.5f;

    void ProcessMouse();
};