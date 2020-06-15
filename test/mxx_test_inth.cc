#include <mxx/mxx.h>

class mxx_test_inth: public mxx::max_class<mxx_test_inth> {
  public:
    mxx_test_inth()
    {
        make_inlet("int in");
        outlet = make_outlet("int out");
    }

    void handle_int(long value, long)
    {
        outlet->send_one(value);
    }

    mxx::outlet_ptr outlet;
};

MXX_EXTERNAL(mxx_test_inth, mxx_test_inth, "mxx_test_inth");
