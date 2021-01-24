#include <mxx/mxx.h>

class mxxx: public mxx::max_class<mxxx> {
  public:
    mxxx()
    {
        add_method("testmethod", [this](mxx::atom::const_span args){
            console << args << mxx::endl;
        });
        
        bind_method("member_test", &mxxx::member_test);
    }
    
    void member_test(const mxx::atom::vector& args)
    {
        console << "Hi from member: " << args << mxx::endl;
    }
    
};

MXX_EXTERNAL(mxxx, mxxx, "mxxx");
