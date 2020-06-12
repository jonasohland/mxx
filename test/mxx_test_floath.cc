#include <mxx/mxx.h>

class mxx_test_floath: public mxx::max_class<mxx_test_floath> {
  public:
    mxx_test_floath()
    {
        make_inlet("float in");
        outlet = make_outlet("float out");
    }

    void handle_float(double value, long inlet)
    {
        outlet->send(value);
    }

    mxx::outlet_ptr outlet;
};

MXX_EXTERNAL(mxx_test_floath, mxx_test_floath, "mxx_test_floath");
