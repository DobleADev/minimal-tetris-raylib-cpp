#pragma once
#include "raylib.h"
#include <functional>

class Button {
private:
    Texture2D texture;          // Textura del botón (hoja de sprites)
    Rectangle bounds;           // Área del botón en pantalla (posición y tamaño)
    Rectangle sourceRec;        // Rectángulo de la textura para el frame actual
    int numFrames;              // Número de frames (normal, hover, presionado)
    int state;                  // Estado actual: 0 normal, 1 hover, 2 presionado
    bool action;                // true si se activó el botón en este frame
    Sound clickSound;           // Sonido a reproducir al hacer clic
    bool hasSound;              // Indica si hay sonido cargado
    std::function<void()> onClick;  // Función de callback opcional

public:
    // Constructor principal
    Button(Texture2D tex, Vector2 position, Vector2 scale = { 1, 1}, int frames = 3, Sound sound = {})
        : texture(tex), numFrames(frames), clickSound(sound), hasSound(sound.frameCount > 0)
    {
        float frameHeight = (float)tex.height * scale.y / numFrames;
        sourceRec = { 0, 0, (float)tex.width * scale.x, frameHeight };
        bounds = { position.x, position.y, (float)tex.width * scale.x, frameHeight};
        state = 0;
        action = false;
        texture.width *= scale.x;
        texture.height *= scale.y;
    }

    // Actualizar estado del botón (llamar cada frame)
    void Update(Vector2 mousePoint)
    {
        action = false;

        if (CheckCollisionPointRec(mousePoint, bounds))
        {
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                state = 2;  // Presionado
            else
                state = 1;  // Hover

            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
            {
                action = true;
                if (hasSound) PlaySound(clickSound);
                if (onClick) onClick();  // Llamar callback si está definido
            }
        }
        else
        {
            state = 0;  // Normal
        }

        // Actualizar el frame a dibujar
        sourceRec.y = state * sourceRec.height;
    }

    // Dibujar el botón en pantalla
    void Draw() const
    {
        DrawTextureRec(texture, sourceRec, { bounds.x, bounds.y }, WHITE);
    }

    // Consultar si el botón fue presionado en este frame
    bool IsPressed() const { return action; }

    // Cambiar la posición del botón
    void SetPosition(Vector2 newPos)
    {
        bounds.x = newPos.x;
        bounds.y = newPos.y;
    }

    // Obtener el rectángulo de colisión (por si se necesita)
    Rectangle GetBounds() const { return bounds; }

    // Asignar una función de callback para cuando se presione el botón
    void SetOnClick(std::function<void()> callback)
    {
        onClick = callback;
    }
};