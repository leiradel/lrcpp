#ifndef LRCPP_COMPONENTS_H__
#define LRCPP_COMPONENTS_H__

#include "libretro.h"

#include <stdarg.h>

namespace lrcpp {
    /**
    * A logger component for Core instances.
    */
    class Logger {
    public:
        // RETRO_ENVIRONMENT_GET_LOG_INTERFACE
        // retro_log_callback.log is set in Frontend and will call the interface methods

        // Interface
        virtual void vprintf(retro_log_level level, char const* format, va_list args) = 0;
    };

    /**
    * A component that returns configuration values for SimpleCore instances.
    */
    class Config {
    public:
        // RETRO_ENVIRONMENT_SET_PERFORMANCE_LEVEL
        virtual bool setPerformanceLevel(unsigned level) = 0;
        // RETRO_ENVIRONMENT_GET_SYSTEM_DIRECTORY
        virtual bool getSystemDirectory(char const** directory) = 0;
        // RETRO_ENVIRONMENT_GET_VARIABLE
        virtual bool getVariable(retro_variable* variable) = 0;
        // RETRO_ENVIRONMENT_SET_VARIABLES
        bool setVariables(retro_variable const* variables);
        // RETRO_ENVIRONMENT_GET_VARIABLE_UPDATE
        virtual bool getVariableUpdate(bool* const updated) = 0;
        // RETRO_ENVIRONMENT_SET_SUPPORT_NO_GAME
        virtual bool setSupportNoGame(bool const supports) = 0;
        // RETRO_ENVIRONMENT_GET_LIBRETRO_PATH
        virtual bool getLibretroPath(char const** path) = 0;
        // RETRO_ENVIRONMENT_GET_CORE_ASSETS_DIRECTORY
        virtual bool getCoreAssetsDirectory(char const** directory) = 0;
        // RETRO_ENVIRONMENT_GET_SAVE_DIRECTORY
        virtual bool getSaveDirectory(char const** directory) = 0;
        // RETRO_ENVIRONMENT_SET_PROC_ADDRESS_CALLBACK
        virtual bool setProcAddressCallback(retro_get_proc_address_interface const* callback) = 0;
        // RETRO_ENVIRONMENT_SET_SUBSYSTEM_INFO
        virtual bool setSubsystemInfo(retro_subsystem_info const* info) = 0;
        // RETRO_ENVIRONMENT_SET_MEMORY_MAPS
        virtual bool setMemoryMaps(retro_memory_map const* map) = 0;
        // RETRO_ENVIRONMENT_GET_USERNAME
        virtual bool getUsername(char const** username) = 0;
        // RETRO_ENVIRONMENT_GET_LANGUAGE
        virtual bool getLanguage(unsigned* language) = 0;
        // RETRO_ENVIRONMENT_SET_SUPPORT_ACHIEVEMENTS
        virtual bool setSupportAchievements(bool supports) = 0;
        // RETRO_ENVIRONMENT_SET_SERIALIZATION_QUIRKS
        virtual bool setSerializationQuirks(uint64_t quirks) = 0;
        // RETRO_ENVIRONMENT_GET_AUDIO_VIDEO_ENABLE
        virtual bool getAudioVideoEnable(int* enabled) = 0;
        // RETRO_ENVIRONMENT_GET_FASTFORWARDING
        virtual bool getFastForwarding(bool* is) = 0;
        // RETRO_ENVIRONMENT_GET_CORE_OPTIONS_VERSION
        bool getCoreOptionsVersion(unsigned* version);
        // RETRO_ENVIRONMENT_SET_CORE_OPTIONS
        virtual bool setCoreOptions(retro_core_option_definition const* options) = 0;
        // RETRO_ENVIRONMENT_SET_CORE_OPTIONS_INTL
        virtual bool setCoreOptionsIntl(retro_core_options_intl const* intl) = 0;
        // RETRO_ENVIRONMENT_SET_CORE_OPTIONS_DISPLAY
        virtual bool setCoreOptionsDisplay(retro_core_option_display const* display) = 0;

    protected:
        // Misc
        static bool preprocessMemoryDescriptors(retro_memory_descriptor* descriptors, unsigned count);
    };

