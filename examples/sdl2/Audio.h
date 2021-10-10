#pragma once

#include "Config.h"

#include <lrcpp/Components.h>

#include <SDL.h>

#include <string>
#include <vector>
#include <stdint.h>

class Audio : public lrcpp::Audio {
public:
    Audio();

    bool init(Config* config, lrcpp::Logger* logger);
    void destroy();

    double getCoreSampleRate() const;

    void clear();
    void present();

    // lrcpp::Audio
    virtual bool setSystemAvInfo(retro_system_av_info const* info) override;
    virtual bool setAudioCallback(retro_audio_callback const* callback) override;
    virtual size_t sampleBatch(int16_t const* data, size_t frames) override;
    virtual void sample(int16_t left, int16_t right) override;

protected:
    void reset();

    lrcpp::Logger* _logger;

    std::string _driverName;
    double _coreSampleRate;
    SDL_AudioDeviceID _audioDev;

    std::vector<int16_t> _samples;
};
