#include "AlsaManager.h"
#include <math.h>
#include <iostream>

AlsaManager::AlsaManager() : handle(nullptr), mixer_handle(nullptr), mixer_elem(nullptr) {}

AlsaManager::~AlsaManager() { 
    if (handle) {
        snd_pcm_drain(handle);
        snd_pcm_close(handle); 
    }
    if (mixer_handle) {
        snd_mixer_close(mixer_handle);
    }
}

bool AlsaManager::initAudio(const std::string& device) {
    int err;
    // PCM Initialization (using "pulse" for WSL)
    if ((err = snd_pcm_open(&handle, "pulse", SND_PCM_STREAM_PLAYBACK, 0)) < 0) {
        if ((err = snd_pcm_open(&handle, "default", SND_PCM_STREAM_PLAYBACK, 0)) < 0) {
            return false;
        }
    }
    snd_pcm_set_params(handle, SND_PCM_FORMAT_S16_LE, SND_PCM_ACCESS_RW_INTERLEAVED, 1, 44100, 1, 500000);

    // Mixer Initialization
    snd_mixer_open(&mixer_handle, 0);
    snd_mixer_attach(mixer_handle, "default");
    snd_mixer_selem_register(mixer_handle, NULL, NULL);
    snd_mixer_load(mixer_handle);

    snd_mixer_selem_id_t *sid;
    snd_mixer_selem_id_alloca(&sid);
    snd_mixer_selem_id_set_name(sid, "Master");
    mixer_elem = snd_mixer_find_selem(mixer_handle, sid);

    if (!mixer_elem) {
        snd_mixer_selem_id_set_name(sid, "PCM");
        mixer_elem = snd_mixer_find_selem(mixer_handle, sid);
    }
    return true;
}

void AlsaManager::playTone(int frequency, int duration_ms) {
    if (!handle) return;
    
    snd_pcm_state_t state = snd_pcm_state(handle);
    if (state != SND_PCM_STATE_RUNNING && state != SND_PCM_STATE_PREPARED) {
        snd_pcm_prepare(handle);
    }

    int count = (44100 * duration_ms) / 1000;
    std::vector<short> buffer(count);
    for (int i = 0; i < count; i++) {
        buffer[i] = 15000 * sin(2.0 * M_PI * frequency * i / 44100.0);
    }

    if (snd_pcm_writei(handle, buffer.data(), count) < 0) {
        snd_pcm_prepare(handle);
    }
}

void AlsaManager::setVolume(long volume) {
    if (!mixer_elem) {
        std::cout << "Mixer element not found, UI Volume: " << volume << std::endl;
        return;
    }

    long min, max;
    snd_mixer_selem_get_playback_volume_range(mixer_elem, &min, &max);
    long value = (volume * (max - min) / 100) + min;
    snd_mixer_selem_set_playback_volume_all(mixer_elem, value);
    std::cout << "Hardware Volume Sync: " << volume << "%" << std::endl;
}
