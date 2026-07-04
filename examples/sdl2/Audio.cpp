#include "Audio.h"

#include <string.h>

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
        _logger->debug("Audio device %d: \"%s\"\n", i, SDL_GetAudioDeviceName(i, 0));
    }

    return true;
}

void Audio::destroy() {
    closeDevice();
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
    reset();
}

void Audio::closeDevice() {
    if (_audioDev != 0) {
        SDL_CloseAudioDevice(_audioDev);
        _audioDev = 0;
    }
}

bool Audio::waitToFill() {
    std::unique_lock<std::mutex> lock(_mutex);
    _cv.wait(lock, [this]() { return _stop || _capacity - _count >= _frameSamples; });
    return !_stop;
}

void Audio::signalStop() {
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _stop = true;
    }

    _cv.notify_one();
}

bool Audio::setSystemAvInfo(retro_system_av_info const* info) {
    if (info->timing.sample_rate == _coreSampleRate && _audioDev != 0) {
        return true;
    }

    closeDevice();

    _coreSampleRate = info->timing.sample_rate;
    _logger->info("Core sample rate set to %f\n", _coreSampleRate);

    double const fps = info->timing.fps > 0.0 ? info->timing.fps : 60.0;
    _frameSamples = static_cast<size_t>(_coreSampleRate / fps) * 2;

    {
        std::lock_guard<std::mutex> lock(_mutex);
        _capacity = _frameSamples * 4;
        _ring.assign(_capacity, 0);
        _count = 0;
        _head = 0;
        _tail = 0;
    }

    SDL_AudioSpec desired, obtained;
    memset(&desired, 0, sizeof(desired));
    memset(&obtained, 0, sizeof(obtained));

    desired.freq = static_cast<int>(_coreSampleRate);
    desired.format = AUDIO_S16SYS;
    desired.channels = 2;
    desired.samples = 1024;
    desired.callback = audioCallback;
    desired.userdata = this;

    char const* const device = _deviceName.length() != 0 ? _deviceName.c_str() : nullptr;
    _logger->info("Using %s audio device\n", device != nullptr ? device : "default");
    _audioDev = SDL_OpenAudioDevice(device, 0, &desired, &obtained, 0);

    if (_audioDev == 0) {
        _logger->error("SDL_OpenAudioDevice() failed: %s\n", SDL_GetError());
        return false;
    }

    _logger->info("Opened audio driver %s\n", SDL_GetCurrentAudioDriver());
    _logger->debug("    %d Hz\n", obtained.freq);
    _logger->debug("    %u channels\n", obtained.channels);
    _logger->debug("    %u bits per sample\n", SDL_AUDIO_BITSIZE(obtained.format));

    _logger->debug(
        "    %s %s\n",
        SDL_AUDIO_ISSIGNED(obtained.format) ? "signed" : "unsigned",
        SDL_AUDIO_ISFLOAT(obtained.format) ? "float" : "integer"
    );

    _logger->debug("    %s endian\n", SDL_AUDIO_ISBIGENDIAN(obtained.format) ? "big" : "little");

    SDL_PauseAudioDevice(_audioDev, 0);
    return true;
}

bool Audio::setAudioCallback(retro_audio_callback const* callback) {
    (void)callback;
    _logger->warn("RETRO_ENVIRONMENT_SET_AUDIO_CALLBACK not implemented\n");
    return false;
}

size_t Audio::sampleBatch(int16_t const* data, size_t frames) {
    size_t const samples = frames * 2;

    std::lock_guard<std::mutex> lock(_mutex);

    size_t const room = _capacity - _count;
    size_t const toWrite = samples < room ? samples : room;
    size_t const firstChunk = _capacity - _tail < toWrite ? _capacity - _tail : toWrite;

    memcpy(_ring.data() + _tail, data, firstChunk * sizeof(int16_t));

    if (toWrite > firstChunk) {
        memcpy(_ring.data(), data + firstChunk, (toWrite - firstChunk) * sizeof(int16_t));
    }

    _tail = (_tail + toWrite) % _capacity;
    _count += toWrite;

    return frames;
}

void Audio::sample(int16_t left, int16_t right) {
    int16_t const frame[2] = {left, right};
    sampleBatch(frame, 1);
}

void SDLCALL Audio::audioCallback(void* userdata, Uint8* stream, int len) {
    Audio* const self = static_cast<Audio*>(userdata);
    int16_t* const out = reinterpret_cast<int16_t*>(stream);
    size_t const wanted = static_cast<size_t>(len) / sizeof(int16_t);
    size_t provided = 0;

    {
        std::lock_guard<std::mutex> lock(self->_mutex);

        provided = wanted < self->_count ? wanted : self->_count;
        size_t const firstChunk = self->_capacity - self->_head < provided ? self->_capacity - self->_head : provided;

        memcpy(out, self->_ring.data() + self->_head, firstChunk * sizeof(int16_t));

        if (provided > firstChunk) {
            memcpy(out + firstChunk, self->_ring.data(), (provided - firstChunk) * sizeof(int16_t));
        }

        self->_head = (self->_head + provided) % self->_capacity;
        self->_count -= provided;

        self->_cv.notify_one();
    }

    if (provided < wanted) {
        memset(out + provided, 0, (wanted - provided) * sizeof(int16_t));
    }
}

void Audio::reset() {
    _logger = nullptr;

    _deviceName.clear();
    _coreSampleRate = 0.0;
    _audioDev = 0;

    _ring.clear();
    _capacity = 0;
    _count = 0;
    _head = 0;
    _tail = 0;
    _frameSamples = 0;

    _stop = false;
}
