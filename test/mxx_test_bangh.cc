#include <mxx/mxx.h>

class mxx_test_bangh: public mxx::max_class<mxx_test_bangh> {
  public:
    mxx_test_bangh()
    {
        make_inlet("bangs in");
        outlet = make_outlet("bangs out");
    }

    void handle_bang(long inlet)
    {
        outlet->bang();
    }

    mxx::outlet_ptr outlet;
};

MXX_EXTERNAL(mxx_test_bangh, mxx_test_bangh, "mxx_test_bangh");
