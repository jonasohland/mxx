#pragma once

#include "lmw_wrapper.h"

namespace lmw {

    namespace type_traits {
        LMW_CREATE_BASIC_MEMBER_CHECK(has_bang_handler, handle_bang);
        LMW_CREATE_BASIC_MEMBER_CHECK(has_int_handler, handle_int);
        LMW_CREATE_BASIC_MEMBER_CHECK(has_float_handler, handle_float);
        LMW_CREATE_BASIC_MEMBER_CHECK(has_list_handler, handle_list);
        LMW_CREATE_BASIC_MEMBER_CHECK(has_dsp_handler, process);
    }

    template <typename user_class>
    void wrapper_handle_bang_impl(c74::max::t_object* obj)
    {
        if constexpr(type_traits::has_bang_handler<user_class>::value) {
            reinterpret_cast<object_wrapper<user_class>*>(obj)
                ->object.handle_bang();
        }
    }

    template <typename user_class>
    void wrapper_handle_int_impl(c74::max::t_object* obj, long n)
    {
        if constexpr(type_traits::has_int_handler<user_class>::value) {
            reinterpret_cast<object_wrapper<user_class>*>(obj)
                ->object.handle_int(n);
        }
    }

    void wrapper_dsp64_setup_impl(c74::max::t_object* x)
    {
        
    }

    template <typename user_class>
    void wrapper_dsp64_perform_impl(c74::max::t_object* x,
                                    c74::max::t_object* dsp64, double** ins,
                                    long numins, double** outs, long numouts,
                                    long sampleframes, long flags,
                                    void* userparam)
    {
        if constexpr(type_traits::has_dsp_handler<user_class>::value)
        {
            reinterpret_cast<object_wrapper<user_class>>(x)->object.process(
                ins, outs, numins, numouts);
        }
    }
}

