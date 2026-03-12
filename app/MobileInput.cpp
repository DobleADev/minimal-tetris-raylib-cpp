// MobileInput.cpp
#include "MobileInput.h"
#include <cmath>   // para sinf, cosf, PI
#include <raymath.h>

// Constantes de gesto (definidas en raylib, pero las repetimos por claridad)
#ifndef GESTURE_NONE
#define GESTURE_NONE        0
#define GESTURE_TAP         1
#define GESTURE_DOUBLETAP   2
#define GESTURE_HOLD        4
#define GESTURE_DRAG        8
#define GESTURE_SWIPE_RIGHT 16
#define GESTURE_SWIPE_LEFT  32
#define GESTURE_SWIPE_UP    64
#define GESTURE_SWIPE_DOWN  128
#define GESTURE_PINCH_IN    256
#define GESTURE_PINCH_OUT   512
#endif

MobileInput::MobileInput()
    : lastGesture(0)
    , previousGesture(0)
    , currentGesture(0)
    , currentDragDegrees(0.0f)
    , currentPitchDegrees(0.0f)
    , dragVector{0,0}
    , touchCount(0)
    , mousePosition{0,0}
    , gestureColor{0,0,0,255}
    , gestureLogIndex(GESTURE_LOG_SIZE)
    , logMode(1)
    , angleLength(90.0f)
    , currentAngleDegrees(0.0f)
    , finalVector{0,0}
    , touchActive(false)
    , touchStartPos{0,0}
    , maxDragDistance(0.0f)
    , clickDetected(false)
    , simulateWithMouse(true)
    , mousePressed(false)
    , mousePressPos{0,0}
    , mouseLastPos{0,0}
    , simulatedDragVector{0,0}
    , simulatedDragDelta{0,0}
    , mouseClickDetected(false)
    , mouseMaxDragDistance(0.0f)
{
    for (int i = 0; i < GESTURE_LOG_SIZE; ++i)
        gestureLog[i][0] = '\0';
}

