cmake_minimum_required(VERSION 3.10)
project(StudentuRezultatai LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

if(MSVC)
    add_compile_options(/O2 /EHsc)
else()
    add_compile_options(-O2)
endif()

add_executable(programa
    ConsoleApplication1/main.cpp
    ConsoleApplication1/io.cpp
    ConsoleApplication1/skaiciavimas.cpp
    ConsoleApplication1/studentas.cpp
    ConsoleApplication1/studentas_utils.cpp
    ConsoleApplication1/testavimas.cpp
)

target_include_directories(programa PRIVATE ConsoleApplication1)
