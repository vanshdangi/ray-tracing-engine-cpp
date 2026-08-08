cmake_minimum_required(VERSION 3.26)
project(atlas_ai LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ---------------- Sources ----------------
set(SOURCES
    src/main.cpp
)

# ---------------- Executable ----------------
add_executable(ray_tracing_engine
    ${SOURCES}
)

# ---------------- Includes ----------------
target_include_directories(ray_tracing_engine
    PRIVATE
        include
)