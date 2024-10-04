cmake_minimum_required(VERSION 3.30)

project(wordle 
    VERSION 0.1
    DESCRIPTION "Search possible wordle results."
    LANGUAGES CXX)

find_package(basic_prj REQUIRED)

add_executable(wordle wordle.cpp)
set_project_warnings(wordle)

target_link_libraries(wordle basic_prj::utils)
