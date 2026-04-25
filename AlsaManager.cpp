#include "AlsaManager.h"
#include <math.h>
#include <iostream>

AlsaManager::AlsaManager() : handle(nullptr) {}

AlsaManager::~AlsaManager() { 
    if (handle) {
        snd_pcm_drain(handle);
        snd_pcm_close(handle); 
    }
}

bool AlsaManager::initAudio(const std::string& device) {
    int err;
    if ((err = snd_pcm_open(&handle, "pulse", SND_PCM_STREAM_PLAYBACK, 0)) < 0) {
        if ((err = snd_pcm_open(&handle, "default", SND_PCM_STREAM_PLAYBACK, 0)) < 0) {
            return false;
        }
    }
    snd_pcm_set_params(handle, SND_PCM_FORMAT_S16_LE, SND_PCM_ACCESS_RW_INTERLEAVED, 1, 44100, 1, 500000);
    return true;
}

void AlsaManager::playTone(int frequency, int duration_ms) {
    if (!handle) return;

    // Check the state. If it's not running, we must prepare it.
    snd_pcm_state_t state = snd_pcm_state(handle);
    if (state != SND_PCM_STATE_RUNNING && state != SND_PCM_STATE_PREPARED) {
        snd_pcm_prepare(handle);
    }

    int count = (44100 * duration_ms) / 1000;
    std::vector<short> buffer(count);
    for (int i = 0; i < count; i++) {
        buffer[i] = 15000 * sin(2.0 * M_PI * frequency * i / 44100.0);
    }

    int err = snd_pcm_writei(handle, buffer.data(), count);
    if (err < 0) {
        snd_pcm_prepare(handle); // Recover if the buffer underruns
    }
}

void AlsaManager::setVolume(long volume) {
    std::cout << "Volume: " << volume << std::endl;
}
