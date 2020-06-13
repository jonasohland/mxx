/*
    This essentially implements the behaviour of the *~ object
    and shows how to provide your own perform-routine to the msp
    compiler.
 
    The signal to the left inlet is multiplied either with the signal
    from the second inlet, or with a scalar value if nothing is connected
    to the second inlet.
*/

#include <mxx/mxx.h>

class mxx_test_userdsp64;

// clang-format off
MXX_DECLARE_PERFROUTINE(perform_scalar);
MXX_DECLARE_PERFROUTINE(perform_signal);
// clang-format on

class mxx_test_userdsp64: public mxx::max_class<mxx_test_userdsp64> {
  public:
    void construct(const mxx::atom::vector& args)
    {
        if (args.size())
            scalar = args[0];

        // left inlet
        make_signal_inlet("signal in");

        // right inlet
        signal_in = make_signal_inlet("signal2 in");

        // outlet
        make_signal_outlet("signal out");
    }

    // define this method on your class to supply your own
    // perform-routine
    c74::max::t_perfroutine64 setup_dsp(double, long)
    {
        // Something is connected to our 2nd inlet.
        if (signal_in->connections())
            // Choose the perform implementation that
            // uses the second signal.
            return &perform_signal;

        // Nothing connected to the 2nd inlet.
        else
            // Choose the perorm implementation that
            // uses a scalar value
            return &perform_scalar;
    }
    
    void handle_float(double sc, long inlet)
    {
        if(inlet)
            scalar = sc;
    }

    double scalar = 0.;
    mxx::inlet_ptr signal_in;
};

void perform_signal(c74::max::t_object*, c74::max::t_object*, double** ins,
                    long numins, double** outs, long numouts, long frames, long,
                    void*)
{
    double* in1 = ins[0];
    double* in2 = ins[1];
    double* out = *outs;

    for (int i = 0; i < frames; ++i) out[i] = in1[i] * in2[i];
}

void perform_scalar(c74::max::t_object* x, c74::max::t_object* dsp64,
                    double** ins, long numins, double** outs, long numouts,
                    long frames, long, void*)
{
    double* in1 = ins[0];
    double* in2 = ins[1];
    double* out = *outs;
    auto* obj   = mxx::find_self<mxx_test_userdsp64>(x);

    for (int i = 0; i < frames; ++i) out[i] = in1[i] * obj->scalar;
}

MXX_EXTERNAL(mxx_test_userdsp64, mxx_test_userdsp64, "mxx_test_userdsp64");
