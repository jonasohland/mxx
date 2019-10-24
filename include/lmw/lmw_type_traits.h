#pragma once

#include "lmw_wrapper_preprocessor.h"

namespace lmw::type_traits {

    LMW_CREATE_BASIC_MEMBER_CHECK(has_bang_handler, handle_bang);

    LMW_CREATE_ADVANCED_MEMBER_CHECK(has_int_handler, handle_int, long);

    LMW_CREATE_ADVANCED_MEMBER_CHECK(has_float_handler, handle_float, double);

    LMW_CREATE_ADVANCED_MEMBER_CHECK(has_list_handler, handle_list,
                                     lmw::atom_span);

    LMW_CREATE_ADVANCED_MEMBER_CHECK(has_dsp_handler, process, double**,
                                     double**, long, long);
    
    LMW_CREATE_ADVANCED_MEMBER_CHECK(has_construct_function, construct, lmw::atom_vector);

    template <typename user_class>
    std::integral_constant<bool, has_dsp_handler_impl<user_class>::value>
        is_dsp_class;

} // namespace lmw::type_traits
