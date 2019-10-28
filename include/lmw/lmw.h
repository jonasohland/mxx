#pragma once

#if __GNUC__ || __clang__

#define LMW_LIKELY(x) __builtin_expect(!!(x), 1)
#define LMW_UNLIKELY(x) __builtin_expect(!!(x), 0)

#define LMW_ALWAYS_INLINE __attribute__((always_inline))

#define LMW_MSVC_IGNORE_POINTER_TRUNCATION(x) x

#define LMW_STRCPY (src, dst) strcpy(src, dest)

#elif _MSC_VER

#define LMW_LIKELY(x) x
#define LMW_UNLIKELY(x) x

#define LMW_ALWAYS_INLINE __forceinline

#define LMW_MSVC_IGNORE_POINTER_TRUNCATION(expr)                               \
  __pragma(warning(push)) __pragma(warning(disable : 4312))                    \
      __pragma(warning(disable : 4311)) __pragma(warning(disable : 4302))      \
          expr __pragma(warning(pop))

#define LMW_STRCPY(src, dest)                                                  \
  __pragma(warning(push)) __pragma(warning(disable : 4996)) strcpy(src, dest)  \
      __pragma(warning(pop))

#else

#define LMW_LIKELY(x) x
#define LME_UNLIKELY(x) x

#define LMW_ALWAYS_INLINE

#endif

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

#include <lmw/lmw_symbol.h>
#include <lmw/lmw_atom.h>
#include <lmw/lmw_types.h>

namespace lmw::sym {
    inline symbol any                 = c74::max::gensym("any");
    inline symbol anything            = c74::max::gensym("anything");
    inline symbol signal              = c74::max::gensym("signal");
    inline symbol multichannelsignal  = c74::max::gensym("multichannelsignal");
    inline symbol getnuminputchannels = c74::max::gensym("getnuminputchannels");
    inline symbol dsp_add64           = c74::max::gensym("dsp_add64");
}

#include <lmw/lmw_wrapper_preprocessor.h>
#include <lmw/lmw_type_traits.h>
#include <lmw/lmw_threads.h>
#include <lmw/lmw_message.h>
#include <lmw/lmw_port.h>
#include <lmw/lmw_outlet.h>
#include <lmw/lmw_inlet.h>
#include <lmw/lmw_cstream.h>
#include <lmw/lmw_class.h>
#include <lmw/lmw_inlet_outlet_impl.h>
#include <lmw/lmw_wrapper.h>
#include <lmw/lmw_wrapper_functions.h>

// clang-format on
