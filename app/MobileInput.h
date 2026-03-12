// MobileInput.h (actualizado)
#pragma once

#include "raylib.h"

#define GESTURE_LOG_SIZE    20
#define MAX_TOUCH_COUNT     32
#define CLICK_DRAG_THRESHOLD 10.0f  // píxeles máximos para considerar click

class MobileInput {
public:
    MobileInput();
    void Update();

    // Getters
    int GetCurrentGesture() const;
    float GetDragAngle() const;
    Vector2 GetDragVector() const;
    Vector2 GetDragDelta() const;
    float GetDragDistance() const;
    float GetPinchAngle() const;
    int GetTouchCount() const;
    Vector2 GetTouchPosition(int index) const;
    Vector2 GetMousePosition() const;

    // Click detection (toque y soltar sin arrastre)
    bool IsClickDetected();  // devuelve true una vez por frame cuando ocurre un click

    // Log
    void SetLogMode(int mode);
    int GetLogMode() const;
    const char* GetLogEntry(int index) const;
    int GetLogSize() const;

    void DrawDebugOverlay(Vector2 lastGesturePos, Vector2 logPos, Vector2 protractorPos);
    void SetSimulateWithMouse(bool enable);  // activar/desactivar simulación
    bool IsSimulatingWithMouse() const;

private:
    Vector2 dragVector;
    Vector2 previousDragVector;     // <-- NUEVO: para calcular delta
    Vector2 dragDelta;
    int lastGesture;
    int previousGesture;
    int currentGesture;
    float currentDragDegrees;
    float currentPitchDegrees;
    int touchCount;
    Vector2 touchPositions[MAX_TOUCH_COUNT];
    Vector2 mousePosition;
    Color gestureColor;

    char gestureLog[GESTURE_LOG_SIZE][12];
    int gestureLogIndex;
    int logMode;

    float angleLength;
    float currentAngleDegrees;
    Vector2 finalVector;

    // Click detection
    bool touchActive;
    Vector2 touchStartPos;
    float maxDragDistance;
    bool clickDetected;

    void UpdateLog(int gesture);
    void UpdateProtractor();
    static const char* GetGestureName(int gesture);
    static Color GetGestureColor(int gesture);

    // Simulación con ratón
    bool simulateWithMouse;          // true = usar ratón cuando no hay toques
    bool mousePressed;               // estado del botón izquierdo
    Vector2 mousePressPos;            // posición donde se presionó
    Vector2 mouseLastPos;             // última posición del ratón (para delta)
    Vector2 simulatedDragVector;      // vector de arrastre simulado (desde press)
    Vector2 simulatedDragDelta;       // delta entre frames
    bool mouseClickDetected;          // click simulado
    float mouseMaxDragDistance;       // distancia máxima durante la presión
};