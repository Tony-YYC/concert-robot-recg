#include "TRTModule.hpp"
#include <algorithm>

TensorRTModel::TensorRTModel(const std::string& modelPath,const Timer & timer): modelPath_(modelPath),smp(44100,44100),prec(44100,44100,236,40) {
    timer_ = timer;
    std::cout << "\033[34m" << timer_ << "[TRTModule][INFO]Start initiating TRTModule\033[0m" << std::endl;
    if(cudaMalloc((void**)&input_buffer, input_size) != 0u)
    {
        std::cerr << "ERROR: CUDA input buffer memory alloc failed!" << std::endl;
        cudaFree(input_buffer);
    }
    if(cudaMalloc((void**)&output_buffer, output_size) != 0u)
    {
        std::cerr << "ERROR: CUDA output buffer memory alloc failed!" << std::endl;
        cudaFree(output_buffer);
    }
    std::cout << "\033[34m" << timer_ << "[TRTModule][INFO]Successfully allocate cuda memory space! Start loading module!\033[0m" << std::endl;
    load_model();
    std::cout << "\033[34m" << timer_ << "[TRTModule][INFO]Successfully load model and create engine!\033[0m" << std::endl;
    emo_thread_handle_ = std::thread(&TensorRTModel::emo_recg_thread,this);
    std::cout << "\033[34m" << timer_ <<  "[TRTModule][INFO]TRTModule thread created!\033[0m" << std::endl;
}

bool TensorRTModel::load_model() {
    // 读取序列化的引擎
    std::ifstream engine_file(modelPath_, std::ios::binary);
    engine_file.seekg(0, engine_file.end);
    long int fsize = engine_file.tellg();
    engine_file.seekg(0, engine_file.beg);

    std::vector<char> engine_data(fsize);
    engine_file.read(engine_data.data(), fsize);
    if (!engine_file)
    {
        std::cerr << "Error loading engine file!" << std::endl;
        return false;
    }

    // 创建运行时
    runtime_ = nvinfer1::createInferRuntime(g_logger);
    std::cout << "\033[34m" << timer_ <<"[TRTModule][INFO]successfully create runtime!\033[0m" << std::endl;
    engine_ = runtime_->deserializeCudaEngine(engine_data.data(), fsize);
    std::cout << "\033[34m" << timer_ << "[TRTModule][INFO]successfully create engine!\033[0m" << std::endl;

    // 创建执行上下文
    context_ = engine_->createExecutionContext();
    std::cout << "\033[34m" << timer_ <<"[TRTModule][INFO]successfully create context!\033[0m" << std::endl;
    return true;
}

bool TensorRTModel::infer() {
    if (context_ == nullptr) {
        std::cerr << "ERROR: Execution context not initialized." << std::endl;
        return false;
    }

    // 设置输入和输出缓冲区
    std::vector<void*> buffers = { input_buffer, output_buffer };

    // 执行推理
    if (!context_->executeV2(buffers.data())) {
        std::cerr << "ERROR: Execution failed." << std::endl;
        return false;
    }

    return true;
}

void TensorRTModel::calc_emo() {
    std::vector<float> input_flattened(236 * 40);
    for (int i = 0; i < 236; ++i) {
        for (int j = 0; j < 40; ++j) {
            input_flattened[i * 40 + j] = mfcc_serial[i][j];
        }
    }
    cudaMemcpy(input_buffer, input_flattened.data(), input_size, cudaMemcpyHostToDevice);
    infer();
    std::vector<float> output_prob(7);
    cudaMemcpy(output_prob.data(), output_buffer, output_size, cudaMemcpyDeviceToHost);
    emo_result = std::distance(output_prob.begin(), std::max_element(output_prob.begin(), output_prob.end()));
    std::cout << "\033[34m" << timer_ <<"[TRTModule][INFO]calc emo successfully! emo_result:" << emo_result <<"\033[0m" << std::endl;
}

void TensorRTModel::emo_recg_thread()
{
    while(true)
    {
        smp.read_audio(audio_smp);
        prec.get_mfccs(audio_smp,mfcc_serial);
        calc_emo();
        sleep(1);
    }
}

int TensorRTModel::get_emo_result()
{
    return emo_result;
}

TensorRTModel::~TensorRTModel() {
    // 释放资源
    if(emo_thread_handle_.joinable())
    {
        emo_thread_handle_.join();
    }
    cudaFree(input_buffer);
    cudaFree(output_buffer);
    if (context_ != nullptr) {
        context_->destroy();
    }
    if (engine_ != nullptr) {
        engine_->destroy();
    }
    if (runtime_ != nullptr) {
        runtime_->destroy();
    }
}