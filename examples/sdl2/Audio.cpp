#include "Audio.h"

Audio::Audio() {
    reset();
}

bool Audio::init(Config* config, lrcpp::Logger* logger) {
    reset();

    _logger = logger;

    if (SDL_InitSubSystem(SDL_INIT_AUDIO) != 0) {
        logger->error("SDL_InitSubSystem(SDL_INIT_AUDIO) failed: %s\n", SDL_GetError());
        return false;
    }

    _logger->info("Audio subsystem initialized\n");

    char const* driverName = nullptr;

    if (config->getOption("sdl2lrcpp_audio_device", &driverName)) {
        _deviceName = driverName;
    }

    int const count = SDL_GetNumAudioDevices(0);

    for (int i = 0; i < count; i++) {
        _logger->info("Audio device %d: %s\n", i, SDL_GetAudioDeviceName(i, 0));
    }

    return true;
}

void Audio::destroy() {
    if (_audioDev != 0) {
        SDL_CloseAudioDevice(_audioDev);
    }

    SDL_QuitSubSystem(SDL_INIT_AUDIO);
    reset();
}

double Audio::getCoreSampleRate() const {
    return _coreSampleRate;
}

void Audio::clear() {
    _samples.clear();
}

void Audio::present() {
    if (SDL_QueueAudio(_audioDev, _samples.data(), _samples.size() * 2) != 0) {
        _logger->error("SDL_QueueAudio() failed: %s\n", SDL_GetError());
        return;
    }

    _logger->debug("%zu audio samples queued, %u bytes in output queue\n", _samples.size(), SDL_GetQueuedAudioSize(_audioDev));
}

bool Audio::setSystemAvInfo(retro_system_av_info const* info) {
    if (info->timing.sample_rate == _coreSampleRate) {
        return true;
    }

    _coreSampleRate = info->timing.sample_rate;
    _logger->info("Core sample rate set to %f\n", _coreSampleRate);

    if (_audioDev != 0) {
        SDL_CloseAudioDevice(_audioDev);
        _audioDev = 0;
    }

    SDL_AudioSpec desired, obtained;
    memset(&desired, 0, sizeof(desired));
    memset(&obtained, 0, sizeof(obtained));

    desired.freq = _coreSampleRate;
    desired.format = AUDIO_S16SYS;
    desired.channels = 2;
    desired.samples = 1024;
    desired.callback = nullptr;
    desired.userdata = nullptr;

    if (_deviceName.length() != 0) {
        _logger->info("Using audio device %s\n", _deviceName.c_str());
        _audioDev = SDL_OpenAudioDevice(_deviceName.c_str(), 0, &desired, &obtained, 0);
    }
    else {
        _logger->info("Using default audio device\n");
        _audioDev = SDL_OpenAudioDevice(nullptr, 0, &desired, &obtained, 0);
    }

    if (_audioDev == 0) {
        _logger->error("SDL_OpenAudioDevice() failed: %s\n", SDL_GetError());
        return false;
    }

    SDL_PauseAudioDevice(_audioDev, 0);

    _logger->info("Opened audio driver %s\n", SDL_GetCurrentAudioDriver());
    _logger->info("    %d Hz\n", obtained.freq);
    _logger->info("    %u channels\n", obtained.channels);
    _logger->info("    %u bits per sample\n", SDL_AUDIO_BITSIZE(obtained.format));

    _logger->info(
        "    %s %s\n",
        SDL_AUDIO_ISSIGNED(obtained.format) ? "signed" : "unsigned",
        SDL_AUDIO_ISFLOAT(obtained.format) ? "float" : "integer"
    );

    _logger->info("    %s endian\n", SDL_AUDIO_ISBIGENDIAN(obtained.format) ? "big" : "little");

    return true;
}

bool Audio::setAudioCallback(retro_audio_callback const* callback) {
    (void)callback;
    _logger->warn("RETRO_ENVIRONMENT_SET_AUDIO_CALLBACK not implemented\n");
    return false;
}

size_t Audio::sampleBatch(int16_t const* data, size_t frames) {
    size_t const size = _samples.size();
    _samples.resize(size + frames * 2);
    memcpy(_samples.data() + size, data, frames * 4);

    _logger->debug("%zu audio frames queued\n", frames);
    return frames;
}

void Audio::sample(int16_t left, int16_t right) {
    int16_t frame[2] = {left, right};
    sampleBatch(frame, 1);
}

void Audio::reset() {
    _logger = nullptr;

    _deviceName.clear();
    _coreSampleRate = 0.0;
    _audioDev = 0;

    _samples.clear();
}
