#include "Audio.h"

Audio::Audio() {
    reset();
}

bool Audio::init(lrcpp::Logger* logger) {
    reset();

    _logger = logger;

    if (SDL_InitSubSystem(SDL_INIT_AUDIO) != 0) {
        logger->error("SDL_InitSubSystem(SDL_INIT_AUDIO) failed: %s", SDL_GetError());
        return false;
    }

    _logger->info("Audio subsystem initialized");
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
        _logger->error("SDL_QueueAudio() failed: %s", SDL_GetError());
        return;
    }

    _logger->debug("%zu audio samples presented, %u bytes in output queue", _samples.size(), SDL_GetQueuedAudioSize(_audioDev));
}

bool Audio::setSystemAvInfo(retro_system_av_info const* info) {
    _coreSampleRate = info->timing.sample_rate;
    _logger->info("Core sample rate set to %f", _coreSampleRate);

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

    _audioDev = SDL_OpenAudioDevice(nullptr, 0, &desired, &obtained, 0);

    if (_audioDev == 0) {
        _logger->error("SDL_OpenAudioDevice() failed: %s", SDL_GetError());
        return false;
    }

    SDL_PauseAudioDevice(_audioDev, 0);

    _logger->info(
        "Audio device opened with %d Hz, %u channels, %u bits per sample, %s %s, %s endian samples",
        obtained.freq, obtained.channels, SDL_AUDIO_BITSIZE(obtained.format),
        SDL_AUDIO_ISSIGNED(obtained.format) ? "signed" : "unsigned",
        SDL_AUDIO_ISFLOAT(obtained.format) ? "float" : "integer",
        SDL_AUDIO_ISBIGENDIAN(obtained.format) ? "big" : "little"
    );

    return true;
}

bool Audio::setAudioCallback(retro_audio_callback const* callback) {
    (void)callback;
    _logger->warn("RETRO_ENVIRONMENT_SET_AUDIO_CALLBACK not implemented");
    return false;
}

size_t Audio::sampleBatch(int16_t const* data, size_t frames) {
    size_t const size = _samples.size();
    _samples.resize(size + frames * 2);
    memcpy(_samples.data() + size, data, frames * 4);

    _logger->debug("%zu audio frames queued", frames);
    return frames;
}

void Audio::sample(int16_t left, int16_t right) {
    int16_t frame[2] = {left, right};
    sampleBatch(frame, 1);
}

void Audio::reset() {
    _logger = nullptr;

    _coreSampleRate = 0.0;
    _audioDev = 0;

    _samples.clear();
}
