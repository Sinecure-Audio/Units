#include "TestIncludes.h"

TEST_CASE("Amplitude Creation", "[Examples]") {
    Amplitude<TestType> a{};
    REQUIRE(a.count() == Catch::Detail::Approx(TestType{0}));

    Amplitude<TestType> a1{1};
    REQUIRE(a1.count() == Catch::Detail::Approx(TestType{1}));
}

TEST_CASE("Amplitude CTAD", "[Examples]") {
    Amplitude<TestType> a{};
    auto a1{Amplitude{TestType{1}}};
    REQUIRE(std::is_same<decltype(a), decltype(a1)>::value);
}

TEST_CASE("Decibel Creation", "[Examples]") {
    Decibel<TestType> d{0};
    REQUIRE(d.count() == Catch::Detail::Approx(TestType{0}));

    Decibel<TestType> d1{1};
    REQUIRE(d1.count() == Catch::Detail::Approx(TestType{1}));
}

TEST_CASE("Decibel CTAD", "[Examples]") {
    Decibel<TestType> d{};
    auto d1{Decibel{TestType{1}}};
    REQUIRE(std::is_same<decltype(d), decltype(d1)>::value);
}

TEST_CASE("Decibel Literal", "[Decibels]") {
    const Decibel<float> d = 0.0_dB;
    REQUIRE_THAT(d.count(), Catch::WithinRel(0.0f));
}

TEST_CASE("Basic Decibel Addition", "[Decibels]") {
    const auto d = Decibel{Amplitude{std::sqrt(2.0)/2.0}};
    const auto d2 = d+d;
//    REQUIRE(d2.count() == Catch::Detail::Approx(0.0));
    REQUIRE_THAT(d2.count(), Catch::WithinAbs(0.0, .00000000001));
}

TEST_CASE("Random Decibel Addition", "[Decibels]") {
    const auto rand = GENERATE(take(100, random(-120.0, 120.0)));
    const auto d = Decibel<double>{rand};
    const auto d2 = d+d;
    const auto dRef = d.count()
                  +Decibel{Amplitude{std::sqrt(2.0)}}.count();
    REQUIRE(d2.count() == Catch::Detail::Approx(dRef));
}

TEST_CASE("Basic Decibel Subtraction", "[Decibels]") {
    const auto d = Decibel{Amplitude{std::sqrt(2.0)}};
    const auto d2 = d-d;
    REQUIRE(d2.count() == d.getMinusInfinityDB().count());
}

TEST_CASE("Decibel Subtraction", "[Decibels]") {
    const Decibel<float> d = 0.0_dB;
    using UnderlyingType = decltype(d.count());
    if constexpr(std::numeric_limits<UnderlyingType>::has_infinity)//subtracting two identical dbfs values should result in -inf or something really close
        REQUIRE((d-d) == d.getMinusInfinityDB());
    else
        REQUIRE((d-d).count() == std::numeric_limits<UnderlyingType>::min());
}

TEST_CASE("Basic Decibel Multiplication", "[Decibels]") {
    const auto d = Decibel{Amplitude{std::sqrt(.5)}};
    REQUIRE_THAT((d*2.0).count(), Catch::WithinAbs(0.0, .00000000001));
}

TEST_CASE("Random Decibel Multiplication", "[Decibels]") {
    const auto rand = GENERATE(take(100, random(-120.0, 120.0)));
    const auto d = Decibel{rand};
    REQUIRE((d*2.0).count()
          == Catch::Detail::Approx((d+d).count()));
}

TEST_CASE("Basic Decibel Division", "[Decibels]") {
    const auto d = Decibel{0.0};
    REQUIRE((d/2.0).count()
          == Catch::Detail::Approx(Decibel{Amplitude{std::sqrt(.5)}}.count()));
}

TEST_CASE("Random Decibel Division", "[Decibels]") {
    const auto rand = GENERATE(take(100, random(-120.0, 120.0)));
    const auto d = Decibel{rand};
    REQUIRE((d/2.0).count()
          == Catch::Detail::Approx(d.count()-std::abs(Decibel{Amplitude{std::sqrt(.5)}}.count())));
}

TEST_CASE("minus Inf dB", "[Decibels]") {
    REQUIRE_THAT(Decibel<float>{0}.getDefaultMinusInifinityDB().count(), Catch::WithinRel(-192.0f));
    REQUIRE_THAT(Decibel<float>{0}.getMinusInfinityDB().count(), Catch::WithinRel(-192.0f));

    REQUIRE_THAT(Decibel<double>{0}.getDefaultMinusInifinityDB().count(), Catch::WithinRel(-384.0));
    REQUIRE_THAT(Decibel<double>{0}.getMinusInfinityDB().count(), Catch::WithinRel(-384.0));

    const auto inf = Decibel<float>{-193.0f};
    const auto a = Amplitude<double>::convertDecibelToAmplitude(Decibel<float>{-193.0f});

    REQUIRE_THAT(a, Catch::WithinAbs(0.0, .000001));
}

TEST_CASE("Decibel String Conversion", "[Decibels]") {
    REQUIRE(std::string(Decibel<int>{-20}) == "-20dB");
    REQUIRE(std::string(Decibel<int>{-0}) == "0dB");
    REQUIRE(std::string(Decibel<int>{0}) == "0dB");
    REQUIRE(std::string(Decibel<int>{20}) == "20dB");

    REQUIRE(std::string(-20.0_dB) == "-20.000000dB");
    REQUIRE(std::string(-0.0_dB) == "-0.000000dB");
    REQUIRE(std::string(0.0_dB) == "0.000000dB");
    REQUIRE(std::string(20.0_dB) == "20.000000dB");

    REQUIRE(std::string{Decibel<long double>{std::numeric_limits<long double>::lowest()}} == "-inf dB");
    REQUIRE(std::string(Decibel<float>{-192}) == "-inf dB");
    REQUIRE(std::string(Decibel<double>{-384}) == "-inf dB");
}


TEST_CASE("Amplitude Examples", "[Examples]") {
    const Amplitude<float> amplitude = .5f;
    const Decibel<float> decibel = amplitude;

    const Decibel<double> halfPowerDecibel = Amplitude<double>{std::sqrt(.5)};
    const Amplitude<double> halfPowerAmplitude = halfPowerDecibel;

    const Amplitude<float> floatHalfPowerAmplitude = halfPowerAmplitude;
    const Decibel<float> floatHalfPowerDecibel = halfPowerDecibel;

    REQUIRE_THAT(decibel.count(), Catch::WithinRel(Decibel<float>{-6.0206_dB}.count()));

    REQUIRE_THAT(static_cast<float>(halfPowerAmplitude.count()), Catch::WithinRel(floatHalfPowerAmplitude.count()));

    REQUIRE_THAT((floatHalfPowerDecibel+floatHalfPowerDecibel).count(), Catch::WithinAbs(0.0, .000005));
}