    /**
    * A Video component.
    */
    class Video {
    public:
        // RETRO_ENVIRONMENT_SET_ROTATION
        virtual bool setRotation(unsigned rotation) = 0;
        // RETRO_ENVIRONMENT_GET_OVERSCAN
        virtual bool getOverscan(bool* data) = 0;
        // RETRO_ENVIRONMENT_GET_CAN_DUPE
        virtual bool getCanDupe(bool* data) = 0;
        // RETRO_ENVIRONMENT_SET_MESSAGE
        virtual bool showMessage(retro_message const* message) = 0;
        // RETRO_ENVIRONMENT_SET_PIXEL_FORMAT
        virtual bool setPixelFormat(retro_pixel_format format) = 0;
        // RETRO_ENVIRONMENT_SET_HW_RENDER
        // get_current_framebuffer and get_proc_address are set by Frontend and will call the interface methods
        virtual bool setHwRender(retro_hw_render_callback* callback) = 0;
        // RETRO_ENVIRONMENT_SET_FRAME_TIME_CALLBACK
        virtual bool setFrameTimeCallback(retro_frame_time_callback const* callback) = 0;
        // RETRO_ENVIRONMENT_SET_SYSTEM_AV_INFO
        virtual bool setSystemAvInfo(retro_system_av_info const* info) = 0;
        // RETRO_ENVIRONMENT_SET_GEOMETRY
        virtual bool setGeometry(retro_game_geometry const* geometry) = 0;
        // RETRO_ENVIRONMENT_GET_CURRENT_SOFTWARE_FRAMEBUFFER
        virtual bool getCurrentSoftwareFramebuffer(retro_framebuffer* framebuffer) = 0;
        // RETRO_ENVIRONMENT_GET_HW_RENDER_INTERFACE
        virtual bool getHwRenderInterface(retro_hw_render_interface const** interface) = 0;
        // RETRO_ENVIRONMENT_SET_HW_RENDER_CONTEXT_NEGOTIATION_INTERFACE
        virtual bool setHwRenderContextNegotiationInterface(retro_hw_render_context_negotiation_interface const* interface) = 0;
        // RETRO_ENVIRONMENT_SET_HW_SHARED_CONTEXT
        virtual bool setHwSharedContext() = 0;
        // RETRO_ENVIRONMENT_GET_TARGET_REFRESH_RATE
        virtual bool getTargetRefreshRate(float* rate) = 0;
        // RETRO_ENVIRONMENT_GET_PREFERRED_HW_RENDER
        virtual bool getPreferredHwRender(unsigned* preferred) = 0;

        // Callbacks
        virtual void refresh(void const* data, unsigned width, unsigned height, size_t pitch) = 0;

        // Interface
        virtual uintptr_t getCurrentFramebuffer() = 0;
        virtual retro_proc_address_t getProcAddress(char const* symbol) = 0;
    };

    /**
     * A component to set LED states.
     */
    class Led {
    public:
        // RETRO_ENVIRONMENT_GET_LED_INTERFACE
        // retro_led_interface.set_led_state is set in Frontend and will call the interface methods

        // Interface
        virtual void setState(int led, int state) = 0;
    };

    /**
    * An audio component.
    */
    class Audio {
    public:
        // RETRO_ENVIRONMENT_SET_SYSTEM_AV_INFO
        virtual bool setSystemAvInfo(retro_system_av_info const* info) = 0;
        // RETRO_ENVIRONMENT_SET_AUDIO_CALLBACK
        virtual bool setAudioCallback(retro_audio_callback const* callback) = 0;

        // Callbacks
        virtual size_t sampleBatch(int16_t const* data, size_t frames) = 0;
        virtual void sample(int16_t left, int16_t right) = 0;
    };

    /**
     * A component to directly use MIDI devices.
     */
    class Midi {
    public:
        // RETRO_ENVIRONMENT_GET_MIDI_INTERFACE
        // retro_midi_interface.input_enabled, output_enabled, read, write, and flush are set in Frontend and will call the
        // interface methods

        // Interface
        virtual bool inputEnabled() = 0;
        virtual bool outputEnabled() = 0;
        virtual bool read(uint8_t* byte) = 0;
        virtual bool write(uint8_t byte, uint32_t deltaTime) = 0;
        virtual bool flush() = 0;
    };

    /**
    * A component that provides input state to CoreWrap instances.
    */
    class Input {
    public:
        // RETRO_ENVIRONMENT_SET_INPUT_DESCRIPTORS
        virtual bool setInputDescriptors(retro_input_descriptor const* descriptors) = 0;
        // RETRO_ENVIRONMENT_SET_KEYBOARD_CALLBACK
        virtual bool setKeyboardCallback(retro_keyboard_callback const* callback) = 0;
        // RETRO_ENVIRONMENT_GET_INPUT_DEVICE_CAPABILITIES
        virtual bool getInputDeviceCapabilities(uint64_t* capabilities) = 0;
        // RETRO_ENVIRONMENT_SET_CONTROLLER_INFO
        virtual bool setControllerInfo(retro_controller_info const* info) = 0;
        // RETRO_ENVIRONMENT_GET_INPUT_BITMASKS
        virtual bool getInputBitmasks(bool* supports) = 0;

        // Callbacks
        virtual int16_t state(unsigned port, unsigned device, unsigned index, unsigned id) = 0;
        virtual void poll() = 0;
    };

    /**
     * A component to make rumble effects.
     */
    class Rumble {
    public:
        // RETRO_ENVIRONMENT_GET_RUMBLE_INTERFACE
        // retro_rumble_interface.set_rumble_state is set in Frontend and will call the interface methods

