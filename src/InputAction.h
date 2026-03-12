// InputAction.h
#pragma once

enum class InputAction {
    None,
    MoveLeft,
    MoveRight,
    Rotate,
    SoftDrop,      // (usado como estado continuo, no como evento)
    HardDrop,      // <-- NUEVO: caída instantánea
    Restart
};