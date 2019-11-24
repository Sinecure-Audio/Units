#pragma once

#include <type_traits>
#include <string>

template <template<typename> class UnitType, typename InputType>
class Unit
{
public:

	constexpr Unit() = default;

	constexpr auto operator-() const {
		return UnitType<InputType>(value * InputType(-1.0));
	}

	constexpr InputType count() const noexcept {
		return value;
	}

	template<typename T>
	constexpr auto operator+= (const UnitType<T>& other) {
		value += other.count();
		return *this;
	}

	template<typename T>
	constexpr auto operator-= (const UnitType<T>& other) {
		value -= other.count();
		return *this;
	}

	template<typename T>
	constexpr auto operator*= (const UnitType<T>& other) {
		value *= other.count();
		return *this;
	}

	template<typename T>
	constexpr auto operator/= (const UnitType<T>& other) {
		value /= other.count();
		return *this;
	}

	template<typename T>
	constexpr auto operator%= (const UnitType<T>& other) {
		if constexpr (std::is_integral_v<InputType>&& std::is_integral_v<T>)
			value %= other.count();
		else
			value = fmod(value, other.count());
		return *this;
	}

	template<typename T>
	constexpr auto operator^= (const UnitType<T>& other) {
		value ^= other.count();
		return *this;
	}

	template<typename T>
	constexpr auto operator&= (const UnitType<T>& other) {
		value &= other.count();
		return *this;
	}

	template<typename T>
	constexpr auto operator|= (const UnitType<T>& other) {
		value |= other.count();
		return *this;
	}

	constexpr auto operator++(int)
	{
		auto out = UnitType<InputType>(value);
		value++;
		return out;
	}

	constexpr auto operator--(int)
	{
		auto out = UnitType<InputType>(value);
		value--;
		return out;
	}

	constexpr auto& operator++()
	{
		++value;
		return *static_cast<UnitType<InputType>*>(this);
	}

	constexpr auto& operator--()
	{
		--value;
		return *static_cast<UnitType<InputType>*>(this);
	}

	constexpr auto operator~ () {
		return UnitType<InputType>(~value);
	}


	operator std::string() const { return std::to_string(value); }

protected:
	InputType value{ 0 };
};

template <template<typename> class T, typename U, typename V>
constexpr bool operator==(const Unit<T, U>& lhs, const Unit<T, V>& rhs) { return lhs.count() == rhs.count(); }
template <template<typename> class T, typename U, typename V>
constexpr bool operator!=(const Unit<T, U>& lhs, const Unit<T, V>& rhs) { return !operator==(lhs, rhs); }
template <template<typename> class T, typename U, typename V>
constexpr bool operator<(const Unit<T, U>& lhs, const Unit<T, V>& rhs) { return lhs.count() < rhs.count(); }
template <template<typename> class T, typename U, typename V>
constexpr bool operator>(const Unit<T, U>& lhs, const Unit<T, V>& rhs) { return  operator< (rhs, lhs); }
template <template<typename> class T, typename U, typename V>
constexpr bool operator<=(const Unit<T, U>& lhs, const Unit<T, V>& rhs) { return !operator> (lhs, rhs); }
template <template<typename> class T, typename U, typename V>
constexpr bool operator>=(const Unit<T, U>& lhs, const Unit<T, V>& rhs) { return !operator< (lhs, rhs); }


template <template<typename> class T, typename U, typename V>
constexpr auto operator+(const Unit<T, U>& lhs, const Unit<T, V>& rhs) {
	using CommonType = std::common_type_t<U, V>;
	return T<CommonType>(lhs.count() + rhs.count());
}

template <template<typename> class T, typename U, typename V>
constexpr auto operator-(const Unit<T, U>& lhs, const Unit<T, V>& rhs) {
	using CommonType = std::common_type_t<U, V>;
	return T<CommonType>(lhs.count() - rhs.count());
}

template <template<typename> class T, typename U, typename V>
constexpr auto operator*(const Unit<T, U>& lhs, const Unit<T, V>& rhs) {
	using CommonType = std::common_type_t<U, V>;
	return T<CommonType>(lhs.count() * rhs.count());
}

template <template<typename> class T, typename U, typename V>
constexpr auto operator/(const Unit<T, U>& lhs, const Unit<T, V>& rhs) {
	using CommonType = std::common_type_t<U, V>;
	return T<CommonType>(lhs.count() / rhs.count());
}

template <template<typename> class T, typename U, typename V>
constexpr auto operator%(const Unit<T, U>& lhs, const Unit<T, V>& rhs) {
	using CommonType = std::common_type_t<U, V>;
	if constexpr (std::is_integral_v<CommonType>)
		return T<CommonType>(lhs.count() % rhs.count());
	else
		return T<CommonType>(fmod(lhs.count(), rhs.count()));
}

template <template<typename> class T, typename U>
std::ostream& operator<<(std::ostream& out, const  Unit<T, U>& unit) {
	return out << unit.count();
}

template <template<typename> class T, typename U>
constexpr auto operator<<(const Unit<T, U>& lhs, const int& rhs) {
	return T<U>(lhs.count() << rhs);
}

template <template<typename> class T, typename U>
constexpr auto operator>>(const Unit<T, U>& lhs, const int& rhs) {
	return T<U>(lhs.count() >> rhs);
}

template <template<typename> class T, typename U, typename V>
constexpr auto operator^(const Unit<T, U>& lhs, const Unit<T, V>& rhs) {
	return T<U>(lhs.count() ^ rhs.count());
}

template <template<typename> class T, typename U, typename V>
constexpr auto operator|(const Unit<T, U>& lhs, const Unit<T, V>& rhs) {
	return T<U>(lhs.count() | rhs.count());
}

template <template<typename> class T, typename U, typename V>
constexpr auto operator&(const Unit<T, U>& lhs, const Unit<T, V>& rhs) {
	return T<U>(lhs.count() & rhs.count());
}template <template<typename> class T, typename U>

template <template<typename> class T, typename U>
void operator <<(std::ostream& out, const Unit<T, U>& unit) { out << std::string{*static_cast<const T<U>*>(&unit)}; }