#pragma once

#include <c74_max.h>

namespace lmw {

    class outlet;
    class inlet;

    using outlet_ptr = std::shared_ptr<outlet>;
    using inlet_ptr = std::shared_ptr<inlet>;

    using t_atom_span = span<c74::max::t_atom>;
    using atom_span = span<atom>;

    using method = std::function<atom::vector(const atom::vector&, long)>;

    using non_owning_method = std::function<atom::vector(atom_span, long)>;

    using max_method = std::function<atom::vector(t_atom_span, long)>;

    namespace detail {

        inline t_atom_span to_span(c74::max::t_atom* argv, long argc) noexcept
        {
            return {argv, static_cast<t_atom_span::index_type>(argc)};
        }

        inline atom::vector to_atom_vector(c74::max::t_atom* argv, long argc) noexcept
        {
            auto spn = to_span(argv, argc);

            return atom::vector(spn.begin(), spn.end());
        }
    }
}
