#pragma once

#define lmw_external_main ext_main

/* -------------------------------------------------------------------------- */
/*               General purpose preprocessor utilities                       */
/* -------------------------------------------------------------------------- */

#define LMW_PASTE(a, b) a##b
#define LMW_XPASTE(a, b) LMW_PASTE(a, b)

/*
 * The PP_NARG macro evaluates to the number of arguments that have been
 * passed to it.
 *
 * Laurent Deniau, "__VA_NARG__," 17 January 2006, <comp.std.c> (29 November
 * 2007).
 */
#define LMW_PP_NARG(...) LMW_PP_NARG_(__VA_ARGS__, LMW_PP_RSEQ_N())
#define LMW_PP_NARG_(...) LMW_PP_ARG_N(__VA_ARGS__)

#define LMW_PP_ARG_N(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13,   \
                     _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24,    \
                     _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35,    \
                     _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46,    \
                     _47, _48, _49, _50, _51, _52, _53, _54, _55, _56, _57,    \
                     _58, _59, _60, _61, _62, _63, N, ...)                     \
    N

#define LMW_PP_RSEQ_N()                                                        \
    63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46,    \
        45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29,    \
        28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12,    \
        11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0

/* APPLYXn variadic X-Macro by M Joshua Ryan      */
/* Free for all uses. Don't be a jerk.            */
/* I got bored after typing 15 of these.          */
/* You could keep going upto 64 (PPNARG's limit). */
#define LMW_APPLYXDECLVAL1(a) std::declval<a>()

#define LMW_APPLYXDECLVAL2(a, b) std::declval<a>(), std::declval<b>()

#define LMW_APPLYXDECLVAL3(a, b, c)                                            \
    std::declval<a>(), std::declval<>(b), std::declval<c>()

#define LMW_APPLYXDECLVAL4(a, b, c, d)                                         \
    std::declval<a>(), std::declval<b>(), std::declval<c>(), std::declval<d>()

#define LMW_APPLYXDECLVAL5(a, b, c, d, e)                                      \
    std::declval<a>(), std::declval<b>(), std::declval<c>(),                   \
        std::declval<d>(), std::declval<e>()

#define LMW_APPLYXDECLVAL6(a, b, c, d, e, f)                                   \
    std::declval<a>(), std::declval<b>(), std::declval<c>(),                   \
        std::declval<d>(), std::declval<e>(), std::declval<f>()

#define LMW_APPLYXDECLVAL7(a, b, c, d, e, f, g)                                \
    std::declval<a>(), std::declval<b>(), std::declval<c>(),                   \
        std::declval<d>(), std::declval<e>(), std::declval<f>(),               \
        std::declval<g>()

#define LMW_APPLYXDECLVAL_(M, ...) M(__VA_ARGS__)

#define LMW_FOREACH_APPLY_DECLVAL(...)                                         \
    LMW_APPLYXDECLVAL_(                                                        \
        LMW_XPASTE(LMW_APPLYXDECLVAL, LMW_PP_NARG(__VA_ARGS__)), __VA_ARGS__)

/// helper for LMW_PREPROCESSOR_CAT
#define LMW_PREPROCESSOR_CAT_IMPL(lhs, rhs) lhs##rhs

/** concat 2 tokens
 @code
 LMW_PREPROCESSOR_CAT(Hello, World) // will expand to: HelloWorld
 @endcode
 */
#define LMW_PREPROCESSOR_CAT(lhs, rhs) LMW_PREPROCESSOR_CAT_IMPL(lhs, rhs)

/// helper for LMW_PREPROCESSOR_CAT_3
#define LMW_PREPROCESSOR_CAT_3_IMPL(one, two, three) one##two##three

/** concat 2 tokens
@code
LMW_PREPROCESSOR_CAT_3(Hello, World, Blub) // will expand to: HelloWorldBlub
@endcode
*/
#define LMW_PREPROCESSOR_CAT_3(one, two, three)                                \
    LMW_PREPROCESSOR_CAT_3_IMPL(one, two, three)

/**
 Generate a type trait to check for the existence of a member function on a
 class
 @code
 LMW_CREATE_BASIC_MEMBER_CHECK(has_member_function_serialize, serialize);

 struct myclass {
    void serialize();
 };

 static_assert(has_member_function_serialize<myclass>::value, "error!");
 @endcode
 */
#define LMW_CREATE_BASIC_MEMBER_CHECK(sname, fname)                            \
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

