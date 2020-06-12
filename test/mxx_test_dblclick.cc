#include <mxx/mxx.h>

class mxx_test_dblclick: public mxx::max_class<mxx_test_dblclick> {
  public:
    mxx_test_dblclick()
    {
        outlet = make_typed_outlet("int", "double-clicks");
    }

    void handle_dblclick()
    {
        outlet->send(++click_count);
    }

    int click_count = 0;
    mxx::outlet_ptr outlet;
};

MXX_EXTERNAL(mxx_test_dblclick, mxx_test_dblclick, "mxx_test_dblclick");
