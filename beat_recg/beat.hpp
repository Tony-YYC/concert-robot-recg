#ifndef BEAT_RECG_H
#define BEAT_RECG_H
#define USE_KISS_FFT
#include "../audio/sampler.hpp"
#include "../third_party/gist/Gist.h"
#include "../core_io/timer.hpp"
#include <thread>

class Beat {
public:
    Beat(int frame_size, int sample_rate,const Timer & timer,const std::vector<float> & audio_smp);
    float get_beat_result();
    ~Beat();

private:
    void get_beat();
    void beat_recg_thread();
    Gist<float> gist;
    float beat_recg_result;
    const std::vector<float> & audio_smp_;
    std::thread beat_thread_handle_;
    Timer timer_;
    bool done;
};
#endif