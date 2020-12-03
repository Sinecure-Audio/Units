#include "TestIncludes.h"

TEST_CASE("Resonance Creation", "[Examples]") {
    ResonanceCoefficient<TestType> r{};
    REQUIRE(r.count() == Catch::Detail::Approx(TestType{0}));

    ResonanceCoefficient<TestType> r1{1};
    REQUIRE(r1.count() == Catch::Detail::Approx(TestType{1}));
}

TEST_CASE("Resonance CTAD", "[Examples]") {
    ResonanceCoefficient<TestType> r{};
    auto r1{ResonanceCoefficient{TestType{1}}};
    REQUIRE(std::is_same<decltype(r), decltype(r1)>::value);
}

TEST_CASE("Q Creation", "[Examples]") {
    ResonanceCoefficient<TestType> q{};
    REQUIRE(q.count() == Catch::Detail::Approx(TestType{0}));

    ResonanceCoefficient<TestType> q1{1};
    REQUIRE(q1.count() == Catch::Detail::Approx(TestType{1}));
}

TEST_CASE("Q CTAD", "[Examples]") {
    QCoefficient<TestType> q{};
    auto q1{QCoefficient{TestType{1}}};
    REQUIRE(std::is_same<decltype(q), decltype(q1)>::value);
}

TEST_CASE("Resonance Q Conversion", "[Examples]") {
    const ResonanceCoefficient<float> resonance{0.0f};
    const QCoefficient<float> q = resonance;
    const ResonanceCoefficient<float> largeResonance{.99999f};

    REQUIRE(q.count() == .5f);
    REQUIRE_THAT(ResonanceCoefficient<float>{q}.count(), Catch::WithinRel(0.0f));
    REQUIRE(QCoefficient<float>{largeResonance}.count() > 420.0f);
}