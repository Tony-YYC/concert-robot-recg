#include "sampler.hpp"

Sampler::Sampler(int sampleRate, int bufferSize, std::vector<float>& output_array):
    audio_smp(output_array),
    sampleRate(sampleRate),
    bufferSize(bufferSize) {
    audio_smp = output_array;
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
    done = false;
    audio_thread_handle_ = std::thread(&Sampler::read_audio_thread, this);
    std::cout << "\033[32m"
              << "[Sampler][INFO]read_audio thread created!\033[0m" << std::endl;
}

Sampler::~Sampler() {
    done = true;
    if (audio_thread_handle_.joinable()) {
        audio_thread_handle_.join();
    }
    snd_pcm_close(capture_handle);
}

void Sampler::read_audio_thread() {
    while (true) {
        if (audio_smp.size() != bufferSize) {
            audio_smp.resize(bufferSize);
        }
        int err;
        if ((err = snd_pcm_readi(capture_handle, audio_smp.data(), bufferSize)) != bufferSize) {
            std::cerr << "read error: " << snd_strerror(err) << std::endl;
            exit(1);
        }
        if(done)
        {
            break;
        }
    }
}