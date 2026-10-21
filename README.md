# C++ Ray Tracing Engine

A CPU-based 3D ray tracing engine built from scratch in C++, featuring geometric primitives, mesh loading, lighting, reflections, and Bounding Volume Hierarchy (BVH) acceleration.

The project explores the fundamentals of 3D rendering, geometric intersection algorithms, spatial acceleration structures, and performance optimization.

## Showcase

![Ray Tracing Engine — Stanford Bunny](images/stanford-bunny.png)

![Ray Tracing Engine — Reflections](images/reflections.png)

## Features

### Rendering

- **Ray tracing:** Casts rays through the scene to calculate pixel colors.
- **Camera:** Perspective camera with configurable position, orientation, field of view, and aspect ratio.
- **Geometric primitives:** Sphere, plane, triangle, and axis-aligned bounding box.
- **Triangle meshes:** Supports rendering complex 3D models composed of triangles.
- **OBJ loading:** Loads triangle mesh geometry from Wavefront OBJ files, including vertex normals and texture coordinates.
- **Object transformations:** Supports translation, rotation, and scaling using transformation matrices.
- **Lighting:** Point lights with direct Lambertian diffuse shading and ambient lighting.
- **Shadows:** Hard shadows calculated using shadow rays.
- **Reflections:** Supports reflective ray tracing.
- **Background:** Gradient background shading.
- **Supersampling:** Supports multiple samples per pixel to improve image quality.
- **Color pipeline:** Uses linear RGB internally and converts to sRGB for final output.

### Performance

- **Bounding Volume Hierarchy (BVH):** Accelerates mesh intersection queries using a recursively constructed, median-split hierarchy of axis-aligned bounding boxes.
- **Performance instrumentation:** Tracks render time, rays traced, shadow rays, triangle intersection tests, and rendering throughput.
- **Benchmarking:** Measures the impact of BVH acceleration against brute-force triangle intersection testing.

## Performance

![Suzanne rendered by the engine](images/suzanne.png)

### BVH acceleration: Brute Force vs. BVH

The following benchmark compares brute-force mesh intersection testing (`--no-bvh`) with BVH-accelerated intersection testing using the same scene and rendering settings. Triangle-test statistics use per-thread counters merged after rendering to avoid shared-counter contention.

**Test configuration**

- Build configuration: Release
- CPU: Intel Core i7-12700F (12 cores, 20 logical processors)
- Compiler: MSVC 19.38.33135
- Build system: CMake + Visual Studio 2022 (MSBuild)
- Target architecture: x64
- Resolution: 1,440 × 810
- Sampling: 16 samples per pixel (4 × 4 grid)
- Scene: Suzanne mesh containing 968 triangles and a plane
- Lighting: Point light

| Metric | Brute force (`--no-bvh`) | With BVH |
|---|---:|---:|
| Render time | 28.360 s | 1.597 s |
| Pixels rendered | 1,166,400 | 1,166,400 |
| Primary rays | 18,662,400 | 18,662,400 |
| Rays traced | 18,662,400 | 18,662,400 |
| Shadow rays | 10,792,680 | 10,792,680 |
| Triangle intersection tests | 28,512,517,440 | 136,887,961 |
| Pixels per second | 41,128.4 | 730,445 |
| Rays per second | 658,055 | 11,687,100 |
| Triangle tests per primary ray | 1,527.81 | 7.33496 |

### Results

- **17.76× faster rendering** with BVH acceleration.
- **94.37% reduction** in render time.
- **99.52% fewer triangle intersection tests.**

The brute-force implementation tests mesh triangles directly. With BVH acceleration, rays first test bounding boxes and traverse only relevant branches of the hierarchy. Triangle intersection tests are performed at leaf nodes rather than against every triangle in the mesh.

In this benchmark, BVH acceleration reduced triangle intersection tests from approximately 28.51 billion to 136.89 million while rendering the same number of pixels and tracing the same number of primary and shadow rays.

*Benchmark results are from the recorded runs and may vary with hardware, compiler settings, and system load.*

## How It Works

### 1. Ray generation

The camera generates rays through the image plane. Each ray is traced into the scene to determine which object it intersects and what color should be returned.

### 2. Intersection testing

Scene objects implement ray intersection queries. When a ray intersects an object, the engine records the intersection distance and relevant primitive information to determine the closest hit.

### 3. Shading

At a hit point, the renderer evaluates the surface material and lighting. Shadow rays determine whether the light is obstructed, while reflective rays contribute reflected color.

### 4. Mesh acceleration

Testing every triangle in a complex mesh for every ray becomes expensive. The BVH organizes triangles into a hierarchy of bounding boxes.

During traversal, the engine rejects entire subtrees when a ray misses their bounding boxes. When a leaf is reached, only its stored triangles need to be tested.

The current implementation uses median splitting along the longest axis of a node's bounding box, with up to four triangles per leaf.

