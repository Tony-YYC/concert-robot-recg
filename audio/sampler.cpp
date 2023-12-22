#include "sampler.hpp"

Sampler::Sampler(int sampleRate, int bufferSize): sampleRate(sampleRate), bufferSize(bufferSize) {
    int err;
    if ((err = snd_pcm_open(&capture_handle, "default", SND_PCM_STREAM_CAPTURE, 0)) < 0) {
        std::cerr << "cannot open audio device: " << snd_strerror(err) << std::endl;
        exit(1);
    }

    if ((err = snd_pcm_set_params(capture_handle,
                                  SND_PCM_FORMAT_FLOAT_LE,
                                  SND_PCM_ACCESS_RW_INTERLEAVED,
                                  1,
                                  sampleRate,
                                  1,
                                  300))
        < 0)
    { // bufferSize in microseconds
        std::cerr << "cannot set parameters: " << snd_strerror(err) << std::endl;
        exit(1);
    }
    if ((err = snd_pcm_prepare(capture_handle)) < 0) {
        std::cerr << "cannot prepare audio interface for use: " << snd_strerror(err) << std::endl;
        exit(1);
    }
}

Sampler::~Sampler() {
    snd_pcm_close(capture_handle);
}

void Sampler::read_audio(std::vector<float>& output_array) {
    if (output_array.size() != bufferSize) {
        output_array.resize(bufferSize);
    }
    int err;
    if ((err = snd_pcm_readi(capture_handle, output_array.data(), bufferSize)) != bufferSize) {
        std::cerr << "read error: " << snd_strerror(err) << std::endl;
        exit(1);
    }
}