cmake_minimum_required(VERSION 3.4)

project(mxx VERSION 0.0.1 LANGUAGES CXX)

option(mxx_generate_docs "Generate Documentation" OFF)

include(CMakePackageConfigHelpers)

set(mxx_version_major           0)
set(mxx_version_minor           0)
set(mxx_version_patch           1)

set(mxx_version                 ${mxx_version_major}.${mxx_version_minor}.${mxx_version_patch})

set(mxx_wrapper_target_name     mxx)
set(mxx_max_api_target_name     maxapi)
set(mxx_export_namespace        mxx::)
set(mxx_export_group            mxx_export)

set(C74_MAX_API_DIR             ${CMAKE_CURRENT_LIST_DIR}/c74/max-api)
set(C74_MAX_API_LIB_DIR         ${C74_MAX_API_DIR}/lib)
set(C74_MAX_API_SCRIPT_DIR      ${C74_MAX_API_DIR}/script)

set(mxx_max_linker_flags_file   ${C74_MAX_API_SCRIPT_DIR}/max-linker-flags.txt)
set(mxx_msp_linker_flags_file   ${C74_MAX_API_SCRIPT_DIR}/msp-linker-flags.txt)

set(mxx_root_install_dir        ${CMAKE_INSTALL_PREFIX})
set(mxx_package_install_dir     ${mxx_root_install_dir}/lib/cmake/${mxx_wrapper_target_name}-${mxx_version})
set(mxx_templates_dir           ${mxx_package_install_dir}/templates)

set(mxx_cmake_scripts_dir       ${CMAKE_CURRENT_LIST_DIR}/cmake)

set(mxx_config_file_template    ${mxx_cmake_scripts_dir}/mxx-config.cmake.in)
set(mxx_cmake_config_file       ${CMAKE_CURRENT_BINARY_DIR}/install_files/mxx-config.cmake)
set(mxx_cmake_version_file      ${CMAKE_CURRENT_BINARY_DIR}/install_files/mxx-config-version.cmake)

set(mxx_max_api_include_dir     ${C74_MAX_API_DIR}/include)
set(mxx_wrapper_include_dir     ${CMAKE_CURRENT_LIST_DIR}/include)

set(mxx_include_install_dir     ${mxx_root_install_dir}/include/${mxx_wrapper_target_name}-${mxx_version})

set(mxx_max_api_install_dir     ${mxx_include_install_dir}/max-api)
set(mxx_wrapper_install_dir     ${mxx_include_install_dir}/${mxx_wrapper_target_name})

