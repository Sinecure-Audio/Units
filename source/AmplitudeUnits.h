#pragma once

#include "UnitBase.h"
#include <cmath>

namespace {
	constexpr auto defaultMinusInfinitydB = -120.0;
}

template <typename InputType>
class Decibel;


template <typename InputType>
class Amplitude : public Unit<Amplitude, InputType>
{
public:
	template <typename T>
	constexpr Amplitude(const T& initValue) noexcept(std::is_nothrow_constructible_v<InputType, T>) : Unit<Amplitude, InputType>(static_cast<InputType>(initValue)) {}

	template <typename T>
	constexpr Amplitude(const Amplitude<T>& other) noexcept(std::is_nothrow_constructible_v<InputType, T>) : Unit<Amplitude, InputType>(static_cast<InputType>(other.count())) {}

    template<typename T>
	constexpr Amplitude(const Decibel<T>& decibelValue)  noexcept(std::is_nothrow_constructible_v<InputType, T>) : Unit<Amplitude, InputType>(convertDecibelToAmplitude(decibelValue.count())) {}

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
	//Converting constructor for Decibel Types other than Decibel<InputType> 
	template <typename T>
	constexpr Decibel(const Decibel<T>& other) noexcept(std::is_nothrow_constructible_v<InputType, T>) : Unit<Decibel, InputType>(static_cast<InputType>(other.count())) {}

	//Explicit converting constructor for algebraic types.
	template <typename T>
	constexpr explicit Decibel(const T& initValue) noexcept(std::is_nothrow_constructible_v<InputType, T>) : Unit<Decibel, InputType>(static_cast<InputType>(initValue)) {}

	template<typename T>
	constexpr Decibel(const Amplitude<T>& amplitudeValue)  noexcept(std::is_nothrow_constructible_v<InputType, T>) : Unit<Decibel, InputType>(convertAmplitudeToDecibel(amplitudeValue.count())) {}

	static constexpr auto convertAmplitudeToDecibel(const InputType& amplitudeValue, const InputType& minusInfinitydB = defaultMinusInfinitydB) {
		return std::max(minusInfinitydB, std::log10(amplitudeValue)* InputType{20});
	}

    
    template<typename OtherType>
	constexpr auto operator+= (const Decibel<OtherType>& other){	
		this->value = static_cast<InputType>((*this + other).count());
		return *this;
	}

	template<typename OtherType>
	constexpr auto operator-= (const Decibel<OtherType>& other){
		this->value = static_cast<InputType>((*this - other).count());
		return *this;
	}

	template<typename OtherType>
	constexpr auto operator*= (const Decibel<OtherType>& other){
		this->value = static_cast<InputType>((*this*other).count());
		return *this;
	}

	template<typename OtherType>
	constexpr auto operator/= (const Decibel<OtherType>& other){
		this->value = static_cast<InputType>((*this/other).count());
		return *this;
	}

	operator std::string() const {
		return (this->value > static_cast<InputType>(defaultMinusInfinitydB) ? std::to_string(this->value) : "-inf ") + "dB";
	}
};

template <typename T, typename U>
constexpr auto operator+(const Decibel<T>& lhs, const Decibel<U>& rhs){
	const auto newValue = std::log(std::pow(T{10}, lhs.count()/T{10}) + std::pow(U{10}, rhs.count()/U{10}));
	return Decibel<std::decay_t<decltype(newValue)>>{newValue};
}

template <typename T, typename U>
constexpr auto operator-(const Decibel<T>& lhs, const Decibel<U>& rhs){
	const auto newValue = std::log(std::pow(T{10}, lhs.count()/T{10}) - std::pow(U{10}, rhs.count()/U{10}));
	return Decibel<std::decay_t<decltype(newValue)>>{newValue};
}

template <typename T, typename U>
constexpr auto operator*(const Decibel<T>& lhs, const Decibel<U>& rhs){
	const auto newValue = lhs.count()+rhs.count();
	return Decibel<std::decay_t<decltype(newValue)>>{newValue};
}

template <typename T, typename U>
constexpr auto operator/(const Decibel<T>& lhs, const Decibel<U>& rhs){
	const auto newValue = lhs.count()-rhs.count();
	return Decibel<std::decay_t<decltype(newValue)>>{newValue};
}

//user defined litereals for making decibel templates
constexpr auto operator"" _dB(long double val) {
	return Decibel<long double>(val);
}

constexpr auto operator"" _dB(unsigned long long val) {
	return Decibel<unsigned long long >(val);
}