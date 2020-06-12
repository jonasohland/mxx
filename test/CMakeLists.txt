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
mxx_test_external(mxx_test_delay1)
mxx_test_external(mxx_test_inletnum)
mxx_test_external(mxx_test_dblclick)
mxx_test_external(mxx_test_con_count)
mxx_test_external(mxxx)

# _tld and _tilde will be replaced with ~ in the output name
add_library(mc.append_tld MODULE "${CMAKE_CURRENT_LIST_DIR}/mc.append_tilde.cc")
add_library(mc.appendmc_tilde MODULE "${CMAKE_CURRENT_LIST_DIR}/mc.append_tilde.cc")

target_link_libraries(mc.append_tld PRIVATE ${mxx_wrapper_target_name})
target_link_libraries(mc.appendmc_tilde PRIVATE ${mxx_wrapper_target_name})

target_compile_definitions(mc.appendmc_tilde PRIVATE APPEND_MC_VERSION=1)

mxx_setup_external(mc.append_tld EXTERNAL_OUTPUT_DIR ${mxx_external_output_dir})
mxx_setup_external(mc.appendmc_tilde EXTERNAL_OUTPUT_DIR ${mxx_external_output_dir})