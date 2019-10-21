#pragma once

#define lmw_external_main ext_main

/* -------------------------------------------------------------------------- */
/*               General purpose preprocessor utilities                       */
/* -------------------------------------------------------------------------- */

#define LMW_PASTE(a,b) a ## b
#define LMW_XPASTE(a,b) LMW_PASTE(a,b)

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

#define LMW_FOREACH_APPLY_DECLVAL(...)                                                \
    LMW_APPLYXDECLVAL_(                                                        \
        LMW_XPASTE(LMW_APPLYXDECLVAL, LMW_PP_NARG(__VA_ARGS__)), __VA_ARGS__)

/// helper for LMW_PREPROCESSOR_CAT
#define LMW_PREPROCESSOR_CAT_IMPL(lhs, rhs) lhs##rhs

/** concat 2 tokens
 \code
 LMW_PREPROCESSOR_CAT(Hello, World) // will expand to: HelloWorld
 \endcode
 */
#define LMW_PREPROCESSOR_CAT(lhs, rhs) LMW_PREPROCESSOR_CAT_IMPL(lhs, rhs)

/// helper for LMW_PREPROCESSOR_CAT_3
#define LMW_PREPROCESSOR_CAT_3_IMPL(one, two, three) one##two##three

/** concat 2 tokens
\code
LMW_PREPROCESSOR_CAT_3(Hello, World, Blub) // will expand to: HelloWorldBlub
\endcode
*/
#define LMW_PREPROCESSOR_CAT_3(one, two, three)                                \
    LMW_PREPROCESSOR_CAT_3_IMPL(one, two, three)

/**
 Generate a type trait to check for the existence of a member function on a class
 \code
 LMW_CREATE_BASIC_MEMBER_CHECK(has_member_function_serialize, serialize);
 
 struct myclass {
    void serialize();
 };
 
 static_assert(has_member_function_serialize<myclass>::value, "error!");
 \endcode
 */
#define LMW_CREATE_BASIC_MEMBER_CHECK(sname, fname)                            \
    template <class T>                                                         \
    struct sname {                                                             \
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
    }

#define LMW_CREATE_ADVANCED_MEMBER_CHECK(sname, fname, ...)                    \
    template <class T>                                                         \
    struct sname {                                                             \
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
    }

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

#define LMW_WRAPPER_FUNCTION_BANG(identifier)                                  \
    LMW_WRAPPER_FUNCTION(_bang, identifier)

#define LMW_WRAPPER_FUNCTION_INT(identifier)                                   \
    LMW_WRAPPER_FUNCTION(_int, identifier)

#define LMW_WRAPPER_FUNCTION_FLOAT(identifier)                                 \
    LMW_WRAPPER_FUNCTION(_float, identifier)

#define LMW_WRAPPER_FUNCTION_LIST(identifier)                                  \
    LMW_WRAPPER_FUNCTION(_list, identifier)

#define LMW_WRAPPER_FUNCTION_OBJECT_NEW(identifier)                            \
    LMW_WRAPPER_FUNCTION(_object_new, identifier)

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
        c74::max::object_method(                                               \
            dsp64, c74::max::gensym("dsp_add64"), x,                           \
            reinterpret_cast<void*>(                                           \
                LMW_WRAPPER_FUNCTION_DSP64_PERFORM(identifier)));              \
    }

#define LMW_CREATE_CLASS_NEW_FUNCTION(identifier, classname)                   \
    void* LMW_WRAPPER_FUNCTION_OBJECT_NEW(identifier)()                        \
    {                                                                          \
        return lmw::wrapper_object_new<classname>();                           \
    }

#define LMW_CREATE_EXT_HANDLER_FUNCTIONS(identifier, classname)                \
    LMW_CREATE_BANG_HANDLER_FUNCTION(identifier, classname)                    \
    LMW_CREATE_INT_HANDLER_FUNCTION(identifier, classname)                     \
    LMW_CREATE_DSP_PERFORM_FUNCTION(identifier, classname)                     \
    LMW_CREATE_DSP_METHOD_FUNCTION(identifier, classname)                      \
    LMW_CREATE_CLASS_NEW_FUNCTION(identifier, classname)

/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */

#define LMW_CLASS_ADD_STATIC_HANDLER(cfunc, classname, class_ptr, str)         \
    c74::max::class_addmethod(                                                 \
        class_ptr, reinterpret_cast<c74::max::method>(cfunc), #str);

#define LMW_CLASS_ADD_STATIC_HANDLER_VAR(cfunc, classname, class_ptr, str,     \
                                         ...)                                  \
    c74::max::class_addmethod(class_ptr,                                       \
                              reinterpret_cast<c74::max::method>(cfunc), #str, \
                              __VA_ARGS__);

#define LMW_ADDMETHOD_FUNCTION_NAME(ident)                                     \
    LMW_PREPROCESSOR_CAT(lmw_ext_addmethod_impl_, ident)

#define LMW_ADDMETHOD_FUNCTION_DECL(ident, template_name,                      \
                                    class_ptr)                                 \
    template <typename template_name>                                          \
    void LMW_ADDMETHOD_FUNCTION_NAME(ident)(c74::max::t_class * class_ptr)

#define LMW_ADDMETHOD_FUNCTION_STATIC_CHECK(classname, static_check)           \
    if                                                                         \
    constexpr(static_check<classname>::value)

#define LMW_ADDMETHOD_CREATE_CHECKED_CALL(classname, class_ptr, static_check,  \
                                          method, str)                    \
    LMW_ADDMETHOD_FUNCTION_STATIC_CHECK(classname, static_check)               \
    {                                                                          \
        LMW_CLASS_ADD_STATIC_HANDLER(                                          \
            method, classname, class_ptr, str)                    \
    }

#define LMW_ADDMETHOD_CREATE_CHECKED_CALL_VAR(classname, class_ptr,            \
                                              static_check, method, str, ...)  \
    LMW_ADDMETHOD_FUNCTION_STATIC_CHECK(classname, static_check)               \
    {                                                                          \
        LMW_CLASS_ADD_STATIC_HANDLER_VAR(                                      \
            method, classname, class_ptr, str, __VA_ARGS__)                    \
    }

#define LMW_ADDMETHOD_CREATE_FUNCTION(identifier)                              \
    LMW_ADDMETHOD_FUNCTION_DECL(identifier, user_class, c)                     \
    {                                                                          \
        LMW_ADDMETHOD_CREATE_CHECKED_CALL(                                     \
            user_class, c, lmw::type_traits::has_bang_handler,                 \
            LMW_WRAPPER_FUNCTION_BANG(identifier), bang)                       \
                                                                               \
        LMW_ADDMETHOD_CREATE_CHECKED_CALL(                                     \
            user_class, c, lmw::type_traits::has_int_handler,                  \
            LMW_WRAPPER_FUNCTION_INT(identifier), int)                         \
                                                                               \
        LMW_ADDMETHOD_CREATE_CHECKED_CALL_VAR(                                 \
            user_class, c, lmw::type_traits::has_dsp_handler,                  \
            LMW_WRAPPER_FUNCTION_DSP64_METHOD(identifier), dsp64,              \
            c74::max::e_max_atomtypes::A_CANT, 0)                              \
    }

/* -------------------------------------------------------------------------- */
//                            THE MAGIC WRAPPER
/* -------------------------------------------------------------------------- */

#define LMW_EXTERNAL(class)                                                    \
    LMW_CREATE_EXT_HANDLER_FUNCTIONS(default, class)                           \
    LMW_ADDMETHOD_CREATE_FUNCTION(default)                                     \
    void lmw_external_main()                                                   \
    {                                                                          \
    }
