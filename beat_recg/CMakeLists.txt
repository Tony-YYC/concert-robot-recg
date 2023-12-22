cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_STANDARD 17)
set(lib_name "beat_recg")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include_directories (${CMAKE_CURRENT_SOURCE_DIR}/third_party/gist)
include_directories (${CMAKE_CURRENT_SOURCE_DIR}/third_party/kiss_fft130)

aux_source_directory(. beat_src)
aux_source_directory(../third_party/gist gist)
aux_source_directory(../third_party/kiss_fft130 kissfft)
add_library(${lib_name} ${beat_src} ${gist} ${kissfft})
target_link_libraries(${lib_name} audio)
target_link_libraries(${lib_name} -lpthread)