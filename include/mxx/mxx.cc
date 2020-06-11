#define mxx_ENABLE_CTTI_DEBUG
#include "mxx.h"

struct test_external: mxx::max_class<test_external> {

    test_external()
    {
        make_mc_inlet("(mc) blub");
    }

    void construct(mxx::atom::vector args)
    {
        std::scoped_lock<mxx::mutex> lock(mtx);
    }

    mxx::mutex mtx;
};


mxx_EXTERNAL(test_external, test_external)
