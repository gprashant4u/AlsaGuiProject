#ifndef ALSAMANAGER_H
#define ALSAMANAGER_H
#include <alsa/asoundlib.h>
#include <string>

class AlsaManager {
public:
    AlsaManager();
    ~AlsaManager();
    bool initAudio(const std::string& device = "default");
    void playTone(int frequency, int duration_ms);
private:
    snd_pcm_t *handle;
};
#endif
