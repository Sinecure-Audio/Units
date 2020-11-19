#include <catch2/catch.hpp>

#include "../source/FrequencyUnits.h"

TEST_CASE("Analog Frequency Construction", "[Analog Frequency]") {
    SECTION("CTAD") {
        const auto freq1 = AnalogFrequency<double>{ 2.0 };
        const auto freq2 = AnalogFrequency{ 2.0 };
        REQUIRE (std::is_same<decltype (freq1), decltype (freq2)>::value);
    }
}

TEST_CASE("Digital Frequency Construction", "[Analog Frequency]") {
    SECTION("CTAD") {
        const auto freq1 = DigitalFrequency<double>{ 2.0 };
        const auto freq2 = DigitalFrequency{ 2.0 };
        REQUIRE (std::is_same<decltype (freq1), decltype (freq2)>::value);
    }
}

TEST_CASE("Frequency Math", "[Analog Frequency]") {

}

TEST_CASE("Frequency Conversions", "[Analog Frequency]") {

}