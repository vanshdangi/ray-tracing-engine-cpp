#pragma once
#include <atomic>
#include <chrono>
#include <cstdint>

struct RenderStats {
    struct Snapshot {
        uint64_t pixelsRendered = 0;
        uint64_t primaryRays = 0;
        uint64_t raysTraced = 0;
        uint64_t shadowRays = 0;
        uint64_t triangleTests = 0;
        double renderTimeMs = 0.0;
        double pixelsPerSecond = 0.0;
        double raysPerSecond = 0.0;
        double triangleTestsPerPrimaryRay = 0.0;
    };

    void reset();
    void beginRender();
    void finishRender();

    void recordPixel();
    void recordPrimaryRay();
    void recordRay();
    void recordShadowRay();
    void recordTriangleTest();

    Snapshot snapshot() const;

private:
    std::atomic<uint64_t> pixelsRendered = 0;
    std::atomic<uint64_t> primaryRays = 0;
    std::atomic<uint64_t> raysTraced = 0;
    std::atomic<uint64_t> shadowRays = 0;
    std::atomic<uint64_t> triangleTests = 0;
    std::chrono::steady_clock::time_point renderStart;
    double renderTimeMs = 0.0;
};

extern RenderStats renderStats;