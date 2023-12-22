#ifndef AUDIO_SAMPLER_H
#define AUDIO_SAMPLER_H
#include <alsa/asoundlib.h>
#include <iostream>
#include <vector>

class Sampler {
public:
    Sampler(int sampleRate, int bufferSize);
    ~Sampler();
    void read_audio(std::vector<float>& output_array);

private:
    int sampleRate;
    int bufferSize;
    snd_pcm_t* capture_handle;
};
#endif