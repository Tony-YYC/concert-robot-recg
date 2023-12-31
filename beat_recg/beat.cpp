#include "beat.hpp"

Beat::Beat(int frame_size, int sample_rate,const Timer & timer,const std::vector<float> & audio_smp): gist(frame_size, sample_rate),audio_smp_(audio_smp)
{
    done = false;
    beat_thread_handle_ = std::thread(&Beat::beat_recg_thread,this);
    timer_ = timer;
    std::cout << "\033[32m" << timer_ << "[Beat_recg][INFO]beat_recg thread created!\033[0m" << std::endl;
}

void Beat::get_beat() {
    gist.processAudioFrame(audio_smp_);
    beat_recg_result = gist.spectralDifference();
}

float Beat::get_beat_result()
{
    return beat_recg_result;
}

void Beat::beat_recg_thread()
{
    sleep(2);
    while(true)
    {
        get_beat();
        std::cout << "\033[32m" << timer_ << "[Beat_recg][INFO]calc beat successfully! beat_result:" << beat_recg_result << "\033[0m" << std::endl;
        sleep(1);
        if(done)
        {
            break;
        }
    }
}

Beat::~Beat()
{
    done = true;
    if(beat_thread_handle_.joinable())
    {
        beat_thread_handle_.join();
    }
}