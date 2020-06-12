#include <mxx/mxx.h>

class mxxx : public mxx::max_class<mxxx> {
public:
    mxxx(mxx::construction_type cty)
    {
        switch (cty) {
            case mxx::construction_type::construct:
                post("construction_type: construct");
                break;
            case mxx::construction_type::wrapper_init:
                post("construction_type: wrapper_init");
                break;
            case mxx::construction_type::max_init:
                post("construction_type: max_init");
                break;
        }
    }
};

MXX_EXTERNAL(mxxx, mxxx);
