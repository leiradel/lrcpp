#ifndef LRCPP_FRONTEND_H__
#define LRCPP_FRONTEND_H__

#include <lrcpp/Components.h>
#include <lrcpp/CoreFsm.h>
#include <lrcpp/libretro.h>

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

namespace lrcpp {
    struct Core final {
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

    class Frontend final {
    public:
        Frontend();

        Frontend(Frontend const&) = delete;
        Frontend(Frontend&&) = delete;
        void operator=(Frontend const&) = delete;
        void operator=(Frontend&&) = delete;

        ~Frontend();

        // Components
        Logger* getLogger();
        Config* getConfig();
        Video* getVideo();
        Led* getLed();
        Audio* getAudio();
        Midi* getMidi();
        Input* getInput();
        Rumble* getRumble();
        Sensor* getSensor();
        Camera* getCamera();
        Location* getLocation();
        VirtualFileSystem* getVirtualFileSystem();
        DiskControl* getDiskControl();
        Perf* getPerf();

        bool setLogger(Logger* logger);
        bool setConfig(Config* config);
        bool setVideo(Video* video);
        bool setLed(Led* led);
        bool setAudio(Audio* audio);
        bool setMidi(Midi* midi);
        bool setInput(Input* input);
        bool setRumble(Rumble* rumble);
        bool setSensor(Sensor* sensor);
        bool setCamera(Camera* camera);
        bool setLocation(Location* location);
        bool setVirtualFileSystem(VirtualFileSystem* virtualFileSystem);
        bool setDiskControl(DiskControl* diskControl);
        bool setPerf(Perf* perf);

        // Core life-cycle
        bool setCore(Core const* core);
        bool loadGame();
        bool loadGame(char const* gamePath);
        bool loadGame(char const* gamePath, void const* data, size_t size);
        bool loadGameSpecial(unsigned gameType, struct retro_game_info const* info, size_t numInfo);
        bool run();
        bool reset();
        bool unloadGame();
        bool unset();

        // Other Libretro API calls
        bool apiVersion(unsigned* version);
        bool getSystemInfo(struct retro_system_info* info);
        bool getSystemAvInfo(struct retro_system_av_info* info);
        bool serializeSize(size_t* size);
        bool serialize(void* data, size_t size);
        bool unserialize(void const* data, size_t size);
        bool cheatReset();
        bool cheatSet(unsigned index, bool enabled, char const* code);
        bool getRegion(unsigned* region);
        bool getMemoryData(unsigned id, void** data);
        bool getMemorySize(unsigned id, size_t* size);
        bool setControllerPortDevice(unsigned port, unsigned device);

        // lrcpp API
        bool shutdownRequested() const;

    protected:
        // Environment functions
        bool setRotation(unsigned data);
        bool getOverscan(bool* data);
        bool getCanDupe(bool* data);
        bool showMessage(struct retro_message const* data);
        bool shutdown();
        bool setPerformanceLevel(unsigned data);
        bool getSystemDirectory(char const** data);
        bool setPixelFormat(enum retro_pixel_format data);
        bool setInputDescriptors(struct retro_input_descriptor const* data);
        bool setKeyboardCallback(struct retro_keyboard_callback const* data);
        bool setDiskControlInterface(struct retro_disk_control_callback const* data);
        bool setHwRender(struct retro_hw_render_callback* data);
        bool getVariable(struct retro_variable* data);
        bool setVariables(struct retro_variable const* data);
        bool getVariableUpdate(bool* data);
        bool setSupportNoGame(bool data);
        bool getLibretroPath(char const** data);
        bool setFrameTimeCallback(struct retro_frame_time_callback const* data);
        bool setAudioCallback(struct retro_audio_callback const* data);
        bool getRumbleInterface(struct retro_rumble_interface* data);
        bool getInputDeviceCapabilities(uint64_t* data);
        bool getSensorInterface(struct retro_sensor_interface* data);
        bool getCameraInterface(struct retro_camera_callback* data);
        bool getLogInterface(struct retro_log_callback* data);
        bool getPerfInterface(struct retro_perf_callback* data);
        bool getLocationInterface(struct retro_location_callback* data);
        bool getCoreAssetsDirectory(char const** data);
        bool getSaveDirectory(char const** data);
        bool setSystemAvInfo(struct retro_system_av_info const* data);
        bool setProcAddressCallback(struct retro_get_proc_address_interface const* data);
        bool setSubsystemInfo(struct retro_subsystem_info const* data);
        bool setControllerInfo(struct retro_controller_info const* data);
        bool setMemoryMaps(struct retro_memory_map const* data);
        bool setGeometry(struct retro_game_geometry const* data);
        bool getUsername(char const** data);
        bool getLanguage(unsigned* data);
        bool getCurrentSoftwareFramebuffer(struct retro_framebuffer* data);
        bool getHwRenderInterface(struct retro_hw_render_interface const** data);
        bool setSupportAchievements(bool data);
        bool setHwRenderContextNegotiationInterface(struct retro_hw_render_context_negotiation_interface const* data);
        bool setSerializationQuirks(uint64_t data);
        bool setHwSharedContext();
        bool getVfsInterface(struct retro_vfs_interface_info* data);
        bool getLedInterface(struct retro_led_interface* data);
        bool getAudioVideoEnable(int* data);
        bool getMidiInterface(struct retro_midi_interface** data);
        bool getFastForwarding(bool* data);
        bool getTargetRefreshRate(float* data);
        bool getInputBitmasks(bool* data);
        bool getCoreOptionsVersion(unsigned* data);
        bool setCoreOptions(struct retro_core_option_definition const* data);
        bool setCoreOptionsIntl(struct retro_core_options_intl const* data);
        bool setCoreOptionsDisplay(struct retro_core_option_display const* data);
        bool getPreferredHwRender(unsigned* data);
        bool getDiskControlInterfaceVersion(unsigned* data);
        bool setDiskControlExtInterface(struct retro_disk_control_ext_callback const* data);

