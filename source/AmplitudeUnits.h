#pragma once

#include "UnitBase.h"
#include <cmath>

template <typename InputType>
class Decibel;

//Formula I learned in school for how many decibels can fit into a type
//Doesn't seem right...
#ifdef _MSC_VER
template<typename NumericType>
struct DummyDB {
private:
	static constexpr NumericType val = std::is_arithmetic<NumericType>::value
					 ? NumericType{ static_cast<int>(sizeof(NumericType) * 8) * -6 }
					 : NumericType{ -120 };
public:
	constexpr const auto& count() const noexcept { 
		return val;
} };
template<typename NumericType>
constexpr DummyDB<NumericType> defaultMinusInfinitydB{};
#else
template<typename NumericType>
constexpr auto defaultMinusInfinitydB = std::is_arithmetic<NumericType>::value
										? Decibel<NumericType>{static_cast<int>(sizeof(NumericType)*8)*-6} 
										: Decibel<NumericType>{-120};
#endif


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

	constexpr operator InputType() const noexcept {return this->count();}

	static constexpr InputType convertDecibelToAmplitude(const InputType& decibelValue, const InputType& minusInfinityDb = defaultMinusInfinitydB<InputType>.count()) {
		return (decibelValue > minusInfinityDb
			? std::pow(InputType(10), decibelValue * InputType(0.05)) 
			: InputType(0));
	}
};



template <typename InputType>
class Decibel : public Unit<Decibel, InputType>
{
public:
	//Converting constructor for Decibel Types other than Decibel<InputType> 
	template <typename T>
	constexpr Decibel(const Decibel<T>& other) noexcept(std::is_nothrow_constructible_v<InputType, T>) : Unit<Decibel, InputType>(static_cast<InputType>(other.count())) {}

	// Explicit converting constructor for non-unit types
    // if they can be converted to the underlying type.
	constexpr explicit Decibel(const InputType& initValue) noexcept(std::is_nothrow_copy_constructible<InputType>::value) : Unit<Decibel, InputType>(initValue) {}

    //Converting constructor for amplitude units
	template<typename T>
	constexpr Decibel(const Amplitude<T>& amplitudeValue)  noexcept(std::is_nothrow_constructible_v<InputType, T>) : Unit<Decibel, InputType>(convertAmplitudeToDecibel(amplitudeValue.count())) {}

	static constexpr auto convertAmplitudeToDecibel(const InputType& amplitudeValue, const InputType& minusInfinitydB = defaultMinusInfinitydB<InputType>.count()) {
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
		return (this->count() > defaultMinusInfinitydB<InputType>.count() ? std::to_string(this->value) : "-inf ") + "dB";
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

//constexpr auto operator"" _dB(unsigned long long val) {
//	return Decibel<unsigned long long >(val);
//}
