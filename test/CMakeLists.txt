macro(mxx_test_external mxx_xt_name)
    add_library(${mxx_xt_name} MODULE ${CMAKE_CURRENT_LIST_DIR}/${mxx_xt_name}.cc)
    target_link_libraries(${mxx_xt_name} ${mxx_wrapper_target_name})
    mxx_setup_external(${mxx_xt_name} EXTERNAL_OUTPUT_DIR ${mxx_external_output_dir})
endmacro()


mxx_test_external(mxx_test_bangh)
mxx_test_external(mxx_test_inth)
mxx_test_external(mxx_test_floath)
mxx_test_external(mxx_test_siginout)
mxx_test_external(mxx_test_basicmc)
