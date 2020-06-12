#include "mxx.h"

struct test_external: mxx::max_class<test_external> {

    test_external()
    {
        make_mc_inlet("(mc) blub");
    }

    void handle_int(long, long) {}

    void handle_float(double, long) {}

    void construct(mxx::atom::vector args)
    {
        std::scoped_lock<mxx::mutex> lock(mtx);
    }

    mxx::perform_function setup_dsp(double, float)
    {
        return nullptr;
    }

    mxx::mutex mtx;
};


MXX_EXTERNAL(test_external, test_external)