void MobileInput::Update()
{
    // Guardar estado anterior de toque real
    bool wasTouchActive = touchActive;

    // Obtener datos de raylib (siempre, aunque luego los sobreescribamos con simulación)
    currentGesture = GetGestureDetected();
    currentDragDegrees = GetGestureDragAngle();
    currentPitchDegrees = GetGesturePinchAngle();
    int realTouchCount = GetTouchPointCount();
    mousePosition = GetMousePosition();

    // --- SIMULACIÓN CON RATÓN ---
    if (simulateWithMouse && realTouchCount == 0)
    {
        TraceLog(LOG_INFO, "SIMulación activa - mousePressed: %d, delta: (%f, %f)", mousePressed, simulatedDragDelta.x, simulatedDragDelta.y);
        // Usamos el ratón como un único punto táctil
        bool leftDown = IsMouseButtonDown(MOUSE_BUTTON_LEFT);
        Vector2 currentMouse = GetMousePosition();

        if (leftDown && !mousePressed)
        {
            // Presionó el ratón
            mousePressed = true;
            mousePressPos = currentMouse;
            mouseLastPos = currentMouse;
            simulatedDragVector = {0,0};
            simulatedDragDelta = {0,0};
            mouseMaxDragDistance = 0.0f;
            touchActive = true;  // para la lógica de click
            touchStartPos = currentMouse;  // reutilizamos variables de toque real
            maxDragDistance = 0.0f;
        }
        else if (leftDown && mousePressed)
        {
            // Arrastrando
            Vector2 delta = currentMouse - mouseLastPos;
            simulatedDragDelta = delta;
            simulatedDragVector = currentMouse - mousePressPos;
            mouseLastPos = currentMouse;

            float dist = Vector2Distance(mousePressPos, currentMouse);
            if (dist > mouseMaxDragDistance) mouseMaxDragDistance = dist;
            // Actualizar también las variables de toque real (para que IsClickDetected funcione)
            maxDragDistance = mouseMaxDragDistance;
        }
        else if (!leftDown && mousePressed)
        {
            // Soltó el ratón
            if (mouseMaxDragDistance < CLICK_DRAG_THRESHOLD)
            {
                mouseClickDetected = true;
            }
            mousePressed = false;
            simulatedDragVector = {0,0};
            simulatedDragDelta = {0,0};
            touchActive = false;  // para la lógica de click
        }

        // Configurar variables de "toque" para el resto del sistema
        touchCount = mousePressed ? 1 : 0;
        if (touchCount > 0)
        {
            touchPositions[0] = currentMouse;
        }
        // Los gestos (currentGesture, etc.) los dejamos como están (0)
        // Pero podríamos simular algunos gestos si quisiéramos (por ahora no)
    }
    else
    {
        // Modo normal: usar datos reales de toque
        touchCount = realTouchCount;
        dragVector = (touchCount > 0) ? GetGestureDragVector() : Vector2{0,0};
        // Nota: dragDelta se calculará más abajo de forma unificada
        // Restaurar variables de simulación a estado neutro
        mousePressed = false;
        simulatedDragVector = {0,0};
        simulatedDragDelta = {0,0};
    }

    // --- Lógica común de detección de click (usando touchActive y maxDragDistance) ---
    // (Ya se maneja dentro de la simulación o con toques reales)
    // Pero necesitamos unificar: para toques reales, la detección de click ya está en el código anterior.
    // Vamos a reestructurar: la detección de click se basa en touchActive y maxDragDistance,
    // que ya se actualizan tanto en modo real como simulado.

    // Para toques reales, necesitamos mantener touchActive y maxDragDistance.
    // Añadimos esa lógica si no estamos en simulación:
    if (!simulateWithMouse || realTouchCount > 0)
    {
        // Toques reales
        if (touchCount > 0)
        {
            Vector2 currentPos = GetTouchPosition(0);
            if (!wasTouchActive)
            {
                touchActive = true;
                touchStartPos = currentPos;
                maxDragDistance = 0.0f;
            }
            else
            {
                float dist = Vector2Distance(touchStartPos, currentPos);
                if (dist > maxDragDistance) maxDragDistance = dist;
            }
            // Actualizar dragVector y dragDelta (para que estén disponibles)
            Vector2 newDrag = GetGestureDragVector(); // ya lo tenemos arriba
            dragDelta = newDrag - previousDragVector;
            previousDragVector = newDrag;
        }
        else
        {
            if (wasTouchActive)
            {
                if (maxDragDistance < CLICK_DRAG_THRESHOLD)
                {
                    clickDetected = true;
                }
                touchActive = false;
            }
            dragVector = {0,0};
            dragDelta = {0,0};
            previousDragVector = {0,0};
        }
    }

    // Para la simulación, ya hemos actualizado clickDetected (usamos mouseClickDetected)
    // Unificamos: al final, clickDetened se obtiene de donde corresponda.
    // Podemos hacer que clickDetected sea true si mouseClickDetened es true.
    if (mouseClickDetected)
    {
        clickDetected = true;
        mouseClickDetected = false;
    }

    // Actualizar último gesto significativo (solo para toques reales, ignoramos simulación)
    if (currentGesture != 0 && currentGesture != GESTURE_HOLD && currentGesture != previousGesture)
        lastGesture = currentGesture;

    // Actualizar registro de gestos (solo para gestos reales)
    UpdateLog(currentGesture);

    // Actualizar transportador (usa currentGesture, que puede ser 0 en simulación)
    UpdateProtractor();

    // Para la simulación, también podríamos generar gestos simulados (como arrastre) si queremos,
    // pero por ahora no es necesario.
}

bool MobileInput::IsClickDetected()
{
    if (clickDetected) {
        clickDetected = false;  // reset para el próximo frame
        return true;
    }
    return false;
}

void MobileInput::UpdateLog(int gesture)
{
    int fillLog = 0;
    if (gesture != 0)
    {
        if (logMode == 3) // ocultar repeticiones y hold
        {
            if ((gesture != GESTURE_HOLD && gesture != previousGesture) || gesture < 3)
                fillLog = 1;
        }
        else if (logMode == 2) // mostrar repeticiones pero ocultar hold
        {
            if (gesture != GESTURE_HOLD)
                fillLog = 1;
        }
        else if (logMode == 1) // ocultar repeticiones
        {
            if (gesture != previousGesture)
                fillLog = 1;
        }
        else // modo 0: mostrar todo
        {
            fillLog = 1;
        }
    }

    if (fillLog)
    {
        previousGesture = gesture;
        gestureColor = GetGestureColor(gesture);

        // Insertar en el registro (circular invertido: nos movemos hacia atrás)
        if (gestureLogIndex <= 0)
            gestureLogIndex = GESTURE_LOG_SIZE;
        gestureLogIndex--;

        TextCopy(gestureLog[gestureLogIndex], GetGestureName(gesture));
    }
}

