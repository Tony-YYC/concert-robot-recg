#include "beat_recg/beat.hpp"
#include "emo_recg/TRTModule.hpp"
#include <mutex>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <thread>

class Recg {
public:
    Recg(): beat_recg(44100, 44100, t), trtmod("../assets/model.trt", t) {
        std::cout << t << "[Main][INFO]Main module start publishing result!" << std::endl;
    }

    ~Recg() = default;

    std::pair<float, int> get_result() {
        // 返回计算的结果
        beat_result = beat_recg.get_beat_result();
        emo_result = trtmod.get_emo_result();
        std::cout << t << "[Main][INFO]Get result from module beat_result: " << beat_result
                  << "\temo_result: " << emo_result << std::endl;
        return { beat_result, emo_result };
    }

private:
    Beat beat_recg;
    TensorRTModel trtmod;
    Timer t;
    float beat_result;
    int emo_result;
};

namespace py = pybind11;

PYBIND11_MODULE(robot, m) {
    py::class_<Recg>(m, "Recg").def(py::init<>()).def("get_result", &Recg::get_result);
}

/*
int main() {
    Timer t;
    Sampler smp(44100,44100);
    std::vector<float> audio_smp;
    smp.read_audio(audio_smp);
    for(const auto & frame : audio_smp)
    {
        std::cout << frame << " ";
    }
    std::cout << audio_smp.size() << std::endl;
    Beat beat_recg(44100,44100);
    float result = beat_recg.get_beat(audio_smp);
    Preprocess prec(44100,44100,236,40);
    std::vector<std::vector<float>> mfccs;
    prec.get_mfccs(audio_smp,mfccs);
    std::cout << mfccs.size() << std::endl;
    for(const auto & item: mfccs)
    {
        std::cout << item.size() << std::endl;
    }
    TensorRTModel trtmod("../assets/model.trt");
    int emo_result = trtmod.calc_emo(mfccs);
    std::cout << emo_result << std::endl;
    std::cout << t << "[Main][INFO]Program starting!" << std::endl;
    Beat beat_recg(44100, 44100, t);
    TensorRTModel trtmod("../assets/model.trt", t);
    sleep(3);
    float beat_result;
    int emo_result;
    std::cout << t << "[Main][INFO]Main module start publishing result!" << std::endl;
    while (true) {
        beat_result = beat_recg.get_beat_result();
        emo_result = trtmod.get_emo_result();
        std::cout << t << "[Main][INFO]Get result from module beat_result: " << beat_result
                  << "\temo_result: " << emo_result << std::endl;
        sleep(1);
    }
    return 0;
}
*/