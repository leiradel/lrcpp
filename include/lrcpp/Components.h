#ifndef LRCPP_COMPONENTS_H__
#define LRCPP_COMPONENTS_H__

#include <lrcpp/libretro.h>
#include <stdarg.h>

namespace lrcpp {
    /**
    * A logger component for Core instances.
    */
    class Logger {
    public:
        virtual ~Logger() {}

        // RETRO_ENVIRONMENT_GET_LOG_INTERFACE
        // retro_log_callback.log is set in Frontend and will call the interface methods

        // Interface
        virtual void vprintf(retro_log_level level, char const* format, va_list args) { (void)level; (void)format; (void)args; }

        // Convenience.
        void debug(char const* format, ...);
        void info(char const* format, ...);
        void warn(char const* format, ...);
        void error(char const* format, ...);
    };

    /**
    * A component that returns configuration values for SimpleCore instances.
    */
    class Config {
    public:
        virtual ~Config() {}

        // RETRO_ENVIRONMENT_SET_PERFORMANCE_LEVEL
        virtual bool setPerformanceLevel(unsigned level) { (void)level; return false; }
        // RETRO_ENVIRONMENT_GET_SYSTEM_DIRECTORY
        virtual bool getSystemDirectory(char const** directory) { (void)directory; return false; }
        // RETRO_ENVIRONMENT_GET_VARIABLE
        virtual bool getVariable(retro_variable* variable) { (void)variable; return false; }
        // RETRO_ENVIRONMENT_SET_VARIABLES
        bool setVariables(retro_variable const* variables);
        // RETRO_ENVIRONMENT_GET_VARIABLE_UPDATE
        virtual bool getVariableUpdate(bool* const updated) { (void)updated; return false; }
        // RETRO_ENVIRONMENT_SET_SUPPORT_NO_GAME
        virtual bool setSupportNoGame(bool const supports) { (void)supports; return false; }
        // RETRO_ENVIRONMENT_GET_LIBRETRO_PATH
        virtual bool getLibretroPath(char const** path) { (void)path; return false; }
        // RETRO_ENVIRONMENT_GET_CORE_ASSETS_DIRECTORY
        virtual bool getCoreAssetsDirectory(char const** directory) { (void)directory; return false;}
        // RETRO_ENVIRONMENT_GET_SAVE_DIRECTORY
        virtual bool getSaveDirectory(char const** directory) { (void)directory; return false; }
        // RETRO_ENVIRONMENT_SET_PROC_ADDRESS_CALLBACK
        virtual bool setProcAddressCallback(retro_get_proc_address_interface const* callback) { (void)callback; return false; }
        // RETRO_ENVIRONMENT_SET_SUBSYSTEM_INFO
        virtual bool setSubsystemInfo(retro_subsystem_info const* info) { (void)info; return false; }
        // RETRO_ENVIRONMENT_SET_MEMORY_MAPS
        virtual bool setMemoryMaps(retro_memory_map const* map) { (void)map; return false; }
        // RETRO_ENVIRONMENT_GET_USERNAME
        virtual bool getUsername(char const** username) { (void)username; return false; }
        // RETRO_ENVIRONMENT_GET_LANGUAGE
        virtual bool getLanguage(unsigned* language) { (void)language; return false; }
        // RETRO_ENVIRONMENT_SET_SUPPORT_ACHIEVEMENTS
        virtual bool setSupportAchievements(bool supports) { (void)supports; return false; }
        // RETRO_ENVIRONMENT_SET_SERIALIZATION_QUIRKS
        virtual bool setSerializationQuirks(uint64_t quirks) { (void)quirks; return false; }
        // RETRO_ENVIRONMENT_GET_AUDIO_VIDEO_ENABLE
        virtual bool getAudioVideoEnable(int* enabled) { (void)enabled; return false; }
        // RETRO_ENVIRONMENT_GET_FASTFORWARDING
        virtual bool getFastForwarding(bool* is) { (void)is; return false; }
        // RETRO_ENVIRONMENT_GET_CORE_OPTIONS_VERSION
        bool getCoreOptionsVersion(unsigned* version);
        // RETRO_ENVIRONMENT_SET_CORE_OPTIONS
        virtual bool setCoreOptions(retro_core_option_definition const* options) { (void)options; return false; }
        // RETRO_ENVIRONMENT_SET_CORE_OPTIONS_INTL
        virtual bool setCoreOptionsIntl(retro_core_options_intl const* intl) { (void)intl; return false; }
        // RETRO_ENVIRONMENT_SET_CORE_OPTIONS_DISPLAY
        virtual bool setCoreOptionsDisplay(retro_core_option_display const* display) { (void)display; return false; }

