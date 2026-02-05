# **lrcpp**

**lrcpp** is a library meant to ease the development of Libretro front-ends. It has three blocks:

* A `Frontend` class that manages a Libretro core instance via the FSM, and takes care of all the callbacks and environment calls. It delegates all functionalities to the component classes, which must be implemented and passed to the `Frontend` instance.
* Components, classes that provide front-end functionalities to the core.
* A [Finite State Machine](https://en.wikipedia.org/wiki/Finite-state_machine) (FSM) that manages the state of the core, preventing it from entering an invalid state like running a frame before initializing the core.

## Usage

In order to have a working application several classes must be implemented. Their methods map almost directly to the Libretro API and should be self explanatory.

The minimum set of components required to run the more simple cores are`Audio` and `Video` (only the software framebuffer methods need be implemented for most of the cores, so `setHwRender` can return `false`). `Input` must also be provided in order to be able to interact with the emulation, of course. Some cores will also need configuration information to run, and thus will need a `Config` component. It doesn't hurt to provide a `Logger` component since it's easy to implement.

## API

### Frontend

The `Frontend` class manages a `Core`'s' life-cycle, and connects it to the platform specific code needed for it to run. It does so via platform dependent components that are responsible for thigs like video and audio output, controller and camera input, and so on. `Frontend` also takes care of calling the core's functions and set the necessary callbacks for it to use, and routes the environment calls from the core to the correct components.

* Life-cycle
  * Since it's not possible to run more than one Libretro core at the same time (imposed by a limitation of the Libretro API), the `Frontend` class is a singleton.
    * `static Frontend& getInstance()`: returns the front-end instance.
* Components, each method will set a new component and return `true` if successful, of `false` if the component cannot be set. Setting (or leaving) a component as `nullptr` will make the corresponding functionality unavailable for the core. Components can only be set when there's no core loaded.
  * `bool setLogger(Logger* logger)`
  * `bool setConfig(Config* config)`
  * `bool setVideo(Video* video)`
  * `bool setLed(Led* led)`
  * `bool setAudio(Audio* audio)`
  * `bool setMidi(Midi* midi)`
  * `bool setInput(Input* input)`
  * `bool setRumble(Rumble* rumble)`
  * `bool setSensor(Sensor* sensor)`
  * `bool setCamera(Camera* camera)`
  * `bool setLocation(Location* location)`
  * `bool setVirtualFileSystem(VirtualFileSystem* virtualFileSystem)`
  * `bool setDiskControl(DiskControl* diskControl)`
  * `bool serPerf(Perf* perf)`
* There are also getters for each of the components.
  * `Logger* getLogger()`
  * `Config* getConfig()`
  * `Video* getVideo()`
  * `Led* getLed()`
  * `Audio* getAudio()`
  * `Midi* getMidi()`
  * `Input* getInput()`
  * `Rumble* getRumble()`
  * `Sensor* getSensor()`
  * `Camera* getCamera()`
  * `Location* getLocation()`
  * `VirtualFileSystem* getVirtualFileSystem()`
  * `DiskControl* getDiskControl()`
  * `Perf* getPerf()`
* Managed core life-cycle. These methods take into account the current state of the core and will return `false` if it detects inconsistencies like trying to run a frame with a core that has not being loaded. They also return `false` if they fail for any other reason.
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

### Components

#### Logger

Logs messages from the core. Only one method must be implemented.

* Interface
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
  * `bool setSupportAchievements(bool supports)`
  * `bool setSerializationQuirks(uint64_t quirks)`
  * `bool getAudioVideoEnable(int* enabled)`
  * `bool getFastForwarding(bool* is)`
  * `bool setCoreOptions(struct retro_core_option_definition const** options)`
  * `bool setCoreOptionsIntl(struct retro_core_options_intl const* intl)`
  * `bool setCoreOptionsDisplay(struct retro_core_option_display const* display)`
* The following methods are already implemented:
  * `bool getLanguage(unsigned* language)`: returns `RETRO_LANGUAGE_ENGLISH`.
  * `bool setVariables(struct retro_variable const* variables)`: Creates an equivalent `retro_core_options_v2_intl` for the variables and calls `setCoreOptionsV2Intl`.
  * `bool setCoreOptions(retro_core_option_definition const* options)`: Same as above.
  * `bool setCoreOptionsIntl(retro_core_options_intl const* intl)`: Same as above.
  * `bool getCoreOptionsVersion(unsigned* version)`: Always sets `version` to 2.
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

* Interface
  * `void setState(int led, int state)`

#### Audio

Component that receives audio frames and must produce the audio output.

* Environment calls
  * `bool setSystemAvInfo(retro_system_av_info const* info)`
  * `bool setAudioCallback(struct retro_audio_callback const* callback)`
* Callbacks
  * `size_t sampleBatch(int16_t const* data, size_t frames)`
  * `void sample(int16_t left, int16_t right)`

#### Midi

Provides the core access to the a MIDI device for raw I/O.

* Interface
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

Allows to set the state of the rumble motors in controllers.

* Interfaces
  * `bool setState(unsigned port, enum retro_rumble_effect effect, uint16_t strength)`

#### Sensor

Controls sensors like accelerometers.

* Interface
  * `bool setState(unsigned port, enum retro_sensor_action action, unsigned rate)`
  * `float getInput(unsigned port, unsigned id)`

#### Camera

Provides access to an attached camera.

* Environment calls
  * `bool getCameraInterface(retro_camera_callback const* callback)`
* Interface
  * `bool start()`
  * `void stop()`

#### Location

Provides location information to the core.

* Environment calls
  * `bool getLocationInterface(retro_location_callback const* callback)`
* Interface
  * `bool start()`
  * `void stop()`
  * `bool getPosition(double* lat, double* lon, double* horizAccuracy, double* vertAccuracy)`
  * `void setInterval(unsigned intervalMs, unsigned intervalDistance)`

#### VirtualFileSystem

Provides the core with a virtual file system interface.

* Environment calls
  * `bool getVfsInterface(retro_vfs_interface_info const* callback)`
* Interface
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

Interface with the core to control removable midia.

* Environment calls
  * `bool getDiskControlInterfaceVersion(unsigned* const version)`
  * `bool setDiskControlInterface(struct retro_disk_control_callback const* interface)`
  * `bool setDiskControlExtInterface(struct retro_disk_control_ext_callback const* interface)`

#### Perf

Utility interface for performance measurement.

* Interfaces
  * `retro_time_t getTimeUsec()`
  * `uint64_t getCpuFeatures()`
  * `retro_perf_tick_t getCounter()`
  * `void register_(struct retro_perf_counter* counter)`
  * `void start(struct retro_perf_counter* counter)`
  * `void stop(struct retro_perf_counter* counter)`
  * `void log()`
