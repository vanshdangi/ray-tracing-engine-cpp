cmake_minimum_required(VERSION 3.26)

project(ray_tracing_engine VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# ------------ Compiler Warnings ------------
function(raytracer_enable_warnings TARGET_NAME)
    if(MSVC)
        target_compile_options(${TARGET_NAME} PRIVATE /W4)
    else()
        target_compile_options(${TARGET_NAME} PRIVATE -Wall -Wextra)
    endif()
endfunction()

# ---------------- Core Library ----------------
set(CORE_SOURCES
    src/scenes/demoScenes.cpp

    src/render_stats.cpp

    src/bvh/boundingBox.cpp
    
    src/rendering/camera.cpp
    src/rendering/renderer.cpp

    src/core/image.cpp

    src/world/scene.cpp
    src/world/hitRecord.cpp
    src/world/lighting.cpp
    src/world/transform.cpp

    src/world/objects/sphere.cpp
    src/world/objects/aabb.cpp
    src/world/objects/plane.cpp
    src/world/objects/triangle.cpp
    src/world/objects/mesh.cpp

    src/parser/objParser.cpp
)

add_library(raytracer_core STATIC ${CORE_SOURCES})
target_include_directories(raytracer_core PUBLIC include)
target_compile_definitions(raytracer_core PRIVATE
    PROJECT_SOURCE_DIR="${CMAKE_SOURCE_DIR}"
)
raytracer_enable_warnings(raytracer_core)

# ---------------- Executable ----------------
add_executable(ray_tracing_engine src/main.cpp)
raytracer_enable_warnings(ray_tracing_engine)

target_compile_definitions(ray_tracing_engine PRIVATE
    PROJECT_SOURCE_DIR="${CMAKE_SOURCE_DIR}"
)

# ---------------- SFML ----------------
find_package(SFML 3 REQUIRED COMPONENTS Graphics)

target_link_libraries(ray_tracing_engine
    PRIVATE
        raytracer_core
        SFML::Graphics
)

# ---------------- Tests ----------------
enable_testing()
add_subdirectory(tests)