#define LMW_CREATE_ADVANCED_MEMBER_CHECK(sname, fname, ...)                    \
    template <class T>                                                         \
    struct sname##_impl {                                                      \
        template <typename C>                                                  \
        static constexpr decltype(                                             \
            std::declval<C>().fname(LMW_FOREACH_APPLY_DECLVAL(__VA_ARGS__)),   \
            bool())                                                            \
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

/* -------------------------------------------------------------------------- */
/*                      PRINT WARNINGS AT COMPILE TIME                        */
/*                              taken from:                                   */
/*    stackoverflow.com/questions/8936063/does-there-exist-a-static-warning   */
/* -------------------------------------------------------------------------- */


#if defined(__GNUC__) || defined(__clang__)
#define LMW_DEPRECATE(foo, msg) foo __attribute__((deprecated(msg)))
#elif defined(_MSC_VER)
#define LMW_DEPRECATE(foo, msg) __declspec(deprecated(msg)) foo
#else
#error This compiler is not supported
#endif

#define LMW_PP_CAT(x, y) LMW_PP_CAT1(x, y)
#define LMW_PP_CAT1(x, y) x##y

namespace lmw::detail {
    struct true_type {
    };
    struct false_type {
    };
    template <int test>
    struct converter : public true_type {
    };
    template <>
    struct converter<0> : public false_type {
    };
}

#define LMW_STATIC_WARNING(cond, msg)                                          \
    struct LMW_PP_CAT(static_warning, __LINE__) {                              \
        LMW_DEPRECATE(void _(::lmw::detail::false_type const&), msg){};        \
        void _(::lmw::detail::true_type const&){};                             \
        LMW_PP_CAT(static_warning, __LINE__)()                                 \
        {                                                                      \
            _(::lmw::detail::converter<(cond)>());                             \
        }                                                                      \
    }

#define LMW_STATIC_WARNING_TEMPLATE(token, cond, msg) \
    LMW_STATIC_WARNING(cond, msg) PP_CAT(PP_CAT(_localvar_, token),__LINE__)

/* -------------------------------------------------------------------------- */
/*                         NAME DECORATORS AND STUFF                          */
/* -------------------------------------------------------------------------- */

#define LMW_MAX_METHOD(methodname)                                             \
    reinterpret_cast<c74::max::method>(&methodname)

/* -------------------------------------------------------------------------- */
/*               MESSAGE HANDLER FUNCTION NAME GENERATORS                     */
/*                                                                            */
/*  These macros generate function names for common methods that are passed   */
/*  to the c74::max::class_addmethod() calls. These are used to automatically */
/*  generate handler functions that are exposed to max as public symbols.     */
/* -------------------------------------------------------------------------- */

#define LMW_WRAPPER_FUNCTION_IMPL(method, prefix)                              \
    LMW_PREPROCESSOR_CAT(prefix, method)

#define LMW_WRAPPER_FUNCTION(method, classname)                                \
    LMW_WRAPPER_FUNCTION_IMPL(                                                 \
        method, LMW_PREPROCESSOR_CAT(lmw_ext_wrap_, classname))

#define LMW_WRAPPER_FUNCTION_DSP64_METHOD(identifier)                          \
    LMW_WRAPPER_FUNCTION(_dsp64_method, identifier)

#define LMW_WRAPPER_FUNCTION_DSP64_PERFORM(identifier)                         \
    LMW_WRAPPER_FUNCTION(_dsp64_perform, identifier)

#define LMW_WRAPPER_FUNCTION_ASSIST(identifier)                                \
    LMW_WRAPPER_FUNCTION(_assist, identifier)

#define LMW_WRAPPER_FUNCTION_INLETINFO(identifier)                             \
    LMW_WRAPPER_FUNCTION(_inletinfo, identifier)

#define LMW_WRAPPER_FUNCTION_BANG(identifier)                                  \
    LMW_WRAPPER_FUNCTION(_bang, identifier)

#define LMW_WRAPPER_FUNCTION_INT(identifier)                                   \
    LMW_WRAPPER_FUNCTION(_int, identifier)

#define LMW_WRAPPER_FUNCTION_FLOAT(identifier)                                 \
    LMW_WRAPPER_FUNCTION(_float, identifier)

#define LMW_WRAPPER_FUNCTION_LIST(identifier)                                  \
    LMW_WRAPPER_FUNCTION(_list, identifier)

#define LMW_WRAPPER_FUNCTION_NAMED_METHOD(identifier)                          \
    LMW_WRAPPER_FUNCTION(_named_method, identifier)

#define LMW_WRAPPER_FUNCTION_CLASS_NEW(identifier)                             \
    LMW_WRAPPER_FUNCTION(_class_new, identifier)

/* -------------------------------------------------------------------------- */
/*                   MESSAGE HANDLER FUNCTION GENERATORS                      */
/* -------------------------------------------------------------------------- */

