cmake_minimum_required(VERSION 3.16)

project(MOBIUS_BRIDGE C)

enable_testing()

add_library(mobius_bridge SHARED
    src/bridge_stub.c
)

target_include_directories(mobius_bridge
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_definitions(mobius_bridge
    PRIVATE
        MOBIUS_BRIDGE_BUILD
)

set_target_properties(mobius_bridge PROPERTIES
    C_STANDARD 11
    C_STANDARD_REQUIRED YES
    C_EXTENSIONS NO

    C_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN YES

    OUTPUT_NAME "mobius_bridge"
)

add_executable(test_bridge
    tests/test_bridge.c
)

target_link_libraries(test_bridge
    PRIVATE
        mobius_bridge
)

add_test(
    NAME bridge_runtime_test
    COMMAND test_bridge
)