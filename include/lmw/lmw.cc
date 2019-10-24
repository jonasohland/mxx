#include "lmw.h"

struct test_external : public lmw::object<test_external> {

    void handle_bang()
    {
        post("Hello!");
        outlet->send("Hello");
    }

    void handle_int(long num)
    {
        post("Hello %s!", num);
    }

    lmw::outlet_ptr outlet = make_outlet("int", "an outlet");
};

LMW_EXTERNAL(test_external)
