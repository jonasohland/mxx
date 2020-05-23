#define LMW_ENABLE_CTTI_DEBUG
#include "lmw.h"

struct test_external: lmw::max_class<test_external> {

    test_external()
    {
        make_mc_inlet("(mc) blub");
    }

    void construct(lmw::atom::vector args)
    {
        std::scoped_lock<lmw::mutex> lock(mtx);
    }

    lmw::mutex mtx;
};

lmw::basic_attribute<std::vector<int>> attr;


LMW_EXTERNAL(test_external, test_external)
