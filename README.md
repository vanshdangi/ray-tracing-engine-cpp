# C++ Ray Tracing Engine

[![C++ CI](https://github.com/vanshdangi/ray-tracing-engine-cpp/actions/workflows/ci.yml/badge.svg)](https://github.com/vanshdangi/ray-tracing-engine-cpp/actions/workflows/ci.yml)

A C++20 CPU ray tracer for rendering 3D scenes built from geometric primitives and Wavefront OBJ triangle meshes. It features perspective cameras, diffuse and ambient lighting, hard shadows, reflections, supersampling, and an optional median-split bounding volume hierarchy (BVH) for accelerating mesh intersections. SFML displays rendered images.

## Highlights

- **BVH acceleration:** 17.76× faster rendering and 99.52% fewer triangle tests on the benchmark scene (1,440 × 810, 16 samples per pixel, Intel Core i7-12700F, MSVC Release). See [Recorded BVH results](#recorded-bvh-results).
- **Real meshes:** renders the 69,451-triangle Stanford Bunny with the BVH enabled.
- **Built from scratch:** vector and matrix math, intersection routines, BVH construction and traversal, the OBJ loader, and the multithreaded renderer all live in this repository. SFML is used only to display and encode the final image, and GoogleTest only for the tests.
- **Tested:** 80+ unit tests, including BVH-versus-brute-force comparisons on seeded random rays, run by GitHub Actions on every push and pull request.
- **Shading pipeline:** linear-space lighting with sRGB output, smooth shading from OBJ vertex normals, and ray offsets that stay correct on meshes with inverted winding.

## Showcase

<table>
  <tr>
    <td align="center" width="33%">
      <img src="images/stanford-bunny.png" alt="Stanford Bunny scene" width="100%">
      <br>
      <sub>Stanford Bunny</sub>
    </td>
    <td align="center" width="33%">
      <img src="images/reflections.png" alt="Reflective scene" width="100%">
      <br>
      <sub>Reflections</sub>
    </td>
    <td align="center" width="33%">
      <img src="images/suzanne.png" alt="Suzanne benchmark scene" width="100%">
      <br>
      <sub>Suzanne benchmark scene</sub>
    </td>
  </tr>
</table>

## Features

- CPU ray tracing with perspective camera controls.
- Sphere, plane, triangle, and axis-aligned bounding-box geometry; triangle meshes loaded from Wavefront OBJ.
- OBJ loader with polygon triangulation, negative indices, and per-corner normals and texture coordinates.
- Translation, rotation, and scaling transformations.
- Point-light diffuse shading, ambient lighting, hard shadows, and reflections.
- Smooth shading from OBJ vertex normals.
- Supersampling and linear RGB to sRGB output conversion.
- Median-split BVH acceleration for mesh intersection queries.
- Render statistics for timing, rays, triangle tests, and throughput.
- Command-line control of scene, resolution, samples per pixel, and BVH use.
- SFML window display or headless rendering to PNG.

## How it works

### Rendering pipeline

1. **Camera.** For each pixel, `--spp` sub-pixel samples are placed on a regular grid (4 × 4 by default). Each sample becomes a perspective primary ray.
2. **Intersection.** The renderer tests every scene object and keeps the closest hit. Each object intersects the ray in its own object space: the ray is transformed by the object's inverse transform, and normals are transformed back using the inverse scale. Rotation, translation, and non-uniform scale therefore work for every shape.
3. **Meshes.** A mesh builds a median-split BVH over its triangles. A ray walks the hierarchy and runs a Möller–Trumbore test only against triangles in leaves whose bounding boxes it hits; the test also returns barycentric coordinates for shading. With `--no-bvh`, every triangle is tested instead.
4. **Shading.** Ambient light plus Lambertian diffuse from a single point light. Hard shadows come from a shadow ray toward the light. Reflections recurse to a depth of 3 and are skipped for materials with zero reflectivity.
5. **Output.** Samples are averaged in linear RGB, clamped, and converted to sRGB for the PNG and the display.

Rendering is parallelized by splitting the image rows into contiguous bands, one per worker thread.

### Implementation notes

- **Scale-invariant triangle test.** The ray–triangle "parallel" check compares the determinant against a threshold relative to the edge and ray lengths instead of a fixed epsilon. A fixed `1e-5` threshold silently rejected almost every triangle of the Stanford Bunny, whose edges are on the order of 10⁻³ model units. Regression tests cover triangles of widely different sizes.
- **Orientation-independent shading and ray offsets.** The shading normal is oriented toward the incoming ray, and ray origins are offset along an oriented normal, so meshes with inverted winding shade, shadow, and reflect like correctly wound ones. Tests cover reversed winding and reflection self-intersection.
- **Smooth shading.** Per-corner OBJ normals are interpolated with the barycentric coordinates from the intersection test. If normals are missing, zero-length, or non-finite, shading falls back to the triangle's geometric normal. Reflection directions use the same interpolated normal.
- **OBJ loading.** Supports `v`, `vt`, `vn`, and `f` in all index forms (`v`, `v/vt`, `v//vn`, `v/vt/vn`), negative indices, and polygons (fan-triangulated). Malformed or out-of-range input is rejected with a line number, and `loadOBJ` throws if no usable triangles are loaded.
- **Low-overhead statistics.** Triangle-test counts, by far the hottest counter, accumulate in per-thread counters that are merged after the render (see [Measurement note](#measurement-note)).

## Build

### Requirements

- C++20 compiler
- CMake 3.26 or later
- SFML 3 Graphics

SFML is required even for `--headless` runs, because it encodes the PNG. The test targets use GoogleTest, which CMake fetches during configuration.

### Configure and build

Install the requirements for your environment, then configure and build:

**Windows (PowerShell):**

```powershell
cmake -S . -B build
cmake --build build --config Release
```

**Linux:**

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

The main executable target is `ray_tracing_engine`; the reusable library target is `raytracer_core`. The project also builds GoogleTest targets.

## Scene selection

Choose a demo scene with `--scene`. If omitted, the engine renders `benchmark`.

| Scene | Description |
|---|---|
| `bunny` | Stanford Bunny mesh |
| `reflections` | Reflective scene |
| `benchmark` | Suzanne mesh benchmark scene |

### Bundled models

| Model | File | Triangles | Normals in file |
|---|---|---:|---|
| Suzanne | `objects/suzanne.obj` | 968 (quads fan-triangulated) | Per-face, so it shades flat |
| Stanford Bunny | `objects/stanford-bunny.obj` | 69,451 | None, so it shades flat per triangle |

## Command-line options

| Option | Description | Default |
|---|---|---|
| `--scene <name>` | Select `bunny`, `reflections`, or `benchmark` | `benchmark` |
| `--output <file>` | Set the output PNG filename | `render.png` |
| `--width <pixels>` | Set the render width; must be a positive integer | `1440` |
| `--height <pixels>` | Set the render height; must be a positive integer | `810` |
| `--spp <count>` | Set samples per pixel; must be a positive integer | `16` |
| `--no-bvh` | Disable BVH acceleration and use brute-force triangle tests | BVH enabled |
| `--headless` | Save the image without opening an SFML window | Display enabled |

For example:

**Windows (PowerShell):**

```powershell
.\build\Release\ray_tracing_engine.exe --scene bunny --output bunny.png
```

**Linux:**

```bash
./build/ray_tracing_engine --scene bunny --output bunny.png
```

By default, the engine saves the PNG and displays it in an SFML window. Add `--headless` to save the image and exit without opening a window.

### Reproducing the showcase images

The images in this README were rendered with the default resolution (1,440 × 810) and 16 samples per pixel:

```bash
./build/ray_tracing_engine --scene bunny --headless --output images/stanford-bunny.png
./build/ray_tracing_engine --scene reflections --headless --output images/reflections.png
./build/ray_tracing_engine --scene benchmark --headless --output images/suzanne.png
```

On Windows, use `.\build\Release\ray_tracing_engine.exe` instead of `./build/ray_tracing_engine`.

## Benchmarking

Compare BVH acceleration with brute-force triangle testing by rendering the same scene with and without `--no-bvh`. These commands use the default benchmark scene, resolution, and sample count:

**Windows (PowerShell):**

```powershell
.\build\Release\ray_tracing_engine.exe --scene benchmark --headless --output bvh.png
.\build\Release\ray_tracing_engine.exe --scene benchmark --headless --no-bvh --output no-bvh.png
```

**Linux:**

```bash
./build/ray_tracing_engine --scene benchmark --headless --output bvh.png
./build/ray_tracing_engine --scene benchmark --headless --no-bvh --output no-bvh.png
```

For a meaningful comparison, use the same build configuration, scene, resolution, and samples per pixel for both runs.

### Recorded BVH results

The following recorded Release-build runs used the same Suzanne scene (968 triangles and a plane), point light, 1,440 × 810 resolution, and 16 samples per pixel (4 × 4 grid). The machine was an Intel Core i7-12700F (12 cores, 20 logical processors), using MSVC 19.38.33135, CMake with Visual Studio 2022/MSBuild, and x64 architecture. Triangle-test statistics use per-thread counters merged after rendering.

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

In these runs, BVH acceleration rendered 17.76× faster and reduced triangle intersection tests by 99.52%. Results can vary with hardware, compiler settings, and system load.

The benchmark scene has no reflective materials, so rays traced equals primary rays; shadow rays are counted separately. Brute-force tests equal 968 triangles × (primary + shadow rays), which matches the table.

### Measurement note

An earlier version of this benchmark reported a 101.30× BVH speedup (199.992 s brute force at 800 × 600). That version counted triangle tests with a single atomic counter shared by every thread. The contention made the brute-force baseline artificially slow: about 60 million triangle tests per second then, versus about 1,005 million per second after switching to per-thread counters. The figures above use per-thread counters and replace the earlier ones.

## Testing

Build the project, then run the test suite with CTest:

```bash
ctest --test-dir build --build-config Release --output-on-failure
```

The suite covers:

- Math types, colors (including sRGB conversion), and rays.
- Sphere, plane, AABB, triangle, and bounding-box intersection, including triangles of widely different sizes.
- Transforms: round trips, inverse matrices, and normals under non-uniform scale.
- OBJ parsing: polygons, negative indices, normals and texture coordinates, and malformed input.
- Mesh and BVH behavior: BVH results compared with brute force on seeded random rays, edge hits, misses, and overlapping triangles.
- Renderer correctness: interpolated and fallback shading normals, reversed winding, ambient and diffuse lighting, shadows, reflection depth and offsets, and a triangle-mesh sphere compared with an analytic sphere.
- Camera ray generation.

GitHub Actions builds the project and runs the full suite on every push and pull request.

## Project structure

```text
.
├── include/              Public headers (mirrors src/)
├── src/
│   ├── bvh/              Bounding boxes and BVH nodes
│   ├── core/             Image
│   ├── parser/           OBJ parser and loadOBJ
│   ├── rendering/        Camera and renderer
│   ├── scenes/           Demo scenes
│   ├── world/            Scene, transforms, lighting, hit records
│   │   └── objects/      Sphere, plane, AABB, triangle, mesh
│   ├── render_stats.cpp  Render statistics
│   └── main.cpp          Command-line entry point and SFML display
├── tests/                GoogleTest suites
├── objects/              Bundled OBJ models
├── images/               Showcase renders
└── .github/workflows/    CI configuration
```

## License

Licensed under the terms in [LICENSE](LICENSE).

## Credits

The Stanford Bunny is courtesy of the Stanford Computer Graphics Laboratory, and Suzanne is Blender's test mesh. Both models retain their original terms.