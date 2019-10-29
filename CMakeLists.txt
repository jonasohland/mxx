cmake_minimum_required(VERSION 3.4)

project(lmw VERSION 0.0.1 LANGUAGES CXX)

option(lmw_generate_docs "Generate Documentation" OFF)

include(CMakePackageConfigHelpers)

set(lmw_version_major           0)
set(lmw_version_minor           0)
set(lmw_version_patch           1)

set(lmw_version                 ${lmw_version_major}.${lmw_version_minor}.${lmw_version_patch})

set(lmw_wrapper_target_name     lmw)
set(lmw_max_api_target_name     maxapi)
set(lmw_export_namespace        lmw::)
set(lmw_export_group            lmw_export)

set(C74_MAX_API_DIR             ${CMAKE_CURRENT_LIST_DIR}/c74/max-api)
set(C74_MAX_API_LIB_DIR         ${C74_MAX_API_DIR}/lib)
set(C74_MAX_API_SCRIPT_DIR      ${C74_MAX_API_DIR}/script)

set(lmw_max_linker_flags_file   ${C74_MAX_API_SCRIPT_DIR}/max-linker-flags.txt)
set(lmw_msp_linker_flags_file   ${C74_MAX_API_SCRIPT_DIR}/msp-linker-flags.txt)

set(lmw_root_install_dir        ${CMAKE_INSTALL_PREFIX})
set(lmw_package_install_dir     ${lmw_root_install_dir}/lib/cmake/${lmw_wrapper_target_name}-${lmw_version})
set(lmw_templates_dir           ${lmw_package_install_dir}/templates)

set(lmw_cmake_scripts_dir       ${CMAKE_CURRENT_LIST_DIR}/cmake)

set(lmw_config_file_template    ${lmw_cmake_scripts_dir}/lmw-config.cmake.in)
set(lmw_cmake_config_file       ${CMAKE_CURRENT_BINARY_DIR}/install_files/lmw-config.cmake)
set(lmw_cmake_version_file      ${CMAKE_CURRENT_BINARY_DIR}/install_files/lmw-config-version.cmake)

set(lmw_max_api_include_dir     ${C74_MAX_API_DIR}/include)
set(lmw_wrapper_include_dir     ${CMAKE_CURRENT_LIST_DIR}/include)

set(lmw_include_install_dir     ${lmw_root_install_dir}/include/${lmw_wrapper_target_name}-${lmw_version})

set(lmw_max_api_install_dir     ${lmw_include_install_dir}/max-api)
set(lmw_wrapper_install_dir     ${lmw_include_install_dir}/${lmw_wrapper_target_name})

