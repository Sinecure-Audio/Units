project("Units Tests" CXX)

cmake_minimum_required(VERSION 3.8)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
  set(WARNING_FLAGS /W4 /w14545 /w34242 /w34254
          /w34287 /w44263 /w44265 /w44296
          /w44311
          /w44826 /we4289 /w14546
          /w14547 /w14549 /w14555 /w14619 /w14905 /w14906 /w14928)
else()
  set(WARNING_FLAGS -Wall -Wextra -Wconversion -Wshadow
          -Wnon-virtual-dtor -Wold-style-cast -Wcast-align -Wcast-qual -Wunused -Woverloaded-virtual -Wno-noexcept-type -Wpedantic)
endif()

find_package(Catch2 REQUIRED)
enable_testing()

add_executable(UnitsTests
        ${CMAKE_CURRENT_LIST_DIR}/TestMain.cpp
        ${CMAKE_CURRENT_LIST_DIR}/UnitsTests.cpp
        )

add_executable(FrequencyUnitsTests
        ${CMAKE_CURRENT_LIST_DIR}/TestMain.cpp
        ${CMAKE_CURRENT_LIST_DIR}/FrequencyUnitsTests.cpp
        )

add_executable(ResonanceUnitsTests
        ${CMAKE_CURRENT_LIST_DIR}/TestMain.cpp
        ${CMAKE_CURRENT_LIST_DIR}/ResonanceUnitsTests.cpp
        )

add_executable(AmplitudeUnitsTests
        ${CMAKE_CURRENT_LIST_DIR}/TestMain.cpp
        ${CMAKE_CURRENT_LIST_DIR}/AmplitudeUnitsTests.cpp
        )

target_link_libraries(UnitsTests          PRIVATE Catch2::Catch2)
target_link_libraries(FrequencyUnitsTests PRIVATE Catch2::Catch2)
target_link_libraries(ResonanceUnitsTests PRIVATE Catch2::Catch2)
target_link_libraries(AmplitudeUnitsTests PRIVATE Catch2::Catch2)

target_compile_definitions(UnitsTests          PRIVATE WARNING_FLAGS)
target_compile_definitions(FrequencyUnitsTests PRIVATE WARNING_FLAGS)
target_compile_definitions(ResonanceUnitsTests PRIVATE WARNING_FLAGS)
target_compile_definitions(AmplitudeUnitsTests PRIVATE WARNING_FLAGS)


include(Catch)
#Remove ZERO_CHECK target
set(CMAKE_SUPPRESS_REGENERATION true)

#If no NUM_CORES argument given, make one equal to half the cores on this system
if(NOT DEFINED NUM_CORES)
  #Set the number of parallel jobs ctest can use to the number of cores on the machine
  include(ProcessorCount)
  ProcessorCount(NUM_CORES)
  math(EXPR NumCores "${NUM_CORES}/2")
endif()

#Add the number of cores to use, as well as desired verbosity to CTest's arguments
list(APPEND CMAKE_CTEST_ARGUMENTS -j${NUM_CORES} --extra-verbose)

catch_discover_tests(UnitsTests)
catch_discover_tests(FrequencyUnitsTests)
catch_discover_tests(ResonanceUnitsTests)
catch_discover_tests(AmplitudeUnitsTests)