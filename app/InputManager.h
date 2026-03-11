#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <raylib.h>
#include <unordered_map>

class InputManager {
public:
    // Obtiene la única instancia (singleton)
    static InputManager& GetInstance();

    // Debe llamarse una vez al inicio de cada frame
    void Update();

    // --- Teclado ------------------------------------------------------------
    bool IsKeyPressed(int key) const;
    bool IsKeyDown(int key) const;
    bool IsKeyReleased(int key) const;
    int GetKeyPressed() const;                     // Última tecla pulsada en el frame actual

    // --- Ratón ---------------------------------------------------------------
    bool IsMouseButtonPressed(int button) const;
    bool IsMouseButtonDown(int button) const;
    bool IsMouseButtonReleased(int button) const;
    Vector2 GetMousePosition() const;
    Vector2 GetMouseDelta() const;                 // Movimiento del ratón desde el último frame
    float GetMouseWheelMove() const;                // Desplazamiento de la rueda en el frame actual

    // --- Gamepad (hasta 4 mandos) -------------------------------------------
    static const int MAX_GAMEPADS = 4;              // GAMEPAD_PLAYER1 ... GAMEPAD_PLAYER4

    bool IsGamepadAvailable(int gamepad) const;
    const char* GetGamepadName(int gamepad) const;
    bool IsGamepadButtonPressed(int gamepad, int button) const;
    bool IsGamepadButtonDown(int gamepad, int button) const;
    bool IsGamepadButtonReleased(int gamepad, int button) const;
    float GetGamepadAxisValue(int gamepad, int axis) const;   // Aplica zona muerta
    int GetGamepadAxisCount(int gamepad) const;
    void SetGamepadVibration(int gamepad, float leftMotor, float rightMotor, float duration = 0.0f) const;

    // Configuración de zona muerta (deadzone) para los ejes del mando
    void SetDefaultGamepadDeadzone(float deadzone);                 // Valor por defecto para todos los ejes
    void SetGamepadAxisDeadzone(int gamepad, int axis, float deadzone);  // Por eje específico
    float GetGamepadAxisDeadzone(int gamepad, int axis) const;      // Obtiene la zona muerta actual

    // --- Táctil / Pantalla táctil -------------------------------------------
    int GetTouchPointCount() const;
    Vector2 GetTouchPosition(int index) const;
    int GetTouchPointId(int index) const;

    // --- Gestos --------------------------------------------------------------
    int GetDetectedGesture() const;                 // Gesto detectado en el frame actual (0 si ninguno)
    bool IsGestureDetected(int gesture) const;      // Comprueba si un gesto concreto está activo
    float GetGestureDragAngle() const;
    float GetGesturePinchAngle() const;
    Vector2 GetGestureDragVector() const;
    float GetGestureDragDistance() const;
    Vector2 GetGesturePinchVector() const;
    float GetGesturePinchDistance() const;

private:
    InputManager();
    ~InputManager();

    // Prohibir copia y asignación
    InputManager(const InputManager&) = delete;
    InputManager& operator=(const InputManager&) = delete;

    // Estado interno
    Vector2 _previousMousePosition;
    Vector2 _mouseDelta;
    float _mouseWheelMove;

    float _defaultDeadzone;                                     // Zona muerta por defecto para gamepad
    std::unordered_map<int, float> _axisDeadzones[MAX_GAMEPADS]; // Zonas muertas por eje y mando
};

#endif // INPUTMANAGER_H