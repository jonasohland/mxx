#include <mxx/mxx.h>

class mxx_test_inletnum: public mxx::max_class<mxx_test_inletnum> {
  public:
    void construct(const mxx::atom::vector& args)
    {
        int count = 1;

        if (args.size())
            count = args[0];

        for (int i = 0; i < count; ++i)
            make_inlet(std::string("inlet #").append(std::to_string(i + 1)))->hot(true);

        outlet = make_typed_outlet("int", "inlet number");
    }

    void handle_bang(long inletn)
    {
        send(inletn);
    }

    void handle_int(long, long inletn)
    {
        send(inletn);
    }

    void handle_float(double, long inletn)
    {
        send(inletn);
    }

    void handle_any_msg(const mxx::symbol&, const mxx::atom::vector&, long inletn)
    {
        send(inletn);
    }

    void send(long num)
    {
        outlet->send(num + 1);
    }

    mxx::outlet_ptr outlet;
};

MXX_EXTERNAL(mxx_test_inletnum, mxx_test_inletnum, "mxx_test_inletnum");
