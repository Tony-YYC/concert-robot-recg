#ifndef EMO_RECG_H
#define EMO_RECG_H
#include <NvInfer.h>
#include <NvOnnxParser.h>
#include <cuda_runtime_api.h>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <thread>
#include "../audio/sampler.hpp"
#include "../audio/preprocess.hpp"
#include "../core_io/timer.hpp"

using namespace nvinfer1;

class Logger: public ILogger {
    void log(Severity severity, const char* msg) noexcept override {
        // 过滤掉不需要的日志等级
        if (severity <= Severity::kWARNING) {
            std::cout << msg << std::endl;
        }
    }
};

class TensorRTModel {
public:
    explicit TensorRTModel(const std::string& modelPath,const Timer & timer);
    ~TensorRTModel();
    int get_emo_result();

private:
    bool load_model();
    bool infer();
    void emo_recg_thread();
    void calc_emo();

    std::vector<float> audio_smp;
    std::vector<std::vector<float>> mfcc_serial;
    int emo_result;
    Sampler smp;
    Preprocess prec;

    std::string modelPath_;
    IRuntime* runtime_ = nullptr;
    ICudaEngine* engine_ = nullptr;
    IExecutionContext* context_ = nullptr;
    float* input_buffer;
    float* output_buffer;
    const int input_size = sizeof(float) * 236 * 40;
    const int output_size = sizeof(float) * 7;
    Logger g_logger;

    std::thread emo_thread_handle_;
    Timer timer_;
};
#endif