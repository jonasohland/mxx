#define LMW_ENABLE_CTTI_DEBUG
#include "lmw.h"

struct test_external : lmw::object<test_external> {

    void handle_list(const lmw::atom_vector& list, long inlet)
    {
        
    }
};

LMW_EXTERNAL(test_external)
