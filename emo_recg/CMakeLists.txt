cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
set(CMAKE_CXX_STANDARD 17)
set(lib_name "emo_recg")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# 指定 C++ 标准
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 找到 CUDA 包
find_package(CUDA REQUIRED)

# 包含 TensorRT 头文件
# 替换为你的 TensorRT 安装路径
include_directories(tensorrt)
include_directories(/usr/local/cuda/include)

# 链接 TensorRT 库
# 替换为你的 TensorRT 库路径
link_directories(tensorrt)
link_directories(/usr/local/cuda/lib64)

aux_source_directory(. emo_recg)

# 指定源文件
add_library(${lib_name} ${emo_recg})

# 链接库
# 确保链接 CUDA 和 TensorRT 库
target_link_libraries(
    ${lib_name}
    nvinfer
    cudart
    # 可能还需要链接其他 TensorRT、CUDA 或系统库
)
target_link_libraries(${lib_name} audio)
target_link_libraries(${lib_name} -lpthread)