## Technologies

- **C++** — Rendering engine, geometry, intersection algorithms, and acceleration structures.
- **SFML 3** — Image display.
- **Wavefront OBJ** — 3D mesh input format.

## Building and Running

### Requirements

- A C++ compiler supporting the C++ standard specified in `CMakeLists.txt`
- CMake
- SFML 3

### Windows

Install a compatible C++ compiler and SFML 3, then configure and build using CMake.

```powershell
cmake -S . -B build
cmake --build build --config Release
```

### Linux

Install a C++ compiler, CMake, and SFML 3 using your distribution's package manager.

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

### macOS

Install Xcode Command Line Tools, CMake, and SFML 3. If using Homebrew, install the required dependencies first.

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

### Selecting a Scene

The engine supports multiple demo scenes, selectable using `--scene`.

| Scene | Description |
|---|---|
| `bunny` | Stanford Bunny mesh showcase |
| `reflections` | Demonstrates recursive reflections |
| `benchmark` | Suzanne mesh benchmark scene |

If no scene is specified, `benchmark` is used by default.

### Command-Line Options

| Option | Description | Default |
|---|---|---|
| `--scene <name>` | Select `bunny`, `reflections`, or `benchmark` | `benchmark` |
| `--output <file>` | Specify the output PNG filename | `render.png` |
| `--width <pixels>` | Set the render width | `1440` |
| `--height <pixels>` | Set the render height | `810` |
| `--spp <count>` | Set the number of samples per pixel | `16` |
| `--no-bvh` | Disable BVH acceleration and use brute-force triangle intersection testing | Enabled by default |
| `--headless` | Render and save the image without opening an SFML window | Disabled |

Width and height can be configured independently. All dimensions and the samples-per-pixel value must be positive integers.

### Running the Engine

The following examples assume the executable is run from the repository root.

**Windows (PowerShell)**

```powershell
.\build\Release\ray_tracing_engine.exe --scene bunny --output bunny.png
.\build\Release\ray_tracing_engine.exe --scene reflections --output reflections.png
.\build\Release\ray_tracing_engine.exe --scene benchmark --output benchmark.png
```

**Linux**

```bash
./build/ray_tracing_engine --scene bunny --output bunny.png
./build/ray_tracing_engine --scene reflections --output reflections.png
./build/ray_tracing_engine --scene benchmark --output benchmark.png
```

**macOS**

```bash
./build/ray_tracing_engine --scene bunny --output bunny.png
./build/ray_tracing_engine --scene reflections --output reflections.png
./build/ray_tracing_engine --scene benchmark --output benchmark.png
```

By default, the engine renders the selected scene, saves it as a PNG, and displays the result in an SFML window. Close the window to exit.

### Headless Rendering

Use `--headless` to render and save an image without creating a display window. This is useful for automated benchmarks, scripts, CI pipelines, and environments without a graphical display.

```powershell
.\build\Release\ray_tracing_engine.exe --scene benchmark --headless
.\build\Release\ray_tracing_engine.exe --scene bunny --headless --output bunny.png
```

The program saves the image and exits automatically after rendering.

### Configuring Resolution and Samples

Use `--width` and `--height` to change the render resolution, and `--spp` to configure samples per pixel.

```powershell
.\build\Release\ray_tracing_engine.exe --scene benchmark --width 800 --height 600 --spp 4
```

For a headless render at a custom resolution and sample count:

```powershell
.\build\Release\ray_tracing_engine.exe --scene reflections --headless --width 1920 --height 1080 --spp 16 --output reflections.png
```

Higher sample counts can improve antialiasing quality at the cost of additional rendering time.

### Benchmarking

The `--no-bvh` argument disables BVH acceleration and uses brute-force triangle intersection testing. This allows you to compare rendering performance with and without BVH acceleration.

**With BVH:**

```powershell
.\build\Release\ray_tracing_engine.exe --scene benchmark --headless --output bvh.png
```

**Without BVH:**

```powershell
.\build\Release\ray_tracing_engine.exe --scene benchmark --headless --no-bvh --output no-bvh.png
```

Run both commands with the same build configuration, resolution, samples per pixel, and scene settings for a meaningful comparison. The engine reports render time, ray counts, triangle intersection statistics, and throughput after rendering.


## Running Tests

Execute the complete test suite:

```bash
ctest --test-dir build --output-on-failure
```

## Project Goals

This project was developed to explore and implement the fundamentals of a ray tracing renderer, with an emphasis on understanding the underlying mathematics and algorithms rather than relying entirely on a prebuilt rendering framework.

Key areas of focus include:

- 3D geometry and coordinate transformations.
- Ray-object intersection algorithms.
- Lighting, shadows, and reflections.
- Recursive spatial acceleration structures.
- Performance measurement and optimization.

## License

Licensed under the terms in `LICENSE`.
