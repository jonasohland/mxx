#define LMW_REQUIRE_BANG_HANDLER
#define LMW_REQUIRE_INT_HANDLER

#include "lmw.h"

namespace my_namespace {
    class test_external {
      public:
        
        void process(double**, double**, long, long)
        {
        }
        
        void handle_int(long num)
        {
        }
        
        void handle_bang()
        {
        }
    };
}

static_assert(lmw::type_traits::is_dsp_class<my_namespace::test_external>(),
               "");

LMW_EXTERNAL(my_namespace::test_external)