void MobileInput::UpdateProtractor()
{
    // Determinar qué ángulo mostrar según el tipo de gesto
    if (currentGesture > 255)  // Pinch
        currentAngleDegrees = currentPitchDegrees;
    else if (currentGesture > 15)  // Swipe
        currentAngleDegrees = currentDragDegrees;
    else
        currentAngleDegrees = 0.0f;

    // Calcular el vector final para la línea del transportador
    float currentAngleRadians = (currentAngleDegrees + 90.0f) * PI / 180.0f;
    finalVector.x = angleLength * sinf(currentAngleRadians);
    finalVector.y = angleLength * cosf(currentAngleRadians);
}

// --- Getters ---

int MobileInput::GetCurrentGesture() const { return currentGesture; }
float MobileInput::GetDragAngle() const { return currentDragDegrees; }
float MobileInput::GetPinchAngle() const { return currentPitchDegrees; }
int MobileInput::GetTouchCount() const { return touchCount; }

Vector2 MobileInput::GetTouchPosition(int index) const
{
    if (index >= 0 && index < touchCount && index < MAX_TOUCH_COUNT)
        return touchPositions[index];
    return {0, 0};
}

Vector2 MobileInput::GetMousePosition() const { return mousePosition; }

void MobileInput::SetLogMode(int mode) { if (mode >= 0 && mode <= 3) logMode = mode; }
int MobileInput::GetLogMode() const { return logMode; }

const char* MobileInput::GetLogEntry(int index) const
{
    if (index < 0 || index >= GESTURE_LOG_SIZE)
        return "";
    // El índice 0 es el más reciente: partimos de gestureLogIndex y avanzamos
    int realIndex = (gestureLogIndex + index) % GESTURE_LOG_SIZE;
    return gestureLog[realIndex];
}

int MobileInput::GetLogSize() const { return GESTURE_LOG_SIZE; }

// --- Dibujado de depuración ---

