// InputHandler.h
#pragma once
#include "InputAction.h"
#include "MobileInput.h"

class InputHandler {
public:
    InputHandler();
    
    void UpdateTouch();
    InputAction GetAction();        // ya no es const porque modifica acumuladores
    bool IsSoftDropHeld() const;

private:
    MobileInput mobileInput;
    float dragAccumX;                // ya no es mutable
    static constexpr float DRAG_THRESHOLD = 30.0f;      // píxeles para mover una casilla
    static constexpr float SOFT_DROP_THRESHOLD = 5.0f;  // píxeles/frame para activar soft drop

    void ProcessDrag();
};