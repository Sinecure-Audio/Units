project("Units Tests" CXX)

cmake_minimum_required(VERSION 3.8)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Catch2 REQUIRED)
enable_testing()

add_library(UnitsTests
        ${CMAKE_CURRENT_LIST_DIR}/TestMain.cpp
        ${CMAKE_CURRENT_LIST_DIR}/UnitsTests.cpp
#        ${CMAKE_CURRENT_LIST_DIR}/ResonanceUnitsTests.cpp
#        ${CMAKE_CURRENT_LIST_DIR}/AmplitudeUnitsTests.cpp
        ${CMAKE_CURRENT_LIST_DIR}/FrequencyUnitsTests.cpp)

target_link_libraries(UnitsTests PRIVATE Catch2::Catch2)

if(MSVC)
  target_compile_options(UnitsTests PRIVATE /W4 /w14545 /w34242 /w34254 /w34287 /w44263 /w44265 /w44296
          /w44311
          /w44826 /we4289 /w14546
          /w14547 /w14549 /w14555 /w14619 /w14905 /w14906 /w14928)
else()
  target_compile_options(UnitsTests PRIVATE -Wall -Wextra -Wconversion -Wshadow
          -Wnon-virtual-dtor -Wold-style-cast -Wcast-align -Wcast-qual -Wunused -Woverloaded-virtual -Wno-noexcept-type -Wpedantic)
endif()
#
#include(CTest)
#include(Catch)