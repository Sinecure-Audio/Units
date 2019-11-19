#pragma once

#include "./Units.h"

template <typename InputType>
class QCoefficient;

template <typename InputType>
class ResonanceCoefficient : public Unit<ResonanceCoefficient, InputType>
{
public:
	constexpr ResonanceCoefficient() = default;

	constexpr explicit ResonanceCoefficient(const InputType& initValue) {this->value = initValue;}

	template <typename T>//	, std::enable_if_t<std::is_convertible_v<InputType, T>>>
	constexpr ResonanceCoefficient(const ResonanceCoefficient<T>& other) { this->value = InputType(other.count()); };

	ResonanceCoefficient& operator=(const QCoefficient<InputType>&& newCoefficient) noexcept { this->value = convertQToResonance(newCoefficient.count()); return *this; };

	static InputType convertQToResonance(const InputType& QCoefficient) {
		return InputType{1.0} - InputType{1.0} / (InputType{2.0}*QCoefficient);
	}
};

template <typename InputType>
class QCoefficient : public Unit<QCoefficient, InputType>
{
public:
	constexpr QCoefficient() = default;

	constexpr explicit QCoefficient(const InputType& initValue) {this->value = initValue;}

	template <typename T, std::enable_if_t<std::is_convertible_v<InputType, T>>>
	constexpr QCoefficient(const QCoefficient<T>& other) { this->value = InputType(other.count()); };

	constexpr QCoefficient(const ResonanceCoefficient<InputType>& resonanceValue)   { this->value = convertResonanceToQ (resonanceValue .count()); };

	QCoefficient& operator=(const ResonanceCoefficient<InputType>& newCoefficient)  noexcept { this->value = convertResonanceToQ(newCoefficient .count()); return *this; };

	static InputType convertResonanceToQ(const InputType& resonanceCoefficient) {
		return static_cast<InputType>(1.0 / (2.0*(1.0 - resonanceCoefficient)));
	}
};