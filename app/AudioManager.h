// AudioManager.h
#pragma once
#include "raylib.h"

class AudioManager {
public:
    AudioManager();
    ~AudioManager();
    void UpdateMusic();
    void PlayRotateSound();
    void PlayClearSound();
private:
    Music music;
    Sound rotateSound;
    Sound clearSound;
};