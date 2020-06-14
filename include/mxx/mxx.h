/**
 @File main include
 */
#pragma once

#if __GNUC__ || __clang__

#    define MXX_LIKELY(x)   __builtin_expect(!!(x), 1)
#    define MXX_UNLIKELY(x) __builtin_expect(!!(x), 0)

#    define MXX_ALWAYS_INLINE __attribute__((always_inline)) inline

#    define MXX_MSVC_IGNORE_POINTER_TRUNCATION(x) x

#    define MXX_STRCPY(src, dst) strcpy(src, dst)

#elif _MSC_VER

#    define MXX_LIKELY(x)   x
#    define MXX_UNLIKELY(x) x

#    define MXX_ALWAYS_INLINE __forceinline

// clang-format off

#define MXX_MSVC_IGNORE_POINTER_TRUNCATION(expr)                               \
    __pragma(warning(push))                                                    \
    __pragma(warning(disable : 4312))                                          \
    __pragma(warning(disable : 4311))                                          \
    __pragma(warning(disable : 4302))                                          \
    expr                                                                       \
    __pragma(warning(pop))

#define MXX_STRCPY(src, dest)                                                  \
    __pragma(warning(push))                                                    \
    __pragma(warning(disable : 4996))                                          \
    strcpy(src, dest)                                                          \
    __pragma(warning(pop))

// clang-format on

#else

#    define MXX_LIKELY(x)   x
#    define LME_UNLIKELY(x) x

#    define MXX_ALWAYS_INLINE

#endif

#define MXX_STRING_LITERAL_IMPL(x) #x
#define MXX_STRING_LITERAL(x)      MXX_STRING_LITERAL_IMPL(x)

#include <cstring>
#include <functional>
#include <algorithm>
#include <array>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>

#include <c74_max.h>
#include <c74_msp.h>

// clang-format off

#include <mxx/mxx_symbol.h>
#include <mxx/mxx_atom.h>
#include <mxx/mxx_types.h>

namespace mxx::sym {

    /// Symbol "any".
    inline symbol any                 = c74::max::gensym("any");
    
    /// Symbol "anything".
    inline symbol anything            = c74::max::gensym("anything");
    
    /// Symbol "signal". Used to create signal outlets.
    inline symbol signal              = c74::max::gensym("signal");
    
    /// Symbol "bang". Used to send bangs
    inline symbol bang                = c74::max::gensym("bang");
    
    /// Symbol "multichannelsignal". Used to create multichannelsignal outlets.
    inline symbol multichannelsignal  = c74::max::gensym("multichannelsignal");
    
    /// Symbol "getnuminputchannels". Used to obtain information about input channels on a msp object.
    inline symbol getnuminputchannels = c74::max::gensym("getnuminputchannels");
    
    /// Symbol "dsp_add64". Used to register an msp object to the msp compiler.
    inline symbol dsp_add64           = c74::max::gensym("dsp_add64");
    
    /// Empty Symbol
    inline symbol empty = c74::max::gensym("");
}

namespace mxx {
    using perform_function = c74::max::t_perfroutine64;

    enum class construction_type {
        max_init,
        wrapper_init,
        construct
    };
}

#include <mxx/mxx_wrapper_preprocessor.h>
#include <mxx/mxx_type_traits.h>
#include <mxx/mxx_mutex.h>
#include <mxx/mxx_threads.h>
#include <mxx/mxx_message.h>
#include <mxx/mxx_attribute.h>
#include <mxx/mxx_port.h>
#include <mxx/mxx_outlet.h>
#include <mxx/mxx_inlet.h>
#include <mxx/mxx_cstream.h>
#include <mxx/mxx_class.h>
#include <mxx/mxx_inlet_outlet_impl.h>
#include <mxx/mxx_wrapper.h>
#include <mxx/mxx_wrapper_functions.h>
#include <mxx/mxx_universal_helper.h>

// clang-format on
