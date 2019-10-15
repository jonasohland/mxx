#pragma once

#define lmw_external_main ext_main

/* -------------------------------------------------------------------------- */
/*               General purpose preprocessor utilities                       */
/* -------------------------------------------------------------------------- */

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

#define LMW_WRAPPER_FUNCTION_DSP64(identifier)                                 \
    LMW_WRAPPER_FUNCTION(_dsp64, identifier)

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
    void LMW_WRAPPER_FUNCTION_DSP64(identifier)(                               \
        c74::max::t_object * x, c74::max::t_object * dsp64, double** ins,      \
        long numins, double** outs, long numouts, long sampleframes,           \
        long flags, void* userparam)                                           \
    {                                                                          \
        lmw::wrapper_dsp64_perform_impl<classname>(                            \
            x, dsp64, ins, numins, outs, numouts, sampleframes, flags,         \
            userparam);                                                        \
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
    LMW_CREATE_CLASS_NEW_FUNCTION(identifier, classname)

/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */

#define LMW_CLASS_ADD_STATIC_HANDLER(cfunc, classname, class_ptr, str)         \
    c74::max::class_addmethod(                                                 \
        class_ptr, reinterpret_cast<c74::max::method>(cfunc), #str);

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
                                          method, str)                         \
    LMW_ADDMETHOD_FUNCTION_STATIC_CHECK(classname, static_check)               \
    {                                                                          \
        LMW_CLASS_ADD_STATIC_HANDLER(method, classname, class_ptr, str)        \
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
        LMW_ADDMETHOD_CREATE_CHECKED_CALL(                                     \
            user_class, c, lmw::type_traits::has_dsp_handler,                  \
            LMW_WRAPPER_FUNCTION_DSP64(identifier), dsp64)                     \
    }


/* -------------------------------------------------------------------------- */
//                              THE MAGIC WRAPPER
/* -------------------------------------------------------------------------- */

#define LMW_EXTERNAL(class)                                                    \
    LMW_CREATE_EXT_HANDLER_FUNCTIONS(default, class)                           \
    LMW_ADDMETHOD_CREATE_FUNCTION(default)                                     \
    void lmw_external_main()                                                   \
    {                                                                          \
    }
