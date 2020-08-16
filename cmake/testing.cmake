function(custom_cxx_test target source)
    # adjust names a generated cpp file
    get_filename_component(CPP_FILE_NAME ${source} NAME)
    string(REGEX REPLACE "h$|hpp$" "cpp" CPP_FILE_NAME ${CPP_FILE_NAME})
    set(CPP_FULL_NAME "${CMAKE_CURRENT_BINARY_DIR}/${CPP_FILE_NAME}")

    add_custom_command(
            OUTPUT "${CPP_FULL_NAME}"
            COMMAND cxxtestgen --runner=ErrorPrinter --output "${CPP_FULL_NAME}" "${source}"
            DEPENDS "${source}"
    )
    add_executable(${target} ${CPP_FULL_NAME})
    set_target_properties(${target} PROPERTIES COMPILE_FLAGS "-Wno-effc++")

    # INCLUDES, LIBS and DEFS are given from the global scope defined above
    target_include_directories(${target} PUBLIC ${TEST_INCLUDES})
    target_link_libraries(${target} ${TEST_LIBS})

    add_test(NAME "${target}" COMMAND ${target})
endfunction(custom_cxx_test)