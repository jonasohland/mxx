#pragma once
#include <exception>
#include <functional>

namespace mxx {

    template <typename Operator>
    struct add: public std::binary_function<mxx::atom, mxx::atom, mxx::atom> {

        using internal_operation_type = typename Operator::first_argument_type;

        mxx::atom operator()(const mxx::atom& lhs, const mxx::atom& rhs)
        {
            static_assert(
                std::is_same<typename Operator::first_argument_type,
                             typename Operator::second_argument_type>(),
                "nooo!");

            return Operator()(static_cast<internal_operation_type>(lhs),
                              static_cast<internal_operation_type>(rhs));
        }
    };

    template <typename InputIt, typename T = long>
    atom reduce(InputIt first, InputIt last)
    {
        return std::reduce(first, last, 0, add<std::plus<T>>());
    }

    template <typename T>
    inline std::size_t safe_size_t(const T& value)
    {
        if (value > -1)
            return static_cast<std::size_t>(value);
        else
            throw std::range_error(
                "Could not convert signed to unsigend size type");
    }
}    // namespace mxx