set(lmw_doxyfile_template       ${CMAKE_CURRENT_LIST_DIR}/docs/Doxyfile.in)
set(lmw_doxyfile                ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

if(APPLE)

    set(lmw_frameworks_install_dir              ${lmw_root_install_dir}/Frameworks/${lmw_wrapper_target_name}-${lmw_version})
    set(lmw_mac_jitterapi_framework             ${C74_MAX_API_LIB_DIR}/mac/JitterAPI.framework)
    set(lmw_mac_jitterapi_installed_framework   ${lmw_frameworks_install_dir}/JitterAPI.framework)

    set(lmw_info_plist_template                 ${C74_MAX_API_SCRIPT_DIR}/Info.plist.in)
    set(lmw_installed_info_plist_template       ${lmw_templates_dir}/Info.plist.in)    

endif()

if(WIN32)

    if("${CMAKE_SIZEOF_VOID_P}" STREQUAL "4")
        set(lmw_win_lib_dir                     ${C74_MAX_API_LIB_DIR}/win32)
    else()
        set(lmw_win_lib_dir                     ${C74_MAX_API_LIB_DIR}/win64)   
    endif()

    set(lmw_win_installed_lib_dir               ${lmw_root_install_dir}/lib)

    set(lmw_win_maxapi_lib                      ${lmw_win_lib_dir}/MaxAPI.lib)
    set(lmw_win_mspapi_lib                      ${lmw_win_lib_dir}/MaxAudio.lib)
    set(lmw_win_jitapi_lib                      ${lmw_win_lib_dir}/jitlib.lib)

    set(lmw_win_installed_maxapi_lib            ${lmw_win_installed_lib_dir}/MaxAPI.lib)
    set(lmw_win_installed_mspapi_lib            ${lmw_win_installed_lib_dir}/MaxAudio.lib)
    set(lmw_win_installed_jitapi_lib            ${lmw_win_installed_lib_dir}/jitlib.lib)

endif()

add_library(${lmw_wrapper_target_name} INTERFACE)
add_library(${lmw_max_api_target_name} INTERFACE)

target_compile_features(${lmw_wrapper_target_name} INTERFACE cxx_std_17)

if(APPLE)

    file(
        STRINGS ${lmw_max_linker_flags_file} 
        lmw_max_linker_flags_string
    )

    file(
        STRINGS ${lmw_msp_linker_flags_file} 
        lmw_msp_linker_flags_string
    )

    set(
        lmw_max_api_linker_flags_string 
        "${lmw_max_linker_flags_string} ${lmw_msp_linker_flags_string}"
    )

    string(
        REPLACE " " ";" 
        lmw_max_api_linker_flags 
        ${lmw_max_api_linker_flags_string}
    )

    foreach(__flag ${lmw_max_api_linker_flags})
        string(REPLACE "'" "" _flag ${__flag})
        target_link_libraries(${lmw_max_api_target_name} INTERFACE ${_flag})
    endforeach(__flag)

    target_link_libraries(
        ${lmw_max_api_target_name}
        INTERFACE
        $<BUILD_INTERFACE:${lmw_mac_jitterapi_framework}>
        $<INSTALL_INTERFACE:${lmw_mac_jitterapi_installed_framework}>
    )

endif()

if(WIN32)

    target_link_libraries(
        ${lmw_max_api_target_name}
        INTERFACE
        $<BUILD_INTERFACE:${lmw_win_maxapi_lib}>
        $<INSTALL_INTERFACE:${lmw_win_installed_maxapi_lib}>
    )

    target_link_libraries(
        ${lmw_max_api_target_name}
        INTERFACE
        $<BUILD_INTERFACE:${lmw_win_mspapi_lib}>
        $<INSTALL_INTERFACE:${lmw_win_installed_mspapi_lib}>
    )

    target_link_libraries(
        ${lmw_max_api_target_name}
        INTERFACE
        $<BUILD_INTERFACE:${lmw_win_jitapi_lib}>
        $<INSTALL_INTERFACE:${lmw_win_installed_jitapi_lib}>
    )

endif()

target_link_libraries(
    ${lmw_wrapper_target_name} 
    INTERFACE 
    ${lmw_max_api_target_name}
)

file(
    GLOB lmw_helper_module_files
    ${lmw_cmake_scripts_dir}/*.cmake
)

file(
    GLOB lmw_max_api_header_files 
    "${lmw_max_api_include_dir}/*.h"
)

file(
    GLOB lmw_wrapper_header_files 
    "${lmw_wrapper_include_dir}/${lmw_wrapper_target_name}/*.h"
)

configure_file(${lmw_config_file_template} ${lmw_cmake_config_file} @ONLY)

write_basic_package_version_file(
    ${lmw_cmake_version_file} 
    VERSION ${lmw_version}
    COMPATIBILITY SameMinorVersion
)

target_include_directories(
    ${lmw_wrapper_target_name} 
    INTERFACE 
    $<BUILD_INTERFACE:${lmw_wrapper_include_dir}> 
    $<INSTALL_INTERFACE:${lmw_wrapper_install_dir}>
)

target_include_directories(
    ${lmw_max_api_target_name} 
    INTERFACE 
    $<BUILD_INTERFACE:${lmw_max_api_include_dir}> 
    $<INSTALL_INTERFACE:${lmw_max_api_install_dir}>
)

install(
    FILES               ${lmw_wrapper_header_files}
    DESTINATION         ${lmw_wrapper_install_dir}/${lmw_wrapper_target_name}
)

install(
    FILES               ${lmw_max_api_header_files}
    DESTINATION         ${lmw_max_api_install_dir}
)

install(
    FILES               ${lmw_helper_module_files}
    DESTINATION         ${lmw_package_install_dir}
)

install(
    FILES               ${lmw_info_plist_template}
    DESTINATION         ${lmw_templates_dir}
)

install(
    FILES               ${lmw_cmake_config_file}
    DESTINATION         ${lmw_package_install_dir}
)

install(
    FILES               ${lmw_cmake_version_file}
    DESTINATION         ${lmw_package_install_dir}
)

if(APPLE)
    install(
        DIRECTORY       ${lmw_mac_jitterapi_framework}
        DESTINATION     ${lmw_frameworks_install_dir}
    )
endif()

if(WIN32)
    install(
        FILES       
        ${lmw_win_maxapi_lib}
        ${lmw_win_mspapi_lib}
        ${lmw_win_jitapi_lib}
        DESTINATION     
        ${lmw_win_installed_lib_dir}
    )
endif()

install(
    TARGETS             ${lmw_wrapper_target_name} ${lmw_max_api_target_name}
    EXPORT              ${lmw_export_group}
    DESTINATION         ${lmw_package_install_dir}
)

install(
    EXPORT              ${lmw_export_group}
    NAMESPACE           ${lmw_export_namespace} 
    DESTINATION         ${lmw_package_install_dir}
)

if(${lmw_generate_docs})

	find_package(Doxygen)
		
		configure_file(${lmw_doxyfile_template} ${lmw_doxyfile} @ONLY)
	
		add_custom_target( 
			libdotadd_docs ALL
			COMMAND ${DOXYGEN_EXECUTABLE} ${lmw_doxyfile}
			WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
			COMMENT "Generating API documentation with Doxygen"
			VERBATIM 
		)
    
endif()

include(${lmw_cmake_scripts_dir}/lmw.cmake)

if(${CMAKE_GENERATOR} MATCHES "Visual Studio" 
    OR ${CMAKE_GENERATOR} MATCHES "Xcode")

    add_library(
        lmw_header_files
        MODULE
        "${lmw_wrapper_include_dir}/${lmw_wrapper_target_name}/lmw.cc"
        "${lmw_wrapper_include_dir}/${lmw_wrapper_target_name}/lmw_2.cc"
        ${lmw_wrapper_header_files}
    )

    target_link_libraries(lmw_header_files PUBLIC ${lmw_wrapper_target_name})
    
    lmw_setup_external(lmw_header_files)

endif()