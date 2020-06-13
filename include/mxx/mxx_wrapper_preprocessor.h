/** @file */
#pragma once

#define mxx_external_main ext_main

/* -------------------------------------------------------------------------- */
/*               General purpose preprocessor utilities                       */
/* -------------------------------------------------------------------------- */

#define MXX_FORCE_EXPAND(x) x

#define MXX_PASTE(a, b)  a##b
#define MXX_XPASTE(a, b) MXX_PASTE(a, b)

/// helper for MXX_PREPROCESSOR_CAT
#define MXX_PREPROCESSOR_CAT_IMPL(lhs, rhs) lhs##rhs

/** concat 2 tokens
 @code
 MXX_PREPROCESSOR_CAT(Hello, World) // will expand to: HelloWorld
 @endcode
 */
#define MXX_PREPROCESSOR_CAT(lhs, rhs) MXX_PREPROCESSOR_CAT_IMPL(lhs, rhs)

/// helper for MXX_PREPROCESSOR_CAT_3
#define MXX_PREPROCESSOR_CAT_3_IMPL(one, two, three) one##two##three

/** concat 2 tokens
@code
MXX_PREPROCESSOR_CAT_3(Hello, World, Blub) // will expand to: HelloWorldBlub
@endcode
*/
#define MXX_PREPROCESSOR_CAT_3(one, two, three)                                \
    MXX_PREPROCESSOR_CAT_3_IMPL(one, two, three)

/**
 Generate a type trait to check for the existence of a member function on a
 class
 @code
 MXX_CREATE_BASIC_MEMBER_CHECK(has_member_function_serialize, serialize);

 struct myclass {
    void serialize();
 };

 static_assert(has_member_function_serialize<myclass>::value, "error!");
 @endcode
 */
#define MXX_CREATE_BASIC_MEMBER_CHECK(sname, fname)                            \
    template <class T>                                                         \
    struct sname##_impl {                                                      \
        template <typename C>                                                  \
        static constexpr decltype(std::declval<C>().fname(), bool()) test(int) \
        {                                                                      \
            return true;                                                       \
        }                                                                      \
                                                                               \
        template <typename C>                                                  \
        static constexpr bool test(...)                                        \
        {                                                                      \
            return false;                                                      \
        }                                                                      \
                                                                               \
        static constexpr bool value = test<T>(int());                          \
    };                                                                         \
    template <typename user_class>                                             \
    std::integral_constant<bool, sname##_impl<user_class>::value> sname;

#define MXX_CREATE_ADVANCED_MEMBER_CHECK(sname, fname, ...)                    \
    template <class T>                                                         \
    struct sname##_impl {                                                      \
        template <typename C>                                                  \
        static constexpr decltype(std::declval<C>().fname(__VA_ARGS__),        \
                                  bool())                                      \
        test(int)                                                              \
        {                                                                      \
            return true;                                                       \
        }                                                                      \
                                                                               \
        template <typename C>                                                  \
        static constexpr bool test(...)                                        \
        {                                                                      \
            return false;                                                      \
        }                                                                      \
                                                                               \
        static constexpr bool value = test<T>(int());                          \
    };                                                                         \
    template <typename user_class>                                             \
    std::integral_constant<bool, sname##_impl<user_class>::value> sname;

#define MMX_CREATE_MEMBER_VARIABLE_CHECK(sname, mname)                         \
    template <typename T, typename = void>                                     \
    struct sname##_impl: std::false_type {                                     \
    };                                                                         \
                                                                               \
    template <typename T>                                                      \
    struct sname##_impl<T, decltype((void) T::mname, void())>                  \
        : std::true_type {                                                     \
    };                                                                         \
    template <typename user_class>                                             \
    std::integral_constant<bool, sname##_impl<user_class>::value> sname;
/* -------------------------------------------------------------------------- */
/*                      PRINT WARNINGS AT COMPILE TIME                        */
/*                              taken from:                                   */
/*    stackoverflow.com/questions/8936063/does-there-exist-a-static-warning   */
/* -------------------------------------------------------------------------- */

