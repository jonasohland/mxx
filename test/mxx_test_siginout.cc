#include <iostream>
#include <mxx/mxx.h>

class mxx_test_siginout: public mxx::max_class<mxx_test_siginout> {
  public:
    mxx_test_siginout()
    {
        make_signal_inlet("signal in");
        make_signal_outlet("signal out");
    }

    void
    process(double** ins, double** outs, long inchs, long outchs, long vect)
    {
        if (inchs != outchs)
            return;

        for (int chi = 0; chi < inchs; ++chi) {
            for (int vi = 0; vi < vect; ++vi) outs[chi][vi] = ins[chi][vi];
        }
    }
};

MXX_EXTERNAL(mxx_test_siginout, mxx_test_siginout, "mxx_test_siginout");
