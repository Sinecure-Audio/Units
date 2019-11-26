#pragma once

#include "UnitBase.h"

template <typename InputType>
class QCoefficient;

template <typename InputType>
class ResonanceCoefficient : public Unit<ResonanceCoefficient, InputType>
{
public:
	template <typename T>
	constexpr explicit ResonanceCoefficient(const T& initValue) noexcept(std::is_nothrow_constructible_v<InputType, T>) : Unit<ResonanceCoefficient, InputType>(static_cast<InputType>(initValue)) {}

	template <typename T>
	constexpr ResonanceCoefficient(const ResonanceCoefficient<T>& other) noexcept(std::is_nothrow_constructible_v<InputType, T>) : Unit<ResonanceCoefficient, InputType>(static_cast<InputType>(other.count())) {}

	template <typename T>
	constexpr ResonanceCoefficient(const QCoefficient<T>& qValue)  noexcept(std::is_nothrow_constructible_v<InputType, T>) : Unit<ResonanceCoefficient, InputType>(static_cast<InputType>(convertQToResonance (qValue .count()))) {}

	static constexpr InputType convertQToResonance(const InputType& qValue) noexcept {
		return InputType{1.0} - InputType{1.0} / (InputType{2.0}*qValue);
	}
};

template <typename InputType>
class QCoefficient : public Unit<QCoefficient, InputType>
{
public:
	template <typename T>
	constexpr explicit QCoefficient(const T& initValue) noexcept(std::is_nothrow_constructible_v<InputType, T>) : Unit<QCoefficient, InputType>(static_cast<InputType>(initValue)) {}

	template <typename T>
	constexpr QCoefficient(const QCoefficient<T>& other) noexcept(std::is_nothrow_constructible_v<InputType, T>) : Unit<QCoefficient, InputType>(static_cast<InputType>(other.count())) {}

	template <typename T>
	constexpr QCoefficient(const ResonanceCoefficient<T>& resonanceCoefficient)  noexcept(std::is_nothrow_constructible_v<InputType, T>) : Unit<QCoefficient, InputType>(static_cast<InputType>(convertResonanceToQ (resonanceCoefficient.count()))) {}

	static InputType convertResonanceToQ(const InputType& resonanceValue) {
		return InputType{1.0} / (InputType{2.0}*(InputType{1.0} - resonanceValue));
	}
};