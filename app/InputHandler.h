// InputHandler.h
#pragma once
#include "InputAction.h"

class InputHandler {
public:
    InputAction GetAction() const;
    bool IsSoftDropHeld() const;
};