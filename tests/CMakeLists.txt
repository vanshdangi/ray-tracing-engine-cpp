include(FetchContent)

cmake_policy(SET CMP0135 NEW)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
)

FetchContent_MakeAvailable(googletest)

include(GoogleTest)

function(add_ray_tracing_engine_test TEST_NAME TEST_FILE)

    add_executable(${TEST_NAME}
        ${TEST_FILE}
    )

    target_include_directories(${TEST_NAME}
        PRIVATE
            ${PROJECT_SOURCE_DIR}/include
    )

    target_link_libraries(${TEST_NAME}
        PRIVATE
            GTest::gtest_main
    )

    set_target_properties(${TEST_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests
    )

    gtest_discover_tests(${TEST_NAME})

endfunction()


# Maths

add_ray_tracing_engine_test(
    Vec3Test
    maths/vec3Tests.cpp
)