set(mxx_doxyfile_template       ${CMAKE_CURRENT_LIST_DIR}/docs/Doxyfile.in)
set(mxx_doxyfile                ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

if(APPLE)

    set(mxx_frameworks_install_dir              ${mxx_root_install_dir}/Frameworks/${mxx_wrapper_target_name}-${mxx_version})
    set(mxx_mac_jitterapi_framework             ${C74_MAX_API_LIB_DIR}/mac/JitterAPI.framework)
    set(mxx_mac_jitterapi_installed_framework   ${mxx_frameworks_install_dir}/JitterAPI.framework)

    set(mxx_info_plist_template                 ${C74_MAX_API_SCRIPT_DIR}/Info.plist.in)
    set(mxx_installed_info_plist_template       ${mxx_templates_dir}/Info.plist.in)    

endif()

if(WIN32)

    if("${CMAKE_SIZEOF_VOID_P}" STREQUAL "4")
        set(mxx_win_lib_dir                     ${C74_MAX_API_LIB_DIR}/win32)
    else()
        set(mxx_win_lib_dir                     ${C74_MAX_API_LIB_DIR}/win64)   
    endif()

    set(mxx_win_installed_lib_dir               ${mxx_root_install_dir}/lib)

    set(mxx_win_maxapi_lib                      ${mxx_win_lib_dir}/MaxAPI.lib)
    set(mxx_win_mspapi_lib                      ${mxx_win_lib_dir}/MaxAudio.lib)
    set(mxx_win_jitapi_lib                      ${mxx_win_lib_dir}/jitlib.lib)

    set(mxx_win_installed_maxapi_lib            ${mxx_win_installed_lib_dir}/MaxAPI.lib)
    set(mxx_win_installed_mspapi_lib            ${mxx_win_installed_lib_dir}/MaxAudio.lib)
    set(mxx_win_installed_jitapi_lib            ${mxx_win_installed_lib_dir}/jitlib.lib)

endif()

add_library(${mxx_wrapper_target_name} INTERFACE)
add_library(${mxx_max_api_target_name} INTERFACE)

target_compile_features(${mxx_wrapper_target_name} INTERFACE cxx_std_17)

if(APPLE)

    file(
        STRINGS ${mxx_max_linker_flags_file} 
        mxx_max_linker_flags_string
    )

    file(
        STRINGS ${mxx_msp_linker_flags_file} 
        mxx_msp_linker_flags_string
    )

    set(
        mxx_max_api_linker_flags_string 
        "${mxx_max_linker_flags_string} ${mxx_msp_linker_flags_string}"
    )

    string(
        REPLACE " " ";" 
        mxx_max_api_linker_flags 
        ${mxx_max_api_linker_flags_string}
    )

    foreach(__flag ${mxx_max_api_linker_flags})
        string(REPLACE "'" "" _flag ${__flag})
        target_link_libraries(${mxx_max_api_target_name} INTERFACE ${_flag})
    endforeach(__flag)

    target_link_libraries(
        ${mxx_max_api_target_name}
        INTERFACE
        $<BUILD_INTERFACE:${mxx_mac_jitterapi_framework}>
        $<INSTALL_INTERFACE:${mxx_mac_jitterapi_installed_framework}>
    )

endif()

if(WIN32)

    target_link_libraries(
        ${mxx_max_api_target_name}
        INTERFACE
        $<BUILD_INTERFACE:${mxx_win_maxapi_lib}>
        $<INSTALL_INTERFACE:${mxx_win_installed_maxapi_lib}>
    )

    target_link_libraries(
        ${mxx_max_api_target_name}
        INTERFACE
        $<BUILD_INTERFACE:${mxx_win_mspapi_lib}>
        $<INSTALL_INTERFACE:${mxx_win_installed_mspapi_lib}>
    )

    target_link_libraries(
        ${mxx_max_api_target_name}
        INTERFACE
        $<BUILD_INTERFACE:${mxx_win_jitapi_lib}>
        $<INSTALL_INTERFACE:${mxx_win_installed_jitapi_lib}>
    )

endif()

target_link_libraries(
    ${mxx_wrapper_target_name} 
    INTERFACE 
    ${mxx_max_api_target_name}
)

file(
    GLOB mxx_helper_module_files
    ${mxx_cmake_scripts_dir}/*.cmake
)

file(
    GLOB mxx_max_api_header_files 
    "${mxx_max_api_include_dir}/*.h"
)

file(
    GLOB mxx_wrapper_header_files 
    "${mxx_wrapper_include_dir}/${mxx_wrapper_target_name}/*.h"
)

configure_file(${mxx_config_file_template} ${mxx_cmake_config_file} @ONLY)

write_basic_package_version_file(
    ${mxx_cmake_version_file} 
    VERSION ${mxx_version}
    COMPATIBILITY SameMinorVersion
)

target_include_directories(
    ${mxx_wrapper_target_name} 
    INTERFACE 
    $<BUILD_INTERFACE:${mxx_wrapper_include_dir}> 
    $<INSTALL_INTERFACE:${mxx_wrapper_install_dir}>
)

target_include_directories(
    ${mxx_max_api_target_name} 
    INTERFACE 
    $<BUILD_INTERFACE:${mxx_max_api_include_dir}> 
    $<INSTALL_INTERFACE:${mxx_max_api_install_dir}>
)

install(
    FILES               ${mxx_wrapper_header_files}
    DESTINATION         ${mxx_wrapper_install_dir}/${mxx_wrapper_target_name}
)

install(
    FILES               ${mxx_max_api_header_files}
    DESTINATION         ${mxx_max_api_install_dir}
)

install(
    FILES               ${mxx_helper_module_files}
    DESTINATION         ${mxx_package_install_dir}
)

install(
    FILES               ${mxx_info_plist_template}
    DESTINATION         ${mxx_templates_dir}
)

install(
    FILES               ${mxx_cmake_config_file}
    DESTINATION         ${mxx_package_install_dir}
)

install(
    FILES               ${mxx_cmake_version_file}
    DESTINATION         ${mxx_package_install_dir}
)

if(APPLE)
    install(
        DIRECTORY       ${mxx_mac_jitterapi_framework}
        DESTINATION     ${mxx_frameworks_install_dir}
    )
endif()

if(WIN32)
    install(
        FILES       
        ${mxx_win_maxapi_lib}
        ${mxx_win_mspapi_lib}
        ${mxx_win_jitapi_lib}
        DESTINATION     
        ${mxx_win_installed_lib_dir}
    )
endif()

install(
    TARGETS             ${mxx_wrapper_target_name} ${mxx_max_api_target_name}
    EXPORT              ${mxx_export_group}
    DESTINATION         ${mxx_package_install_dir}
)

install(
    EXPORT              ${mxx_export_group}
    NAMESPACE           ${mxx_export_namespace} 
    DESTINATION         ${mxx_package_install_dir}
)

if(${mxx_generate_docs})

	find_package(Doxygen)
		
		configure_file(${mxx_doxyfile_template} ${mxx_doxyfile} @ONLY)
	
		add_custom_target( 
			mxx_docs ALL
			COMMAND ${DOXYGEN_EXECUTABLE} ${mxx_doxyfile}
			WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
			COMMENT "Generating API documentation with Doxygen"
			VERBATIM 
		)
    
endif()

include(${mxx_cmake_scripts_dir}/mxx.cmake)

if(${CMAKE_GENERATOR} MATCHES "Visual Studio" 
    OR ${CMAKE_GENERATOR} MATCHES "Xcode")

    add_library(
        mxx_header_files
        MODULE
        "${mxx_wrapper_include_dir}/${mxx_wrapper_target_name}/mxx.cc"
        "${mxx_wrapper_include_dir}/${mxx_wrapper_target_name}/mxx_2.cc"
        ${mxx_wrapper_header_files}
    )

    target_link_libraries(mxx_header_files PUBLIC ${mxx_wrapper_target_name})
    
    mxx_setup_external(mxx_header_files)

endif()