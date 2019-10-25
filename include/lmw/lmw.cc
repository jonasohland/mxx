#define LMW_REQUIRE_PROCESS_FUNCTION

#include "lmw.h"

struct test_external : lmw::object<test_external> {
    
    lmw::outlet_ptr out = make_signal_outlet("hello", "world");
    
    void process(double**, double**, long, long, long)
    {
        
    }
};

LMW_EXTERNAL(test_external)