#if defined(__GNUC__) || defined(__clang__)
#    define MXX_DEPRECATE(foo, msg) foo __attribute__((deprecated(msg)))
#elif defined(_MSC_VER)
#    define MXX_DEPRECATE(foo, msg) __declspec(deprecated(msg)) foo
#else
#    error This compiler is not supported
#endif

#define MXX_PP_CAT(x, y)  MXX_PP_CAT1(x, y)
#define MXX_PP_CAT1(x, y) x##y

namespace mxx::detail {
    struct true_type {
    };
    struct false_type {
    };
    template <int test>
    struct converter: public true_type {
    };
    template <>
    struct converter<0>: public false_type {
    };
}    // namespace mxx::detail

#define MXX_STATIC_WARNING_IMPL(cond, msg, counter)                            \
    struct MXX_PP_CAT(static_warning, counter) {                               \
        MXX_DEPRECATE(void _(::mxx::detail::false_type const&), msg) {};       \
        void _(::mxx::detail::true_type const&) {};                            \
        MXX_PP_CAT(static_warning, counter)()                                  \
        {                                                                      \
            _(::mxx::detail::converter<(cond)>());                             \
        }                                                                      \
    }

#define MXX_STATIC_WARNING(cond, msg)                                          \
    MXX_STATIC_WARNING_IMPL(cond, msg, __COUNTER__)

#ifdef MXX_ENABLE_CTTI_DEBUG

#    define MXX_DEBUG_MSG_PREFIX "MXX DEBUG: "

#    define MXX_CTTI_DEBUG_SECTION(user_class)                                 \
        MXX_STATIC_WARNING(!mxx::type_traits::has_bang_handler<user_class>(),  \
                           MXX_DEBUG_MSG_PREFIX "bang handler enabled");       \
        MXX_STATIC_WARNING(                                                    \
            !mxx::type_traits::has_construct_function<user_class>(),           \
            MXX_DEBUG_MSG_PREFIX "construct function enabled");                \
        MXX_STATIC_WARNING(!mxx::type_traits::has_int_handler<user_class>(),   \
                           MXX_DEBUG_MSG_PREFIX "int handler enabled");        \
        MXX_STATIC_WARNING(!mxx::type_traits::has_float_handler<user_class>(), \
                           MXX_DEBUG_MSG_PREFIX "float handler enabled");      \
        MXX_STATIC_WARNING(!mxx::type_traits::has_list_handler<user_class>(),  \
                           MXX_DEBUG_MSG_PREFIX "list handler enabled");       \
        MXX_STATIC_WARNING(                                                    \
            !mxx::type_traits::has_any_msg_handler<user_class>(),              \
            MXX_DEBUG_MSG_PREFIX "any message handler enabled");               \
        MXX_STATIC_WARNING(                                                    \
            !mxx::type_traits::has_raw_list_handler<user_class>(),             \
            MXX_DEBUG_MSG_PREFIX "raw list handler enabled");                  \
        MXX_STATIC_WARNING(!mxx::type_traits::has_dsp_handler<user_class>(),   \
                           MXX_DEBUG_MSG_PREFIX                                \
                           "DSP perform routine enabled");                     \
        MXX_STATIC_WARNING(                                                    \
            !mxx::type_traits::has_input_changed_function<user_class>(),       \
            MXX_DEBUG_MSG_PREFIX "inputchanged handler enabled");              \
        MXX_STATIC_WARNING(                                                    \
            !mxx::type_traits::has_dblclick_handler<user_class>(),             \
            MXX_DEBUG_MSG_PREFIX "dblclick handler enabled");                  \
        MXX_STATIC_WARNING(                                                    \
            !mxx::type_traits::has_setup_dsp_function<user_class>(),           \
            MXX_DEBUG_MSG_PREFIX "setup_dsp function enabled");
#else

#    define MXX_CTTI_DEBUG_SECTION(user_class) ;

#endif

/* -------------------------------------------------------------------------- */
/*                         NAME DECORATORS AND STUFF                          */
/* -------------------------------------------------------------------------- */