    protected:
        // Misc
        static bool preprocessMemoryDescriptors(retro_memory_descriptor* descriptors, unsigned count);
    };

    /**
    * A Video component.
    */
    class Video {
    public:
        virtual ~Video() {}

        // RETRO_ENVIRONMENT_SET_ROTATION
        virtual bool setRotation(unsigned rotation) { (void)rotation; return false; }
        // RETRO_ENVIRONMENT_GET_OVERSCAN
        virtual bool getOverscan(bool* overscan) { (void)overscan; return false; }
        // RETRO_ENVIRONMENT_GET_CAN_DUPE
        virtual bool getCanDupe(bool* canDupe) { (void)canDupe; return false; }
        // RETRO_ENVIRONMENT_SET_MESSAGE
        virtual bool showMessage(retro_message const* message) { (void)message; return false; }
        // RETRO_ENVIRONMENT_SET_PIXEL_FORMAT
        virtual bool setPixelFormat(retro_pixel_format format) { (void)format; return false; }
        // RETRO_ENVIRONMENT_SET_HW_RENDER
        // get_current_framebuffer and get_proc_address are set by Frontend and will call the interface methods
        virtual bool setHwRender(retro_hw_render_callback* callback) { (void)callback; return false; }
        // RETRO_ENVIRONMENT_SET_FRAME_TIME_CALLBACK
        virtual bool setFrameTimeCallback(retro_frame_time_callback const* callback) { (void)callback; return false; }
        // RETRO_ENVIRONMENT_SET_SYSTEM_AV_INFO
        virtual bool setSystemAvInfo(retro_system_av_info const* info) { (void)info; return false; }
        // RETRO_ENVIRONMENT_SET_GEOMETRY
        virtual bool setGeometry(retro_game_geometry const* geometry) { (void)geometry; return false; }
        // RETRO_ENVIRONMENT_GET_CURRENT_SOFTWARE_FRAMEBUFFER
        virtual bool getCurrentSoftwareFramebuffer(retro_framebuffer* framebuffer) { (void)framebuffer; return false; }
        // RETRO_ENVIRONMENT_GET_HW_RENDER_INTERFACE
        virtual bool getHwRenderInterface(retro_hw_render_interface const** ifc) { (void)ifc; return false; }

        // RETRO_ENVIRONMENT_SET_HW_RENDER_CONTEXT_NEGOTIATION_INTERFACE
        virtual bool setHwRenderContextNegotiationInterface(retro_hw_render_context_negotiation_interface const* ifc) {
            (void)ifc;
            return false;
        }

        // RETRO_ENVIRONMENT_SET_HW_SHARED_CONTEXT
        virtual bool setHwSharedContext() { return false; }
        // RETRO_ENVIRONMENT_GET_TARGET_REFRESH_RATE
        virtual bool getTargetRefreshRate(float* rate) { (void)rate; return false; }
        // RETRO_ENVIRONMENT_GET_PREFERRED_HW_RENDER
        virtual bool getPreferredHwRender(unsigned* preferred) { (void)preferred; return false; }

        // Callbacks
        virtual void refresh(void const* data, unsigned width, unsigned height, size_t pitch) {
            (void)data;
            (void)width;
            (void)height;
            (void)pitch;
        }

        // Interface
        virtual uintptr_t getCurrentFramebuffer() { return 0; }
        virtual retro_proc_address_t getProcAddress(char const* symbol) { (void)symbol; return nullptr; }
    };

    /**
     * A component to set LED states.
     */
    class Led {
    public:
        virtual ~Led() {}

        // RETRO_ENVIRONMENT_GET_LED_INTERFACE
        // retro_led_interface.set_led_state is set in Frontend and will call the interface methods

        // Interface
        virtual void setState(int led, int state) { (void)led; (void)state; }
    };

    /**
    * An audio component.
    */
    class Audio {
    public:
        virtual ~Audio() {}

        // RETRO_ENVIRONMENT_SET_SYSTEM_AV_INFO
        virtual bool setSystemAvInfo(retro_system_av_info const* info) { (void)info; return false; }
        // RETRO_ENVIRONMENT_SET_AUDIO_CALLBACK
        virtual bool setAudioCallback(retro_audio_callback const* callback) { (void)callback; return false; }

