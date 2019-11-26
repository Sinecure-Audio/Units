#pragma once

#include <type_traits>
#include <string>

template <template<typename> class UnitType, typename InputType>
class Unit
{
public:
	constexpr Unit(const InputType& initialValue) noexcept(noexcept(std::is_nothrow_constructible_v<InputType>)) : value(initialValue) {}

	constexpr auto operator-()  const noexcept { return UnitType<InputType>(value * InputType{-1.0}); }

	constexpr InputType count() const noexcept {return value;}

	template<typename T>
	constexpr auto operator+= (const UnitType<T>& other) noexcept(noexcept(this->value += other.count())) { value += other.count(); return *this; }
	template<typename T>
	constexpr auto operator-= (const UnitType<T>& other) noexcept(noexcept(this->value -= other.count())) { value -= other.count(); return *this; }

	// operators * and / work with normal types and not units to avoid having to deal with multiple dimensions of units.
	template<typename T>
	constexpr auto operator*= (const T& other)           noexcept(noexcept(this->value *= other)) { value *= other; return *this; }
	template<typename T>
	constexpr auto operator/= (const T& other) 			 noexcept(noexcept(this->value /= other)) { value /= other; return *this; }
	template<typename T>
	constexpr auto operator%= (const UnitType<T>& other) {
		if constexpr (std::is_integral_v<InputType> && std::is_integral_v<T>)
			value %= other.count();
		else
			value = static_cast<InputType>(fmod(value, other.count()));
		return *this;
	}

	template<typename T>
	constexpr auto operator^= (const T& other) noexcept(noexcept(this->value ^= other)) { value ^= other; return *this; }

	template<typename T>
	constexpr auto operator&= (const T& other) noexcept(noexcept(this->value &= other)) { value &= other; return *this; }

	template<typename T>
	constexpr auto operator|= (const T other)  noexcept(noexcept(this->value |= other)) { value |= other; return *this; }

	constexpr auto operator++(int) noexcept(noexcept(this->value++) && std::is_nothrow_constructible_v<UnitType<InputType>, InputType>) {
		const auto out{UnitType<InputType>{value}};
		value++;
		return out;
	}

	constexpr auto operator--(int) noexcept(noexcept(this->value--) && std::is_nothrow_constructible_v<UnitType<InputType>, InputType>)  {
		auto out{UnitType<InputType>(value)};
		value--;
		return out;
	}

	constexpr auto& operator++() noexcept(noexcept(++this->value)) {
		++value;
		return *static_cast<UnitType<InputType>*>(this);
	}

	constexpr auto& operator--() noexcept(noexcept(--this->value)) {
		--value;
		return *static_cast<UnitType<InputType>*>(this);
	}


	constexpr auto operator~ () noexcept(noexcept(~this->value)) { return UnitType<InputType>(~value); }

	operator std::string() const { return std::to_string(value); }

protected:
	InputType value{ 0 };
};

template <template<typename> class T, typename U, typename V>
constexpr bool operator==(const Unit<T, U>& lhs, const Unit<T, V>& rhs) noexcept(noexcept(lhs.count() == rhs.count())) { return lhs.count() == rhs.count(); }
template <template<typename> class T, typename U, typename V>
constexpr bool operator!=(const Unit<T, U>& lhs, const Unit<T, V>& rhs) noexcept(noexcept(!operator==(lhs, rhs)))      { return !operator==(lhs, rhs); }
template <template<typename> class T, typename U, typename V>
constexpr bool operator<(const Unit<T, U>& lhs, const Unit<T, V>& rhs)  noexcept(noexcept(lhs.count() < rhs.count()))  { return lhs.count() < rhs.count(); }
template <template<typename> class T, typename U, typename V>
constexpr bool operator>(const Unit<T, U>& lhs, const Unit<T, V>& rhs)  noexcept(noexcept(operator<(lhs, rhs)))        { return  operator < (rhs, lhs); }
template <template<typename> class T, typename U, typename V>
constexpr bool operator<=(const Unit<T, U>& lhs, const Unit<T, V>& rhs) noexcept(noexcept(!operator>(lhs, rhs)))       { return !operator> (lhs, rhs); }
template <template<typename> class T, typename U, typename V>
constexpr bool operator>=(const Unit<T, U>& lhs, const Unit<T, V>& rhs) noexcept(noexcept(!operator<(lhs, rhs)))       { return !operator< (lhs, rhs); }


