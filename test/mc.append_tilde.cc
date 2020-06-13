#include <mxx/mxx.h>

class mc_append_t: public mxx::max_class<mc_append_t> {
  public:
    void construct(const mxx::atom::vector& args)
    {
        if (!args.empty())
            sigcount = static_cast<int>(args[0]);

        mcinlet  = make_mc_inlet("(mc) signals in");
        mcoutlet = make_mc_outlet("(mc) signals out");

        for (int i = 0; i < sigcount; ++i) {
#ifdef APPEND_MC_VERSION
            mc_inlets.push_back(make_mc_inlet(
                std::string("(mc) signals #").append(std::to_string(i))));
#else
            make_signal_inlet(
                std::string("(signal) signal #").append(std::to_string(i)));
#endif
        }
    }

    void inputchanged(long)
    {
#ifdef APPEND_MC_VERSION
        mcoutlet->signal_count(
            mcinlet->signal_count()
            + std::accumulate(mc_inlets.begin(), mc_inlets.end(), 0,
                              [](int num, const auto inlet) {
                                  return num + inlet->signal_count();
                              }));
#else
        mcoutlet->signal_count(mcinlet->signal_count() + sigcount);
#endif
    }

    void process(double** ins, double** outs, long inchs, long outchs,
                 long vect)
    {
        if (inchs != outchs)
            return;

        for (int chi = 0; chi < inchs; ++chi) {
            for (int vi = 0; vi < vect; ++vi) outs[chi][vi] = ins[chi][vi];
        }
    }

    int sigcount = 1;
    mxx::inlet_ptr mcinlet;
    mxx::outlet_ptr mcoutlet;

    // #ifdef APPEND_MC_VERSION
    std::vector<mxx::inlet_ptr> mc_inlets;
    // #endif
};

#ifdef APPEND_MC_VERSION
MXX_EXTERNAL(mc_append_t, mc_append_t, "mc.appendmc~");
#else
MXX_EXTERNAL(mc_append_t, mc_append_t, "mc.append~");
#endif