        // Callbacks
        virtual size_t sampleBatch(int16_t const* data, size_t frames) { (void)data; (void)frames; return 0; }
        virtual void sample(int16_t left, int16_t right);
    };

    /**
     * A component to directly use MIDI devices.
     */
    class Midi {
    public:
        virtual ~Midi() {}

        // RETRO_ENVIRONMENT_GET_MIDI_INTERFACE
        // retro_midi_interface.input_enabled, output_enabled, read, write, and flush are set in Frontend and will call the
        // interface methods

        // Interface
        virtual bool inputEnabled() { return false; }
        virtual bool outputEnabled() { return false; }
        virtual bool read(uint8_t* byte) { (void)byte; return false; }
        virtual bool write(uint8_t byte, uint32_t deltaTime) { (void)byte; (void)deltaTime; return false; }
        virtual bool flush() { return false; }
    };

    /**
    * A component that provides input state to CoreWrap instances.
    */
    class Input {
    public:
        virtual ~Input() {}

        // RETRO_ENVIRONMENT_SET_INPUT_DESCRIPTORS
        virtual bool setInputDescriptors(retro_input_descriptor const* descriptors) { (void)descriptors; return false; }
        // RETRO_ENVIRONMENT_SET_KEYBOARD_CALLBACK
        virtual bool setKeyboardCallback(retro_keyboard_callback const* callback) { (void)callback; return false; }
        // RETRO_ENVIRONMENT_GET_INPUT_DEVICE_CAPABILITIES
        virtual bool getInputDeviceCapabilities(uint64_t* capabilities) { (void)capabilities; return false; }
        // RETRO_ENVIRONMENT_SET_CONTROLLER_INFO
        virtual bool setControllerInfo(retro_controller_info const* info) { (void)info; return false; }
        // RETRO_ENVIRONMENT_GET_INPUT_BITMASKS
        virtual bool getInputBitmasks(bool* supports) { (void)supports; return false; }

        // Callbacks
        virtual int16_t state(unsigned port, unsigned device, unsigned index, unsigned id) {
            (void)port;
            (void)device;
            (void)index;
            (void)id;
            return 0;
        }

        virtual void poll() {}
    };

    /**
     * A component to make rumble effects.
     */
    class Rumble {
    public:
        virtual ~Rumble() {}

        // RETRO_ENVIRONMENT_GET_RUMBLE_INTERFACE
        // retro_rumble_interface.set_rumble_state is set in Frontend and will call the interface methods

        // Interface
        virtual bool setState(unsigned port, retro_rumble_effect effect, uint16_t strength) {
            (void)port;
            (void)effect;
            (void)strength;
            return false;
        }
    };

    /**
     * A component to read sensor information.
     */
    class Sensor {
    public:
        virtual ~Sensor() {}

        // RETRO_ENVIRONMENT_GET_SENSOR_INTERFACE
        // retro_sensor_interface.set_sensor_state and get_sensor_input are set in Frontend and will call the interface methods

        // Interfaces
        virtual bool setState(unsigned port, retro_sensor_action action, unsigned rate) {
            (void)port;
            (void)action;
            (void)rate;
            return false;
        }

        virtual float getInput(unsigned port, unsigned id) { (void)port; (void)id; return 0.0f; }
    };

    /**
     * A component to read camera framebuffers.
     */
    class Camera {
    public:
        virtual ~Camera() {}

        // RETRO_ENVIRONMENT_GET_CAMERA_INTERFACE
        // retro_camera_callback.start and stop are set by Frontend and will call the interface methods
        virtual bool getCameraInterface(retro_camera_callback const* callback) { (void)callback; return false; }

        // Interface
        virtual bool start() { return false; }
        virtual void stop() {}
    };

    /**
     * A component to get location data.
     */
    class Location {
    public:
        virtual ~Location() {}

        // RETRO_ENVIRONMENT_GET_LOCATION_INTERFACE
        // retro_location_callback.start, stop, get_position, and set_interval are set by Frontend and will call the interface
        // methods
        virtual bool getLocationInterface(retro_location_callback const* callback) { (void)callback; return false; }

        // Interface
        virtual bool start() { return false; }
        virtual void stop() {}

        virtual bool getPosition(double* lat, double* lon, double* horizAccuracy, double* vertAccuracy) {
            (void)lat;
            (void)lon;
            (void)horizAccuracy;
            (void)vertAccuracy;
            return false;
        }

