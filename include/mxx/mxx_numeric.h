#pragma once
#include <exception>
#include <functional>

namespace mxx {

template <typename InputIt, typename T = long>
atom reduce(InputIt first, InputIt last)
{
    return std::reduce(
        first, last, 0, [](const auto& lhs, const auto& rhs) { return static_cast<T>(lhs) + static_cast<T>(rhs); });
}

template <typename T>
inline std::size_t safe_size_t(const T& value)
{
    if (value > -1)
        return static_cast<std::size_t>(value);
    else
        throw std::range_error("Could not convert signed to unsigend size type");
}
}    // namespace mxx
