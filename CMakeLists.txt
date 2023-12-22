cmake_minimum_required(VERSION 3.15)
project(concert.robot.recg)
set(CMAKE_CXX_STANDARD 17)

include_directories(.)
add_subdirectory(audio)
add_subdirectory(beat_recg)
add_subdirectory(core_io)
add_subdirectory(emo_recg)
find_package(pybind11 REQUIRED)
## Export compile commands for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

pybind11_add_module(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE beat_recg)
target_link_libraries(${PROJECT_NAME} PRIVATE emo_recg)