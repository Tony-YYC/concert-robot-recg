#include "beat.hpp"

Beat::Beat(int frame_size, int sample_rate,const Timer & timer): gist(frame_size, sample_rate),smp(sample_rate,frame_size) 
{
    beat_thread_handle_ = std::thread(&Beat::beat_recg_thread,this);
    timer_ = timer;
    std::cout << "\033[32m" << timer_ << "[Beat_recg][INFO]beat_recg thread created!\033[0m" << std::endl;
}

void Beat::get_beat() {
    gist.processAudioFrame(audio_smp);
    beat_recg_result = gist.energyDifference();
}

float Beat::get_beat_result()
{
    return beat_recg_result;
}

void Beat::beat_recg_thread()
{
    while(true)
    {
        smp.read_audio(audio_smp);
        get_beat();
        std::cout << "\033[32m" << timer_ << "[Beat_recg][INFO]calc beat successfully! beat_result:" << beat_recg_result << "\033[0m" << std::endl;
        sleep(1);
    }
}

Beat::~Beat()
{
    if(beat_thread_handle_.joinable())
    {
        beat_thread_handle_.join();
    }
}