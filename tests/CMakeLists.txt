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
        ${ARGN}
    )

    target_include_directories(${TEST_NAME}
        PRIVATE
            ${PROJECT_SOURCE_DIR}/include
    )

    target_compile_definitions(${TEST_NAME} PRIVATE
        PROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
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
    ${PROJECT_SOURCE_DIR}/src/world/objects/sphere.cpp
    ${PROJECT_SOURCE_DIR}/src/world/transform.cpp
)
add_ray_tracing_engine_test(
    PlaneTest
    objects/planeTests.cpp
    ${PROJECT_SOURCE_DIR}/src/world/objects/plane.cpp
    ${PROJECT_SOURCE_DIR}/src/world/transform.cpp
)
add_ray_tracing_engine_test(
    AABBTest
    objects/aabbTests.cpp
    ${PROJECT_SOURCE_DIR}/src/world/objects/aabb.cpp
    ${PROJECT_SOURCE_DIR}/src/world/transform.cpp
)
add_ray_tracing_engine_test(
    BoundingBoxTest
    objects/boundingBoxTests.cpp
    ${PROJECT_SOURCE_DIR}/src/bvh/boundingBox.cpp
)
add_ray_tracing_engine_test(
    TriangleTest
    objects/triangleTests.cpp
    ${PROJECT_SOURCE_DIR}/src/world/objects/triangle.cpp
    ${PROJECT_SOURCE_DIR}/src/world/transform.cpp
)
add_ray_tracing_engine_test(
    MeshTest
    objects/meshTests.cpp
    ${PROJECT_SOURCE_DIR}/src/world/objects/mesh.cpp
    ${PROJECT_SOURCE_DIR}/src/world/objects/triangle.cpp
    ${PROJECT_SOURCE_DIR}/src/world/transform.cpp
    ${PROJECT_SOURCE_DIR}/src/bvh/boundingBox.cpp
    ${PROJECT_SOURCE_DIR}/src/render_stats.cpp
)

#Rendering
add_ray_tracing_engine_test(
    CameraTest
    rendering/cameraTests.cpp
    ${PROJECT_SOURCE_DIR}/src/rendering/camera.cpp
)
add_ray_tracing_engine_test(
    RendererTest
    rendering/rendererTests.cpp
    ${PROJECT_SOURCE_DIR}/src/rendering/renderer.cpp
    ${PROJECT_SOURCE_DIR}/src/rendering/camera.cpp
    ${PROJECT_SOURCE_DIR}/src/core/image.cpp
    ${PROJECT_SOURCE_DIR}/src/world/scene.cpp
    ${PROJECT_SOURCE_DIR}/src/world/hitRecord.cpp
    ${PROJECT_SOURCE_DIR}/src/world/lighting.cpp
    ${PROJECT_SOURCE_DIR}/src/world/objects/sphere.cpp
    ${PROJECT_SOURCE_DIR}/src/world/transform.cpp
    ${PROJECT_SOURCE_DIR}/src/render_stats.cpp
)
add_ray_tracing_engine_test(
    RenderingCorrectnessTest
    rendering/renderingCorrectnessTests.cpp
    ${PROJECT_SOURCE_DIR}/src/rendering/renderer.cpp
    ${PROJECT_SOURCE_DIR}/src/rendering/camera.cpp
    ${PROJECT_SOURCE_DIR}/src/core/image.cpp
    ${PROJECT_SOURCE_DIR}/src/world/scene.cpp
    ${PROJECT_SOURCE_DIR}/src/world/hitRecord.cpp
    ${PROJECT_SOURCE_DIR}/src/world/lighting.cpp
    ${PROJECT_SOURCE_DIR}/src/world/objects/sphere.cpp
    ${PROJECT_SOURCE_DIR}/src/world/objects/mesh.cpp
    ${PROJECT_SOURCE_DIR}/src/world/objects/triangle.cpp
    ${PROJECT_SOURCE_DIR}/src/world/transform.cpp
    ${PROJECT_SOURCE_DIR}/src/bvh/boundingBox.cpp
    ${PROJECT_SOURCE_DIR}/src/render_stats.cpp
)

#Parser
add_ray_tracing_engine_test(
    OBJParserTest
    parser/objParserTests.cpp
    ${PROJECT_SOURCE_DIR}/src/parser/objParser.cpp
    ${PROJECT_SOURCE_DIR}/src/world/objects/triangle.cpp
)

#World
add_ray_tracing_engine_test(
    TransformTest
    world/transformTests.cpp
    ${PROJECT_SOURCE_DIR}/src/world/transform.cpp
)