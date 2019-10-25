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

#include <string>
#include <sstream>
#include <vector>
#include <array>
#include <algorithm>
#include <numeric>

#include <c74_max.h>

// clang-format off

#include "lmw_symbol.h"

namespace lmw::sym {
    symbol signal              = c74::max::gensym("signal");
    symbol multichannelsignal  = c74::max::gensym("multichannelsignal");
}

#include "lmw_types.h"
#include "lmw_atom.h"
#include "lmw_threads.h"
#include "lmw_message.h"
#include "lmw_port.h"
#include "lmw_outlet.h"
#include "lmw_inlet.h"
#include "lmw_cstream.h"
#include "lmw_object.h"
#include "lmw_object_impl.h"
#include "lmw_wrapper.h"
#include "lmw_wrapper_functions.h"

// clang-format on
