project("Units Tests" CXX)

cmake_minimum_required(VERSION 3.8)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Catch2 REQUIRED)

# enable_testing()

if(MSVC)
  add_definitions(/W4 /w14545 /w34242 /w34254 /w34287 /w44263 /w44265 /w44296 /w44311 /w44826 /we4289 /w14546 /w14547 /w14549 /w14555 /w14619 /w14905 /w14906 /w14928)
else()
  add_definitions(-Wall -Wextra -Wconversion -Wshadow -Wnon-virtual-dtor -Wold-style-cast -Wcast-align -Wcast-qual -Wunused -Woverloaded-virtual -Wno-noexcept-type -Wpedantic)
endif()

add_executable(UnitsTests ${CMAKE_CURRENT_LIST_DIR}/TestMain.cpp ${CMAKE_CURRENT_LIST_DIR}/UnitsTests.cpp)
target_link_libraries(UnitsTests Catch2::Catch2)

#include(Catch)