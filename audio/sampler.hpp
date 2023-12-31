#ifndef AUDIO_SAMPLER_H
#define AUDIO_SAMPLER_H
#include <alsa/asoundlib.h>
#include <iostream>
#include <vector>
#include <thread>

class Sampler {
public:
    Sampler(int sampleRate, int bufferSize,std::vector<float>& output_array);
    ~Sampler();

private:
    int sampleRate;
    int bufferSize;
    snd_pcm_t* capture_handle;
    std::vector<float>& audio_smp;
    std::thread audio_thread_handle_;
    bool done;

    void read_audio_thread();
};
#endif