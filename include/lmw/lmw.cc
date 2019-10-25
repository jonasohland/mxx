
#include "lmw.h"

struct test_external : public lmw::object<test_external> {

    void handle_bang()
    {
        console << "Hello!" << lmw::endl;
        console_warn << "Oh No!" << lmw::endl;
        console_error << "bruuuuuuuh" << lmw::endl;
    }

    void handle_int(long num)
    {
        post("Hello %s!", num);
    }

    lmw::outlet_ptr outlet = make_outlet("int", "an outlet");
    lmw::inlet_ptr inlet = make_inlet();
};

LMW_EXTERNAL(test_external)
