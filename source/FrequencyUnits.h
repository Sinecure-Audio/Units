#pragma once

#include "UnitBase.h"

template<typename T>
struct AnalogFrequency;

template<typename T>
struct DigitalFrequency : Unit<DigitalFrequency, T>
{
    constexpr DigitalFrequency() = default;

    explicit constexpr DigitalFrequency(const T& initValue)
        : Unit<DigitalFrequency, T>(initValue) {}

    template<typename U>
    constexpr DigitalFrequency(const DigitalFrequency<U>& initValue)
        : Unit<DigitalFrequency, T>(static_cast<T>(initValue)) {}

    constexpr DigitalFrequency(const AnalogFrequency<T>& initValue,
                               const T& sampleRate)
        : Unit<DigitalFrequency, T>(warpAnalogFrequency(initValue.count(), sampleRate)) {}

    constexpr explicit operator T() const noexcept {
        return this->count();
    }

    static constexpr auto warpAnalogFrequency(const T& frequency,
                                              const T& sampleRate) {

        const auto t = T{1}/sampleRate;
        return DigitalFrequency{T{2}*atan(t*frequency/T{2})/t};
    }
};

template<typename T>
struct AnalogFrequency : Unit<AnalogFrequency, T>
{
    constexpr AnalogFrequency() = default;

    template<typename U, typename V>
    constexpr AnalogFrequency(DigitalFrequency<U> initValue,
                              const V& sampleRate)
            : Unit<AnalogFrequency, T>(unwarpDigitalFrequency(initValue.count(), sampleRate).count()) {}

    constexpr AnalogFrequency(const T& initValue)
        : Unit<AnalogFrequency, T>(initValue) {}

    template<typename U>
    constexpr AnalogFrequency(const AnalogFrequency<U>& initValue)
        : Unit<AnalogFrequency, T>(static_cast<T>(initValue)) {}

    constexpr operator T() const noexcept {
        return this->count();
    }

    static constexpr auto unwarpDigitalFrequency(const T& frequency,
                                                 const T& sampleRate) {

        const auto t = T{1}/sampleRate;
        return AnalogFrequency{T{2}*tan(t*frequency/T{2})/t};
    }
};

template<typename... Ts>
using Frequency = AnalogFrequency<Ts...>;

template<typename T>
AnalogFrequency(DigitalFrequency<T> a, T b) -> AnalogFrequency<T>;

template<typename T>
AnalogFrequency(T a, T b) -> AnalogFrequency<T>;

template<typename T>
DigitalFrequency(T b) -> DigitalFrequency<T>;

template<typename T>
DigitalFrequency(AnalogFrequency<T> a, T b) -> DigitalFrequency<T>;

template<typename T>
DigitalFrequency(T a, T b) -> DigitalFrequency<T>;