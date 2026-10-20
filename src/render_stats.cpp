#include <render_stats.hpp>

RenderStats renderStats;
thread_local RenderStats* RenderStats::threadTriangleTestOwner = nullptr;
thread_local uint64_t* RenderStats::threadTriangleTests = nullptr;

RenderStats::TriangleTestCounterScope::TriangleTestCounterScope(
	RenderStats& stats, uint64_t* counter)
	: previousStats(RenderStats::threadTriangleTestOwner),
	  previousCounter(RenderStats::threadTriangleTests) {
	RenderStats::threadTriangleTestOwner = &stats;
	RenderStats::threadTriangleTests = counter;
}

RenderStats::TriangleTestCounterScope::~TriangleTestCounterScope() {
	RenderStats::threadTriangleTestOwner = previousStats;
	RenderStats::threadTriangleTests = previousCounter;
}

void RenderStats::reset() {
	pixelsRendered.store(0);
	primaryRays.store(0);
	raysTraced.store(0);
	shadowRays.store(0);
	triangleTests.store(0);
	renderTimeMs = 0.0;
}

void RenderStats::beginRender() {
	reset();
	renderStart = std::chrono::steady_clock::now();
}

void RenderStats::finishRender() {
	const auto elapsed = std::chrono::steady_clock::now() - renderStart;
	renderTimeMs = std::chrono::duration<double, std::milli>(elapsed).count();
}

void RenderStats::recordPixel() {
	++pixelsRendered;
}

void RenderStats::recordPrimaryRay() {
	++primaryRays;
}

void RenderStats::recordRay() {
	++raysTraced;
}

void RenderStats::recordShadowRay() {
	++shadowRays;
}

void RenderStats::recordTriangleTests(uint64_t count) {
	if (count == 0) {
		return;
	}
	if (threadTriangleTestOwner == this && threadTriangleTests != nullptr) {
		*threadTriangleTests += count;
		return;
	}
	triangleTests.fetch_add(count);
}

void RenderStats::mergeTriangleTests(uint64_t count) {
	triangleTests.fetch_add(count);
}

RenderStats::Snapshot RenderStats::snapshot() const {
	const uint64_t pixels = pixelsRendered.load();
	const uint64_t primary = primaryRays.load();
	const uint64_t traced = raysTraced.load();
	const uint64_t triangles = triangleTests.load();
	const double seconds = renderTimeMs / 1000.0;

	Snapshot result{
		pixels,
		primary,
		traced,
		shadowRays.load(),
		triangles,
		renderTimeMs
	};
	result.pixelsPerSecond = seconds > 0.0 ? pixels / seconds : 0.0;
	result.raysPerSecond = seconds > 0.0 ? traced / seconds : 0.0;
	result.triangleTestsPerPrimaryRay = primary > 0
		? static_cast<double>(triangles) / primary
		: 0.0;
	return result;
}