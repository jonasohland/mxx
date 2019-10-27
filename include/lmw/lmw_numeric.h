#pragma once

namespace lmw {
    template <typename Operator>
    struct add : public std::binary_function<lmw::atom, lmw::atom, lmw::atom> {

        using internal_operation_type = typename Operator::first_argument_type;

        lmw::atom operator()(const lmw::atom& lhs, const lmw::atom& rhs)
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
}
