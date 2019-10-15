#pragma once

#include "lmw_wrapper_preprocessor.h"
#include <c74_max.h>

namespace lmw {

    template <typename user_class>
    struct object_wrapper {

        c74::max::t_object object_header;
        user_class object;

        operator c74::max::t_object*()
        {
            return &object_header;
        }

        bool init(t_atom_span args)
        {
        }
    };

    template<typename user_class>
    void* wrapper_object_new()
    {
        return nullptr;
    }

    template <typename user_class>
    bool object_new_impl(c74::max::t_symbol* name, long ac,
                         c74::max::t_atom* av, c74::max::t_class* c)
    {
        auto args = detail::to_span(av, ac);

        auto* mobj =
            static_cast<object_wrapper<user_class>*>(c74::max::object_alloc(c));

        mobj->user_class.t_obj_instance_ptr =
            reinterpret_cast<c74::max::t_object*>(mobj);

        if (!mobj->init(args)) return false;

        return true;
    }

    
    template<typename user_class>
    c74::max::t_class* wrap_as_maxclass()
    {
        return c74::max::class_new("test", nullptr, nullptr, 0, nullptr, 0);
    }

    template<typename user_class>
    void finish_class_wrap(c74::max::t_class* c)
    {
        
    }

} // namespace lmw
