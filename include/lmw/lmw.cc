#define LMW_ENABLE_CTTI_DEBUG
#include "lmw.h"

struct test_external : lmw::max_class<test_external> {
    
    test_external()
    {
        make_mc_inlet(10, "(mc) blub");
    }
    
    void process(double**, double**, long, long, long)
    {
        
    }

};


LMW_EXTERNAL(test_external)
