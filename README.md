# **lrcpp**

**lrcpp** is a library meant to ease the development of Libretro frontends. It has three components:

* A `Core` class that can load a Libretro core from disk, and has methods that map 1:1 with the ones of the Libretro API.
* A `Frontend` class that manages a `Core` instance, and takes care of all the callbacks and environment calls. It delegates all functionalities to other classes, which must be implemented and passed to the `Frontend` instance.

## Usage

### Core

The `Core` class manages the loading and unloading of a shared object that exposts the functions of the Libretro API. It has the following methods:

* Lifecycle
  * `Core()`: Contructs an empty core.
  * `~Core()`: Destructs the core, unloading the shared object that was load by `load`.
  * `bool load(char const* path)`: Loads a shared object and gets all the symbols of the Libretro API from it. Returns true if on success.
  * `void unload()` Unloads the shared object.
* Libretro API
  * `void init()`
  * `void deinit()`
  * `unsigned apiVersion()`
  * `void getSystemInfo(struct retro_system_info* info)`
  * `void getSystemAvInfo(struct retro_system_av_info* info)`
  * `void setEnvironment(retro_environment_t cb)`
  * `void setVideoRefresh(retro_video_refresh_t cb)`
  * `void setAudioSample(retro_audio_sample_t cb)`
  * `void setAudioSampleBatch(retro_audio_sample_batch_t cb)`
  * `void setInputPoll(retro_input_poll_t cb)`
  * `void setInputState(retro_input_state_t cb)`
  * `void setControllerPortDevice(unsigned port, unsigned device)`
  * `void reset()`
  * `void run()`
  * `size_t serializeSize()`
  * `bool serialize(void* data, size_t size)`
  * `bool unserialize(void const* data, size_t size)`
  * `void cheatReset()`
  * `void cheatSet(unsigned index, bool enabled, char const* code)`
  * `bool loadGame(struct retro_game_info const* game)`
  * `bool loadGameSpecial(unsigned game_type, struct retro_game_info const* info, size_t num_info)`
  * `void unloadGame()`
  * `unsigned getRegion()`
  * `void* getMemoryData(unsigned id)`
  * `size_t getMemorySize(unsigned id)`

### Frontend

The `Frontend` class manages a `Core`'s' lifecycle, and connects it to the platform specific code needed for it to run. It does so via platform dependent components that are responsible for thigs like video and audio output, controller and camera input, and so on. `Frontend` also takes care of calling the core's functions and set the necessary callbacks for it to use, and routes the environment calls from the core to the correct components.

* Lifecycle
  * `Frontend()`: Constructs a frontend without any components.
  * `~Frontend()`: Destructs the frontend. The destructor will handle the correct shutdown of the core it manages irrespectively of its state: game loaded, running etc.
* Components, each method will set a new component and return the old one. Setting (or leaving) a component as `nullptr` will make the corresponding functionality unavailable for the core.
  * `Logger* setLogger(Logger* logger)`
  * `Config* setConfig(Config* config)`
  * `Video* setVideo(Video* video)`
  * `Led* setLed(Led* led)`
  * `Audio* setAudio(Audio* audio)`
  * `Midi* setMidi(Midi* midi)`
  * `Input* setInput(Input* input)`
  * `Rumble* setRumble(Rumble* rumble)`
  * `Sensor* setSensor(Sensor* sensor)`
  * `Camera* setCamera(Camera* camera)`
  * `Location* setLocation(Location* location)`
  * `VirtualFileSystem* setVirtualFileSystem(VirtualFileSystem* virtualFileSystem)`
  * `DiskControl* setDiskControl(DiskControl* diskControl)`
  * `Perf* serPerf(Perf* perf)`
  * `FileSystem* setFileSystem(FileSystem* fileSystem)`: The file system component is not used by the core, but by the frontend to load content and pass it to the core.
* Managed core fife-cycle. These methods take into account the current state of the core and will return `false` if it detects inconsistencies like trying to run a frame with a core that has not being loaded. They also return `false` if they fail for any other reason.
  * `bool load(char const* corePath)`: Loads a core from the file system.
  * `bool loadGame()`: Starts the core without a game, only available for cores that call `RETRO_ENVIRONMENT_SET_SUPPORT_NO_GAME` with `true`.
  * `bool loadGame(char const* gamePath)`: Starts a core with the given content path. The core will be responsible for loading the content. Only valid if `retro_system_info.need_fullpath` is `true`.
  * `bool loadGame(char const* gamePath, void const* data, size_t size)`: Starts a core with the given content. `data` and `size` must be valid, but `gamePath` is allowed to be `nullptr`.
  * `bool loadGameSpecial(unsigned gameType, struct retro_game_info const* info, size_t numInfo)`: As the Libretro API says, loads a "special" kind of game.
  * `bool run()`: Runs one video frame worth of emulation, generating enough audio frames to cover for the time it takes to generate the video.
  * `bool reset()`: Resets the current game.
  * `bool unloadGame()`: Unloads the game.
  * `bool unload()`: Unloads the core.
