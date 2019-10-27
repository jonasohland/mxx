#pragma once

namespace lmw::type_traits {

    LMW_CREATE_ADVANCED_MEMBER_CHECK(has_bang_handler, handle_bang, long);

    LMW_CREATE_ADVANCED_MEMBER_CHECK(has_int_handler, handle_int, long, long);

    LMW_CREATE_ADVANCED_MEMBER_CHECK(has_float_handler, handle_float, double, long);

    LMW_CREATE_ADVANCED_MEMBER_CHECK(has_list_handler, handle_list,
                                     ::lmw::atom::vector, long);

    LMW_CREATE_ADVANCED_MEMBER_CHECK(has_raw_list_handler, handle_raw_list,
                                     ::lmw::atom::span, long);

    LMW_CREATE_ADVANCED_MEMBER_CHECK(has_dsp_handler, process, double**,
                                     double**, long, long, long);
    
    LMW_CREATE_ADVANCED_MEMBER_CHECK(has_prepare_function, prepare, double, long);
    
    LMW_CREATE_ADVANCED_MEMBER_CHECK(has_construct_function, construct, lmw::atom::vector);
    
    LMW_CREATE_BASIC_MEMBER_CHECK(has_input_changed_function, inputchanged);

    template <typename user_class>
    std::integral_constant<bool, has_dsp_handler_impl<user_class>::value>
        is_dsp_class;

} // namespace lmw::type_traits