template <template<typename> class T, typename U, typename V>
constexpr auto operator+(const Unit<T, U>& lhs, const Unit<T, V>& rhs) noexcept(noexcept(lhs.count() + rhs.count()) && std::is_nothrow_constructible_v<T<U>, U>) {
	const auto newValue{lhs.count() + rhs.count()};
	return T<std::decay_t<decltype(newValue)>>{newValue};
}

template <template<typename> class T, typename U, typename V>
constexpr auto operator-(const Unit<T, U>& lhs, const Unit<T, V>& rhs) noexcept(noexcept(lhs.count() - rhs.count()) && std::is_nothrow_constructible_v<T<U>, U>) {
	const auto newValue{lhs.count() - rhs.count()};	
	return T<std::decay_t<decltype(newValue)>>{newValue};
}

template <template<typename> class T, typename U>
constexpr auto operator*(const Unit<T, U>& lhs, const U& rhs) noexcept(noexcept(lhs.count() * rhs) && std::is_nothrow_constructible_v<T<U>, U>) {
	const auto newValue{lhs.count() * rhs};
	return T<std::decay_t<decltype(newValue)>>{newValue};
}

template <template<typename> class T, typename U>
constexpr auto operator/(const Unit<T, U>& lhs, const U& rhs) noexcept(noexcept(lhs.count() / rhs) && std::is_nothrow_constructible_v<T<U>, U>) {
	const auto newValue{lhs.count() / rhs};
	return T<std::decay_t<decltype(newValue)>>{newValue};
}

// operators * and / work with normal types and not units to avoid having to deal with multiple dimensions of units.
template <template<typename> class T, typename U, typename V>
constexpr auto operator*(const V& lhs, const Unit<T, U>& rhs) noexcept(noexcept(rhs * lhs)) { return rhs*lhs; }
template <template<typename> class T, typename U>
constexpr auto operator/(const U& lhs, const Unit<T, U>& rhs) noexcept(noexcept(lhs / rhs.count()) && std::is_nothrow_constructible_v<T<U>, U>) { return T<U>{lhs/rhs.count()};}

template <template<typename> class T, typename U, typename V>
constexpr auto operator%(const Unit<T, U>& lhs, const Unit<T, V>& rhs) {
	if constexpr (std::is_integral_v<U> && std::is_integral_v<V>) {
		const auto newValue = lhs.count() % rhs.count();
		return T<std::decay_t<decltype(newValue)>>{newValue};
	} else {
		const auto newValue = fmod(lhs.count(), rhs.count());
		return T<std::decay_t<decltype(newValue)>>{newValue};
	}
}

template <template<typename> class T, typename U>
constexpr auto operator<<(const Unit<T, U>& lhs, const int& rhs) noexcept(noexcept(lhs.count() << rhs) && std::is_nothrow_constructible_v<T<U>, U>) { return T<U>{lhs.count() << rhs}; }
template <template<typename> class T, typename U>
constexpr auto operator>>(const Unit<T, U>& lhs, const int& rhs) noexcept(noexcept(lhs.count() >> rhs) && std::is_nothrow_constructible_v<T<U>, U>) { return T<U>(lhs.count() >> rhs); }
template <template<typename> class T, typename U, typename V>
constexpr auto operator^(const Unit<T, U>& lhs, const V& rhs)    noexcept(noexcept(lhs.count() ^  rhs) && std::is_nothrow_constructible_v<T<U>, U>) { return T<U>(lhs.count() ^ rhs); }
template <template<typename> class T, typename U, typename V>
constexpr auto operator|(const Unit<T, U>& lhs, const V& rhs)    noexcept(noexcept(lhs.count() |  rhs) && std::is_nothrow_constructible_v<T<U>, U>) { return T<U>(lhs.count() | rhs); }
template <template<typename> class T, typename U, typename V>
constexpr auto operator&(const Unit<T, U>& lhs, const V& rhs)    noexcept(noexcept(lhs.count() &  rhs) && std::is_nothrow_constructible_v<T<U>, U>) { return T<U>(lhs.count() & rhs); }

template <template<typename> class T, typename U>
void operator <<(std::ostream& out, const Unit<T, U>& unit) { out << std::string{*static_cast<const T<U>*>(&unit)}; }