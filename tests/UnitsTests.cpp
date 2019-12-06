#define CATCH_CONFIG_MAIN	
#include "catch.hpp"

#include "../include/Units.h"

TEST_CASE("Unit Creation", "[Units]") {
	const Decibel<float> d(0.0f);
	REQUIRE(d.count() == 0.0f);
}

TEST_CASE("Unit Addition", "[Units]") {
	const Amplitude<float> d(0.0f);
	const Amplitude<float> e(1.0f);
	REQUIRE(d+e == Amplitude<float>(1.0f));
	REQUIRE(e+d == Amplitude<float>(1.0f));
}

TEST_CASE("Unit Subtraction", "[Units]") {
	const Amplitude<float> d(0.0f);
	const Amplitude<float> e(1.0f);
	REQUIRE(d-e == Amplitude<float>(-1.0f));
	REQUIRE(e-d == Amplitude<float>(1.0f));
}

TEST_CASE("Unit Multiplication", "[Units]") {
	const Amplitude<float> d{0.0f};
	const float e{1.0f};
	REQUIRE(d*e == Amplitude<float>(0.0f));
	REQUIRE(e*d == Amplitude<float>(0.0f));
}

TEST_CASE("Unit Division", "[Units]") {
	const Amplitude<float> d{4.0f};
	const float e{2.0f};
	REQUIRE(d/e == Amplitude<float>{2.0f});
	REQUIRE(e/d == Amplitude<float>{.5f});
}

TEST_CASE("Unit Modulo", "[Units]") {
	const Decibel<float> d(5.0f);
	const Decibel<float> e(4.0f);
	REQUIRE(d%e == Decibel<float>(1.0f));
	REQUIRE(e%d == Decibel<float>(4.0f));
}

TEST_CASE("Unit Negation", "[Units]") {
	const Decibel<float> d(1.0f);
	REQUIRE(-d == Decibel<float>(-1.0f));
}

TEST_CASE("Unit Left Shift", "[Units]") {
	const Decibel<int> d(16);
	const Decibel<int> e = d << 1;
	REQUIRE(e == Decibel<int>(32));
}

TEST_CASE("Unit Right Shift", "[Units]") {
	const Decibel<int> d(16);
	const Decibel<int> e = d >> 1;
	REQUIRE(e == Decibel<int>(8));
}

TEST_CASE("Unit Negate", "[Units]") {
	int i = 238764;
	Decibel<int> d(i);
	REQUIRE((~d).count() == ~i);
}

TEST_CASE("Unit Xor", "[Units]") {
	int i = 238764;
	int j = 987345;
	Decibel<int> d(i);
	int e(j);
	auto f = d^e;
	auto g = i^j;
	REQUIRE(f.count() == g);
}

TEST_CASE("Unit And", "[Units]") {
	int i = 238764;
	int j = 987345;
	Decibel<int> d(i);
	int e(j);
	auto f = d&e;
	auto g = i&j;
	REQUIRE(f.count() == g);
}

TEST_CASE("Unit Or", "[Units]") {
	int i = 238764;
	int j = 987345;
	Decibel<int> d(i);
	int e(j);
	auto f = d|e;
	auto g = i|j;
	REQUIRE(f.count() == g);
}

TEST_CASE("Unit Xor Assign", "[Units]") {
	int i = 238764;
	int j = 987345;
	Decibel<int> d(i);
	int e(j);
	d ^= e;
	i ^= j;
	REQUIRE(d.count() == i);
}

TEST_CASE("Unit And Assign", "[Units]") {
	int i = 238764;
	int j = 987345;
	Decibel<int> d(i);
	int e(j);
	d &= e;
	i &= j;
	REQUIRE(d.count() == i);
}

TEST_CASE("Unit Or Assign", "[Units]") {
	int i = 238764;
	int j = 987345;
	Decibel<int> d(i);
	int e(j);
	d |= e;
	i |= j;
	REQUIRE(d.count() == i);
}


TEST_CASE("Unit Left Shift1", "[Units]") {
	const Decibel<int> d(16);
	REQUIRE(d<<+1 == Decibel<int>(32));
}

TEST_CASE("Unit Right Shift1", "[Units]") {
	const Decibel<int> d(16);
	REQUIRE(d>>+1 == Decibel<int>(8));
}

TEST_CASE("Unit Post-Increment", "[Units]") {
	Decibel<int> d(16);
	d++;
	REQUIRE(d == Decibel<int>(17));
}

TEST_CASE("Unit Post-Decrement", "[Units]") {
	Decibel<int> d(16);
	d--;
	REQUIRE(d == Decibel<int>(15));
}

