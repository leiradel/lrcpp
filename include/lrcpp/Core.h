#ifndef LRCPP_CORE_H__
#define LRCPP_CORE_H__

#include <lrcpp/libretro.h>
#include <lrcpp/dynlib.h>

namespace lrcpp {
    struct CoreFunctions {
        void (*init)();
        void (*deinit)();
        unsigned (*apiVersion)();
        void (*getSystemInfo)(struct retro_system_info*);
        void (*getSystemAvInfo)(struct retro_system_av_info*);
        void (*setEnvironment)(retro_environment_t);
        void (*setVideoRefresh)(retro_video_refresh_t);
        void (*setAudioSample)(retro_audio_sample_t);
        void (*setAudioSampleBatch)(retro_audio_sample_batch_t);
        void (*setInputPoll)(retro_input_poll_t);
        void (*setInputState)(retro_input_state_t);
        void (*setControllerPortDevice)(unsigned, unsigned);
        void (*reset)();
        void (*run)();
        size_t (*serializeSize)();
        bool (*serialize)(void*, size_t);
        bool (*unserialize)(void const*, size_t);
        void (*cheatReset)();
        void (*cheatSet)(unsigned, bool, char const*);
        bool (*loadGame)(struct retro_game_info const*);
        bool (*loadGameSpecial)(unsigned, struct retro_game_info const*, size_t);
        void (*unloadGame)();
        unsigned (*getRegion)();
        void* (*getMemoryData)(unsigned);
        size_t (*getMemorySize)(unsigned);

    };

    class Core final {
    public:
        Core() : _handle(nullptr) {}
        ~Core();

        bool load(char const* path);
        bool use(CoreFunctions const* functions);
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

#define LRCPP_CORE_INIT_FUNCTIONS_INTERNAL(functions, prefix) \
    do { \
        functions.init = prefix ## init; \
        functions.deinit = prefix ## deinit; \
        functions.apiVersion = prefix ## api_version; \
        functions.getSystemInfo = prefix ## get_system_info; \
        functions.getSystemAvInfo = prefix ## get_system_av_info; \
        functions.setEnvironment = prefix ## set_environment; \
        functions.setVideoRefresh = prefix ## set_video_refresh; \
        functions.setAudioSample = prefix ## set_audio_sample; \
        functions.setAudioSampleBatch = prefix ## set_audio_sample_batch; \
        functions.setInputPoll = prefix ## set_input_poll; \
        functions.setInputState = prefix ## set_input_state; \
        functions.setControllerPortDevice = prefix ## set_controller_port_device; \
        functions.reset = prefix ## reset; \
        functions.run = prefix ## run; \
        functions.serializeSize = prefix ## serialize_size; \
        functions.serialize = prefix ## serialize; \
        functions.unserialize = prefix ## unserialize; \
        functions.cheatReset = prefix ## cheat_reset; \
        functions.cheatSet = prefix ## cheat_set; \
        functions.loadGame = prefix ## load_game; \
        functions.loadGameSpecial = prefix ## load_game_special; \
        functions.unloadGame = prefix ## unload_game; \
        functions.getRegion = prefix ## get_region; \
        functions.getMemoryData = prefix ## get_memory_data; \
        functions.getMemorySize = prefix ## get_memory_size; \
    } while (0)

#define LRCPP_CORE_INIT_FUNCTIONS_DEFAULT(functions) LRCPP_CORE_INIT_FUNCTIONS_INTERNAL(functions, retro_)
#define LRCPP_CORE_INIT_FUNCTIONS(functions, prefix) LRCPP_CORE_INIT_FUNCTIONS_INTERNAL(functions, prefix ## retro_)

#endif // LRCPP_CORE_H__
