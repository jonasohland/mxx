#pragma once

#include <c74_max.h>

namespace lmw {

    class atom;

    template <typename T>
    class span;

    class outlet;
    class inlet;

    using outlet_ptr = std::shared_ptr<outlet>;
    using inlet_ptr = std::shared_ptr<inlet>;

    using atom_vector = std::vector<atom>;

    template <size_t Size>
    using atom_array = std::array<atom, Size>;

    using t_atom_span = span<c74::max::t_atom>;
    using atom_span = span<atom>;

    using method = std::function<atom_vector(const atom_vector&, long)>;

    using non_owning_method = std::function<atom_vector(atom_span, long)>;

    using max_method = std::function<atom_vector(t_atom_span, long)>;
}
