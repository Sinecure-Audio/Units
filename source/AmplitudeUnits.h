#pragma once

#include "Units.h"
#include <algorithm>
#include <string>

namespace {
	constexpr auto defaultMinusInfinitydB = -120.0;
}

template <typename InputType>
class Decibel;


template <typename InputType>
class Amplitude : public Unit<Amplitude, InputType>
{
public:
	constexpr Amplitude() = default;
	constexpr Amplitude(const InputType& initValue) { this->value = initValue; };

	template <typename T, std::enable_if_t<std::is_convertible_v<InputType, T>>>
	constexpr Amplitude(const Amplitude<T>& other) { this->value = InputType(other.count()); };

    template<typename T>
	constexpr Amplitude(const Decibel<T>& decibelValue)  { this->value = convertDecibelToAmplitude(decibelValue.count()); }
    template<typename T>
	Amplitude& operator=(const Decibel<T>& decibelValue) { this->value = convertDecibelToAmplitude(decibelValue.count()); return *this; };

	static constexpr InputType convertDecibelToAmplitude(const InputType& decibelValue, const InputType& minusInfinityDb = defaultMinusInfinitydB) {
		return decibelValue > minusInfinityDb
			? std::pow(InputType{10}, decibelValue * InputType{0.05}) 
			: InputType{0};
	}
};




template <typename InputType>
class Decibel : public Unit<Decibel, InputType>
{
public:
	constexpr Decibel() = default;

	template <typename T> //, std::enable_if_t<std::is_convertible_v<InputType, T>>>
	constexpr Decibel(const Decibel<T>& other) {this->value = InputType(other.count());};
	constexpr explicit Decibel(const InputType& initValue) { this->value = initValue; };
	constexpr Decibel(const Amplitude<InputType>& amplitudeValue) {this->value = convertAmplitudeToDecibel(amplitudeValue.count());}

	Decibel& operator=(const Amplitude<InputType>& amplitudeValue)  { this->value = convertAmplitudeToDecibel(amplitudeValue.count()); return *this; };

	static constexpr auto convertAmplitudeToDecibel(const InputType& amplitudeValue, const InputType& minusInfinitydB = defaultMinusInfinitydB) {
		return std::max(minusInfinitydB, std::log10(amplitudeValue)* InputType{20});
	}

    //...
    template<typename T>
	constexpr auto operator+= (const Decibel<InputType>& other){	
     const auto copy = *this;
		this->value = (copy + other.count()).count();
		return *this;
	}

	template<typename T>
	constexpr auto operator-= (const Decibel<InputType>& other){
		const auto copy = *this;
		this->value = (copy - other.count()).count();
		return *this;
	}

	template<typename T>
	constexpr auto operator*= (const Decibel<InputType>& other){
		const auto copy = *this;
		this->value = (copy * other.count()).count();
		return *this;
	}

	template<typename T>
	constexpr auto operator/= (const Decibel<InputType>& other){
        const auto copy = *this;
		this->value = (copy / other.count()).count();
		return *this;
	}
};

template <typename T, typename U>
constexpr auto operator+(const Decibel<T>& lhs, const Decibel<U>& rhs){
	using CommonType = std::common_type_t<T, U>;
	return Decibel<CommonType>{CommonType{10}*std::log(std::pow(CommonType{10}, lhs.count()/CommonType{10}) + std::pow(CommonType{10}, rhs.count()/CommonType{10}))};
}

template <typename T, typename U>
constexpr auto operator-(const Decibel<T>& lhs, const Decibel<U>& rhs){
	using CommonType = std::common_type_t<T, U>;
	return Decibel<CommonType>{CommonType{10}*std::log(std::pow(CommonType{10}, lhs.count()/CommonType{10}) - std::pow(CommonType{10}, rhs.count()/CommonType{10}))};
}

template <typename T, typename U>
constexpr auto operator*(const Decibel<T>& lhs, const Decibel<U>& rhs){
	using CommonType = std::common_type_t<T, U>;
	return Decibel<CommonType>{lhs.count() + rhs.count()};
}

template <typename T, typename U>
constexpr auto operator/(const Decibel<T>& lhs, const Decibel<U>& rhs){
	using CommonType = std::common_type_t<T, U>;
	return Decibel<CommonType>{lhs.count() - rhs.count()};
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const  Decibel<T>& decibel) {
	return out << (decibel.count() > defaultMinusInfinitydB ? std::to_string(decibel.count()) : "-inf ") + "dB";
}

//user defined litereals for making decibel templates
constexpr auto operator"" _dB(long double val) {
	return Decibel<long double>(val);
};

constexpr auto operator"" _dB(unsigned long long val) {
	return Decibel<unsigned long long >(val);
};