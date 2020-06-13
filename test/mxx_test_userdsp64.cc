#include <mxx/mxx.h>

class mxx_test_userdsp64;

// clang-format off
void perform_scalar(c74::max::t_object*, c74::max::t_object*, double**, long, double**, long, long, long, void*);
void perform_signal(c74::max::t_object*, c74::max::t_object*, double**, long, double**, long, long, long, void*);
// clang-format on

class mxx_test_userdsp64: public mxx::max_class<mxx_test_userdsp64> {
  public:
    void construct(const mxx::atom::vector& args)
    {
        if (args.size())
            scalar = args[0];

        make_signal_inlet("signal in");
        inlet2 = make_signal_inlet("signal2 in");
        make_signal_outlet("signal out");
    }

    c74::max::t_perfroutine64 setup_dsp(double, long)
    {
        if (inlet2->connections())    // Something is connected to our 2nd inlet.
            return &perform_signal;    // Choose the perform implementation that
                                       // uses the second signal.

        else                           // Nothing connected to the 2nd inlet.
            return &perform_scalar;    // Choose the perorm implementation that
                                       // uses a scalar value
    }

    double scalar = 0.;
    mxx::inlet_ptr inlet2;
};

void perform_signal(c74::max::t_object*,
                    c74::max::t_object*,
                    double** ins,
                    long numins,
                    double** outs,
                    long numouts,
                    long frames,
                    long,
                    void*)
{
    double* in1 = ins[0];
    double* in2 = ins[1];
    double* out = *outs;

    for (int i = 0; i < frames; ++i) out[i] = in1[i] * in2[i];
}

void perform_scalar(c74::max::t_object* x,
                    c74::max::t_object* dsp64,
                    double** ins,
                    long numins,
                    double** outs,
                    long numouts,
                    long frames,
                    long,
                    void*)
{
    double* in1 = ins[0];
    double* in2 = ins[1];
    double* out = *outs;

    for (int i = 0; i < frames; ++i)
        out[i] = in1[i] * mxx::find_self<mxx_test_userdsp64>(x)->scalar;
}

MXX_EXTERNAL(mxx_test_userdsp64, mxx_test_userdsp64, "mxx_test_userdsp64");
