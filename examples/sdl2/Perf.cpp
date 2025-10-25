#include "Perf.h"

#include <SDL.h>
#include <time.h>

bool Perf::init() {
    return true;
}

void Perf::destroy() {}

uint64_t Perf::getTimeUs() {
    struct timespec ts = {0};

    if (clock_gettime(CLOCK_MONOTONIC, &ts) != 0) {
        return 0;
    }

    return static_cast<int64_t>(ts.tv_sec * 1000000 + ts.tv_nsec / 1000);
}

uint64_t Perf::getTimeNs() {
    struct timespec ts = {0};

    if (clock_gettime(CLOCK_MONOTONIC, &ts) != 0) {
        return 0;
    }

    return static_cast<int64_t>(ts.tv_sec * 1000000000 + ts.tv_nsec);
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
