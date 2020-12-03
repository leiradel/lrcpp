#ifndef LRCPP_CORE_H__
#define LRCPP_CORE_H__

#include "libretro.h"
#include "dynlib.h"

namespace lrcpp {
    class Core {
    public:
        Core() : _handle(nullptr) {}
        virtual ~Core();

        bool load(char const* path);
        void unload();

        // All the remaining methods map 1:1 to the Libretro API.
        void init() const { _init(); }
        void deinit() const { _deinit(); }
        unsigned apiVersion() const { return _apiVersion(); }
        void getSystemInfo(struct retro_system_info* info) const { _getSystemInfo(info); }
        void getSystemAvInfo(struct retro_system_av_info* info) const { _getSystemAvInfo(info); }
        void setEnvironment(retro_environment_t cb) const { _setEnvironment(cb); }
        void setVideoRefresh(retro_video_refresh_t cb) const { _setVideoRefresh(cb); }
        void setAudioSample(retro_audio_sample_t cb) const { _setAudioSample(cb); }
        void setAudioSampleBatch(retro_audio_sample_batch_t cb) const { _setAudioSampleBatch(cb); }
        void setInputPoll(retro_input_poll_t cb) const { _setInputPoll(cb); }
        void setInputState(retro_input_state_t cb) const { _setInputState(cb); }
        void setControllerPortDevice(unsigned port, unsigned device) const { _setControllerPortDevice(port, device); }
        void reset() const { _reset(); }
        void run() const { _run(); }
        size_t serializeSize() const { return _serializeSize(); }
        bool serialize(void* data, size_t size) const { return _serialize(data, size); }
        bool unserialize(void const* data, size_t size) const { return _unserialize(data, size); }
        void cheatReset() const { _cheatReset(); }
        void cheatSet(unsigned index, bool enabled, char const* code) const { _cheatSet(index, enabled, code); }
        bool loadGame(struct retro_game_info const* game) const { return _loadGame(game); }

        bool loadGameSpecial(unsigned game_type, struct retro_game_info const* info, size_t num_info) const {
            return _loadGameSpecial(game_type, info, num_info);
        }

        void unloadGame() const { _unloadGame(); }
        unsigned getRegion() const { return _getRegion(); }
        void* getMemoryData(unsigned id) const { return _getMemoryData(id); }
        size_t getMemorySize(unsigned id) const { return _getMemorySize(id); }

    protected:
        dynlib_t _handle;

        void (*_init)();
        void (*_deinit)();
        unsigned (*_apiVersion)();
        void (*_getSystemInfo)(struct retro_system_info*);
        void (*_getSystemAvInfo)(struct retro_system_av_info*);
        void (*_setEnvironment)(retro_environment_t);
        void (*_setVideoRefresh)(retro_video_refresh_t);
        void (*_setAudioSample)(retro_audio_sample_t);
        void (*_setAudioSampleBatch)(retro_audio_sample_batch_t);
        void (*_setInputPoll)(retro_input_poll_t);
        void (*_setInputState)(retro_input_state_t);
        void (*_setControllerPortDevice)(unsigned, unsigned);
        void (*_reset)();
        void (*_run)();
        size_t (*_serializeSize)();
        bool (*_serialize)(void*, size_t);
        bool (*_unserialize)(void const*, size_t);
        void (*_cheatReset)();
        void (*_cheatSet)(unsigned, bool, char const*);
        bool (*_loadGame)(struct retro_game_info const*);
        bool (*_loadGameSpecial)(unsigned, struct retro_game_info const*, size_t);
        void (*_unloadGame)();
        unsigned (*_getRegion)();
        void* (*_getMemoryData)(unsigned);
        size_t (*_getMemorySize)(unsigned);
    };
} // namespace lrcpp

#endif // LRCPP_CORE_H__
