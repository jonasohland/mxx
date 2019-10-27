#pragma once

#if __GNUC__ || __clang__

#define LMW_LIKELY(x) __builtin_expect(!!(x), 1)
#define LMW_UNLIKELY(x) __builtin_expect(!!(x), 0)

#define LMW_ALWAYS_INLINE __attribute__((always_inline))

#elif _MSC_VER

#define LMW_LIKELY(x) x
#define LME_UNLIKELY(x) x

#define LMW_ALWAYS_INLINE __forceinline

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

#include "lmw_types.h"
#include "lmw_symbol.h"

namespace lmw::sym {
    symbol any                 = c74::max::gensym("any");
    symbol anything            = c74::max::gensym("anything");
    symbol signal              = c74::max::gensym("signal");
    symbol multichannelsignal  = c74::max::gensym("multichannelsignal");
    symbol getnuminputchannels = c74::max::gensym("getnuminputchannels");
    symbol dsp_add64           = c74::max::gensym("dsp_add64");
}

#include "lmw_wrapper_preprocessor.h"
#include "lmw_atom.h"
#include "lmw_type_traits.h"
#include "lmw_threads.h"
#include "lmw_message.h"
#include "lmw_port.h"
#include "lmw_outlet.h"
#include "lmw_inlet.h"
#include "lmw_cstream.h"
#include "lmw_class.h"
#include "lmw_inlet_outlet_impl.h"
#include "lmw_wrapper.h"
#include "lmw_wrapper_functions.h"

// clang-format on
