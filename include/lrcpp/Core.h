#ifndef LRCPP_CORE_H__
#define LRCPP_CORE_H__

#include <lrcpp/libretro.h>
#include <lrcpp/dynlib.h>

namespace lrcpp {
    struct Core {
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
}

#define LRCPP_CORE_INIT_FUNCTIONS_INTERNAL(core, prefix) \
    do { \
        core.init = prefix ## init; \
        core.deinit = prefix ## deinit; \
        core.apiVersion = prefix ## api_version; \
        core.getSystemInfo = prefix ## get_system_info; \
        core.getSystemAvInfo = prefix ## get_system_av_info; \
        core.setEnvironment = prefix ## set_environment; \
        core.setVideoRefresh = prefix ## set_video_refresh; \
        core.setAudioSample = prefix ## set_audio_sample; \
        core.setAudioSampleBatch = prefix ## set_audio_sample_batch; \
        core.setInputPoll = prefix ## set_input_poll; \
        core.setInputState = prefix ## set_input_state; \
        core.setControllerPortDevice = prefix ## set_controller_port_device; \
        core.reset = prefix ## reset; \
        core.run = prefix ## run; \
        core.serializeSize = prefix ## serialize_size; \
        core.serialize = prefix ## serialize; \
        core.unserialize = prefix ## unserialize; \
        core.cheatReset = prefix ## cheat_reset; \
        core.cheatSet = prefix ## cheat_set; \
        core.loadGame = prefix ## load_game; \
        core.loadGameSpecial = prefix ## load_game_special; \
        core.unloadGame = prefix ## unload_game; \
        core.getRegion = prefix ## get_region; \
        core.getMemoryData = prefix ## get_memory_data; \
        core.getMemorySize = prefix ## get_memory_size; \
    } while (0)

#define LRCPP_CORE_INIT_FUNCTIONS_DEFAULT(core) LRCPP_CORE_INIT_FUNCTIONS_INTERNAL(core, retro_)
#define LRCPP_CORE_INIT_FUNCTIONS(core, prefix) LRCPP_CORE_INIT_FUNCTIONS_INTERNAL(core, prefix ## retro_)

#endif // LRCPP_CORE_H__
