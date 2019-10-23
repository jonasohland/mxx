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

// LMW_EXTERNAL(test_external)

c74::max::t_class* lmw_ext_maxclass_default;
void* lmw_ext_newinstance_impl_default(c74::max::t_symbol* ext_name,
                                       long ext_ac, c74::max::t_atom* ext_av)
{
    return lmw::wrapper_object_new<test_external>(
        lmw_ext_maxclass_default, ext_name, ext_ac, ext_av);
}

void lmw_ext_freeinstance_impl_default(c74::max::t_object* ext_obj)
{
    lmw::wrapper_object_free<test_external>(ext_obj);
}

void lmw_ext_wrap_default_bang(c74::max::t_object* obj)
{
    lmw::wrapper_handle_bang_impl<test_external>(obj);
}

void lmw_ext_wrap_default_int(c74::max::t_object* obj, long n)
{
    lmw::wrapper_handle_int_impl<test_external>(obj, n);
}

void lmw_ext_wrap_default_float(c74::max::t_object* obj, double n)
{
    lmw::wrapper_handle_int_impl<test_external>(obj, n);
}

void lmw_ext_wrap_default_dsp64_perform(c74::max::t_object* x,
                                        c74::max::t_object* dsp64, double** ins,
                                        long numins, double** outs,
                                        long numouts, long sampleframes,
                                        long flags, void* userparam)
{
    lmw::wrapper_dsp64_perform_impl<test_external>(
        x, dsp64, ins, numins, outs, numouts, sampleframes, flags, userparam);
}

void lmw_ext_wrap_default_dsp64_method(c74::max::t_object* x,
                                       c74::max::t_object* dsp64, short* count,
                                       double samplerate, long maxvectorsize,
                                       long flags)
{
    c74::max::object_method(
        dsp64, c74::max::gensym("dsp_add64"), x,
        reinterpret_cast<void*>(lmw_ext_wrap_default_dsp64_perform));
}

void lmw_ext_wrap_default_named_method(c74::max::t_object* o,
                                       c74::max::t_symbol* s, long ac,
                                       c74::max::t_atom* av)
{
    lmw::wrapper_msg_call<test_external>(o, s, ac, av);
}

void ext_main()
{
    lmw_ext_maxclass_default = lmw::wrapper_class_new<test_external>(
        lmw_ext_maxclass_default,
        reinterpret_cast<c74::max::method>(&lmw_ext_newinstance_impl_default),
        reinterpret_cast<c74::max::method>(&lmw_ext_freeinstance_impl_default),
        reinterpret_cast<c74::max::method>(&lmw_ext_wrap_default_named_method),
        reinterpret_cast<c74::max::method>(&lmw_ext_wrap_default_int),
        reinterpret_cast<c74::max::method>(&lmw_ext_wrap_default_bang),
        reinterpret_cast<c74::max::method>(&lmw_ext_wrap_default_float),
        reinterpret_cast<c74::max::method>(&lmw_ext_wrap_default_dsp64_method));
    c74::max::class_register(c74::max::CLASS_BOX, lmw_ext_maxclass_default);
}