* Other Libretro API calls. These methods, like the ones above, will also return `false` if called when the core is in an invalid state. As an example, `getSystemAvInfo` can only be called when a game has been loaded or is running. The core states and available calls per state are in `CoreFsm.fsm`.
  * `bool apiVersion(unsigned* version)`
  * `bool getSystemInfo(struct retro_system_info* info)`
  * `bool getSystemAvInfo(struct retro_system_av_info* info)`
  * `bool serializeSize(size_t* size)`
  * `bool serialize(void* data, size_t size)`
  * `bool unserialize(void const* data, size_t size)`
  * `bool cheatReset()`
  * `bool cheatSet(unsigned index, bool enabled, char const* code)`
  * `bool getRegion(unsigned* region)`
  * `bool getMemoryData(unsigned id, void** data)`
  * `bool getMemorySize(unsigned id, size_t* size)`

In order to have a working frontend, several classes must be implemented. Their methods map almost directly to the Libretro API and should be self explanatory.

The minimum set of components required to run the more simple cores are`Audio` and `Video` (only the software framebuffer methods need be implemented for most of the cores, so `setHwRender` can return `false`). `Input` must also be provided in order to be able to interact with the emulation, of course. Some cores will also need configuration information to run, and thus will need a `Config` component. It doesn't hurt to provide a `Logger` component, since it's easy to implement.

#### Logger

Logs messages from the core. Only one method must be implemented.

* Interfaces
  * `void vprintf(enum retro_log_level level, char const* format, va_list args)`

#### Config

Deals with everything related to configuration and then some. Several methods must be implemented.

* Environment calls
  * `bool setPerformanceLevel(unsigned level)`
  * `bool getSystemDirectory(char const** directory)`
  * `bool getVariable(struct retro_variable* variable)`
  * `bool getVariableUpdate(bool* const updated)`
  * `bool setSupportNoGame(bool const supports)`
  * `bool getLibretroPath(char const** path)`
  * `bool getCoreAssetsDirectory(char const** directory)`
  * `bool getSaveDirectory(char const** directory)`
  * `bool setProcAddressCallback(struct retro_get_proc_address_interface const* callback)`
  * `bool setSubsystemInfo(struct retro_subsystem_info const* info)`
  * `bool setMemoryMaps(struct retro_memory_map const* map)`
  * `bool getUsername(char const** username)`
  * `bool getLanguage(unsigned* language)`
  * `bool setSupportAchievements(bool supports)`
  * `bool setSerializationQuirks(uint64_t quirks)`
  * `bool getAudioVideoEnable(int* enabled)`
  * `bool getFastForwarding(bool* is)`
  * `bool setCoreOptions(struct retro_core_option_definition const** options)`
  * `bool setCoreOptionsIntl(struct retro_core_options_intl const* intl)`
  * `bool setCoreOptionsDisplay(struct retro_core_option_display const* display)`
  * The following methods are already implemented:
    * `bool setVariables(struct retro_variable const* variables)`: Creates equivalent `retro_core_option_definition`s for the variables and calls `setCoreOptions`.
    * `bool getCoreOptionsVersion(unsigned* version)`: Always sets `version` to 1.
* The following method that doesn't belong to the libretro API is implemented:
  * `bool preprocessMemoryDescriptors(struct retro_memory_descriptor* descriptors, unsigned count)`: Pre-processes the memory descriptors that are received in the `setMemoryMaps` method.

#### Video

Takes care of everything related with video output. The methods that must be implemented are the following:

* Environment calls
  * `bool setRotation(unsigned rotation)`
  * `bool getOverscan(bool* data)`
  * `bool getCanDupe(bool* data)`
  * `bool showMessage(struct retro_message const* message)`
  * `bool setPixelFormat(enum retro_pixel_format format)`
  * `bool setHwRender(struct retro_hw_render_callback* callback)`
  * `bool setFrameTimeCallback(struct retro_frame_time_callback const* callback)`
  * `bool setSystemAvInfo(struct retro_system_av_info const* info)`
  * `bool setGeometry(struct retro_game_geometry const* geometry)`
  * `bool getCurrentSoftwareFramebuffer(struct retro_framebuffer* const framebuffer)`
  * `bool getHwRenderInterface(struct retro_hw_render_interface const** interface)`
  * `bool setHwRenderContextNegotiationInterface(struct retro_hw_render_context_negotiation_interface const* interface)`
  * `bool setHwSharedContext()`
  * `bool getTargetRefreshRate(float* rate)`
  * `bool getPreferredHwRender(unsigned* preferred)`
