#include "lmw.h"


class test_external : public lmw::object<test_external> {
  public:
    
    test_external() {}
    
    void init(lmw::atom_vector args){
    }
    
    lmw::message msg{this, "int", [](lmw::atom_vector atms, long n) {
        return lmw::atom_vector();
    }};
};

LMW_EXTERNAL(test_external)
