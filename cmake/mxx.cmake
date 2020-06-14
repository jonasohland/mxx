function(_mxx_get_external_name _name _out_var)
    string(REGEX REPLACE "^([A-z._]+)_tilde$" "\\1~" _out ${_name})
    string(REGEX REPLACE "^([A-z._]+)_tld$" "\\1~" _out ${_out})
    set(${_out_var} ${_out} PARENT_SCOPE)
endfunction()

macro(_mxx_setup_macos_bundle)

    if(NOT DEFINED _mxx_BUNDLE_IDENTIFIER)
        set(_mxx_BUNDLE_IDENTIFIER myexternal)
    endif()

    if(NOT DEFINED _mxx_BUNDLE_IDENTIFIER)
        set(_mxx_BUNDLE_AUTHOR_DOMAIN com.acme.inc)
    endif()

    if(NOT DEFINED _mxx_BUNDLE_COPYRIGHT)
        set(_mxx_BUNDLE_COPYRIGHT "(c) 2019 MyCompany")
    endif()

    if(NOT DEFINED _mxx_EXCLUDE_FROM_COLLECTIVES)
        set(_mxx_EXCLUDE_FROM_COLLECTIVES False)
    endif()

    set(_mxx_macos_bundle_plist_file ${CMAKE_CURRENT_BINARY_DIR}/Info.plist)
        
    if(${_mxx_EXCLUDE_FROM_COLLECTIVES})
        set(EXCLUDE_FROM_COLLECTIVES yes)
    else()
        set(EXCLUDE_FROM_COLLECTIVES no)
    endif()

    set(AUTHOR_DOMAIN               ${_mxx_BUNDLE_AUTHOR_DOMAIN})
    set(BUNDLE_IDENTIFIER           ${_mxx_BUNDLE_IDENTIFIER})
    set(COPYRIGHT_STRING            ${_mxx_BUNDLE_COPYRIGHT})

    string(REPLACE "." ";" _mxx_temp_version_list ${_mxx_VERSION})

    list(GET _mxx_temp_version_list 0 GIT_VERSION_MAJ)
    list(GET _mxx_temp_version_list 1 GIT_VERSION_MIN)
    list(GET _mxx_temp_version_list 2 GIT_VERSION_SUB)

    message(STATUS "Setting up External: ${_mxx_external_name} version: ${_mxx_VERSION}")

    configure_file(${mxx_info_plist_template} ${_mxx_macos_bundle_plist_file} @ONLY)

    set_target_properties(
        ${_mxx_target_name}                 PROPERTIES 
        BUNDLE                              True
        BUNDLE_EXTENSION                    "mxo"
        XCODE_ATTRIBUTE_WRAPPER_EXTENSION   "mxo"
        MACOSX_BUNDLE_BUNDLE_VERSION        ${_mxx_VERSION}
        MACOSX_BUNDLE_INFO_PLIST            ${_mxx_macos_bundle_plist_file}

    )

endmacro(_mxx_setup_macos_bundle)

macro(_mxx_setup_win_dll)

    set_target_properties(
        ${_mxx_target_name}                 PROPERTIES
        SUFFIX                              ".mxe64"
    )

    target_compile_definitions(
        ${_mxx_target_name}                 
        PUBLIC
        MAXAPI_USE_MSCRT
        WIN_VERSION
        _USE_MATH_DEFINES
    )
    
endmacro(_mxx_setup_win_dll)

function(mxx_setup_external _mxx_target_name)

    set(
        _mxx_oneval_keywords
        VERSION
        EXTERNAL_OUTPUT_DIR
        BUNDLE_IDENTIFIER
        BUNDLE_AUTHOR_DOMAIN
        BUNDLE_COPYRIGHT
        EXCLUDE_FROM_COLLECTIVES
    )

    set(_mxx_parse_options)
    set(_mxx_parse_multival_keywords)

    cmake_parse_arguments(
        _mxx
        "${_mxx_parse_options}"
        "${_mxx_oneval_keywords}"
        "${_mxx_parse_multival_keywords}"
        ${ARGN}
    )

    if(NOT DEFINED _mxx_VERSION)
        set(_mxx_VERSION 1.0.0)
    endif()

    if(NOT DEFINED _mxx_EXTERNAL_OUTPUT_DIR)
        set(_mxx_EXTERNAL_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/externals)
    endif()

    _mxx_get_external_name(${_mxx_target_name} _mxx_external_name)

    if(APPLE)
        _mxx_setup_macos_bundle()
    else()
        _mxx_setup_win_dll()
    endif()

    set_target_properties(
        ${_mxx_target_name}                 PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY_DEBUG      ${_mxx_EXTERNAL_OUTPUT_DIR}
        LIBRARY_OUTPUT_DIRECTORY_RELEASE    ${_mxx_EXTERNAL_OUTPUT_DIR}
        LIBRARY_OUTPUT_NAME_DEBUG           ${_mxx_external_name}
        LIBRARY_OUTPUT_NAME_RELEASE         ${_mxx_external_name}
    )

    target_compile_definitions(
        ${_mxx_target_name}                 
        PRIVATE
        MXX_UNIVERSAL_HELPER_NAME=__mxx_${_mxx_target_name}_helper
    )
    
endfunction()