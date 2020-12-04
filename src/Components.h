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
        // Interfaces
        virtual void vprintf(enum retro_log_level level, char const* format, va_list args) = 0;
    };

    /**
    * A component that returns configuration values for SimpleCore instances.
    */
    class Config {
    public:
        // Environment calls
        virtual bool setPerformanceLevel(unsigned level) = 0;
        virtual bool getSystemDirectory(char const** directory) = 0;

        virtual bool getVariable(struct retro_variable* variable) = 0;
        bool setVariables(struct retro_variable const* variables);
        virtual bool getVariableUpdate(bool* const updated) = 0;

        virtual bool setSupportNoGame(bool const supports) = 0;

        virtual bool getLibretroPath(char const** path) = 0;
        virtual bool getCoreAssetsDirectory(char const** directory) = 0;
        virtual bool getSaveDirectory(char const** directory) = 0;

        virtual bool setProcAddressCallback(struct retro_get_proc_address_interface const* callback) = 0;
        virtual bool setSubsystemInfo(struct retro_subsystem_info const* info) = 0;
        virtual bool setMemoryMaps(struct retro_memory_map const* map) = 0;

        virtual bool getUsername(char const** data) = 0;
        virtual bool getLanguage(unsigned* language) = 0;

        virtual bool setSupportAchievements(bool supports) = 0;
        virtual bool setSerializationQuirks(uint64_t quirks) = 0;

        virtual bool getAudioVideoEnable(int* data) = 0;
        virtual bool getFastForwarding(bool* is) = 0;

        bool getCoreOptionsVersion(unsigned* version);
        virtual bool setCoreOptions(struct retro_core_option_definition const** options) = 0;
        virtual bool setCoreOptionsIntl(struct retro_core_options_intl const* intl) = 0;
        virtual bool setCoreOptionsDisplay(struct retro_core_option_display const* display) = 0;

    protected:
        // Misc
        static bool preprocessMemoryDescriptors(struct retro_memory_descriptor* descriptors, unsigned count);
    };

    /**
    * A Video component.
    */
    class Video {
    public:
        // Environment calls
        virtual bool setRotation(unsigned rotation) = 0;
        virtual bool getOverscan(bool* data) = 0;
        virtual bool getCanDupe(bool* data) = 0;

        virtual bool showMessage(struct retro_message const* message) = 0;

        virtual bool setPixelFormat(enum retro_pixel_format format) = 0;
        virtual bool setHwRender(struct retro_hw_render_callback* callback) = 0;

        virtual bool setFrameTimeCallback(struct retro_frame_time_callback const* callback) = 0;
        virtual bool setSystemAvInfo(struct retro_system_av_info const* info) = 0;
        virtual bool setGeometry(struct retro_game_geometry const* geometry) = 0;

        virtual bool getCurrentSoftwareFramebuffer(struct retro_framebuffer* const framebuffer) = 0;
        virtual bool getHwRenderInterface(struct retro_hw_render_interface const** interface) = 0;

        virtual bool setHwRenderContextNegotiationInterface(
            struct retro_hw_render_context_negotiation_interface const* interface) = 0;

        virtual bool setHwSharedContext() = 0;

        virtual bool getTargetRefreshRate(float* rate) = 0;
        virtual bool getPreferredHwRender(unsigned* preferred) = 0;

        // Callbacks
        virtual void refresh(void const* data, unsigned width, unsigned height, size_t pitch) = 0;
    };

    /**
     * A component to set LED states.
     */
    class Led {
    public:
        virtual void setState(int led, int state) = 0;
    };

    /**
    * An audio component.
    */
    class Audio {
    public:
        // Environment calls
        virtual bool setAudioCallback(struct retro_audio_callback const* callback) = 0;

        // Callbacks
        virtual size_t sampleBatch(int16_t const* data, size_t frames) = 0;
        virtual void sample(int16_t left, int16_t right) = 0;
    };

    /**
     * A component to directly use MIDI devices.
     */
    class Midi {
    public:
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
        // Environment calls
        virtual bool setInputDescriptors(struct retro_input_descriptor const* descriptors) = 0;
        virtual bool setKeyboardCallback(struct retro_keyboard_callback const* callback) = 0;

        virtual bool getInputDeviceCapabilities(uint64_t* capabilities) = 0;

        virtual bool setControllerInfo(struct retro_controller_info const* info) = 0;

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
        virtual bool setState(unsigned port, enum retro_rumble_effect effect, uint16_t strength) = 0;
    };

    /**
     * A component to read sensor information.
     */
    class Sensor {
    public:
        virtual bool setState(unsigned port, enum retro_sensor_action action, unsigned rate) = 0;
        virtual float getInput(unsigned port, unsigned id) = 0;
    };

    /**
     * A component to read camera framebuffers.
     */
    class Camera {
    public:
        virtual bool start() = 0;
        virtual void stop() = 0;
    };

    /**
     * A component to get location data.
     */
    class Location {
    public:
        virtual bool start() = 0;
        virtual void stop() = 0;
        virtual bool getPosition(double* lat, double* lon, double* horizAccuracy, double* vertAccuracy) = 0;
        virtual void setInterval(unsigned intervalMs, unsigned intervalDistance) = 0;
    };

    class VirtualFileSystem {
    public:
        virtual unsigned getVirtualFileSystemInterfaceVersion() = 0;

        virtual char const* getPath(struct retro_vfs_file_handle* stream) = 0;
        virtual struct retro_vfs_file_handle* open(char const* path, unsigned mode, unsigned hints) = 0;
        virtual int close(struct retro_vfs_file_handle* stream) = 0;
        virtual int64_t size(struct retro_vfs_file_handle* stream) = 0;
        virtual int64_t truncate(struct retro_vfs_file_handle* stream, int64_t length) = 0;
        virtual int64_t tell(struct retro_vfs_file_handle* stream) = 0;
        virtual int64_t seek(struct retro_vfs_file_handle* stream, int64_t offset, int seekPosition) = 0;
        virtual int64_t read(struct retro_vfs_file_handle* stream, void* s, uint64_t len) = 0;
        virtual int64_t write(struct retro_vfs_file_handle* stream, void const* s, uint64_t len) = 0;
        virtual int flush(struct retro_vfs_file_handle* stream) = 0;
        virtual int remove(char const* path) = 0;
        virtual int rename(char const* old_path, char const* new_path) = 0;
        virtual int stat(char const* path, int32_t* size) = 0;
        virtual int mkDir(char const* dir) = 0;
        virtual struct retro_vfs_dir_handle* openDir(char const* dir, bool includeHidden) = 0;
        virtual bool readDir(struct retro_vfs_dir_handle* dirstream) = 0;
        virtual char const* direntGetName(struct retro_vfs_dir_handle* dirstream) = 0;
        virtual bool direntIsDir(struct retro_vfs_dir_handle* dirstream) = 0;
        virtual int closeDir(struct retro_vfs_dir_handle* dirstream) = 0;
    };

    class DiskControl {
    public:
        virtual bool getDiskControlInterfaceVersion(unsigned* const version) = 0;
        virtual bool setDiskControlInterface(struct retro_disk_control_callback const* interface);
        virtual bool setDiskControlExtInterface(struct retro_disk_control_ext_callback const* interface);
    };

    /**
     * A component to get performance information from the system.
     */
    class Perf {
    public:
        virtual retro_time_t getTimeUsec() = 0;
        virtual uint64_t getCpuFeatures() = 0;
        virtual retro_perf_tick_t getCounter() = 0;
        virtual void register_(struct retro_perf_counter* counter) = 0;
        virtual void start(struct retro_perf_counter* counter) = 0;
        virtual void stop(struct retro_perf_counter* counter) = 0;
        virtual void log() = 0;
    };
}

#endif // LRCPP_COMPONENTS_H__
