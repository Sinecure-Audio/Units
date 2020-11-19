#pragma once

#include "UnitBase.h"

template <typename NumericType>
class QCoefficient;

template <typename NumericType>
class ResonanceCoefficient : public Unit<ResonanceCoefficient, NumericType>
{
public:
    constexpr ResonanceCoefficient() = default;

	template <typename T>
	constexpr explicit ResonanceCoefficient(const T& initValue) noexcept(std::is_nothrow_constructible_v<NumericType, T>) : Unit<ResonanceCoefficient, NumericType>(static_cast<NumericType>(initValue)) {}

	template <typename T>
	constexpr ResonanceCoefficient(const ResonanceCoefficient<T>& other) noexcept(std::is_nothrow_constructible_v<NumericType, T>) : Unit<ResonanceCoefficient, NumericType>(static_cast<NumericType>(other.count())) {}

	template <typename T>
	constexpr ResonanceCoefficient(const QCoefficient<T>& qValue)  noexcept(std::is_nothrow_constructible_v<NumericType, T>) : Unit<ResonanceCoefficient, NumericType>(static_cast<NumericType>(convertQToResonance (qValue .count()))) {}

	static constexpr NumericType convertQToResonance(const NumericType& qValue) noexcept {
		return NumericType{1.0} - NumericType{1.0} / (NumericType{2.0}*qValue);
	}
};

template <typename NumericType>
class QCoefficient : public Unit<QCoefficient, NumericType>
{
public:
    constexpr QCoefficient() = default;

	template <typename T>
	constexpr explicit QCoefficient(const T& initValue) noexcept(std::is_nothrow_constructible_v<NumericType, T>) : Unit<QCoefficient, NumericType>(static_cast<NumericType>(initValue)) {}

	template <typename T>
	constexpr QCoefficient(const QCoefficient<T>& other) noexcept(std::is_nothrow_constructible_v<NumericType, T>) : Unit<QCoefficient, NumericType>(static_cast<NumericType>(other.count())) {}

	template <typename T>
	constexpr QCoefficient(const ResonanceCoefficient<T>& resonanceCoefficient)  noexcept(std::is_nothrow_constructible_v<NumericType, T>) : Unit<QCoefficient, NumericType>(static_cast<NumericType>(convertResonanceToQ (resonanceCoefficient.count()))) {}

	static NumericType convertResonanceToQ(const NumericType& resonanceValue) {
		return NumericType{1.0} / (NumericType{2.0}*(NumericType{1.0} - resonanceValue));
	}
};

template <typename NumericType>
class Bandwidth : public Unit<Bandwidth, NumericType>
{
public:
    template <typename T>
    constexpr explicit Bandwidth(const T& initValue, const T& centerFrequency) noexcept(std::is_nothrow_constructible_v<NumericType, T>)
    : Unit<Bandwidth, NumericType>(static_cast<NumericType>(initValue)), centerFrequency(centerFrequency),
      lowerBound(centerFrequency-this->value/T{2}),
      upperBound(centerFrequency+this->value/T{2}) {}

    template <typename T>
    constexpr Bandwidth(const QCoefficient<T>& qCoefficient, const T& centerFrequency) noexcept(std::is_nothrow_constructible_v<NumericType, T>)
    : Unit<Bandwidth, NumericType>(static_cast<NumericType>(centerFrequency)/static_cast<NumericType>(qCoefficient.count())),
      centerFrequency(static_cast<NumericType>(centerFrequency)),
      lowerBound(centerFrequency-this->value/T{2}),
      upperBound(centerFrequency+this->value/T{2}) {}

    template <typename T>
    constexpr Bandwidth(const Bandwidth<T>& otherBandwidth)  noexcept(std::is_nothrow_constructible_v<NumericType, T>)
    : Unit<Bandwidth, NumericType>(static_cast<NumericType>(otherBandwidth.count())),
    centerFrequency(static_cast<NumericType>(otherBandwidth.centerFrequency)),
    lowerBound(static_cast<NumericType>(otherBandwidth.lowerBound)),
    upperBound(static_cast<NumericType>(otherBandwidth.upperBound)) {}

    static constexpr NumericType convertQToBandwidth(const QCoefficient<NumericType>& qValue,
                                                     const NumericType& centerFrequency) {
        return Bandwidth{NumericType{1}/qValue, centerFrequency};
    }

    constexpr auto getLowerBound() const noexcept {
        return lowerBound;
    }

    constexpr auto getUpperBound() const noexcept {
        return upperBound;
    }

    constexpr auto getBounds() const noexcept {
        return std::tuple{lowerBound, upperBound};
    }
    
private:
    NumericType centerFrequency, lowerBound, upperBound;
};

template<typename T>
ResonanceCoefficient(T) -> ResonanceCoefficient<T>;

template<typename T>
QCoefficient(T) -> QCoefficient<T>;

template<typename T>
Bandwidth(T, T) -> Bandwidth<T>;

template<typename T>
Bandwidth(QCoefficient<T>, T) -> Bandwidth<T>;