#include "lmw.h"

using namespace lmw;

void func(const symbol& sym)
{
    c74::max::object_post(nullptr, sym);
}

void c74func(c74::max::t_object* me, c74::max::t_atom* argv, long argc)
{
    auto args = lmw::detail::to_atom_vector(argv, argc);
}

int ext_main()
{
}
