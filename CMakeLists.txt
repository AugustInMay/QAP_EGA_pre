cmake_minimum_required(VERSION 3.14)
project(QAP_ega_pre)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")

add_executable(QAP_ega_pre main.cpp progeny.cpp progeny.h procreator_choice.cpp procreator_choice.h crossover.cpp crossover.h mutation.cpp mutation.h selection.cpp selection.h)

find_package(Python2 COMPONENTS Development NumPy)
target_include_directories(QAP_ega_pre PRIVATE ${Python2_INCLUDE_DIRS} ${Python2_NumPy_INCLUDE_DIRS})
target_link_libraries(QAP_ega_pre Python2::Python Python2::NumPy)