#include "TestIncludes.h"

TEST_CASE("Analog Frequency Construction", "[Analog Frequency]") {
    SECTION("Basic Construct") {
        const auto freq1 = AnalogFrequency<double>{};
        REQUIRE(freq1.count() == Catch::Detail::Approx(0.0));
        const auto freq2 = AnalogFrequency<double>{ 440.0 };
        REQUIRE(freq2.count() == Catch::Detail::Approx(440.0));
    }
    SECTION("CTAD") {
        const auto freq1 = AnalogFrequency<double>{ 2.0 };
        const auto freq2 = AnalogFrequency{ 2.0 };
        REQUIRE (std::is_same<decltype (freq1), decltype (freq2)>::value);
    }
}

TEST_CASE("Digital Frequency Construction", "[Analog Frequency]") {
    SECTION("Basic Construct") {
        const auto freq1 = DigitalFrequency<double>{};
        REQUIRE(freq1.count() == Catch::Detail::Approx(0.0));
        const auto freq2 = DigitalFrequency<double>{ 440.0 };
        REQUIRE(freq2.count() == Catch::Detail::Approx(440.0));
    }
    SECTION("CTAD") {
        const auto freq1 = DigitalFrequency<double>{ 2.0 };
        const auto freq2 = DigitalFrequency{ 2.0 };
        REQUIRE (std::is_same<decltype (freq1), decltype (freq2)>::value);
    }
}

TEST_CASE("Frequency Conversions", "[Analog Frequency]") {
    //As analog sampling rates get closer to infinity, they should be represented
    // by smaller and smaller digital freqs? Or the opposite
    const auto rand = GENERATE(take(100, random(0.0, 20000.0)));
    const auto df1 = DigitalFrequency{rand};
    const auto af1 = AnalogFrequency{rand};
    REQUIRE(rand > DigitalFrequency{af1, 44100.0}.count());
    REQUIRE(rand < AnalogFrequency {df1, 44100.0}.count());
}