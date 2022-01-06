#include <mxx/mxx.h>

class mxx_test_hotcold_inlet: public mxx::max_class<mxx_test_hotcold_inlet> {
  public:
    mxx_test_hotcold_inlet()
    {
        make_inlet("Hot inlet")->hot(true);
        make_inlet("Cold inlet")->hot(false);

        outlet = make_outlet("Outlet");
    }

    void handle_int(long value, long inlet)
    {
        values[inlet] = value;

        // alternative: if (inlets(inlet)->hot())
        if (inlet_is_hot(inlet))
            outlet->send(values[0], values[1]);
    }

    std::array<long, 2> values;
    mxx::outlet_ptr outlet;
    mxx::inlet_ptr inlet;
};

MXX_EXTERNAL(mxx_test_hotcold_inlet, mxx_test_hotcold_inlet, "mxx_test_hotcold_inlet");
