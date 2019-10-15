#include "lmw.h"

namespace my_namespace {
    class test_external {
      public:
        void process(double** in, double** out, long ins, long outs)
        {
        }
    };
}

LMW_EXTERNAL(my_namespace::test_external)
