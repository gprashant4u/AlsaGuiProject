#ifndef ALSAMANAGER_H
#define ALSAMANAGER_H

#include <alsa/asoundlib.h>
#include <string>
#include <vector>

class AlsaManager {
public:
    AlsaManager();
    ~AlsaManager();
    bool initAudio(const std::string& device = "default");
    void playTone(int frequency, int duration_ms);
    void setVolume(long volume);

private:
    snd_pcm_t *handle;
    snd_mixer_t *mixer_handle;      // The Master Mixer handle
    snd_mixer_elem_t *mixer_elem;   // The Volume Control element
};

#endif
