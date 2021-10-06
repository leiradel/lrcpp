#include "Perf.h"

#include <chrono>

bool Perf::init() {
    return true;
}

void Perf::destroy() {}

retro_time_t Perf::getTimeUsec() {
    auto now_us = std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::system_clock::now());
    return static_cast<retro_time_t>(now_us.time_since_epoch().count());
}

uint64_t Perf::getCpuFeatures() {
    // TODO actually probe the CPU for features
    return 0;
}

retro_perf_tick_t Perf::getCounter() {
    auto now_ns = std::chrono::time_point_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now());
    return static_cast<retro_perf_tick_t>(now_ns.time_since_epoch().count());
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