#define MXX_MAX_METHOD(methodname)                                             \
    reinterpret_cast<c74::max::method>(&methodname)

#define MXX_DECLARE_PERFROUTINE(name)                                          \
    void name(c74::max::t_object*, c74::max::t_object*, double**, long,        \
              double**, long, long, long, void*);

/* -------------------------------------------------------------------------- */
/*               MESSAGE HANDLER FUNCTION NAME GENERATORS                     */
/*                                                                            */
/*  These macros generate function names for common methods that are passed   */
/*  to the c74::max::class_addmethod() calls. These are used to automatically */
/*  generate handler functions that are exposed to max as public symbols.     */
/* -------------------------------------------------------------------------- */

#define MXX_WRAPPER_FUNCTION_IMPL(method, prefix)                              \
    MXX_PREPROCESSOR_CAT(prefix, method)

#define MXX_WRAPPER_FUNCTION(method, classname)                                \
    MXX_WRAPPER_FUNCTION_IMPL(                                                 \
        method, MXX_PREPROCESSOR_CAT(mxx_ext_wrap_, classname))

#define MXX_WRAPPER_FUNCTION_DSP64_METHOD(identifier)                          \
    MXX_WRAPPER_FUNCTION(_dsp64_method, identifier)

#define MXX_WRAPPER_FUNCTION_DSP64_USER_METHOD(identifier)                     \
    MXX_WRAPPER_FUNCTION(_dsp64_user_method, identifier)

#define MXX_WRAPPER_FUNCTION_DSP64_PERFORM(identifier)                         \
    MXX_WRAPPER_FUNCTION(_dsp64_perform, identifier)

#define MXX_WRAPPER_FUNCTION_ASSIST(identifier)                                \
    MXX_WRAPPER_FUNCTION(_assist, identifier)

#define MXX_WRAPPER_FUNCTION_INLETINFO(identifier)                             \
    MXX_WRAPPER_FUNCTION(_inletinfo, identifier)

#define MXX_WRAPPER_FUNCTION_DBLCLICK(identifier)                              \
    MXX_WRAPPER_FUNCTION(_dblclick, identifier)

#define MXX_WRAPPER_FUNCTION_INPUTCHANGED(identifier)                          \
    MXX_WRAPPER_FUNCTION(_inputchanged, identifier)

#define MXX_WRAPPER_FUNCTION_MULTICHANNELOUTPUTS(identifier)                   \
    MXX_WRAPPER_FUNCTION(_multichanneloutputs, identifier)

#define MXX_WRAPPER_FUNCTION_BANG(identifier)                                  \
    MXX_WRAPPER_FUNCTION(_bang, identifier)

#define MXX_WRAPPER_FUNCTION_INT(identifier)                                   \
    MXX_WRAPPER_FUNCTION(_int, identifier)

#define MXX_WRAPPER_FUNCTION_FLOAT(identifier)                                 \
    MXX_WRAPPER_FUNCTION(_float, identifier)

#define MXX_WRAPPER_FUNCTION_LIST(identifier)                                  \
    MXX_WRAPPER_FUNCTION(_list, identifier)

#define MXX_WRAPPER_FUNCTION_ANY_MSG(identifier)                               \
    MXX_WRAPPER_FUNCTION(_any, identifier)

#define MXX_WRAPPER_FUNCTION_NAMED_METHOD(identifier)                          \
    MXX_WRAPPER_FUNCTION(_named_method, identifier)

#define MXX_WRAPPER_FUNCTION_CLASS_NEW(identifier)                             \
    MXX_WRAPPER_FUNCTION(_class_new, identifier)

/* -------------------------------------------------------------------------- */
/*                   MESSAGE HANDLER FUNCTION GENERATORS                      */
/* -------------------------------------------------------------------------- */

#define MXX_CREATE_BANG_HANDLER_FUNCTION(identifier, classname)                \
    void MXX_WRAPPER_FUNCTION_BANG(identifier)(c74::max::t_object * obj)       \
    {                                                                          \
        mxx::wrapper_handle_bang_impl<classname>(obj);                         \
    }

