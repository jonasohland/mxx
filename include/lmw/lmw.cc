#define LMW_REQUIRE_INT_HANDLER
#define LMW_REQUIRE_PROCESS_FUNCTION
#define LMW_REQUIRE_BANG_HANDLER

#include <numeric>
#include "lmw.h"
#include "lmw_numeric.h"

namespace my_namespace {
    class test_external {
      public:
        
        void handle_bang()
        {
        }
        
        void handle_int(int)
        {
            
        }
        
        void process(double** in, double** out, long ins, long outs)
        {
        }

        void handle_named_message(lmw::t_atom_span args)
        {
            // long test = lmw::reduce(args.begin(), args.end());
        }
    };
}

LMW_EXTERNAL(my_namespace::test_external)
