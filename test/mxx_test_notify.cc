// #define MXX_REQUIRE_NOTIFY_FUNCTION
// #define MXX_REQUIRE_LIST_HANDLER
#include <mxx/mxx.h>

class mxx_test_notify: public mxx::max_class<mxx_test_notify> {
  public:
    void construct(const mxx::atom::vector& args)
    {
        inlet = make_inlet("inlet")->hot(true);
        buffer_ref.bind("buf");
    }

    void notify(mxx::symbol s, mxx::symbol msg, void* sender, void* data)
    {
        auto sn = s.make_unique();
    }

    mxx::inlet_ptr inlet;
    mxx::buffer_reference buffer_ref { this };
};

MXX_EXTERNAL(mxx_test_notify, mxx_test_notify, "mxx_test_notify")