        // Interface
        virtual bool setState(unsigned port, retro_rumble_effect effect, uint16_t strength) = 0;
    };

    /**
     * A component to read sensor information.
     */
    class Sensor {
    public:
        // RETRO_ENVIRONMENT_GET_SENSOR_INTERFACE
        // retro_sensor_interface.set_sensor_state and get_sensor_input are set in Frontend and will call the interface methods

        // Interfaces
        virtual bool setState(unsigned port, retro_sensor_action action, unsigned rate) = 0;
        virtual float getInput(unsigned port, unsigned id) = 0;
    };

    /**
     * A component to read camera framebuffers.
     */
    class Camera {
    public:
        // RETRO_ENVIRONMENT_GET_CAMERA_INTERFACE
        // retro_camera_callback.start and stop are set by Frontend and will call the interface methods
        virtual bool getCameraInterface(retro_camera_callback const* callback) = 0;

        // Interface
        virtual bool start() = 0;
        virtual void stop() = 0;
    };

    /**
     * A component to get location data.
     */
    class Location {
    public:
        // RETRO_ENVIRONMENT_GET_LOCATION_INTERFACE
        // retro_location_callback.start, stop, get_position, and set_interval are set by Frontend and will call the interface
        // methods
        virtual bool getLocationInterface(retro_location_callback const* callback) = 0;

        // Interface
        virtual bool start() = 0;
        virtual void stop() = 0;
        virtual bool getPosition(double* lat, double* lon, double* horizAccuracy, double* vertAccuracy) = 0;
        virtual void setInterval(unsigned intervalMs, unsigned intervalDistance) = 0;
    };

    class VirtualFileSystem {
    public:
        // RETRO_ENVIRONMENT_GET_VFS_INTERFACE
        // retro_vfs_interface_info.retro_vfs_interface is set by Frontend and will call the interface methods
        virtual bool getVfsInterface(retro_vfs_interface_info const* callback) = 0;

        // Interface
        virtual char const* getPath(retro_vfs_file_handle* stream) = 0;
        virtual retro_vfs_file_handle* open(char const* path, unsigned mode, unsigned hints) = 0;
        virtual int close(retro_vfs_file_handle* stream) = 0;
        virtual int64_t size(retro_vfs_file_handle* stream) = 0;
        virtual int64_t truncate(retro_vfs_file_handle* stream, int64_t length) = 0;
        virtual int64_t tell(retro_vfs_file_handle* stream) = 0;
        virtual int64_t seek(retro_vfs_file_handle* stream, int64_t offset, int seekPosition) = 0;
        virtual int64_t read(retro_vfs_file_handle* stream, void* s, uint64_t len) = 0;
        virtual int64_t write(retro_vfs_file_handle* stream, void const* s, uint64_t len) = 0;
        virtual int flush(retro_vfs_file_handle* stream) = 0;
        virtual int remove(char const* path) = 0;
        virtual int rename(char const* oldPath, char const* newPath) = 0;
        virtual int stat(char const* path, int32_t* size) = 0;
        virtual int mkDir(char const* dir) = 0;
        virtual retro_vfs_dir_handle* openDir(char const* dir, bool includeHidden) = 0;
        virtual bool readDir(retro_vfs_dir_handle* dirstream) = 0;
        virtual char const* direntGetName(retro_vfs_dir_handle* dirstream) = 0;
        virtual bool direntIsDir(retro_vfs_dir_handle* dirstream) = 0;
        virtual int closeDir(retro_vfs_dir_handle* dirstream) = 0;
    };

    class DiskControl {
    public:
        // RETRO_ENVIRONMENT_GET_DISK_CONTROL_INTERFACE_VERSION
        virtual bool getDiskControlInterfaceVersion(unsigned* const version) = 0;
        // RETRO_ENVIRONMENT_SET_DISK_CONTROL_INTERFACE
        virtual bool setDiskControlInterface(retro_disk_control_callback const* interface) = 0;
        // RETRO_ENVIRONMENT_SET_DISK_CONTROL_EXT_INTERFACE
        virtual bool setDiskControlExtInterface(retro_disk_control_ext_callback const* interface) = 0;
    };

    /**
     * A component to get performance information from the system.
     */
    class Perf {
    public:
        // RETRO_ENVIRONMENT_GET_PERF_INTERFACE
        // retro_perf_callback.get_time_usec, get_cpu_features, get_perf_counter, perf_register, perf_start, perf_stop, and
        // perf_log are set by Frontend and will call the interface methods

        // Interface
        virtual retro_time_t getTimeUsec() = 0;
        virtual uint64_t getCpuFeatures() = 0;
        virtual retro_perf_tick_t getCounter() = 0;
        virtual void register_(retro_perf_counter* counter) = 0;
        virtual void start(retro_perf_counter* counter) = 0;
        virtual void stop(retro_perf_counter* counter) = 0;
        virtual void log() = 0;
    };
}

#endif // LRCPP_COMPONENTS_H__
