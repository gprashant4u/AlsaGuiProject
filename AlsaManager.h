#include <alsa/asoundlib.h>
#include <vector>
#include <string>

class AlsaManager {
public:
    AlsaManager();
    ~AlsaManager();

    // Device Management
    std::vector<std::string> getDevices();
    bool initAudio(const std::string& device = "default");

    // Volume Control (Mixer API)
    void setVolume(long volume); // 0 to 100
    long getVolume();

    // Playback
    void playFrequency(int hz);
    void stop();

private:
    snd_pcm_t *pcm_handle;
    snd_mixer_t *mixer_handle;
    snd_mixer_elem_t *master_element;
};