#pragma once
#include <cmath>
#include "Tetromino.h"
#include <algorithm>
struct PieceVisual {
    float visualRow;      // in grid cells (float)
    float visualCol;
    float visualRotation; // in degrees (0, 90, 180, 270)

    void Update(float deltaTime, const Tetromino& logicalPiece, const float fallProgress) {
        // Target values from logical piece
        float targetRow = logicalPiece.GetRow();
        float targetCol = logicalPiece.GetCol();
        float targetRot = logicalPiece.GetRotation() * 90.0f;

        // Lerp smoothly (adjust speed as needed)
        float seconds = 0.02f; // units per second
        // visualRow = Approach(visualRow, targetRow, deltaTime * lerpSpeed);
        // visualRow = logicalPiece.GetRow();
        // visualCol = logicalPiece.GetCol();
        visualRow = targetRow + fallProgress;
        visualCol = Lerp(visualCol, targetCol, deltaTime, seconds);
        visualRotation = LerpAngle(visualRotation, targetRot, deltaTime, seconds); // 300°/s
    }

    void Snap(const Tetromino& logicalPiece) {
        visualRow = logicalPiece.GetRow();
        visualCol = logicalPiece.GetCol();
        visualRotation = logicalPiece.GetRotation() * 90.0f;
    }

private:
    float Lerp(float from, float to, const float deltaTime, const float seconds) {
        from += (to - from) * (1 - powf(0.5f, deltaTime / seconds));
        return from;
    }

    float LerpAngle(float from, float to, const float deltaTime, const float seconds) {
        float diff = from - to;
        if (diff > 0) {
            from -= 360.0f;
        }
        from += (to - from) * (1 - powf(0.5f, deltaTime / seconds));
        return from;
    }

    // float Repeat(float t, float length) {
    //     return t - std::floor(t / length) * length;
    // }

    // float LerpAngle(float a, float b, const float deltaTime, const float seconds) {
    //     // 1. Calculate the raw difference
    //     float t = (1 - powf(0.5f, deltaTime / seconds));
    //     float delta = Repeat(b - a, t);
        
    //     // 2. Adjust to find the shortest path (-180 to 180)
    //     if (delta > 180.0f) {
    //         delta -= 360.0f;
    //     }
        
    //     // 3. Perform the standard lerp on the adjusted delta
    //     return a + delta * std::clamp(t, 0.0f, 1.0f);
    // }
};