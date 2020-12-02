#pragma once

#include "UnitBase.h"
#include <cmath>

template <typename NumericType>
class Decibel;

template <typename NumericType>
class Amplitude : public Unit<Amplitude, NumericType>
{
public:
    //Less generic than second (commented out constructor), but CTAD friendly. Worth it?	
    constexpr Amplitude(const NumericType& initValue = NumericType{0}) noexcept(std::is_nothrow_constructible_v<NumericType>) : Unit<Amplitude, NumericType>(initValue) {}
    
//	template <typename T>
//	constexpr Amplitude(const T& initValue) noexcept(std::is_nothrow_constructible_v<NumericType, T>) : Unit<Amplitude, NumericType>(static_cast<NumericType>(initValue)) {}

	template <typename T>
	constexpr Amplitude(const Amplitude<T>& other) noexcept(std::is_nothrow_constructible_v<NumericType, T>) : Unit<Amplitude, NumericType>(static_cast<NumericType>(other.count())) {}

    template<typename T>
	constexpr Amplitude(const Decibel<T>& decibelValue)  noexcept(std::is_nothrow_constructible_v<NumericType, T>) : Unit<Amplitude, NumericType>(convertDecibelToAmplitude(decibelValue)) {}

	constexpr operator NumericType() const noexcept {return this->count();}

    static constexpr NumericType convertDecibelToAmplitude(const Decibel<NumericType>& decibelValue) {
        return convertDecibelToAmplitude(decibelValue, decibelValue.getMinusInfinityDB());
    }
    static constexpr NumericType convertDecibelToAmplitude(const Decibel<NumericType>& decibelValue, const Decibel<NumericType>& minusInfinityDb) {
		return (decibelValue > minusInfinityDb
			? std::pow(NumericType(10), decibelValue.count() * NumericType(0.05))
			: NumericType(0));
	}
};


template <typename NumericType>
class Decibel : public Unit<Decibel, NumericType>
{
    //Formula I learned in school for how many decibels can fit into a type
    //Doesn't seem right...
    static constexpr auto getDefaultMinusInifinityDBValue () noexcept {
        using ReturnType = std::conditional_t<std::is_signed_v<NumericType>, NumericType, int>;
        return std::is_arithmetic<NumericType>::value
               ? static_cast<ReturnType>(sizeof(NumericType)*8)*-6
               : NumericType{-120};
    }

    //Because the construction of a decibel needs to not depend on this conversion function when using a default -inf dB, this function exists to calculate that without unit types
    static constexpr auto convertAmplitudeToDecibelValue(const NumericType& amplitudeValue,
                                                         const NumericType& minusInfinitydB = getDefaultMinusInifinityDBValue())
    {
        const decltype(minusInfinitydB) a = std::log10(amplitudeValue)* NumericType{20};
        return std::max(minusInfinitydB, a);
    }
    
public:
    constexpr Decibel()
    : Unit<Decibel, NumericType>(getDefaultMinusInifinityDBValue()) {}

	//Converting constructor for Decibel Types other than Decibel<NumericType>
	template <typename T>
	constexpr Decibel(const Decibel<T>& other)
	noexcept(std::is_nothrow_constructible_v<NumericType, T>)
	: Unit<Decibel, NumericType>(static_cast<NumericType>(other.count())) {}

	// Explicit converting constructor for non-unit types
    // if they can be converted to the underlying type.
    constexpr explicit Decibel(const NumericType& initValue)
    noexcept(std::is_nothrow_copy_constructible<NumericType>::value)
    : Unit<Decibel, NumericType>(initValue) {}

	constexpr explicit Decibel(const NumericType& initValue,
                               const NumericType& initMinusInifnityDB)
    noexcept(std::is_nothrow_copy_constructible<NumericType>::value)
    : Unit<Decibel, NumericType>(initValue),
      minusInfinitydB(initMinusInifnityDB) {}

    //Converting constructor for amplitude units
	template<typename T>
	constexpr Decibel(const Amplitude<T>& amplitudeValue)
	noexcept(std::is_nothrow_constructible_v<NumericType, T>)
	: Unit<Decibel, NumericType>(convertAmplitudeToDecibelValue(static_cast<NumericType>(amplitudeValue.count()))) {}