void MobileInput::DrawDebugOverlay(Vector2 lastGesturePos, Vector2 logPos, Vector2 protractorPos)
{
    // 1. Dibujar iconos del último gesto (similar al ejemplo)
    DrawText("Last gesture", (int)lastGesturePos.x + 33, (int)lastGesturePos.y - 47, 20, BLACK);
    DrawText("Swipe         Tap       Pinch  Touch", (int)lastGesturePos.x + 17, (int)lastGesturePos.y - 18, 10, BLACK);

    // Cuadros de swipe
    DrawRectangle((int)lastGesturePos.x + 20, (int)lastGesturePos.y, 20, 20, lastGesture == GESTURE_SWIPE_UP ? RED : LIGHTGRAY);
    DrawRectangle((int)lastGesturePos.x, (int)lastGesturePos.y + 20, 20, 20, lastGesture == GESTURE_SWIPE_LEFT ? RED : LIGHTGRAY);
    DrawRectangle((int)lastGesturePos.x + 40, (int)lastGesturePos.y + 20, 20, 20, lastGesture == GESTURE_SWIPE_RIGHT ? RED : LIGHTGRAY);
    DrawRectangle((int)lastGesturePos.x + 20, (int)lastGesturePos.y + 40, 20, 20, lastGesture == GESTURE_SWIPE_DOWN ? RED : LIGHTGRAY);

    // Tap y drag
    DrawCircle((int)lastGesturePos.x + 80, (int)lastGesturePos.y + 16, 10, lastGesture == GESTURE_TAP ? BLUE : LIGHTGRAY);
    DrawRing({lastGesturePos.x + 103, lastGesturePos.y + 16}, 6.0f, 11.0f, 0.0f, 360.0f, 0, lastGesture == GESTURE_DRAG ? LIME : LIGHTGRAY);

    // Doble tap (dos círculos)
    DrawCircle((int)lastGesturePos.x + 80, (int)lastGesturePos.y + 43, 10, lastGesture == GESTURE_DOUBLETAP ? SKYBLUE : LIGHTGRAY);
    DrawCircle((int)lastGesturePos.x + 103, (int)lastGesturePos.y + 43, 10, lastGesture == GESTURE_DOUBLETAP ? SKYBLUE : LIGHTGRAY);

    // Pinch out (triángulos que apuntan hacia afuera)
    DrawTriangle({lastGesturePos.x + 122, lastGesturePos.y + 16},
                 {lastGesturePos.x + 137, lastGesturePos.y + 26},
                 {lastGesturePos.x + 137, lastGesturePos.y + 6},
                 lastGesture == GESTURE_PINCH_OUT ? ORANGE : LIGHTGRAY);
    DrawTriangle({lastGesturePos.x + 147, lastGesturePos.y + 6},
                 {lastGesturePos.x + 147, lastGesturePos.y + 26},
                 {lastGesturePos.x + 162, lastGesturePos.y + 16},
                 lastGesture == GESTURE_PINCH_OUT ? ORANGE : LIGHTGRAY);

    // Pinch in (triángulos que apuntan hacia adentro)
    DrawTriangle({lastGesturePos.x + 125, lastGesturePos.y + 33},
                 {lastGesturePos.x + 125, lastGesturePos.y + 53},
                 {lastGesturePos.x + 140, lastGesturePos.y + 43},
                 lastGesture == GESTURE_PINCH_IN ? VIOLET : LIGHTGRAY);
    DrawTriangle({lastGesturePos.x + 144, lastGesturePos.y + 43},
                 {lastGesturePos.x + 159, lastGesturePos.y + 53},
                 {lastGesturePos.x + 159, lastGesturePos.y + 33},
                 lastGesture == GESTURE_PINCH_IN ? VIOLET : LIGHTGRAY);

    // Indicadores de toque (puntos)
    for (int i = 0; i < 4; i++)
        DrawCircle((int)lastGesturePos.x + 180, (int)lastGesturePos.y + 7 + i*15, 5, touchCount <= i ? LIGHTGRAY : gestureColor);

    // 2. Dibujar el registro de gestos (log)
    DrawText("Log", (int)logPos.x, (int)logPos.y, 20, BLACK);
    for (int i = 0; i < GESTURE_LOG_SIZE; i++)
    {
        const char* entry = GetLogEntry(i);
        Color color = (i == 0) ? gestureColor : LIGHTGRAY;
        DrawText(entry, (int)logPos.x, (int)logPos.y + 410 - i * 20, 20, color);
    }

    // 3. Dibujar transportador
    DrawText("Angle", (int)protractorPos.x + 55, (int)protractorPos.y + 76, 10, BLACK);
    const char *angleString = TextFormat("%f", currentAngleDegrees);
    int dotPos = TextFindIndex(angleString, ".");
    if (dotPos > 0)
        angleString = TextSubtext(angleString, 0, dotPos + 3); // recortar a 2 decimales
    DrawText(angleString, (int)protractorPos.x + 55, (int)protractorPos.y + 92, 20, gestureColor);

    // Círculo base y líneas de referencia
    DrawCircleV(protractorPos, 80.0f, WHITE);
    DrawLineEx({protractorPos.x - 90, protractorPos.y}, {protractorPos.x + 90, protractorPos.y}, 3.0f, LIGHTGRAY);
    DrawLineEx({protractorPos.x, protractorPos.y - 90}, {protractorPos.x, protractorPos.y + 90}, 3.0f, LIGHTGRAY);
    DrawLineEx({protractorPos.x - 80, protractorPos.y - 45}, {protractorPos.x + 80, protractorPos.y + 45}, 3.0f, GREEN);
    DrawLineEx({protractorPos.x - 80, protractorPos.y + 45}, {protractorPos.x + 80, protractorPos.y - 45}, 3.0f, GREEN);

    // Marcas de ángulos
    DrawText("0",   (int)protractorPos.x + 96, (int)protractorPos.y - 9, 20, BLACK);
    DrawText("30",  (int)protractorPos.x + 74, (int)protractorPos.y - 68, 20, BLACK);
    DrawText("90",  (int)protractorPos.x - 11, (int)protractorPos.y - 110, 20, BLACK);
    DrawText("150", (int)protractorPos.x - 100, (int)protractorPos.y - 68, 20, BLACK);
    DrawText("180", (int)protractorPos.x - 124, (int)protractorPos.y - 9, 20, BLACK);
    DrawText("210", (int)protractorPos.x - 100, (int)protractorPos.y + 50, 20, BLACK);
    DrawText("270", (int)protractorPos.x - 18, (int)protractorPos.y + 92, 20, BLACK);
    DrawText("330", (int)protractorPos.x + 72, (int)protractorPos.y + 50, 20, BLACK);

    // Línea del ángulo actual (si es distinto de cero)
    if (currentAngleDegrees != 0.0f)
    {
        Vector2 end = { protractorPos.x + finalVector.x, protractorPos.y + finalVector.y };
        DrawLineEx(protractorPos, end, 3.0f, gestureColor);
    }

    // 4. Dibujar puntos táctiles o ratón
    if (currentGesture != GESTURE_NONE)
    {
        if (touchCount > 0)
        {
            for (int i = 0; i < touchCount; i++)
            {
                DrawCircleV(touchPositions[i], 50.0f, Fade(gestureColor, 0.5f));
                DrawCircleV(touchPositions[i], 5.0f, gestureColor);
            }
            if (touchCount == 2)
            {
                float thickness = (currentGesture == GESTURE_PINCH_OUT) ? 8.0f : 12.0f;
                DrawLineEx(touchPositions[0], touchPositions[1], thickness, gestureColor);
            }
        }
        else
        {
            DrawCircleV(mousePosition, 35.0f, Fade(gestureColor, 0.5f));
            DrawCircleV(mousePosition, 5.0f, gestureColor);
        }
    }
}

