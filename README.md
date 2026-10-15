# ray-tracing-engine-cpp

## Performance Metrics

### BVH Acceleration: Before vs. After

Benchmarking the renderer with and without a Bounding Volume Hierarchy (BVH) demonstrates a significant reduction in rendering time and triangle intersection tests.

**Test configuration**
- Resolution: 800 × 600
- Supersampling: 16 samples per pixel (16×16 grid)
- Scene: Suzanne mesh (968 triangles) and a plane
- Camera: Origin `(0, 0, 0)`, looking along `+Z`
- Point light: `(0, 5, 2)`
- Suzanne transform:
  - Position: `(0, 0, 10)`
  - Rotation: `(0, 180, 0)`
  - Scale: `(2, 2, 2)`
- Both benchmarks use the same scene and rendering settings.

### Results

| Metric | Before BVH | After BVH |
|---|---:|---:|
| Render time | 426,053 ms (426.05 s) | 13,533 ms (13.53 s) |
| Pixels rendered | 480,000 | 480,000 |
| Primary rays | 7,680,000 | 7,680,000 |
| Rays traced | 12,029,500 | 12,029,500 |
| Shadow rays | 4,371,233 | 4,371,233 |
| Triangle tests | 15,875,909,544 | 26,775,562 |
| Pixels per second | 1,126.62 | 35,469 |
| Rays per second | 28,234.8 | 888,904 |
| Triangle tests per primary ray | 2,067.18 | 3.4864 |

### Performance Improvement

| Metric | Improvement |
|---|---:|
| Rendering speed | **31.48× faster** |
| Render time | **96.82% reduction** |
| Triangle intersection tests | **99.83% reduction** |
| Pixels per second | **31.48× increase** |
| Rays per second | **31.48× increase** |