* Callbacks
  * `void refresh(void const* data, unsigned width, unsigned height, size_t pitch)`

#### Led

Changes the state of the system's LEDs.

* Environment calls
  * `void setState(int led, int state)`

#### Audio

Component that receives audio frames and must produce the audio output.

* Environment calls
  * `bool setAudioCallback(struct retro_audio_callback const* callback)`
* Callbacks
  * `size_t sampleBatch(int16_t const* data, size_t frames)`
  * `void sample(int16_t left, int16_t right)`

#### Midi

Provides the core access to the system's MIDI device.

* Interfaces
  * `bool inputEnabled()`
  * `bool outputEnabled()`
  * `bool read(uint8_t* byte)`
  * `bool write(uint8_t byte, uint32_t deltaTime)`
  * `bool flush()`

#### Input

Deals with all input devices.

* Environment calls
  * `bool setInputDescriptors(struct retro_input_descriptor const* descriptors)`
  * `bool setKeyboardCallback(struct retro_keyboard_callback const* callback)`
  * `bool getInputDeviceCapabilities(uint64_t* capabilities)`
  * `bool setControllerInfo(struct retro_controller_info const* info)`
  * `bool getInputBitmasks(bool* supports)`
* Callbacks
  * `int16_t state(unsigned port, unsigned device, unsigned index, unsigned id)`
  * `void poll()`

#### Rumble

* Interfaces
  * `bool setState(unsigned port, enum retro_rumble_effect effect, uint16_t strength)`

#### Sensor

* Interfaces
  * `bool setState(unsigned port, enum retro_sensor_action action, unsigned rate)`
  * `float getInput(unsigned port, unsigned id)`

#### Camera

* Interfaces
  * `bool start()`
  * `void stop()`

#### Location

* Interfaces
  * `bool start()`
  * `void stop()`
  * `bool getPosition(double* lat, double* lon, double* horizAccuracy, double* vertAccuracy)`
  * `void setInterval(unsigned intervalMs, unsigned intervalDistance)`

#### VirtualFileSystem

* Interfaces
  * `unsigned getVirtualFileSystemInterfaceVersion()`
  * `char const* getPath(struct retro_vfs_file_handle* stream)`
  * `struct retro_vfs_file_handle* open(char const* path, unsigned mode, unsigned hints)`
  * `int close(struct retro_vfs_file_handle* stream)`
  * `int64_t size(struct retro_vfs_file_handle* stream)`
  * `int64_t truncate(struct retro_vfs_file_handle* stream, int64_t length)`
  * `int64_t tell(struct retro_vfs_file_handle* stream)`
  * `int64_t seek(struct retro_vfs_file_handle* stream, int64_t offset, int seekPosition)`
  * `int64_t read(struct retro_vfs_file_handle* stream, void* s, uint64_t len)`
  * `int64_t write(struct retro_vfs_file_handle* stream, void const* s, uint64_t len)`
  * `int flush(struct retro_vfs_file_handle* stream)`
  * `int remove(char const* path)`
  * `int rename(char const* old_path, char const* new_path)`
  * `int stat(char const* path, int32_t* size)`
  * `int mkDir(char const* dir)`
  * `struct retro_vfs_dir_handle* openDir(char const* dir, bool includeHidden)`
  * `bool readDir(struct retro_vfs_dir_handle* dirstream)`
  * `char const* direntGetName(struct retro_vfs_dir_handle* dirstream)`
  * `bool direntIsDir(struct retro_vfs_dir_handle* dirstream)`
  * `int closeDir(struct retro_vfs_dir_handle* dirstream)`

#### DiskControl

* Environment calls
  * `bool getDiskControlInterfaceVersion(unsigned* const version)`
  * `bool setDiskControlInterface(struct retro_disk_control_callback const* interface)`
  * `bool setDiskControlExtInterface(struct retro_disk_control_ext_callback const* interface)`

#### Perf

* Interfaces
  * `retro_time_t getTimeUsec()`
  * `uint64_t getCpuFeatures()`
  * `retro_perf_tick_t getCounter()`
  * `void register_(struct retro_perf_counter* counter)`
  * `void start(struct retro_perf_counter* counter)`
  * `void stop(struct retro_perf_counter* counter)`
  * `void log()`