    static constexpr auto getDefaultMinusInifinityDB () noexcept {
        return Decibel<NumericType>{Amplitude<NumericType>{getDefaultMinusInifinityDBValue()}};
    }

    static constexpr auto convertAmplitudeToDecibel(const NumericType& amplitudeValue,
                                                    const Decibel<NumericType>& minusInfinitydB = getDefaultMinusInifinityDB())
    {
        return Decibel{std::max(minusInfinitydB.count(), std::log10(amplitudeValue)* NumericType{20})};
    }



    template<typename OtherType>
	constexpr auto operator+= (const Decibel<OtherType>& other){	
		this->value = static_cast<NumericType>((*this + other).count());
		return *this;
	}

	template<typename OtherType>
	constexpr auto operator-= (const Decibel<OtherType>& other){
		this->value = static_cast<NumericType>((*this - other).count());
		return *this;
	}

	template<typename OtherType>
	constexpr auto operator*= (const Decibel<OtherType>& other){
		this->value = static_cast<NumericType>((*this*other).count());
		return *this;
	}

	template<typename OtherType>
	constexpr auto operator/= (const Decibel<OtherType>& other){
		this->value = static_cast<NumericType>((*this/other).count());
		return *this;
	}

	operator std::string() const {
		return (this->count() > minusInfinitydB ? std::to_string(this->value) : "-inf ") + "dB";
	}
    
    constexpr auto getMinusInfinityDB() const noexcept { return Decibel<NumericType>{minusInfinitydB}; }
    
    void setMinusInfinitydB(const Decibel<NumericType>& newMinusInfinitydB) noexcept {
        minusInfinitydB = newMinusInfinitydB.count();
    }
    
private:
    NumericType minusInfinitydB{getDefaultMinusInifinityDBValue()};
};

template<typename T>
std::ostream& operator << ( std::ostream& os, const Decibel<T>& value ) {
    os << std::string( value );
    return os;
}

template <typename T, typename U>
constexpr auto operator+(const Decibel<T>& lhs, const Decibel<U>& rhs){
    const auto a = std::pow(T{10}, lhs.count()/T{10});
    if(isinf(a))
        return Decibel<T>(lhs.getMinusInfinityDB());

    const auto b = std::pow(U{10}, rhs.count()/U{10});
    if(isinf(b))
        return Decibel<T>(lhs.getMinusInfinityDB());
    else {
        const auto c = 10 * std::log (a + b);
        if(isinf(c))
            return Decibel<T>(lhs.getMinusInfinityDB());
        else
            return Decibel<T>(c);
    }
}

template <typename T, typename U>
constexpr Decibel<T> operator-(const Decibel<T>& lhs, const Decibel<U>& rhs){
    const auto a = Amplitude<T>{lhs};
    const auto b = Amplitude<U>{rhs};
    auto sign = std::signbit(Amplitude<T>{lhs}.count()-Amplitude<U>{rhs}.count()) ? -T{1} : T{1};
    return sign*Decibel<T>{ Amplitude<T>{lhs}-Amplitude<U>{rhs} };
//    }
}

template <typename T, typename U>
constexpr auto operator*(const Decibel<T>& lhs, const Decibel<U>& rhs){
	const auto newValue = lhs.count()+rhs.count();
	return Decibel<std::decay_t<decltype(newValue)>>{newValue};
}

template <typename T, typename U>
constexpr auto operator*(const Decibel<T>& lhs, const U& rhs){
    const Amplitude<T> newValue = Amplitude<T>{lhs}*rhs;
    return Decibel<T>{newValue};
}

template <typename T, typename U>
constexpr auto operator*(const T& lhs, const Decibel<U>& rhs){
    return rhs*lhs;
}

template <typename T, typename U>
constexpr auto operator/(const Decibel<T>& lhs, const U& rhs){
    const Amplitude<T> newValue = Amplitude<T>{lhs}/rhs;
    return Decibel<T>{newValue};
}

template <typename T, typename U>
constexpr auto operator/(const T& lhs, const Decibel<U>& rhs){
    return rhs/lhs;
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


template<typename T>
Amplitude(T) -> Amplitude<T>;

template<typename T>
Amplitude(Decibel<T>) -> Amplitude<T>;

template<typename T>
Decibel(T) -> Decibel<T>;

template<typename T>
Decibel(Amplitude<T>) -> Decibel<T>;
