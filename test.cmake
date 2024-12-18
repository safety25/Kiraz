
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

include(GoogleTest)

#
# tests
#

# test_parser
add_executable(test_parser kiraz/test/test_parser.cc)
target_link_libraries(test_parser kiraz GTest::gtest_main ${FLEX_LIBRARIES})
gtest_discover_tests(test_parser)

# test_semantics
add_executable(test_semantics kiraz/test/test_semantics.cc)
target_link_libraries(test_semantics kiraz GTest::gtest_main ${FLEX_LIBRARIES})
gtest_discover_tests(test_semantics)


# test_wasmgen
option(KIRAZ_TEST_WASMGEN "Enable wasmgen tests" TRUE)

if (KIRAZ_TEST_WASMGEN)
    add_executable(test_wasmgen kiraz/test/test_wasmgen.cc
        ${FLEX_KIRAZ_LEXER_OUTPUTS} ${BISON_KIRAZ_PARSER_OUTPUTS}
    )

    ## test_wasmgen: common
    target_link_libraries(test_wasmgen GTest::gtest ${FLEX_LIBRARIES} kiraz)
    gtest_discover_tests(test_wasmgen)

    ## test_wasmgen: wabt integration
    include(wabt.cmake)
    target_include_directories(test_wasmgen SYSTEM PUBLIC ${WABT_INCLUDE_DIRS})
    target_link_libraries(test_wasmgen ${WABT_STATIC_LIBRARIES})
    add_dependencies(test_wasmgen wabt)

    ## test_wasmgen: mozjs integration
    option(KIRAZ_TEST_WASMGEN_MOZJS "Enable wasmgen tests using spidermonkey" FALSE)

    if (KIRAZ_TEST_WASMGEN_MOZJS)
        find_package(PkgConfig REQUIRED)
        pkg_check_modules(MOZJS REQUIRED mozjs-115)

        # mozjs integration
        add_library(mozjs-i9n STATIC
            kiraz/test/wasm.h
            kiraz/test/wasm.cc
        )

        target_link_libraries(mozjs-i9n ${MOZJS_LIBRARIES})
        target_include_directories(mozjs-i9n SYSTEM PUBLIC ${MOZJS_INCLUDE_DIRS})
        #target_compile_definitions(mozjs-i9n PUBLIC DEBUG)

        target_include_directories(test_wasmgen SYSTEM PUBLIC ${MOZJS_INCLUDE_DIRS})
        target_link_libraries(test_wasmgen mozjs-i9n ${MOZJS_LIBRARIES})
        target_compile_definitions(test_wasmgen PRIVATE KIRAZ_HAVE_MOZJS)

    endif()
endif()
