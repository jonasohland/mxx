#include <mxx/mxx.h>
#include <iostream>

template <typename user_class>
void test(mxx::console_stream<mxx::console::normal>& stream)
{
    if constexpr(mxx::type_traits::has_mxx_class_flags_variable<user_class>())
        stream << "hi" << user_class::mxx_class_flags << mxx::endl;
}

class mxx_test_delay1: public mxx::max_class<mxx_test_delay1> {
  public:
    mxx_test_delay1()
    {
        inlet  = make_inlet("Data in");
        outlet = make_outlet("Data out");
        
        test<mxx_test_delay1>(console);
    }

    void
    handle_any_msg(const mxx::symbol& msg, const mxx::atom::vector& args, long)
    {
        post(msg);
    }
    
    static constexpr int mxx_class_flags = 1;

    mxx::atom::vector buffer;

    mxx::inlet_ptr inlet;
    mxx::outlet_ptr outlet;
};


MXX_EXTERNAL(mxx_test_delay1, mxx_test_delay1);
