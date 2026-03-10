// AudioManager.cpp
#include "AudioManager.h"

AudioManager::AudioManager() {
    music = LoadMusicStream("resources/Minimal Tetris A-Type Music.ogg");
    SetMusicVolume(music, 0.75f);
    PlayMusicStream(music);
    rotateSound = LoadSound("resources/rotate-block.wav");
    clearSound = LoadSound("resources/line-clear.wav");
}

AudioManager::~AudioManager() {
    UnloadSound(rotateSound);
    UnloadSound(clearSound);
    UnloadMusicStream(music);
    // No cerrar audio aquí porque puede que otros lo usen
}

void AudioManager::UpdateMusic() {
    UpdateMusicStream(music);
}

void AudioManager::PlayRotateSound() {
    PlaySound(rotateSound);
}

void AudioManager::PlayClearSound() {
    PlaySound(clearSound);
}