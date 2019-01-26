#include "Struct.h"

typedef struct {
    Uint8 *position;
    Uint32 length;
} AudioData;

AudioData audio[13] = {{0, 0},
                       {0, 0},
                       {0, 0},
                       {0, 0},
                       {0, 0},
                       {0, 0},
                       {0, 0},
                       {0, 0},
                       {0, 0},
                       {0, 0},
                       {0, 0},
                       {0, 0},
                       {0, 0}};
SDL_AudioSpec wavSpec[13];
Uint8 *wavStart[13];
Uint32 wavLength[13];
SDL_AudioDeviceID audioDevice[13];
char files[][100] = {"../Sounds/\u200CBackground.wav", "../Sounds/Button_Click.wav", "../Sounds/Button_Toggle.wav", "../Sounds/Explosion.wav", "../Sounds/Finish_Sound.wav", "../Sounds/Game_Opening.wav", "../Sounds/Game_Over.wav", "../Sounds/Laser_Shoot.wav", "../Sounds/Mining.wav", "../Sounds/PowerUp_Achievement.wav", "../Sounds/PowerUp_Appearance.wav", "../Sounds/Regeneration_Sound.wav", "../Sounds/Shoot.wav"};

void audioCallback(void *userData, Uint8 *stream, int streamLength) {
    AudioData *audio = (AudioData *) userData;
    if (audio->length == 0) {
        return;
    }
    Uint32 length = (Uint32) streamLength;
    length = (length > audio->length ? audio->length : length);
    SDL_memcpy(stream, audio->position, length);
    audio->position += length;
    audio->length -= length;
}

void load_audio() {
    for (Sint8 i = 0; i < 13; i++) {
        if (SDL_LoadWAV(files[i], &wavSpec[i], &wavStart[i], &wavLength[i]) == NULL) {
            printf("%d : Error: file could not be loaded as an audio file.\n", i);
        }
    }
}

void play_sound(Sint8 i) {
    if (audio[i].length <= 0 || i) {
        SDL_CloseAudioDevice(audioDevice[i]);
        if (SDL_LoadWAV(files[i], &wavSpec[i], &wavStart[i], &wavLength[i]) == NULL) {
            printf("Error: file could not be loaded as an audio file.\n");
        }
        audio[i].position = wavStart[i];
        audio[i].length = wavLength[i];
        wavSpec[i].callback = audioCallback;
        wavSpec[i].userdata = &audio[i];
        audioDevice[i] = SDL_OpenAudioDevice(NULL, 0, &wavSpec[i], NULL, SDL_AUDIO_ALLOW_ANY_CHANGE);
        if (audioDevice[i] == 0) {
            printf("Error!: %s\n", SDL_GetError());
            return;
        }
        SDL_PauseAudioDevice(audioDevice[i], 0);
    }
}

void pause_background() {
    if (audio[BACKGROUND_S].length > 0) {
        SDL_PauseAudioDevice(audioDevice[BACKGROUND_S], 1);
        audio[BACKGROUND_S].length = 0;
    }
}

void handle_sounds() {
    for (Sint8 i = 1; i < 13; i++) {
        if (!audio[i].length && audioDevice[i]) {
            SDL_PauseAudioDevice(audioDevice[i], 1);
        }
    }
}