        virtual void setInterval(unsigned intervalMs, unsigned intervalDistance) { (void)intervalMs; (void)intervalDistance; }
    };

    class VirtualFileSystem {
    public:
        virtual ~VirtualFileSystem() {}

        // RETRO_ENVIRONMENT_GET_VFS_INTERFACE
        // retro_vfs_interface_info.retro_vfs_interface is set by Frontend and will call the interface methods
        virtual bool getVfsInterface(retro_vfs_interface_info* callback) { (void)callback; return false; }

        // Interface
        virtual char const* getPath(retro_vfs_file_handle* stream) { (void)stream; return nullptr; }

        virtual retro_vfs_file_handle* open(char const* path, unsigned mode, unsigned hints) {
            (void)path;
            (void)mode;
            (void)hints;
            return nullptr;
        }

        virtual int close(retro_vfs_file_handle* stream) { (void)stream; return -1; }
        virtual int64_t size(retro_vfs_file_handle* stream) { (void)stream; return -1; }
        virtual int64_t truncate(retro_vfs_file_handle* stream, int64_t length) { (void)stream; (void)length; return -1; }
        virtual int64_t tell(retro_vfs_file_handle* stream) { (void)stream; return -1; }

        virtual int64_t seek(retro_vfs_file_handle* stream, int64_t offset, int seekPosition) {
            (void)stream;
            (void)offset;
            (void)seekPosition;
            return -1;
        }

        virtual int64_t read(retro_vfs_file_handle* stream, void* s, uint64_t len) {
            (void)stream;
            (void)s;
            (void)len;
            return -1;
        }

        virtual int64_t write(retro_vfs_file_handle* stream, void const* s, uint64_t len) {
            (void)stream;
            (void)s;
            (void)len;
            return -1;
        }

        virtual int flush(retro_vfs_file_handle* stream) { (void)stream; return -1; }
        virtual int remove(char const* path) { (void)path; return -1; }
        virtual int rename(char const* oldPath, char const* newPath) { (void)oldPath; (void)newPath; return -1; }
        virtual int stat(char const* path, int32_t* size) { (void)path; (void)size; return -1; }
        virtual int mkDir(char const* dir) { (void)dir; return -1; }

        virtual retro_vfs_dir_handle* openDir(char const* dir, bool includeHidden) {
            (void)dir;
            (void)includeHidden;
            return nullptr;
        }

        virtual bool readDir(retro_vfs_dir_handle* dirstream) { (void)dirstream; return false; }
        virtual char const* direntGetName(retro_vfs_dir_handle* dirstream) { (void)dirstream; return nullptr; }
        virtual bool direntIsDir(retro_vfs_dir_handle* dirstream) { (void)dirstream; return false; }
        virtual int closeDir(retro_vfs_dir_handle* dirstream) { (void)dirstream; return -1; }
    };

    class DiskControl {
    public:
        virtual ~DiskControl() {}

        // RETRO_ENVIRONMENT_GET_DISK_CONTROL_INTERFACE_VERSION
        virtual bool getDiskControlInterfaceVersion(unsigned* const version) { (void)version; return false; }
        // RETRO_ENVIRONMENT_SET_DISK_CONTROL_INTERFACE
        virtual bool setDiskControlInterface(retro_disk_control_callback const* ifc) { (void)ifc; return false; }

        // RETRO_ENVIRONMENT_SET_DISK_CONTROL_EXT_INTERFACE
        virtual bool setDiskControlExtInterface(retro_disk_control_ext_callback const* ifc) {
            (void)ifc;
            return false;
        }
    };

    /**
     * A component to get performance information from the system.
     */
    class Perf {
    public:
        virtual ~Perf() {}

        // RETRO_ENVIRONMENT_GET_PERF_INTERFACE
        // retro_perf_callback.get_time_usec, get_cpu_features, get_perf_counter, perf_register, perf_start, perf_stop, and
        // perf_log are set by Frontend and will call the interface methods

        // Interface
        virtual retro_time_t getTimeUsec() { return 0; }
        virtual uint64_t getCpuFeatures() { return 0; }
        virtual retro_perf_tick_t getCounter() { return 0; }
        virtual void register_(retro_perf_counter* counter) { (void)counter; }
        virtual void start(retro_perf_counter* counter) { (void)counter; }
        virtual void stop(retro_perf_counter* counter) { (void)counter; }
        virtual void log() {}
    };
}

#endif // LRCPP_COMPONENTS_H__
