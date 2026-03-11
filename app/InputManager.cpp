#include "InputManager.h"
#include <algorithm> // para std::abs

InputManager& InputManager::GetInstance() {
    static InputManager instance;
    return instance;
}

InputManager::InputManager()
    : _previousMousePosition{ 0, 0 }
    , _mouseDelta{ 0, 0 }
    , _mouseWheelMove(0.0f)
    , _defaultDeadzone(0.1f)   // Valor típico para joysticks
{
    // Inicialmente todos los mapas de deadzone están vacíos
}

InputManager::~InputManager() = default;

void InputManager::Update() {
    // Calcular delta del ratón
    Vector2 currentMousePos = GetMousePosition();
    _mouseDelta.x = currentMousePos.x - _previousMousePosition.x;
    _mouseDelta.y = currentMousePos.y - _previousMousePosition.y;
    _previousMousePosition = currentMousePos;

    // Obtener movimiento de la rueda
    _mouseWheelMove = GetMouseWheelMove();
}

// --- Teclado ----------------------------------------------------------------
bool InputManager::IsKeyPressed(int key) const {
    return ::IsKeyPressed(key);
}

bool InputManager::IsKeyDown(int key) const {
    return ::IsKeyDown(key);
}

bool InputManager::IsKeyReleased(int key) const {
    return ::IsKeyReleased(key);
}

int InputManager::GetKeyPressed() const {
    return ::GetKeyPressed();
}

// --- Ratón ------------------------------------------------------------------
bool InputManager::IsMouseButtonPressed(int button) const {
    return ::IsMouseButtonPressed(button);
}

bool InputManager::IsMouseButtonDown(int button) const {
    return ::IsMouseButtonDown(button);
}

bool InputManager::IsMouseButtonReleased(int button) const {
    return ::IsMouseButtonReleased(button);
}

Vector2 InputManager::GetMousePosition() const {
    return ::GetMousePosition();
}

Vector2 InputManager::GetMouseDelta() const {
    return _mouseDelta;
}

float InputManager::GetMouseWheelMove() const {
    return _mouseWheelMove;
}

// --- Gamepad ----------------------------------------------------------------
bool InputManager::IsGamepadAvailable(int gamepad) const {
    return ::IsGamepadAvailable(gamepad);
}

const char* InputManager::GetGamepadName(int gamepad) const {
    return ::GetGamepadName(gamepad);
}

bool InputManager::IsGamepadButtonPressed(int gamepad, int button) const {
    return ::IsGamepadButtonPressed(gamepad, button);
}

bool InputManager::IsGamepadButtonDown(int gamepad, int button) const {
    return ::IsGamepadButtonDown(gamepad, button);
}

bool InputManager::IsGamepadButtonReleased(int gamepad, int button) const {
    return ::IsGamepadButtonReleased(gamepad, button);
}

float InputManager::GetGamepadAxisValue(int gamepad, int axis) const {
    float value = ::GetGamepadAxisMovement(gamepad, axis);
    float deadzone = GetGamepadAxisDeadzone(gamepad, axis);
    if (std::abs(value) < deadzone) {
        return 0.0f;
    }
    return value;
}

int InputManager::GetGamepadAxisCount(int gamepad) const {
    return ::GetGamepadAxisCount(gamepad);
}

void InputManager::SetGamepadVibration(int gamepad, float leftMotor, float rightMotor, float duration) const {
    ::SetGamepadVibration(gamepad, leftMotor, rightMotor, duration);
}

void InputManager::SetDefaultGamepadDeadzone(float deadzone) {
    _defaultDeadzone = deadzone;
}

void InputManager::SetGamepadAxisDeadzone(int gamepad, int axis, float deadzone) {
    if (gamepad >= 0 && gamepad < MAX_GAMEPADS) {
        _axisDeadzones[gamepad][axis] = deadzone;
    }
}

float InputManager::GetGamepadAxisDeadzone(int gamepad, int axis) const {
    if (gamepad >= 0 && gamepad < MAX_GAMEPADS) {
        auto it = _axisDeadzones[gamepad].find(axis);
        if (it != _axisDeadzones[gamepad].end()) {
            return it->second;
        }
    }
    return _defaultDeadzone;
}

// --- Táctil -----------------------------------------------------------------
int InputManager::GetTouchPointCount() const {
    return ::GetTouchPointCount();
}

Vector2 InputManager::GetTouchPosition(int index) const {
    return ::GetTouchPosition(index);
}

int InputManager::GetTouchPointId(int index) const {
    return ::GetTouchPointId(index);
}

// --- Gestos -----------------------------------------------------------------
int InputManager::GetDetectedGesture() const {
    return ::GetGestureDetected();
}

bool InputManager::IsGestureDetected(int gesture) const {
    return (::GetGestureDetected() & gesture) != 0;  // Si se usan máscaras, aunque normalmente es un único valor
}

float InputManager::GetGestureDragAngle() const {
    return ::GetGestureDragAngle();
}

float InputManager::GetGesturePinchAngle() const {
    return ::GetGesturePinchAngle();
}

Vector2 InputManager::GetGestureDragVector() const {
    return ::GetGestureDragVector();
}

float InputManager::GetGestureDragDistance() const {
    return 0;
    // return ::GetGestureDragDistance();
}

Vector2 InputManager::GetGesturePinchVector() const {
    return ::GetGesturePinchVector();
}

float InputManager::GetGesturePinchDistance() const {
    return 0;
    // return ::GetGesturePinchDistance();
}