#define LMW_CREATE_BANG_HANDLER_FUNCTION(identifier, classname)                \
    void LMW_WRAPPER_FUNCTION_BANG(identifier)(c74::max::t_object * obj)       \
    {                                                                          \
        lmw::wrapper_handle_bang_impl<classname>(obj);                         \
    }

#define LMW_CREATE_INT_HANDLER_FUNCTION(identifier, classname)                 \
    void LMW_WRAPPER_FUNCTION_INT(identifier)(                                 \
        c74::max::t_object * obj, long n)                                      \
    {                                                                          \
        lmw::wrapper_handle_int_impl<classname>(obj, n);                       \
    }

#define LMW_CREATE_FLOAT_HANDLER_FUNCTION(identifier, classname)               \
    void LMW_WRAPPER_FUNCTION_FLOAT(identifier)(                               \
        c74::max::t_object * obj, double n)                                    \
    {                                                                          \
        lmw::wrapper_handle_float_impl<classname>(obj, n);                     \
    }

#define LMW_CREATE_LIST_HANDLER_FUNCTION(identifier, classname)                \
    void LMW_WRAPPER_FUNCTION_LIST(identifier)(                                \
        c74::max::t_object * obj, c74::max::t_symbol * s, long argc,           \
        c74::max::t_atom* argv)                                                \
    {                                                                          \
        lmw::wrapper_handle_list_impl<classname>(obj, s, argc, argv);          \
    }

#define LMW_CREATE_ASSIST_FUNCTION(identifier, classname)                      \
    void LMW_WRAPPER_FUNCTION_ASSIST(identifier)(                              \
        c74::max::t_object * x, void* b, long io, long index, char* s)         \
    {                                                                          \
        lmw::wrapper_assist_impl<classname>(x, b, io, index, s);               \
    }

#define LMW_CREATE_INLETINFO_FUNCTION(identifier, classname)                   \
    void LMW_WRAPPER_FUNCTION_INLETINFO(identifier)(                           \
        c74::max::t_object * x, void* b, long index, char* t)                  \
    {                                                                          \
        lmw::wrapper_inletinfo_impl<classname>(x, b, index, t);                \
    }

#define LMW_CREATE_DSP_PERFORM_FUNCTION(identifier, classname)                 \
    void LMW_WRAPPER_FUNCTION_DSP64_PERFORM(identifier)(                       \
        c74::max::t_object * x, c74::max::t_object * dsp64, double** ins,      \
        long numins, double** outs, long numouts, long sampleframes,           \
        long flags, void* userparam)                                           \
    {                                                                          \
        lmw::wrapper_dsp64_perform_impl<classname>(                            \
            x, dsp64, ins, numins, outs, numouts, sampleframes, flags,         \
            userparam);                                                        \
    }

#define LMW_CREATE_DSP_METHOD_FUNCTION(identifier, classname)                  \
    void LMW_WRAPPER_FUNCTION_DSP64_METHOD(identifier)(                        \
        c74::max::t_object * x, c74::max::t_object * dsp64, short* count,      \
        double samplerate, long maxvectorsize, long flags)                     \
    {                                                                          \
        lmw::wrapper_dsp64_setup<classname>(                                   \
            x, dsp64, count, samplerate, maxvectorsize, flags);                \
                                                                               \
        c74::max::object_method(                                               \
            dsp64, c74::max::gensym("dsp_add64"), x,                           \
            reinterpret_cast<void*>(                                           \
                LMW_WRAPPER_FUNCTION_DSP64_PERFORM(identifier)));              \
    }

#define LMW_CREATE_NAMED_METHOD_FUNCTION(identifier, classname)                \
    void LMW_WRAPPER_FUNCTION_NAMED_METHOD(identifier)(                        \
        c74::max::t_object * o, c74::max::t_symbol * s, long ac,               \
        c74::max::t_atom* av)                                                  \
    {                                                                          \
        lmw::wrapper_msg_call<classname>(o, s, ac, av);                        \
    }

#define LMW_EXT_HANDLER_FUNCTIONS(identifier, classname)                       \
    LMW_CREATE_BANG_HANDLER_FUNCTION(identifier, classname)                    \
    LMW_CREATE_INT_HANDLER_FUNCTION(identifier, classname)                     \
    LMW_CREATE_FLOAT_HANDLER_FUNCTION(identifier, classname)                   \
    LMW_CREATE_LIST_HANDLER_FUNCTION(identifier, classname)                    \
    LMW_CREATE_DSP_PERFORM_FUNCTION(identifier, classname)                     \
    LMW_CREATE_DSP_METHOD_FUNCTION(identifier, classname)                      \
    LMW_CREATE_ASSIST_FUNCTION(identifier, classname)                          \
    LMW_CREATE_INLETINFO_FUNCTION(identifier, classname)                       \
    LMW_CREATE_NAMED_METHOD_FUNCTION(identifier, classname)

