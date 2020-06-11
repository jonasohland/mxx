/** @file */
#pragma once

namespace mxx {

    template <typename user_class>
    MXX_ALWAYS_INLINE decltype(auto) get_wrapper(c74::max::t_object* obj)
    {
        return reinterpret_cast<object_wrapper<user_class>*>(obj);
    }

    template <typename user_class>
    MXX_ALWAYS_INLINE void wrapper_handle_bang_impl(c74::max::t_object* obj)
    {
        if constexpr (type_traits::has_bang_handler<user_class>())
            get_wrapper<user_class>(obj)->object.handle_bang(
                c74::max::proxy_getinlet(obj));

#ifdef MXX_REQUIRE_BANG_HANDLER
        static_assert(
            type_traits::has_bang_handler<user_class>(),
            "Missing required handle_bang function on external class");
#endif
    }

    template <typename user_class>
    MXX_ALWAYS_INLINE void wrapper_handle_int_impl(c74::max::t_object* obj,
                                                   long n)
    {
        if constexpr (type_traits::has_int_handler<user_class>())
            get_wrapper<user_class>(obj)->object.handle_int(
                n, c74::max::proxy_getinlet(obj));

#ifdef MXX_REQUIRE_INT_HANDLER
        static_assert(mxx::type_traits::has_int_handler<user_class>(),
                      "Missing required handle_int function on external class");
#endif
    }

    template <typename user_class>
    MXX_ALWAYS_INLINE void wrapper_handle_float_impl(c74::max::t_object* obj,
                                                     double n)
    {
        if constexpr (type_traits::has_float_handler<user_class>())
            get_wrapper<user_class>(obj)->object.handle_float(
                n, c74::max::proxy_getinlet(obj));

#ifdef MXX_REQUIRE_FLOAT_HANDLER
        static_assert(
            mxx::type_traits::has_float_handler<user_class>(),
            "Missing required handle_float function on external class");
#endif
    }

    template <typename user_class>
    MXX_ALWAYS_INLINE void wrapper_handle_list_impl(c74::max::t_object* obj,
                                                    c74::max::t_symbol* s,
                                                    long ac,
                                                    c74::max::t_atom* av)
    {
        if constexpr (type_traits::has_list_handler<user_class>())
            get_wrapper<user_class>(obj)->object.handle_list(
                detail::to_atom_vector(av, ac), c74::max::proxy_getinlet(obj));

        if constexpr (type_traits::has_raw_list_handler<user_class>())
            get_wrapper<user_class>(obj)->object.handle_raw_list(
                detail::to_span(av, ac), c74::max::proxy_getinlet(obj));

#ifdef MXX_REQUIRE_LIST_HANDLER
        static_assert(type_traits::has_list_handler<user_class>(),
                      "Missing required handle_list(atom_vector) function on "
                      "external class");
#endif
#ifdef MXX_REQUIRE_RAW_LIST_HANDLER
        static_assert(type_traits::has_raw_list_handler<user_class>(),
                      "Missing required handle_list(t_atom_span) function on "
                      "external class");
#endif
    }

    template <typename user_class>
    MXX_ALWAYS_INLINE void wrapper_assist_impl(
        c74::max::t_object* x, void* b, long io, long index, char* s)
    {
        auto wrapper = get_wrapper<user_class>(x);

        if (io - 1)
            MXX_STRCPY(s, wrapper->object.description_for_outlet(index));
        else
            MXX_STRCPY(s, wrapper->object.description_for_inlet(index));
    }

    template <typename user_class>
    MXX_ALWAYS_INLINE void
    wrapper_inletinfo_impl(c74::max::t_object* x, void* b, long index, char* t)
    {
        auto wrapper = get_wrapper<user_class>(x);

        if (!wrapper->object.inlet_is_hot(index)) *t = 1;
    }

    template <typename user_class>
    MXX_ALWAYS_INLINE long
    wrapper_inputchanged_impl(c74::max::t_object* x, long index, long chans)
    {
        auto* wrapper = get_wrapper<user_class>(x);

        if (MXX_UNLIKELY(index >= wrapper->object.m_inlets.size())) return;

        wrapper->object.m_inlets[index]->signal_count(chans);

        if constexpr (type_traits::has_input_changed_function<user_class>())
            wrapper->object.inputchanged(index);

        return wrapper->object.mxx_internal_sigcnt_changed();
    }

    template <typename user_class>
    MXX_ALWAYS_INLINE long
    wrapper_multichanneloutputs_impl(c74::max::t_object* x, long idx)
    {
        auto* wrapper = get_wrapper<user_class>(x);

        if (MXX_UNLIKELY(idx >= wrapper->object.m_outlets.size())) return 1;

        return wrapper->object.m_outlets[idx]->signal_count();
    }

    template <typename user_class>
    MXX_ALWAYS_INLINE void wrapper_dsp64_setup(c74::max::t_object* x,
                                               c74::max::t_object* dspman,
                                               short* count,
                                               double srate,
                                               long vsize,
                                               long flags,
                                               c74::max::t_perfroutine64 r)
    {
        auto* wrapper = get_wrapper<user_class>(x);

        bool is_mc = wrapper->object.mc();

        for (long i = 0; i < wrapper->object.streams(); ++i) {
            wrapper->object.m_inlets[i]->m_connections = *(count + i);

            if (is_mc)
                wrapper->object.m_inlets[i]->signal_count(
                    reinterpret_cast<long>(c74::max::object_method(
                        dspman,
                        sym::getnuminputchannels,
                        wrapper,
                        MXX_MSVC_IGNORE_POINTER_TRUNCATION(
                            reinterpret_cast<void*>(i)))));
        }

        wrapper->object.prepare(srate, vsize);

        c74::max::dsp_add64(dspman, x, r, 0, nullptr);
    }

    template <typename user_class>
    MXX_ALWAYS_INLINE void wrapper_dsp64_perform_impl(c74::max::t_object* x,
                                                      c74::max::t_object* dsp64,
                                                      double** ins,
                                                      long numins,
                                                      double** outs,
                                                      long numouts,
                                                      long frames,
                                                      long flags,
                                                      void* userparam)
    {
        if constexpr (mxx::type_traits::has_dsp_handler<user_class>())
            get_wrapper<user_class>(x)->object.process(
                ins, outs, numins, numouts, frames);

#ifdef MXX_REQUIRE_PROCESS_FUNCTION
        static_assert(type_traits::has_dsp_handler<user_class>(),
                      "Missing required perform function on external class");
#endif
    }
}    // namespace mxx