Vector2 MobileInput::GetDragVector() const
{
    if (simulateWithMouse && mousePressed)
        return simulatedDragVector;
    else
        return dragVector;
}

Vector2 MobileInput::GetDragDelta() const
{
    if (simulateWithMouse && mousePressed)
        return simulatedDragDelta;
    else
        return dragDelta;
}

void MobileInput::SetSimulateWithMouse(bool enable)
{
    simulateWithMouse = enable;
    // Reiniciar estado de simulación al cambiar
    mousePressed = false;
    mouseClickDetected = false;
    simulatedDragVector = {0,0};
    simulatedDragDelta = {0,0};
}

bool MobileInput::IsSimulatingWithMouse() const
{
    return simulateWithMouse;
}

// --- Funciones estáticas auxiliares ---

const char* MobileInput::GetGestureName(int gesture)
{
    switch (gesture)
    {
        case GESTURE_NONE:       return "None";
        case GESTURE_TAP:        return "Tap";
        case GESTURE_DOUBLETAP:  return "Double Tap";
        case GESTURE_HOLD:       return "Hold";
        case GESTURE_DRAG:       return "Drag";
        case GESTURE_SWIPE_RIGHT:return "Swipe Right";
        case GESTURE_SWIPE_LEFT: return "Swipe Left";
        case GESTURE_SWIPE_UP:   return "Swipe Up";
        case GESTURE_SWIPE_DOWN: return "Swipe Down";
        case GESTURE_PINCH_IN:   return "Pinch In";
        case GESTURE_PINCH_OUT:  return "Pinch Out";
        default:                  return "Unknown";
    }
}

Color MobileInput::GetGestureColor(int gesture)
{
    switch (gesture)
    {
        case GESTURE_TAP:        return BLUE;
        case GESTURE_DOUBLETAP:  return SKYBLUE;
        case GESTURE_DRAG:       return LIME;
        case GESTURE_SWIPE_RIGHT:
        case GESTURE_SWIPE_LEFT:
        case GESTURE_SWIPE_UP:
        case GESTURE_SWIPE_DOWN: return RED;
        case GESTURE_PINCH_IN:   return VIOLET;
        case GESTURE_PINCH_OUT:  return ORANGE;
        default:                  return BLACK;
    }
}