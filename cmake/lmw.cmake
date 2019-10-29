macro(_lmw_setup_macos_bundle)

    if(NOT DEFINED _lmw_BUNDLE_IDENTIFIER)
        set(_lmw_BUNDLE_IDENTIFIER myexternal)
    endif()

    if(NOT DEFINED _lmw_BUNDLE_IDENTIFIER)
        set(_lmw_BUNDLE_AUTHOR_DOMAIN com.acme.inc)
    endif()

    if(NOT DEFINED _lmw_BUNDLE_COPYRIGHT)
        set(_lmw_BUNDLE_COPYRIGHT "(c) 2019 MyCompany")
    endif()

    if(NOT DEFINED _lmw_EXCLUDE_FROM_COLLECTIVES)
        set(_lmw_EXCLUDE_FROM_COLLECTIVES False)
    endif()

    set(_lmw_macos_bundle_plist_file ${CMAKE_CURRENT_BINARY_DIR}/Info.plist)
        
    if(${_lmw_EXCLUDE_FROM_COLLECTIVES})
        set(EXCLUDE_FROM_COLLECTIVES yes)
    else()
        set(EXCLUDE_FROM_COLLECTIVES no)
    endif()

    set(AUTHOR_DOMAIN               ${_lmw_BUNDLE_AUTHOR_DOMAIN})
    set(BUNDLE_IDENTIFIER           ${_lmw_BUNDLE_IDENTIFIER})
    set(COPYRIGHT_STRING            ${_lmw_BUNDLE_COPYRIGHT})

    string(REPLACE "." ";" _lmw_temp_version_list ${_lmw_VERSION})

    list(GET _lmw_temp_version_list 0 GIT_VERSION_MAJ)
    list(GET _lmw_temp_version_list 1 GIT_VERSION_MIN)
    list(GET _lmw_temp_version_list 2 GIT_VERSION_SUB)

    message(STATUS "Setting up External: ${_lmw_target_name} version: ${_lmw_VERSION}")

    configure_file(${lmw_info_plist_template} ${_lmw_macos_bundle_plist_file} @ONLY)

    set_target_properties(
        ${_lmw_target_name}                 PROPERTIES 
        BUNDLE                              True
        BUNDLE_EXTENSION                    "mxo"
        XCODE_ATTRIBUTE_WRAPPER_EXTENSION   "mxo"
        MACOSX_BUNDLE_BUNDLE_VERSION        ${_lmw_VERSION}
        MACOSX_BUNDLE_INFO_PLIST            ${_lmw_macos_bundle_plist_file}
        LIBRARY_OUTPUT_DIRECTORY            ${_lmw_EXTERNAL_OUTPUT_DIR}
    )

endmacro(_lmw_setup_macos_bundle)

macro(_lmw_setup_win_dll)

    set_target_properties(
        ${_lmw_target_name}                 PROPERTIES
        SUFFIX                              ".mxe64"
        LIBRARY_OUTPUT_DIRECTORY            ${_lmw_EXTERNAL_OUTPUT_DIR}
    )

    target_compile_definitions(
        ${_lmw_target_name}                 
        PUBLIC
        MAXAPI_USE_MSCRT
        WIN_VERSION
        _USE_MATH_DEFINES
    )
    
endmacro(_lmw_setup_win_dll)



function(lmw_setup_external _lmw_target_name)

    set(
        _lmw_oneval_keywords
        VERSION
        EXTERNAL_OUTPUT_DIR
        BUNDLE_IDENTIFIER
        BUNDLE_AUTHOR_DOMAIN
        BUNDLE_COPYRIGHT
        EXCLUDE_FROM_COLLECTIVES
    )

    set(_lmw_parse_options)
    set(_lmw_parse_multival_keywords)

    cmake_parse_arguments(
        _lmw
        "${_lmw_parse_options}"
        "${_lmw_oneval_keywords}"
        "${_lmw_parse_multival_keywords}"
        ${ARGN}
    )

    if(NOT DEFINED _lmw_VERSION)
        set(_lmw_VERSION 1.0.0)
    endif()

    if(NOT DEFINED _lmw_EXTERNAL_OUTPUT_DIR)
        set(_lmw_EXTERNAL_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/externals)
    endif()

    if(APPLE)
        _lmw_setup_macos_bundle()
    else()
        _lmw_setup_win_dll()
    endif()
    
endfunction()