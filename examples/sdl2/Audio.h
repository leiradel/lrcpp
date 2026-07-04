#pragma once

#include "Config.h"

#include <lrcpp/Components.h>

#include <SDL.h>

#include <string>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <stdint.h>

class Audio : public lrcpp::Audio {
public:
    Audio();

    bool init(Config* config, lrcpp::Logger* logger);
    void destroy();

    bool waitToFill();
    void signalStop();

    // lrcpp::Audio
    virtual bool setSystemAvInfo(retro_system_av_info const* info) override;
    virtual bool setAudioCallback(retro_audio_callback const* callback) override;
    virtual size_t sampleBatch(int16_t const* data, size_t frames) override;
    virtual void sample(int16_t left, int16_t right) override;

protected:
    void reset();
    void closeDevice();

    static void SDLCALL audioCallback(void* userdata, Uint8* stream, int len);

    lrcpp::Logger* _logger;

    std::string _deviceName;
    double _coreSampleRate;
    SDL_AudioDeviceID _audioDev;

    std::vector<int16_t> _ring;
    size_t _capacity;
    size_t _count;
    size_t _head;
    size_t _tail;
    size_t _frameSamples;

    std::mutex _mutex;
    std::condition_variable _cv;
    bool _stop;
};
