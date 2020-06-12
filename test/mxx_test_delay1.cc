#include <mxx/mxx.h>

class mxx_test_delay1: public mxx::max_class<mxx_test_delay1> {
  public:
    mxx_test_delay1()
    {
        inlet  = make_inlet("Data in");
        outlet = make_outlet("Data out");
    }

    void
    handle_any_msg(const mxx::symbol& msg, const mxx::atom::vector& args, long)
    {
        post(msg);
    }

    static constexpr int mxx_class_flags = 1;

    mxx::atom::vector buffer;

    mxx::inlet_ptr inlet;
    mxx::outlet_ptr outlet;
};


MXX_EXTERNAL(mxx_test_delay1, mxx_test_delay1, "mxx_test_delay1");
