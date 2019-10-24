#include "lmw.h"

class test_external : public lmw::object<test_external> {
  public:
    test_external()
    {
    }

    void construct(const lmw::atom_vector& args)
    {
        default_outlet->send("blub", args, 1, lmw::atom_vector({1, 2, 3}));
    }
    
    lmw::atom_vector handle_stuff(const lmw::atom_vector& args, long l)
    {
        return {};
    }
    
    lmw::message msg {this, "test", lmw::bind(&test_external::handle_stuff, this)};

    lmw::outlet_ptr default_outlet = new_outlet();
};

LMW_EXTERNAL(test_external)
