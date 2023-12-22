#ifndef AUDIO_PREPROCESS_H
#define AUDIO_PREPROCESS_H
#include <vector>
#define USE_KISS_FFT
#include "../third_party/gist/Gist.h"

class Preprocess
{
public:
    Preprocess(int total_frame_size,int sample_rate,int frame_num,int coeffs);
    void get_mfccs(const std::vector<float> &input_sample,std::vector<std::vector<float>> & mfcc_result);
private:
    Gist<float> gist;
    int frame_size_;
    int frame_num_;
}
;
#endif