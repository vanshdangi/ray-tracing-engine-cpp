#include <render_stats.hpp>

RenderStats renderStats;

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

void RenderStats::recordTriangleTest() {
	++triangleTests;
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