/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */

#define LMW_USER_CLASS_MAXCLASS_SYMBOL(ident)                                  \
    LMW_PREPROCESSOR_CAT(lmw_ext_maxclass_, ident)

#define LMW_USER_CLASS_MAXCLASS_DECL(ident)                                    \
    static c74::max::t_class* LMW_USER_CLASS_MAXCLASS_SYMBOL(ident);

#define LMW_NEW_INSTANCE_FUNCTION_NAME(ident)                                  \
    LMW_PREPROCESSOR_CAT(lmw_ext_newinstance_impl_, ident)

#define LMW_NEW_INSTANCE_FUNCTION_DECL(ident, name, ac, av)                    \
    void* LMW_NEW_INSTANCE_FUNCTION_NAME(ident)(                               \
        c74::max::t_symbol * name, long ac, c74::max::t_atom* av)

#define LMW_NEW_INSTANCE_FUNCTION(ident, user_class)                           \
    LMW_NEW_INSTANCE_FUNCTION_DECL(ident, ext_name, ext_ac, ext_av)            \
    {                                                                          \
        return lmw::wrapper_object_new<user_class>(                            \
            LMW_USER_CLASS_MAXCLASS_SYMBOL(ident), ext_name, ext_ac, ext_av);  \
    }

#define LMW_FREE_INSTANCE_FUNCTION_NAME(ident)                                 \
    LMW_PREPROCESSOR_CAT(lmw_ext_freeinstance_impl_, ident)

#define LMW_FREE_INSTANCE_FUNCTION_DECL(ident, obj)                            \
    void LMW_FREE_INSTANCE_FUNCTION_NAME(ident)(c74::max::t_object * obj)

#define LMW_FREE_INSTANCE_FUNCTION(ident, user_class)                          \
    LMW_FREE_INSTANCE_FUNCTION_DECL(ident, ext_obj)                            \
    {                                                                          \
        lmw::wrapper_object_free<user_class>(ext_obj);                         \
    }

#define LMW_MAXCLASS_DEF_IMPL(ident, class)                                    \
                                                                               \
    LMW_USER_CLASS_MAXCLASS_SYMBOL(ident) = lmw::wrapper_class_new<class>(     \
        LMW_USER_CLASS_MAXCLASS_SYMBOL(ident),                                 \
        LMW_MAX_METHOD(LMW_NEW_INSTANCE_FUNCTION_NAME(ident)),                 \
        LMW_MAX_METHOD(LMW_FREE_INSTANCE_FUNCTION_NAME(ident)),                \
        LMW_MAX_METHOD(LMW_WRAPPER_FUNCTION_NAMED_METHOD(ident)),              \
        LMW_MAX_METHOD(LMW_WRAPPER_FUNCTION_INT(ident)),                       \
        LMW_MAX_METHOD(LMW_WRAPPER_FUNCTION_BANG(ident)),                      \
        LMW_MAX_METHOD(LMW_WRAPPER_FUNCTION_FLOAT(ident)),                     \
        LMW_MAX_METHOD(LMW_WRAPPER_FUNCTION_LIST(ident)),                      \
        LMW_MAX_METHOD(LMW_WRAPPER_FUNCTION_ASSIST(ident)),                    \
        LMW_MAX_METHOD(LMW_WRAPPER_FUNCTION_INLETINFO(ident)),                 \
        LMW_MAX_METHOD(LMW_WRAPPER_FUNCTION_DSP64_METHOD(ident)));             \
                                                                               \
    c74::max::class_register(                                                  \
        c74::max::CLASS_BOX, LMW_USER_CLASS_MAXCLASS_SYMBOL(ident));

/* -------------------------------------------------------------------------- */
//                            THE MAGIC WRAPPER
/* -------------------------------------------------------------------------- */

#define LMW_EXTERNAL(class)                                                    \
    LMW_USER_CLASS_MAXCLASS_DECL(default)                                      \
    LMW_NEW_INSTANCE_FUNCTION(default, class)                                  \
    LMW_FREE_INSTANCE_FUNCTION(default, class)                                 \
    LMW_EXT_HANDLER_FUNCTIONS(default, class)                                  \
    void lmw_external_main(void*)                                              \
    {                                                                          \
        LMW_MAXCLASS_DEF_IMPL(default, class)                                  \
    }
