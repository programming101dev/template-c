# Standalone Unity test tree — intentionally SEPARATE from the project's main
# CMakeLists so vendored Unity and test code are NOT subjected to the project's
# strict clang-tidy / cppcheck / clang-format / -Werror pipeline. Run via
# ../test.sh, which configures this with the same compiler as the main build.
cmake_minimum_required(VERSION 3.14)
project(p101_tests C)

enable_testing()

# Vendored Unity, compiled leniently (third-party code, not ours to lint).
add_library(unity STATIC unity/unity.c)
target_include_directories(unity PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/unity")
target_compile_options(unity PRIVATE -w)

set(P101_PUBLIC_INCLUDE_DIRS "" CACHE STRING "Extra p101 include dirs")
set(P101_PUBLIC_LINK_DIRS "" CACHE STRING "Extra p101 link dirs")
separate_arguments(P101_PUBLIC_INCLUDE_DIRS_LIST NATIVE_COMMAND "${P101_PUBLIC_INCLUDE_DIRS}")
separate_arguments(P101_PUBLIC_LINK_DIRS_LIST NATIVE_COMMAND "${P101_PUBLIC_LINK_DIRS}")
list(APPEND P101_PUBLIC_INCLUDE_DIRS_LIST /usr/local/include)
list(APPEND P101_PUBLIC_LINK_DIRS_LIST /usr/local/lib /usr/local/lib64)
if (APPLE)
    list(APPEND P101_PUBLIC_INCLUDE_DIRS_LIST /opt/homebrew/include /opt/local/include)
    list(APPEND P101_PUBLIC_LINK_DIRS_LIST /opt/homebrew/lib /opt/local/lib)
endif ()

# Optional coverage of the code under test (test.sh --coverage passes this).
option(P101_TEST_COVERAGE "Instrument the code under test for coverage" OFF)
set(_P101_COV "")
if (P101_TEST_COVERAGE)
    set(_P101_COV --coverage)
endif ()

# The production sources to exercise (everything the tests call, minus main.c).
set(P101_CODE_UNDER_TEST
        "${CMAKE_CURRENT_SOURCE_DIR}/../src/display.c"
)

# Register a test executable: p101_add_test(<name> <test-source>...)
function(p101_add_test name)
    add_executable(${name} ${ARGN} ${P101_CODE_UNDER_TEST})
    target_include_directories(${name} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../include")
    target_include_directories(${name} PRIVATE ${P101_PUBLIC_INCLUDE_DIRS_LIST})
    target_link_directories(${name} PRIVATE ${P101_PUBLIC_LINK_DIRS_LIST})
    target_link_libraries(${name} PRIVATE unity p101_error p101_env p101_c p101_io)
    # A p101 library can itself depend on another p101 shared library. Keep the
    # complete admitted search path in the test executable so those transitive
    # @rpath dependencies remain resolvable in a copied standalone project.
    set_target_properties(${name} PROPERTIES BUILD_RPATH "${P101_PUBLIC_LINK_DIRS_LIST}")
    target_compile_options(${name} PRIVATE ${_P101_COV})
    target_link_options(${name} PRIVATE ${_P101_COV})
    add_test(NAME ${name} COMMAND ${name})
endfunction()

p101_add_test(test_display test_display.c)
