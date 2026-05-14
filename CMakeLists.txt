cmake_minimum_required(VERSION 3.14)
project(StudentuRezultatai LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

if(MSVC)
    add_compile_options(/O2 /EHsc)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS _CRT_DECLARE_NONSTDC_NAMES=1)
else()
    add_compile_options(-O2)
endif()

add_executable(programa
    ConsoleApplication1/main.cpp
    ConsoleApplication1/studentu_io.cpp
    ConsoleApplication1/skaiciavimas.cpp
    ConsoleApplication1/zmogus.cpp
    ConsoleApplication1/studentas.cpp
    ConsoleApplication1/studentas_utils.cpp
    ConsoleApplication1/testavimas.cpp
)
target_include_directories(programa PRIVATE ConsoleApplication1)


include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        release-1.12.1
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(unit_testai
    tests/test_studentas.cpp
    tests/test_vector.cpp
    ConsoleApplication1/zmogus.cpp
    ConsoleApplication1/studentas.cpp
)
target_include_directories(unit_testai PRIVATE ConsoleApplication1)
target_link_libraries(unit_testai PRIVATE GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(unit_testai)