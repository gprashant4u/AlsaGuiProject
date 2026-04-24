#include "AlsaManager.h"
#include <math.h>
AlsaManager::AlsaManager() : handle(nullptr) {}
AlsaManager::~AlsaManager() { if (handle) snd_pcm_close(handle); }
bool AlsaManager::initAudio(const std::string& device) {
    if (snd_pcm_open(&handle, device.c_str(), SND_PCM_STREAM_PLAYBACK, 0) < 0) return false;
    snd_pcm_set_params(handle, SND_PCM_FORMAT_S16_LE, SND_PCM_ACCESS_RW_INTERLEAVED, 1, 44100, 1, 500000);
    return true;
}
void AlsaManager::playTone(int frequency, int duration_ms) {
    int count = (44100 * duration_ms) / 1000;
    short buffer[count];
    for (int i = 0; i < count; i++) {
        buffer[i] = 3000 * sin(2.0 * M_PI * frequency * i / 44100.0);
    }
    snd_pcm_writei(handle, buffer, count);
}