#define MXX_CREATE_INT_HANDLER_FUNCTION(identifier, classname)                 \
    void MXX_WRAPPER_FUNCTION_INT(identifier)(                                 \
        c74::max::t_object * obj, long n)                                      \
    {                                                                          \
        mxx::wrapper_handle_int_impl<classname>(obj, n);                       \
    }

#define MXX_CREATE_FLOAT_HANDLER_FUNCTION(identifier, classname)               \
    void MXX_WRAPPER_FUNCTION_FLOAT(identifier)(                               \
        c74::max::t_object * obj, double n)                                    \
    {                                                                          \
        mxx::wrapper_handle_float_impl<classname>(obj, n);                     \
    }

#define MXX_CREATE_LIST_HANDLER_FUNCTION(identifier, classname)                \
    void MXX_WRAPPER_FUNCTION_LIST(identifier)(                                \
        c74::max::t_object * obj, c74::max::t_symbol * s, long argc,           \
        c74::max::t_atom* argv)                                                \
    {                                                                          \
        mxx::wrapper_handle_list_impl<classname>(obj, s, argc, argv);          \
    }

#define MXX_CREATE_ANY_MSG_HANDLER_FUNCTION(identifier, classname)             \
    void MXX_WRAPPER_FUNCTION_ANY_MSG(identifier)(                             \
        c74::max::t_object * obj, c74::max::t_symbol * s, long argc,           \
        c74::max::t_atom* argv)                                                \
    {                                                                          \
        mxx::wrapper_handle_any_msg_impl<classname>(obj, s, argc, argv);       \
    }

#define MXX_CREATE_ASSIST_FUNCTION(identifier, classname)                      \
    void MXX_WRAPPER_FUNCTION_ASSIST(identifier)(                              \
        c74::max::t_object * x, void* b, long io, long index, char* s)         \
    {                                                                          \
        mxx::wrapper_assist_impl<classname>(x, b, io, index, s);               \
    }

#define MXX_CREATE_INLETINFO_FUNCTION(identifier, classname)                   \
    void MXX_WRAPPER_FUNCTION_INLETINFO(identifier)(                           \
        c74::max::t_object * x, void* b, long index, char* t)                  \
    {                                                                          \
        mxx::wrapper_inletinfo_impl<classname>(x, b, index, t);                \
    }

#define MXX_CREATE_INPUTCHANGED_FUNCTION(identifier, classname)                \
    void MXX_WRAPPER_FUNCTION_INPUTCHANGED(identifier)(                        \
        c74::max::t_object * x, long index, long chans)                        \
    {                                                                          \
        mxx::wrapper_inputchanged_impl<classname>(x, index, chans);            \
    }

#define MXX_CREATE_DBLCLICK_FUNCTION(identifier, classname)                    \
    void MXX_WRAPPER_FUNCTION_DBLCLICK(identifier)(c74::max::t_object * x)     \
    {                                                                          \
        mxx::wrapper_dblclick_impl<classname>(x);                              \
    }

#define MXX_CREATE_MULTICHANNELOUTPUTS_FUNCTION(identifier, classname)         \
    long MXX_WRAPPER_FUNCTION_MULTICHANNELOUTPUTS(identifier)(                 \
        c74::max::t_object * x, long idx)                                      \
    {                                                                          \
        return mxx::wrapper_multichanneloutputs_impl<classname>(x, idx);       \
    }

#define MXX_CREATE_DSP_PERFORM_FUNCTION(identifier, classname)                 \
    void MXX_WRAPPER_FUNCTION_DSP64_PERFORM(identifier)(                       \
        c74::max::t_object * x, c74::max::t_object * dsp64, double** ins,      \
        long numins, double** outs, long numouts, long sampleframes,           \
        long flags, void* userparam)                                           \
    {                                                                          \
        mxx::wrapper_dsp64_perform_impl<classname>(                            \
            x, dsp64, ins, numins, outs, numouts, sampleframes, flags,         \
            userparam);                                                        \
    }

