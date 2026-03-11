#pragma once
#include <raylib.h>
#include <cmath>
struct Transform2D {
    Vector2 position;      // world position of the piece's origin (in pixels)
    float rotation;        // in degrees
    Vector2 scale;         // usually {1,1} for Tetris

    // Transform a local point (relative to piece origin) to world space
    Vector2 TransformPoint(Vector2 local) const {
        // Rotate around origin
        float rad = rotation * DEG2RAD;
        float cosA = cosf(rad);
        float sinA = sinf(rad);
        float x = local.x * cosA - local.y * sinA;
        float y = local.x * sinA + local.y * cosA;
        // Scale (optional) then translate
        return { position.x + x * scale.x, position.y + y * scale.y };
    }
};