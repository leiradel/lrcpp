#pragma once

#include <lrcpp/Components.h>

#include <stdint.h>

class Perf : public lrcpp::Perf {
public:
    bool init();
    void destroy();

    static uint64_t getTimeUs();
    static uint64_t getTimeNs();

    // lrcpp::Perf
    virtual retro_time_t getTimeUsec() override;
    virtual uint64_t getCpuFeatures() override;
    virtual retro_perf_tick_t getCounter() override;
    virtual void register_(retro_perf_counter* counter) override;
    virtual void start(retro_perf_counter* counter) override;
    virtual void stop(retro_perf_counter* counter) override;
    virtual void log() override;
};