#define MXX_CREATE_DSP_METHOD_FUNCTION(identifier, classname)                  \
    void MXX_WRAPPER_FUNCTION_DSP64_METHOD(identifier)(                        \
        c74::max::t_object * x, c74::max::t_object * dsp64, short* count,      \
        double samplerate, long maxvectorsize, long flags)                     \
    {                                                                          \
        mxx::wrapper_dsp64_setup<classname>(                                   \
            x, dsp64, count, samplerate, maxvectorsize, flags,                 \
            MXX_WRAPPER_FUNCTION_DSP64_PERFORM(identifier));                   \
    }

#define MXX_CREATE_DSP_USER_METHOD_FUNCTION(identifier, classname)             \
    void MXX_WRAPPER_FUNCTION_DSP64_USER_METHOD(identifier)(                   \
        c74::max::t_object * x, c74::max::t_object * dsp64, short* count,      \
        double samplerate, long maxvectorsize, long flags)                     \
    {                                                                          \
        mxx::wrapper_dsp64_user_setup<classname>(                              \
            x, dsp64, count, samplerate, maxvectorsize, flags);                \
    }

#define MXX_CREATE_NAMED_METHOD_FUNCTION(identifier, classname)                \
    void MXX_WRAPPER_FUNCTION_NAMED_METHOD(identifier)(                        \
        c74::max::t_object * o, c74::max::t_symbol * s, long ac,               \
        c74::max::t_atom* av)                                                  \
    {                                                                          \
        mxx::wrapper_msg_call<classname>(o, s, ac, av);                        \
    }

#define MXX_EXT_HANDLER_FUNCTIONS(identifier, classname)                       \
    MXX_CREATE_BANG_HANDLER_FUNCTION(identifier, classname)                    \
    MXX_CREATE_INT_HANDLER_FUNCTION(identifier, classname)                     \
    MXX_CREATE_FLOAT_HANDLER_FUNCTION(identifier, classname)                   \
    MXX_CREATE_LIST_HANDLER_FUNCTION(identifier, classname)                    \
    MXX_CREATE_ANY_MSG_HANDLER_FUNCTION(identifier, classname)                 \
    MXX_CREATE_DSP_PERFORM_FUNCTION(identifier, classname)                     \
    MXX_CREATE_DSP_METHOD_FUNCTION(identifier, classname)                      \
    MXX_CREATE_ASSIST_FUNCTION(identifier, classname)                          \
    MXX_CREATE_INLETINFO_FUNCTION(identifier, classname)                       \
    MXX_CREATE_DBLCLICK_FUNCTION(identifier, classname)                        \
    MXX_CREATE_INPUTCHANGED_FUNCTION(identifier, classname)                    \
    MXX_CREATE_MULTICHANNELOUTPUTS_FUNCTION(identifier, classname)             \
    MXX_CREATE_NAMED_METHOD_FUNCTION(identifier, classname)                    \
    MXX_CREATE_DSP_USER_METHOD_FUNCTION(identifier, classname)

/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */

#define MXX_USER_CLASS_MAXCLASS_SYMBOL(ident)                                  \
    MXX_PREPROCESSOR_CAT(mxx_ext_maxclass_, ident)

#define MXX_USER_CLASS_MAXCLASS_DECL(ident)                                    \
    static c74::max::t_class* MXX_USER_CLASS_MAXCLASS_SYMBOL(ident);

#define MXX_NEW_INSTANCE_FUNCTION_NAME(ident)                                  \
    MXX_PREPROCESSOR_CAT(mxx_ext_newinstance_impl_, ident)

#define MXX_NEW_INSTANCE_FUNCTION_DECL(ident, name, ac, av)                    \
    void* MXX_NEW_INSTANCE_FUNCTION_NAME(ident)(                               \
        c74::max::t_symbol * name, long ac, c74::max::t_atom* av)

