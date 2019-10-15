#pragma once

#include <c74_max.h>

namespace lmw {

    class object_base {
        
    private:
        c74::max::t_object* t_obj_instance_ptr;
        
        template<typename user_class>
        friend bool object_new_impl(c74::max::t_symbol* name, long ac,
                                         c74::max::t_atom* av, c74::max::t_class* c);
    };

    template <typename user_class>
    class object {
        
    private:
    };
}
