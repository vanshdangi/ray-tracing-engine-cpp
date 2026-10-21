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

    target_compile_definitions(${TEST_NAME} PRIVATE
        PROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
    )

    target_link_libraries(${TEST_NAME}
        PRIVATE
            raytracer_core
            GTest::gtest_main
    )

    raytracer_enable_warnings(${TEST_NAME})

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
add_ray_tracing_engine_test(
    Point3Test
    maths/point3Tests.cpp
)

#Core
add_ray_tracing_engine_test(
    ColorTest
    core/colorTests.cpp
)
add_ray_tracing_engine_test(
    RayTest
    core/rayTests.cpp
)

#Objects
add_ray_tracing_engine_test(
    SphereTest
    objects/sphereTests.cpp
)
add_ray_tracing_engine_test(
    PlaneTest
    objects/planeTests.cpp
)
add_ray_tracing_engine_test(
    AABBTest
    objects/aabbTests.cpp
)
add_ray_tracing_engine_test(
    BoundingBoxTest
    objects/boundingBoxTests.cpp
)
add_ray_tracing_engine_test(
    TriangleTest
    objects/triangleTests.cpp
)
add_ray_tracing_engine_test(
    MeshTest
    objects/meshTests.cpp
)

#Rendering
add_ray_tracing_engine_test(
    CameraTest
    rendering/cameraTests.cpp
)
add_ray_tracing_engine_test(
    RendererTest
    rendering/rendererTests.cpp
)
add_ray_tracing_engine_test(
    RenderingCorrectnessTest
    rendering/renderingCorrectnessTests.cpp
)

#Parser
add_ray_tracing_engine_test(
    OBJParserTest
    parser/objParserTests.cpp
)

#World
add_ray_tracing_engine_test(
    TransformTest
    world/transformTests.cpp
)