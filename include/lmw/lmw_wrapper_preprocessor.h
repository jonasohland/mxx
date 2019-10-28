#pragma once

#define lmw_external_main ext_main

/* -------------------------------------------------------------------------- */
/*               General purpose preprocessor utilities                       */
/* -------------------------------------------------------------------------- */

#define LMW_FORCE_EXPAND(x) x

#define LMW_PASTE(a, b) a##b
#define LMW_XPASTE(a, b) LMW_PASTE(a, b)

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
            std::declval<C>().fname(LMW_FORCE_EXPAND(__VA_ARGS__)),            \
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

#define LMW_STATIC_WARNING_IMPL(cond, msg, counter)                            \
    struct LMW_PP_CAT(static_warning, counter) {                               \
        LMW_DEPRECATE(void _(::lmw::detail::false_type const&), msg){};        \
        void _(::lmw::detail::true_type const&){};                             \
        LMW_PP_CAT(static_warning, counter)()                                  \
        {                                                                      \
            _(::lmw::detail::converter<(cond)>());                             \
        }                                                                      \
    }

#define LMW_STATIC_WARNING(cond, msg)                                          \
    LMW_STATIC_WARNING_IMPL(cond, msg, __COUNTER__)

#ifdef LMW_ENABLE_CTTI_DEBUG

#define LMW_DEBUG_MSG_PREFIX "LMW DEBUG: "

#define LMW_CTTI_DEBUG_SECTION(user_class)                                     \
    LMW_STATIC_WARNING(!lmw::type_traits::has_bang_handler<user_class>(),      \
                       LMW_DEBUG_MSG_PREFIX "bang handler enabled");           \
    LMW_STATIC_WARNING(!lmw::type_traits::has_int_handler<user_class>(),       \
                       LMW_DEBUG_MSG_PREFIX "int handler enabled");            \
    LMW_STATIC_WARNING(!lmw::type_traits::has_float_handler<user_class>(),     \
                       LMW_DEBUG_MSG_PREFIX "float handler enabled");          \
    LMW_STATIC_WARNING(!lmw::type_traits::has_list_handler<user_class>(),      \
                       LMW_DEBUG_MSG_PREFIX "list handler enabled");           \
    LMW_STATIC_WARNING(!lmw::type_traits::has_raw_list_handler<user_class>(),  \
                       LMW_DEBUG_MSG_PREFIX "raw list handler enabled");       \
    LMW_STATIC_WARNING(!lmw::type_traits::has_dsp_handler<user_class>(),       \
                       LMW_DEBUG_MSG_PREFIX "DSP perform routine enabled");    \
    LMW_STATIC_WARNING(                                                        \
        !lmw::type_traits::has_input_changed_function<user_class>(),           \
        LMW_DEBUG_MSG_PREFIX "inputchanged handler enabled");
#else

#define LMW_CTTI_DEBUG_SECTION(user_class) ;

#endif

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

#define LMW_WRAPPER_FUNCTION_INPUTCHANGED(identifier)                          \
    LMW_WRAPPER_FUNCTION(_inputchanged, identifier)

#define LMW_WRAPPER_FUNCTION_MULTICHANNELOUTPUTS(identifier)                   \
    LMW_WRAPPER_FUNCTION(_multichanneloutputs, identifier)

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

#define LMW_CREATE_INPUTCHANGED_FUNCTION(identifier, classname)                \
    void LMW_WRAPPER_FUNCTION_INPUTCHANGED(identifier)(                        \
        c74::max::t_object * x, long index, long chans)                        \
    {                                                                          \
        lmw::wrapper_inputchanged_impl<classname>(x, index, chans);            \
    }

#define LMW_CREATE_MULTICHANNELOUTPUTS_FUNCTION(identifier, classname)         \
    long LMW_WRAPPER_FUNCTION_MULTICHANNELOUTPUTS(identifier)(                 \
        c74::max::t_object * x, long idx)                                      \
    {                                                                          \
        return lmw::wrapper_multichanneloutputs_impl<classname>(x, idx);       \
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
            x, dsp64, count, samplerate, maxvectorsize, flags,                 \
            LMW_WRAPPER_FUNCTION_DSP64_PERFORM(identifier));                   \
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
    LMW_CREATE_INPUTCHANGED_FUNCTION(identifier, classname)                    \
    LMW_CREATE_MULTICHANNELOUTPUTS_FUNCTION(identifier, classname)             \
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
        LMW_MAX_METHOD(LMW_WRAPPER_FUNCTION_INPUTCHANGED(ident)),              \
        LMW_MAX_METHOD(LMW_WRAPPER_FUNCTION_MULTICHANNELOUTPUTS(ident)),       \
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
    LMW_CTTI_DEBUG_SECTION(class)                                              \
    void lmw_external_main(void*)                                              \
    {                                                                          \
        LMW_MAXCLASS_DEF_IMPL(default, class)                                  \
    }
