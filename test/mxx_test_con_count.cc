#include <mxx/mxx.h>

class mxx_test_con_count: public mxx::max_class<mxx_test_con_count> {
  public:
    mxx_test_con_count(mxx::construction_type ty)
    {
        if (ty != mxx::construction_type::construct)
            return;

        sig_in_1  = make_signal_inlet("(signal) in 1");
        sig_in_2  = make_signal_inlet("(signal) in 2");
        sig_out_1 = make_signal_outlet("(signal) out 1");
        sig_out_2 = make_signal_outlet("(signal) out 2");

        info_out = make_typed_outlet("int", "info out");
    }

    void prepare(double, long)
    {
        info_out->send("in", sig_in_1->connections(), sig_in_2->connections(),
                       "out", sig_out_1->connections(),
                       sig_out_2->connections());
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

    mxx::inlet_ptr sig_in_1;
    mxx::inlet_ptr sig_in_2;
    mxx::outlet_ptr sig_out_1;
    mxx::outlet_ptr sig_out_2;
    mxx::outlet_ptr info_out;
};

MXX_EXTERNAL(mxx_test_con_count, mxx_test_con_count, "mxx_test_con_count");
