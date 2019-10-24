#pragma once

#include "lmw_wrapper.h"

namespace lmw {

    template <typename user_class>
    void wrapper_handle_bang_impl(c74::max::t_object* obj)
    {
        if constexpr(type_traits::has_bang_handler<user_class>())
        {
            reinterpret_cast<object_wrapper<user_class>*>(obj)
                ->object.handle_bang();
        }
#ifdef LMW_REQUIRE_BANG_HANDLER
        static_assert(
            type_traits::has_bang_handler<user_class>(),
            "Missing required bang_handler function on external class");
#endif
    }

    template <typename user_class>
    void wrapper_handle_int_impl(c74::max::t_object* obj, long n)
    {
        if constexpr(type_traits::has_int_handler<user_class>())
        {
            reinterpret_cast<object_wrapper<user_class>*>(obj)
                ->object.handle_int(n);
        }
#ifdef LMW_REQUIRE_INT_HANDLER
        static_assert(lmw::type_traits::has_int_handler<user_class>(),
                      "Missing required int_handler function on external class");
#endif
    }

    template <typename user_class>
    void wrapper_dsp64_perform_impl(c74::max::t_object* x,
                                    c74::max::t_object* dsp64, double** ins,
                                    long numins, double** outs, long numouts,
                                    long sampleframes, long flags,
                                    void* userparam)
    {
        if constexpr(lmw::type_traits::has_dsp_handler<user_class>())
        {
            reinterpret_cast<object_wrapper<user_class>*>(x)
                ->object.process(ins, outs, numins, numouts);
        }

#ifdef LMW_REQUIRE_PROCESS_FUNCTION
        static_assert(type_traits::has_dsp_handler<user_class>(),
                      "Missing required perform function on external class");
#endif
    }
}

