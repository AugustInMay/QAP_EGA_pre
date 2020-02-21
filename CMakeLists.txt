cmake_minimum_required(VERSION 3.14)
project(QAP_ega_pre)

set(CMAKE_CXX_STANDARD 17)

add_executable(QAP_ega_pre main.cpp progeny.cpp progeny.h procreator_choice.cpp procreator_choice.h crossover.cpp crossover.h mutation.cpp mutation.h selection.cpp selection.h)