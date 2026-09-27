#include "all.h"
#include <SDL2/SDL.h>

// SDL counterpart of F_WIN/W_SOUND2.CPP. As with DirectSound in the original,
// the game mixes the sound itself (CustomMixer in SDL_HHIGH.CPP) on the main
// thread whenever hang() is called, and the mixed blocks are queued to the
// audio device, so no locking is needed.

int Mostvoltegynemcurpos = 0;

void CustomMixer(void* buffer1, int byteszam1, void* buffer2, int byteszam2, int bitmod16);

namespace {
SDL_AudioDeviceID device = 0;
// Same format and block size as the original: 11025 Hz, 16 bit mono,
// 1100 byte blocks, about two blocks queued ahead of playback.
const int block_samples = 550;
const Uint32 queued_bytes_target = 2 * block_samples * sizeof(short);
short block[block_samples];
}

void initdsound(int) {
    if (!Hangenabled)
        hiba((char*)"initdsound, pedig !Hangenabled!");
    Hangenabled = 0;

    if (SDL_InitSubSystem(SDL_INIT_AUDIO) != 0) {
        fprintf(stderr, "SDL audio: %s\n", SDL_GetError());
        return;
    }
    SDL_AudioSpec want;
    SDL_zero(want);
    want.freq = 11025;
    want.format = AUDIO_S16SYS;
    want.channels = 1;
    want.samples = 512;
    device = SDL_OpenAudioDevice(NULL, 0, &want, NULL, 0);
    if (!device) {
        fprintf(stderr, "SDL audio: %s\n", SDL_GetError());
        SDL_QuitSubSystem(SDL_INIT_AUDIO);
        return;
    }
    SDL_PauseAudioDevice(device, 0);
    Hangenabled = 1;
}

void hang() {
    if (!Hangenabled || !device)
        return;
    while (SDL_GetQueuedAudioSize(device) < queued_bytes_target) {
        CustomMixer(block, sizeof(block), NULL, 0, 1);
        SDL_QueueAudio(device, block, sizeof(block));
    }
}

void nullazhangot() {
    if (Hangenabled && device)
        SDL_ClearQueuedAudio(device);
}
