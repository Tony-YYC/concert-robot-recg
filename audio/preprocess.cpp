#include "preprocess.hpp"
#include <vector>

Preprocess::Preprocess(int total_frame_size, int sample_rate, int frame_num, int coeffs):
    gist(total_frame_size / frame_num, sample_rate) {
    frame_size_ = total_frame_size / frame_num;
    frame_num_ = frame_num;
    gist.setMFCCNumCoefficients(coeffs);
}

void Preprocess::get_mfccs(const std::vector<float>& input_sample,
                           std::vector<std::vector<float>>& mfcc_result) {
    for (int i = 0; i < frame_num_; ++i) {
        std::vector<float> temp_input;
        for (int j = 0; j < frame_size_; ++j) {
            temp_input.push_back(input_sample[i * frame_size_ + j]);
        }
        gist.processAudioFrame(temp_input);
        std::vector<float> temp_result = gist.getMelFrequencyCepstralCoefficients();
        mfcc_result.push_back(temp_result);
    }
}