#include "TestIncludes.h"

TEST_CASE("Unit Creation", "[Units]") {
	const Decibel<float> d(0.0f);
    //Use with abs because rel gets weird with 0
	REQUIRE_THAT(d.count(), Catch::WithinAbs(0.0f, .00001));
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
	REQUIRE_THAT((d%e).count(), Catch::WithinRel(1.0f));
    REQUIRE_THAT((e%d).count(), Catch::WithinRel(4.0f));
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
	REQUIRE((d << 1) == Decibel<int>(32));
}

TEST_CASE("Unit Right Shift1", "[Units]") {
	const Decibel<int> d(16);
	REQUIRE((d >> 1) == Decibel<int>(8));
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
	REQUIRE(std::string(std::to_string(a)) == std::string(std::to_string(0.0)));	
}
