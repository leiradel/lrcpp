#include "Perf.h"

#include <SDL.h>
#include <chrono>

bool Perf::init() {
    return true;
}

void Perf::destroy() {}

uint64_t Perf::getTimeUs() {
    auto const now = std::chrono::steady_clock::now().time_since_epoch();
    return static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::microseconds>(now).count());
}

uint64_t Perf::getTimeNs() {
    auto const now = std::chrono::steady_clock::now().time_since_epoch();
    return static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(now).count());
}

retro_time_t Perf::getTimeUsec() {
    return static_cast<retro_time_t>(getTimeUs());
}

uint64_t Perf::getCpuFeatures() {
    // TODO detect other CPU features.
    uint64_t features = 0;

    features |= SDL_HasAVX() ? RETRO_SIMD_AVX : 0;
    features |= SDL_HasAVX2() ? RETRO_SIMD_AVX2 : 0;
    features |= SDL_HasMMX() ? RETRO_SIMD_MMX : 0;
    features |= SDL_HasSSE() ? RETRO_SIMD_SSE : 0;
    features |= SDL_HasSSE2() ? RETRO_SIMD_SSE2 : 0;
    features |= SDL_HasSSE3() ? RETRO_SIMD_SSE3 : 0;
    features |= SDL_HasSSE42() ? RETRO_SIMD_SSE42 : 0;

    return features;
}

retro_perf_tick_t Perf::getCounter() {
    return static_cast<retro_perf_tick_t>(getTimeNs());
}

void Perf::register_(retro_perf_counter* counter) {
    (void)counter;
}

void Perf::start(retro_perf_counter* counter) {
    (void)counter;
}

void Perf::stop(retro_perf_counter* counter) {
    (void)counter;
}

void Perf::log() {}
