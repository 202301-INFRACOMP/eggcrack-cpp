cmake_minimum_required(VERSION 3.22)

project(eggcrack LANGUAGES CXX)

add_subdirectory("include")

add_executable(main main.cpp)
target_compile_features(main PUBLIC cxx_std_20)
target_link_libraries(main PRIVATE eggcrack)