        // Callbacks and interfaces
        bool environmentCallback(unsigned cmd, void* data);

        static bool staticEnvironmentCallback(unsigned cmd, void* data);

        static bool rumbleSetState(unsigned port, enum retro_rumble_effect effect, uint16_t strength);

        static bool sensorSetState(unsigned port, enum retro_sensor_action action, unsigned rate);
        static float sensorGetInput(unsigned port, unsigned id);

        static bool cameraStart();
        static void cameraStop();

        static void loggerVprintf(enum retro_log_level level, char const* format, ...);

        static retro_time_t perfGetTimeUsec();
        static uint64_t perfGetCpuFeatures();
        static retro_perf_tick_t perfGetCounter();
        static void perfRegister(struct retro_perf_counter* counter);
        static void perfStart(struct retro_perf_counter* counter);
        static void perfStop(struct retro_perf_counter* counter);
        static void perfLog();

        static bool locationStart();
        static void locationStop();
        static bool locationGetPosition(double* lat, double* lon, double* horizAccuracy, double* vertAccuracy);
        static void locationSetInterval(unsigned intervalMs, unsigned intervalDistance);

        static char const* virtualFileSystemGetPath(struct retro_vfs_file_handle* stream);
        static struct retro_vfs_file_handle* virtualFileSystemOpen(char const* path, unsigned mode, unsigned hints);
        static int virtualFileSystemClose(struct retro_vfs_file_handle* stream);
        static int64_t virtualFileSystemSize(struct retro_vfs_file_handle* stream);
        static int64_t virtualFileSystemTruncate(struct retro_vfs_file_handle* stream, int64_t length);
        static int64_t virtualFileSystemTell(struct retro_vfs_file_handle* stream);
        static int64_t virtualFileSystemSeek(struct retro_vfs_file_handle* stream, int64_t offset, int seekPosition);
        static int64_t virtualFileSystemRead(struct retro_vfs_file_handle* stream, void* s, uint64_t len);
        static int64_t virtualFileSystemWrite(struct retro_vfs_file_handle* stream, void const* s, uint64_t len);
        static int virtualFileSystemFlush(struct retro_vfs_file_handle* stream);
        static int virtualFileSystemRemove(char const* path);
        static int virtualFileSystemRename(char const* old_path, char const* new_path);
        static int virtualFileSystemStat(char const* path, int32_t* size);
        static int virtualFileSystemMkDir(char const* dir);
        static struct retro_vfs_dir_handle* virtualFileSystemOpenDir(char const* dir, bool includeHidden);
        static bool virtualFileSystemReadDir(struct retro_vfs_dir_handle* dirstream);
        static char const* virtualFileSystemDirentGetName(struct retro_vfs_dir_handle* dirstream);
        static bool virtualFileSystemDirentIsDir(struct retro_vfs_dir_handle* dirstream);
        static int virtualFileSystemCloseDir(struct retro_vfs_dir_handle* dirstream);

        static void ledSetState(int led, int state);

        static bool midiInputEnabled();
        static bool midiOutputEnabled();
        static bool midiRead(uint8_t* byte);
        static bool midiWrite(uint8_t byte, uint32_t deltaTime);
        static bool midiFlush();

        static uintptr_t videoGetCurrentFramebuffer();
        static retro_proc_address_t videoGetProcAddress(char const* symbol);
        static void videoRefresh(void const* data, unsigned width, unsigned height, size_t pitch);

        static size_t audioSampleBatch(int16_t const* data, size_t frames);
        static void audioSample(int16_t left, int16_t right);

        static int16_t inputState(unsigned port, unsigned device, unsigned index, unsigned id);
        static void inputPoll();

        // Get thread local storage for the current frontend
        static Frontend* getCurrent();
        static void setCurrent(Frontend* frontend);

        Logger* _logger;
        Config* _config;
        Video* _video;
        Led* _led;
        Audio* _audio;
        Midi* _midi;
        Input* _input;
        Rumble* _rumble;
        Sensor* _sensor;
        Camera* _camera;
        Location* _location;
        VirtualFileSystem* _virtualFileSystem;
        DiskControl* _diskControl;
        Perf* _perf;

        Core _core;
        CoreFsm_Context _fsm;

        bool _supportsNoGame;

        struct retro_vfs_interface _virtualFileSystemInterface;
        struct retro_midi_interface _midiInterface;

        bool _shutdownRequested;
    };
} // namespace lrcpp

#endif // LRCPP_FRONTEND_H__
