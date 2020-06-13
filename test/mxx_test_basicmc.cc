#include <mxx/mxx.h>

class mxx_test_basicmc: public mxx::max_class<mxx_test_basicmc> {
  public:
    mxx_test_basicmc()
    {
        inlet  = make_mc_inlet("signals in");
        outlet = make_mc_outlet("signals out");
    }

    void inputchanged(long)
    {
        outlet->signal_count(inlet->signal_count());
    }

    void process(double** ins, double** outs, long inchs, long outchs,
                 long vect)
    {
        if (inchs != outchs)
            return;

        for (int chi = 0; chi < inchs; ++chi) {
            for (int vi = 0; vi < vect; ++vi)
                outs[chi][vi] = ins[chi][vi];
        }
    }

    mxx::inlet_ptr inlet;
    mxx::outlet_ptr outlet;
};

MXX_EXTERNAL(mxx_test_basicmc, mxx_test_basicmc, "mxx_test_basicmc")
