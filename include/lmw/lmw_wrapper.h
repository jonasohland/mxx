#pragma once

#include "lmw_wrapper_preprocessor.h"
#include "lmw_type_traits.h"
#include <c74_max.h>
#include <c74_msp.h>

namespace lmw {

    template <typename user_class>
    struct object_wrapper {

        union object_header_type {
            c74::max::t_object maxobj_header;
            c74::max::t_pxobject mspobj_header;
        };

        operator c74::max::t_object*()
        {
            return &object_header.maxobj_header;
        }

        bool lmw_internal_init(t_atom_span args, long attr_offset)
        {
            // clang-format off

            if constexpr(type_traits::has_construct_function<user_class>()){
                try {
                    object.construct(atom_vector(
                        args.begin() + attr_offset, args.end()));
                }
                catch (std::runtime_error ex) {
                    return false;
                }
            }
            
            object.lmw_internal_finalize();

            // clang-format on

            return true;
        }

        object_header_type object_header;
        user_class object;
    };

    template <typename user_class>
    c74::max::t_class* wrapper_class_new(
        c74::max::t_class* class_ptr, c74::max::method new_instance,
        c74::max::method free_instance, c74::max::method named_meth_wrapper,
        c74::max::method inth, c74::max::method bangh, c74::max::method floath,
        c74::max::method dsp64h)
    {
        class_ptr = c74::max::class_new(
            "testexternal", new_instance, free_instance, sizeof(object_wrapper<user_class>), 0L, c74::max::A_GIMME, 0);

        // clang-format off
        
        if constexpr(type_traits::has_bang_handler<user_class>()){
            c74::max::class_addmethod(class_ptr, bangh, "bang");
        }
        
        if constexpr(type_traits::has_int_handler<user_class>()){
            c74::max::class_addmethod(class_ptr, inth, "int");
        }
        
        if constexpr(type_traits::has_float_handler<user_class>()){
            c74::max::class_addmethod(class_ptr, floath, "float");
        }
        
        if constexpr(type_traits::has_dsp_handler<user_class>()){
            c74::max::class_addmethod(class_ptr, dsp64h, "dsp64");
        }
        
        // clang-format on
        
        user_class dummy;

        for (const auto& msg : dummy.messages()) {
            
            auto[name, handler] = msg;
            
            c74::max::class_addmethod(
                class_ptr, named_meth_wrapper, name.c_str(), handler->type(), 0);
        }

        return class_ptr;
    }

    template <typename user_class>
    void* wrapper_object_new(c74::max::t_class* class_ptr,
                             c74::max::t_symbol* name, long ac,
                             c74::max::t_atom* av)
    {
        static_assert(std::is_constructible<user_class>(),
                      "External class must be constructible without arguments");
    

        auto* obj = c74::max::object_alloc(class_ptr);
        auto* wrapper = reinterpret_cast<object_wrapper<user_class>*>(obj);

        try {
            new (&wrapper->object) user_class();
        }
        catch (std::exception ex) {
            return nullptr;
        };
        
        wrapper->object.lmw_internal_prepare(static_cast<c74::max::t_object*>(obj));
        
        static symbol tsym("__lmw_test_symbol__");
        
        if (static_cast<c74::max::t_symbol*>(tsym))
            wrapper->lmw_internal_init(detail::to_span(av, ac), c74::max::attr_args_offset(ac, av));

        return obj;
    }

    template <typename user_class>
    void wrapper_object_free(c74::max::t_object* instance)
    {
        auto* wrapper = reinterpret_cast<object_wrapper<user_class>*>(instance);

        wrapper->object.~user_class();
    }

    template <typename user_class>
    void wrapper_msg_call(c74::max::t_object* o, c74::max::t_symbol* s, long ac,
                          c74::max::t_atom* av)
    {
        auto args = t_atom_span(av, ac);
        
        reinterpret_cast<object_wrapper<user_class>*>(o)->object.call(
            s->s_name, std::make_shared<atom_vector>(args.begin(), args.end()));
    }

} // namespace lmw
