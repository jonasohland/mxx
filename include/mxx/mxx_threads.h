#pragma once

namespace mxx {

class default_executor {

    struct call_methods_visitor {

        void operator()(std::function<void()>& fun)
        {
            fun();
        }
        void operator()(std::function<void(const atom::vector&)>& fun)
        {
            fun(atom::vector(args.begin(), args.end()));
        }
        void operator()(std::function<void(const atom::vector&, long)>& fun)
        {
            fun(atom::vector(args.begin(), args.end()), inlet);
        }
        void operator()(std::function<atom::vector(const atom::vector&)>& fun)
        {
            fun(atom::vector(args.begin(), args.end()));
        }
        void operator()(std::function<atom::vector(const atom::vector&, long)>& fun)
        {
            fun(atom::vector(args.begin(), args.end()), inlet);
        }
        void operator()(std::function<void(atom::const_span)>& fun)
        {
            auto begin = static_cast<atom::const_span::iterator>(args.begin());
            auto end   = static_cast<atom::const_span::iterator>(args.end());

            fun(atom::const_span(begin, end));
        }

        void operator()(std::function<void(atom::const_span, long)>& fun)
        {
            auto begin = static_cast<atom::const_span::iterator>(args.begin());
            auto end   = static_cast<atom::const_span::iterator>(args.end());

            fun(atom::const_span(begin, end), inlet);
        }

        const t_atom_span& args;
        long inlet;
    };

    template <typename HandlerType, typename TypesContainer>
    struct find_handler_variant {

        using types        = TypesContainer;
        using variant_type = typename type_traits::get_variant<TypesContainer>::type;

        template <typename T>
        struct get_args;

        template <typename R, typename... Args>
        struct get_args<std::function<R(Args...)>> {
            using args_tuple = std::tuple<Args...>;
        };

        find_handler_variant(HandlerType&& handler, variant_type& variant)
        {
            try_emplace<0>(std::forward<HandlerType>(handler), variant);
        }

        template <size_t Index = 0>
        void try_emplace(HandlerType&& handler, variant_type& variant)
        {
            static_assert(Index != std::tuple_size<TypesContainer>::value, "Handler signature not supported");
            if constexpr (Index == std::tuple_size<TypesContainer>::value)
                return;
            else {
                using candidate_function = typename std::tuple_element<Index, types>::type;

                if constexpr (std::is_convertible<HandlerType, candidate_function>::value)
                    try_emplace_check_return<Index>(std::forward<HandlerType>(handler), variant);
                else
                    try_emplace<Index + 1>(std::forward<HandlerType>(handler), variant);
            }
        }

        template <typename Function1, typename Function2, typename Args>
        using has_same_return_type
            = std::is_same<decltype(std::apply(std::declval<Function1>(), std::declval<Args>())),
                           decltype(std::apply(std::declval<Function2>(), std::declval<Args>()))>;

        template <size_t Index>
        void try_emplace_check_return(HandlerType&& handler, variant_type& variant)
        {
            using candidate_function      = typename std::tuple_element<Index, types>::type;
            using candidate_function_args = typename get_args<candidate_function>::args_tuple;

            if constexpr (has_same_return_type<HandlerType, candidate_function, candidate_function_args>::value)
                variant.template emplace<Index>(std::forward<HandlerType>(handler));
            else
                try_emplace<Index + 1>(std::forward<HandlerType>(handler), variant);
        }
    };

  public:
    template <typename HandlerType>
    void set_handler(HandlerType&& handler)
    {
        find_handler_variant<HandlerType, methods> finder(std::forward<HandlerType>(handler), m_method);
    }

    void post(const t_atom_span& args, long inlet)
    {
        std::visit(call_methods_visitor { args, inlet }, m_method);
    }

    bool call_would_block() const
    {
        return false;
    }

  private:
    method_variants m_method;
};
}    // namespace mxx