#define MXX_NEW_INSTANCE_FUNCTION(ident, user_class)                           \
    MXX_NEW_INSTANCE_FUNCTION_DECL(ident, ext_name, ext_ac, ext_av)            \
    {                                                                          \
        return mxx::wrapper_object_new<user_class>(                            \
            MXX_USER_CLASS_MAXCLASS_SYMBOL(ident), ext_name, ext_ac, ext_av);  \
    }

#define MXX_FREE_INSTANCE_FUNCTION_NAME(ident)                                 \
    MXX_PREPROCESSOR_CAT(mxx_ext_freeinstance_impl_, ident)

#define MXX_FREE_INSTANCE_FUNCTION_DECL(ident, obj)                            \
    void MXX_FREE_INSTANCE_FUNCTION_NAME(ident)(c74::max::t_object * obj)

#define MXX_FREE_INSTANCE_FUNCTION(ident, user_class)                          \
    MXX_FREE_INSTANCE_FUNCTION_DECL(ident, ext_obj)                            \
    {                                                                          \
        mxx::wrapper_object_free<user_class>(ext_obj);                         \
    }

#define MXX_MAXCLASS_DEF_IMPL(ident, class, extname)                           \
                                                                               \
    MXX_USER_CLASS_MAXCLASS_SYMBOL(ident) = mxx::wrapper_class_new<class>(     \
        MXX_USER_CLASS_MAXCLASS_SYMBOL(ident), extname,                        \
        MXX_MAX_METHOD(MXX_NEW_INSTANCE_FUNCTION_NAME(ident)),                 \
        MXX_MAX_METHOD(MXX_FREE_INSTANCE_FUNCTION_NAME(ident)),                \
        MXX_MAX_METHOD(MXX_WRAPPER_FUNCTION_NAMED_METHOD(ident)),              \
        MXX_MAX_METHOD(MXX_WRAPPER_FUNCTION_INT(ident)),                       \
        MXX_MAX_METHOD(MXX_WRAPPER_FUNCTION_BANG(ident)),                      \
        MXX_MAX_METHOD(MXX_WRAPPER_FUNCTION_FLOAT(ident)),                     \
        MXX_MAX_METHOD(MXX_WRAPPER_FUNCTION_LIST(ident)),                      \
        MXX_MAX_METHOD(MXX_WRAPPER_FUNCTION_ANY_MSG(ident)),                   \
        MXX_MAX_METHOD(MXX_WRAPPER_FUNCTION_ASSIST(ident)),                    \
        MXX_MAX_METHOD(MXX_WRAPPER_FUNCTION_INLETINFO(ident)),                 \
        MXX_MAX_METHOD(MXX_WRAPPER_FUNCTION_DBLCLICK(ident)),                  \
        MXX_MAX_METHOD(MXX_WRAPPER_FUNCTION_INPUTCHANGED(ident)),              \
        MXX_MAX_METHOD(MXX_WRAPPER_FUNCTION_MULTICHANNELOUTPUTS(ident)),       \
        MXX_MAX_METHOD(MXX_WRAPPER_FUNCTION_DSP64_METHOD(ident)),              \
        MXX_MAX_METHOD(MXX_WRAPPER_FUNCTION_DSP64_USER_METHOD(ident)));        \
                                                                               \
    c74::max::class_register(                                                  \
        c74::max::CLASS_BOX, MXX_USER_CLASS_MAXCLASS_SYMBOL(ident));

/* -------------------------------------------------------------------------- */
//                            THE MAGIC WRAPPER
/* -------------------------------------------------------------------------- */

#define MXX_EXTERNAL(class, identifier, extname)                               \
    MXX_USER_CLASS_MAXCLASS_DECL(identifier)                                   \
    MXX_NEW_INSTANCE_FUNCTION(identifier, class)                               \
    MXX_FREE_INSTANCE_FUNCTION(identifier, class)                              \
    MXX_EXT_HANDLER_FUNCTIONS(identifier, class)                               \
    MXX_CTTI_DEBUG_SECTION(class)                                              \
    void mxx_external_main(void*)                                              \
    {                                                                          \
        MXX_MAXCLASS_DEF_IMPL(identifier, class, extname)                      \
    }
