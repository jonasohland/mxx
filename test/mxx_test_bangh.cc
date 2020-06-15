#include <mxx/mxx.h>

// All externals must derive from the mxx::max_class<user_class> template.
// This will give you access to the make_inlet, make_outlet, (...) functions.
class mxx_test_bangh: public mxx::max_class<mxx_test_bangh> {
  public:
    mxx_test_bangh()
    {
        // create a simple (hot) inlet
        // a referece to inlets and outlets will be stored
        // in the base class
        make_inlet("bangs in");

        // you can also store a reference to an in/outlet yourself
        outlet = make_outlet("bangs out");
    }

    // The mxx::wrapper will find this function using C++ template magic
    // and resolve any calls from max to the wrappers int-handler to this
    // function at -> compile-time <-
    void handle_bang(long)
    {
        outlet->bang();
    }

    // An outlet_ptr is just a std::shared_ptr<outlet>
    // If you want to pass a reference to an in/outlet to something that
    // could have a longer lifetime than your object, you should convert
    // the std::shared_ptr to a std::weak_ptr first
    mxx::outlet_ptr outlet;
};

MXX_EXTERNAL(mxx_test_bangh, mxx_test_bangh, "mxx_test_bangh");
