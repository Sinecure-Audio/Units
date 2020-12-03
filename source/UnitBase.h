#pragma once

#include <type_traits>
#include <string>

template <template<typename> class UnitTemplate, typename NumericType>
class Unit
{
public:
    constexpr Unit() = default;

	constexpr Unit(const NumericType& initialValue) noexcept(noexcept(std::is_nothrow_constructible_v<NumericType>)) : value(initialValue) {}

	constexpr auto operator-()  const noexcept { return UnitTemplate<NumericType>(value * NumericType{-1}); }

	constexpr NumericType count() const noexcept {return value;}
//    constexpr NumericType& count() noexcept {return value;}
//    constexpr const NumericType& count() const noexcept {return value;}

	template<typename T>
	constexpr auto operator+= (const UnitTemplate<T>& other) noexcept(noexcept(std::declval<NumericType>() + std::declval<NumericType>()) && std::is_nothrow_assignable_v<NumericType, T>) { value += other.count(); return *this; }
	template<typename T>
	constexpr auto operator-= (const UnitTemplate<T>& other) noexcept(noexcept(std::declval<NumericType>() - std::declval<NumericType>()) && std::is_nothrow_assignable_v<NumericType, T>) { value -= other.count(); return *this; }

	// operators * and / work with normal types and not units to avoid having to deal with multiple dimensions of units.
	template<typename T>
	constexpr auto operator*= (const T& other)           noexcept(noexcept(std::declval<NumericType>() * std::declval<T>()) && std::is_nothrow_assignable_v<NumericType, T>) { value *= other; return *this; }
	template<typename T>
	constexpr auto operator/= (const T& other) 			 noexcept(noexcept(std::declval<NumericType>() / std::declval<T>()) && std::is_nothrow_assignable_v<NumericType, T>) { value /= other; return *this; }
	template<typename T>
	constexpr auto operator%= (const UnitTemplate<T>& other) {
		if constexpr (std::is_integral_v<NumericType> && std::is_integral_v<T>)
			value %= other.count();
		else
			value = static_cast<NumericType>(fmod(value, other.count()));
		return *this;
	}

	template<typename T>
	constexpr auto operator^= (const T& other) noexcept(noexcept(std::declval<NumericType>() ^ std::declval<T>()) && std::is_nothrow_assignable_v<NumericType, T>) { value ^= other; return *this; }

	template<typename T>
	constexpr auto operator&= (const T& other) noexcept(noexcept(std::declval<NumericType>() & std::declval<T>()) && std::is_nothrow_assignable_v<NumericType, T>) { value &= other; return *this; }

	template<typename T>
	constexpr auto operator|= (const T other)  noexcept(noexcept(std::declval<NumericType>() | std::declval<T>()) && std::is_nothrow_assignable_v<NumericType, T>) { value |= other; return *this; }

	constexpr auto operator++(int) {
		const auto out{UnitTemplate<NumericType>{value}};
		value++;
		return out;
	}

	constexpr auto operator--(int) {
		auto out{UnitTemplate<NumericType>(value)};
		value--;
		return out;
	}

	constexpr auto& operator++() {
		++value;
		return *static_cast<UnitTemplate<NumericType>*>(this);
	}

	constexpr auto& operator--() 	{
		--value;
		return *static_cast<UnitTemplate<NumericType>*>(this);
	}


	constexpr auto operator~ () noexcept(noexcept(~std::declval<NumericType>())) { return UnitTemplate<NumericType>(~value); }

	operator std::string() const { return std::to_string(value); }

protected:
	NumericType value{ 0 };
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
template <template<typename> class T, typename U>
constexpr auto operator*(const U& lhs, const Unit<T, U>& rhs) noexcept(noexcept(rhs * lhs)) { return rhs*lhs; }
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
void operator <<(std::ostream& out, const Unit<T, U>& unit) { out << std::string(*static_cast<const T<U>*>(&unit)); }