TEST_CASE("Unit Pre-Increment", "[Units]") {
	Decibel<int> d(16);
	Decibel<int> e = ++d;
	REQUIRE(d == e);
	REQUIRE(e == Decibel<int>(17));
}

TEST_CASE("Unit Pre-Decrement", "[Units]") {
	Decibel<int> d(16);
	Decibel<int> e = --d;
	REQUIRE(d == e);
	REQUIRE(e == Decibel<int>(15));
}

TEST_CASE("Unit Accum", "[Units]") {
	Amplitude<int> d(16);
	d+=Amplitude<int>(1);
	REQUIRE(d == Amplitude<int>(17));
}

TEST_CASE("Unit DeAccum", "[Units]") {
	Amplitude<int> d(16);
	Amplitude<int> e(1);
	d-=e;
	REQUIRE(d == Amplitude<int>(15));
}

TEST_CASE("Unit MulAccum", "[Units]") {
	Amplitude<int> d(16);
	d *= 2;
	REQUIRE(d == Amplitude<int>(32));
}

TEST_CASE("Unit DivAccum", "[Units]") {
	Amplitude<int> d(16);
	d /= 2;
	REQUIRE(d == Amplitude<int>(8));
}

TEST_CASE("Unit ModAccum", "[Units]") {
	Decibel<float> d(5.0f);
	const Decibel<float> e(4.0f);
	d %= e;
	REQUIRE(d == Decibel<float>(1.0f));
}

TEST_CASE("Unit String", "[Units]") {
	const Amplitude<float> a = 0.0;
	REQUIRE(std::string{a} == std::string{"0.000000"});	
}

TEST_CASE("Unit Ostream", "[Units]") {
	std::ostream o(std::cout.rdbuf());
	o << '\n';
	const Decibel<float> d = 0.0_dB;
	o << d;	
	o << '\n';
	o << '\n';
	REQUIRE(d.count() == 0.0f);	
}

TEST_CASE("Decibel Literal", "[Decibels]") {
	const Decibel<float> d = 0.0_dB;
	REQUIRE_THAT(d.count(), Catch::WithinRel(0.0f));

}

TEST_CASE("Decibel Addition", "[Decibels]") {
	const Decibel<double> d = Amplitude<double>{sqrt(.5)};
	const auto fullPowerAmount = (d+d).count();
	REQUIRE_THAT(fullPowerAmount, Catch::WithinAbs(0.0, .000005));
}

TEST_CASE("Decibel Subtraction", "[Decibels]") {
	const Decibel<float> d = 0.0_dB;
	using UnderlyingType = decltype(d.count());
	if constexpr(std::numeric_limits<UnderlyingType>::has_infinity)//subtracting two identical dbfs values should result in -inf or something really close
		REQUIRE((d-d).count() == std::numeric_limits<UnderlyingType>::infinity()*UnderlyingType{-1});
	else
		REQUIRE((d-d).count() == std::numeric_limits<UnderlyingType>::min());
}

TEST_CASE("Decibel Multiplication", "[Decibels]") {
	const Decibel<float> d = -3.0_dB;
	REQUIRE_THAT((d*d).count(), Catch::WithinRel(-6.0f));
}

TEST_CASE("Decibel Division", "[Decibels]") {
	const Decibel<float> d = -3.0_dB;
	REQUIRE_THAT((d/d).count(), Catch::WithinRel(0.0f));
}

TEST_CASE("Decibel String Conversion", "[Decibels]") {
	REQUIRE(std::string{-200.0_dB} == "-inf dB");
}


TEST_CASE("Amplitude Examples", "[Examples]") {
    const Amplitude<float> amplitude = .5f;
    const Decibel<float> decibel = amplitude;

    const Decibel<double> halfPowerDecibel = Amplitude<double>{std::sqrt(.5)};
    const Amplitude<double> halfPowerAmplitude = halfPowerDecibel;

    const Amplitude<float> floatHalfPowerAmplitude = halfPowerAmplitude;
    const Decibel<float> floatHalfPowerDecibel = halfPowerDecibel;

	REQUIRE_THAT(decibel.count(), Catch::WithinRel(Decibel<float>{-6.0206_dB}.count()));

	REQUIRE_THAT((floatHalfPowerDecibel+floatHalfPowerDecibel).count(), Catch::WithinAbs(0.0, .000005));
}

TEST_CASE("Resonance Examples", "[Examples]") {
    const ResonanceCoefficient<float> resonance{0.0f};
    const QCoefficient<float> q = resonance;
    const ResonanceCoefficient<float> largeResonance{.99999f};

	REQUIRE(q.count() == .5f);
	REQUIRE_THAT(ResonanceCoefficient<float>{q}.count(), Catch::WithinRel(0.0f));
	REQUIRE(QCoefficient<float>{largeResonance}.count() > 420.0f);
}