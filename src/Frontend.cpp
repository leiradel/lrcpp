/*
MIT License

Copyright (c) 2020 Andre Leiradella

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "Frontend.h"

#include <new>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

/**
 * Unavoidable because the Libretro API don't have an userdata pointer to
 * allow us pass and receive back the core instance :/
 */
static thread_local lrcpp::Frontend* s_instance;

namespace {
    // Helper class to set and unset the s_instance thread local.
    class InstanceSetter {
    public:
        InstanceSetter(lrcpp::Frontend* instance) : _previous(s_instance) {
            s_instance = instance;
        }
        
        ~InstanceSetter() {
            s_instance = _previous;
        }

    protected:
        lrcpp::Frontend* const _previous;
    };
}

lrcpp::Frontend::Frontend()
    : _logger(nullptr)
    , _config(nullptr)
    , _video(nullptr)
    , _led(nullptr)
    , _audio(nullptr)
    , _midi(nullptr)
    , _input(nullptr)
    , _rumble(nullptr)
    , _sensor(nullptr)
    , _camera(nullptr)
    , _location(nullptr)
    , _virtualFileSystem(nullptr)
    , _diskControl(nullptr)
    , _perf(nullptr)
    , _fileSystem(nullptr)
    , _supportsNoGame(false)
    , _fsm(_core)
{}

lrcpp::Logger* lrcpp::Frontend::setLogger(Logger* logger) {
    Logger* previous = _logger;
    _logger = logger;
    return previous;
}

lrcpp::Config* lrcpp::Frontend::setConfig(Config* config) {
    Config* previous = _config;
    _config = config;
    return previous;
}

lrcpp::Video* lrcpp::Frontend::setVideo(Video* video) {
    Video* previous = _video;
    _video = video;
    return previous;
}

lrcpp::Led* lrcpp::Frontend::setLed(Led* led) {
    Led* previous = _led;
    _led = led;
    return previous;
}

lrcpp::Audio* lrcpp::Frontend::setAudio(Audio* audio) {
    Audio* previous = _audio;
    _audio = audio;
    return previous;
}

lrcpp::Midi* lrcpp::Frontend::setMidi(Midi* midi) {
    Midi* previous = _midi;
    _midi = midi;
    return previous;
}

lrcpp::Input* lrcpp::Frontend::setInput(Input* input) {
    Input* previous = _input;
    _input = input;
    return previous;
}

lrcpp::Rumble* lrcpp::Frontend::setRumble(Rumble* rumble) {
    Rumble* previous = _rumble;
    _rumble = rumble;
    return previous;
}

lrcpp::Sensor* lrcpp::Frontend::setSensor(Sensor* sensor) {
    Sensor* previous = _sensor;
    _sensor = sensor;
    return previous;
}

lrcpp::Camera* lrcpp::Frontend::setCamera(Camera* camera) {
    Camera* previous = _camera;
    _camera = camera;
    return previous;
}

lrcpp::Location* lrcpp::Frontend::setLocation(Location* location) {
    Location* previous = _location;
    _location = location;
    return previous;
}

lrcpp::VirtualFileSystem* lrcpp::Frontend::setVirtualFileSystem(VirtualFileSystem* virtualFileSystem) {
    VirtualFileSystem* previous = _virtualFileSystem;
    _virtualFileSystem = virtualFileSystem;
    return previous;
}

lrcpp::DiskControl* lrcpp::Frontend::setDiskControl(DiskControl* diskControl) {
    DiskControl* previous = _diskControl;
    _diskControl = diskControl;
    return previous;
}

lrcpp::Perf* lrcpp::Frontend::serPerf(Perf* perf) {
    Perf* previous = _perf;
    _perf = perf;
    return previous;
}

lrcpp::FileSystem* lrcpp::Frontend::setFileSystem(FileSystem* fileSystem) {
    FileSystem* previous = _fileSystem;
    _fileSystem = fileSystem;
    return previous;
}

bool lrcpp::Frontend::load(char const* corePath) {
    InstanceSetter setter(this);

    bool ok = _fsm.load(corePath);
    ok = ok && _fsm.setEnvironment(staticEnvironmentCallback);
    ok = ok && _fsm.init();

    return ok;
}

bool lrcpp::Frontend::loadGame() {
    InstanceSetter setter(this);

    if (!_supportsNoGame) {
        return false;
    }

    struct retro_system_info systemInfo;

    if (!_fsm.getSystemInfo(&systemInfo)) {
        return false;
    }

    retro_game_info game;
    game.path = nullptr;
    game.data = nullptr;
    game.size = 0;
    game.meta = nullptr;

    bool ok = _fsm.loadGame(&game);
    ok = ok && _fsm.setCallbacks(staticVideoRefreshCallback,
                                 staticAudioSampleCallback,
                                 staticAudioSampleBatchCallback,
                                 staticInputPollCallback,
                                 staticInputStateCallback);

    return ok;
}

bool lrcpp::Frontend::loadGame(char const* gamePath) {
    InstanceSetter setter(this);

    if (_fileSystem == nullptr) {
        return false;
    }

    struct retro_system_info systemInfo;

    if (!_fsm.getSystemInfo(&systemInfo)) {
        return false;
    }

    retro_game_info game;
    game.path = gamePath;
    game.data = nullptr;
    game.size = 0;
    game.meta = nullptr;

    if (!systemInfo.need_fullpath) {
        game.data = _fileSystem->load(gamePath, &game.size);

        if (game.data == nullptr) {
            return false;
        }
    }

    bool ok = _fsm.loadGame(&game);
    ok = ok && _fsm.setCallbacks(staticVideoRefreshCallback,
                                 staticAudioSampleCallback,
                                 staticAudioSampleBatchCallback,
                                 staticInputPollCallback,
                                 staticInputStateCallback);

    _fileSystem->free(game.data);
    return ok;
}

bool lrcpp::Frontend::loadGameSpecial(unsigned gameType, struct retro_game_info const* info, size_t numInfo) {
    InstanceSetter setter(this);
    struct retro_system_info systemInfo;

    if (!_fsm.getSystemInfo(&systemInfo)) {
        return false;
    }

    bool ok = _fsm.loadGameSpecial(gameType, info, numInfo);
    ok = ok && _fsm.setCallbacks(staticVideoRefreshCallback,
                                 staticAudioSampleCallback,
                                 staticAudioSampleBatchCallback,
                                 staticInputPollCallback,
                                 staticInputStateCallback);

    return ok;
}

bool lrcpp::Frontend::run() {
    InstanceSetter setter(this);
    return _fsm.run();
}

bool lrcpp::Frontend::reset() {
    InstanceSetter setter(this);
    return _fsm.reset();
}

bool lrcpp::Frontend::unloadGame() {
    InstanceSetter setter(this);
    return _fsm.unloadGame();
}

bool lrcpp::Frontend::unload() {
    InstanceSetter setter(this);
    return _fsm.unload();
}

bool lrcpp::Frontend::apiVersion(unsigned* version) {
    InstanceSetter setter(this);
    return _fsm.apiVersion(version);
}

bool lrcpp::Frontend::getSystemInfo(struct retro_system_info* info) {
    InstanceSetter setter(this);
    return _fsm.getSystemInfo(info);
}

bool lrcpp::Frontend::getSystemAvInfo(struct retro_system_av_info* info) {
    InstanceSetter setter(this);
    return _fsm.getSystemAvInfo(info);
}

bool lrcpp::Frontend::serializeSize(size_t* size) {
    InstanceSetter setter(this);
    return _fsm.serializeSize(size);
}

bool lrcpp::Frontend::serialize(void* data, size_t size) {
    InstanceSetter setter(this);
    return _fsm.serialize(data, size);
}

bool lrcpp::Frontend::unserialize(void const* data, size_t size) {
    InstanceSetter setter(this);
    return _fsm.unserialize(data, size);
}

bool lrcpp::Frontend::cheatReset() {
    InstanceSetter setter(this);
    return _fsm.cheatReset();
}

bool lrcpp::Frontend::cheatSet(unsigned index, bool enabled, char const* code) {
    InstanceSetter setter(this);
    return _fsm.cheatSet(index, enabled, code);
}

bool lrcpp::Frontend::getRegion(unsigned* region) {
    InstanceSetter setter(this);
    return _fsm.getRegion(region);
}

bool lrcpp::Frontend::getMemoryData(unsigned id, void** data) {
    InstanceSetter setter(this);
    return _fsm.getMemoryData(id, data);
}

bool lrcpp::Frontend::getMemorySize(unsigned id, size_t* size) {
    InstanceSetter setter(this);
    return _fsm.getMemorySize(id, size);
}

bool lrcpp::Frontend::setRotation(unsigned data) {
    return _video == nullptr ? false : _video->setRotation(data);
}

bool lrcpp::Frontend::getOverscan(bool* data) {
    return _video == nullptr ? false : _video->getOverscan(data);
}

bool lrcpp::Frontend::getCanDupe(bool* data) {
    return _video == nullptr ? false : _video->getCanDupe(data);
}

bool lrcpp::Frontend::showMessage(struct retro_message const* data) {
    return _video == nullptr ? false : _video->showMessage(data);
}

bool lrcpp::Frontend::shutdown() {
    return _fsm.unload();
}

bool lrcpp::Frontend::setPerformanceLevel(unsigned data) {
    return _config == nullptr ? false : _config->setPerformanceLevel(data);
}

bool lrcpp::Frontend::getSystemDirectory(char const** data) {
    return _config == nullptr ? false : _config->getSystemDirectory(data);
}

bool lrcpp::Frontend::setPixelFormat(enum retro_pixel_format data) {
    return _video == nullptr ? false : _video->setPixelFormat(data);
}

bool lrcpp::Frontend::setInputDescriptors(struct retro_input_descriptor const* data) {
    return _input == nullptr ? false : _input->setInputDescriptors(data);
}

bool lrcpp::Frontend::setKeyboardCallback(struct retro_keyboard_callback const* data) {
    return _input == nullptr ? false : _input->setKeyboardCallback(data);
}

bool lrcpp::Frontend::setDiskControlInterface(struct retro_disk_control_callback const* data) {
    return _diskControl == nullptr ? false : _diskControl->setDiskControlInterface(data);
}

bool lrcpp::Frontend::setHwRender(struct retro_hw_render_callback* data) {
    return _video == nullptr ? false : _video->setHwRender(data);
}

bool lrcpp::Frontend::getVariable(struct retro_variable* data) {
    return _config == nullptr ? false : _config->getVariable(data);
}

bool lrcpp::Frontend::setVariables(struct retro_variable const* data) {
    return _config == nullptr ? false : _config->setVariables(data);
}

bool lrcpp::Frontend::getVariableUpdate(bool* data) {
    return _config == nullptr ? false : _config->getVariableUpdate(data);
}

bool lrcpp::Frontend::setSupportNoGame(bool data) {
    _supportsNoGame = data;
    return _config == nullptr ? false : _config->setSupportNoGame(data);
}

bool lrcpp::Frontend::getLibretroPath(char const** data) {
    return _config == nullptr ? false : _config->getLibretroPath(data);
}

bool lrcpp::Frontend::setFrameTimeCallback(struct retro_frame_time_callback const* data) {
    return _video == nullptr ? false : _video->setFrameTimeCallback(data);
}

bool lrcpp::Frontend::setAudioCallback(struct retro_audio_callback const* data) {
    return _audio == nullptr ? false : _audio->setAudioCallback(data);
}

bool lrcpp::Frontend::getRumbleInterface(struct retro_rumble_interface* data) {
    if (_rumble == nullptr) {
        return false;
    }

    data->set_rumble_state = rumbleSetState;
    return true;
}

bool lrcpp::Frontend::getInputDeviceCapabilities(uint64_t* data) {
    return _input == nullptr ? false : _input->getInputDeviceCapabilities(data);
}

bool lrcpp::Frontend::getSensorInterface(struct retro_sensor_interface* data) {
    if (_sensor == nullptr) {
        return false;
    }

    data->set_sensor_state = sensorSetState;
    data->get_sensor_input = sensorGetInput;
    return true;
}

bool lrcpp::Frontend::getCameraInterface(struct retro_camera_callback* data) {
    if (_camera == nullptr) {
        return false;
    }

    data->start = cameraStart;
    data->stop = cameraStop;
    return true;
}

bool lrcpp::Frontend::getLogInterface(struct retro_log_callback* data) {
    if (_logger == nullptr) {
        return false;
    }

    data->log = loggerPrintf;
    return true;
}

bool lrcpp::Frontend::getPerfInterface(struct retro_perf_callback* data) {
    if (_perf == nullptr) {
        return false;
    }

    data->get_time_usec = perfGetTimeUsec;
    data->get_cpu_features = perfGetCpuFeatures;
    data->get_perf_counter = perfGetCounter;
    data->perf_register = perfRegister;
    data->perf_start = perfStart;
    data->perf_stop = perfStop;
    data->perf_log = perfLog;
    return true;
}

bool lrcpp::Frontend::getLocationInterface(struct retro_location_callback* data) {
    if (_location == nullptr) {
        return false;
    }

    data->start = locationStart;
    data->stop = locationStop;
    data->get_position = locationGetPosition;
    data->set_interval = locationSetInterval;
    return true;
}

bool lrcpp::Frontend::getCoreAssetsDirectory(char const** data) {
    return _config == nullptr ? false : _config->getCoreAssetsDirectory(data);
}

bool lrcpp::Frontend::getSaveDirectory(char const** data) {
    return _config == nullptr ? false : _config->getSaveDirectory(data);
}

bool lrcpp::Frontend::setSystemAvInfo(struct retro_system_av_info const* data) {
    return _video == nullptr ? false : _video->setSystemAvInfo(data);
}

bool lrcpp::Frontend::setProcAddressCallback(struct retro_get_proc_address_interface const* data) {
    return _config == nullptr ? false : _config->setProcAddressCallback(data);
}

bool lrcpp::Frontend::setSubsystemInfo(struct retro_subsystem_info const* data) {
    return _config == nullptr ? false : _config->setSubsystemInfo(data);
}

bool lrcpp::Frontend::setControllerInfo(struct retro_controller_info const* data) {
    return _input == nullptr ? false : _input->setControllerInfo(data);
}

bool lrcpp::Frontend::setMemoryMaps(struct retro_memory_map const* data) {
    return _config == nullptr ? false : _config->setMemoryMaps(data);
}

bool lrcpp::Frontend::setGeometry(struct retro_game_geometry const* data) {
    return _video == nullptr ? false : _video->setGeometry(data);;
}

bool lrcpp::Frontend::getUsername(char const** data) {
    return _config == nullptr ? false : _config->getUsername(data);
}

bool lrcpp::Frontend::getLanguage(unsigned* data) {
    return _config == nullptr ? false : _config->getLanguage(data);
}

bool lrcpp::Frontend::getCurrentSoftwareFramebuffer(struct retro_framebuffer* data) {
    return _video == nullptr ? false : _video->getCurrentSoftwareFramebuffer(data);
}

bool lrcpp::Frontend::getHwRenderInterface(struct retro_hw_render_interface const** data) {
    return _video == nullptr ? false : _video->getHwRenderInterface(data);
}

bool lrcpp::Frontend::setSupportAchievements(bool data) {
    return _config == nullptr ? false : _config->setSupportAchievements(data);
}

bool lrcpp::Frontend::setHwRenderContextNegotiationInterface(struct retro_hw_render_context_negotiation_interface const* data) {
    return _video == nullptr ? false : _video->setHwRenderContextNegotiationInterface(data);
}

bool lrcpp::Frontend::setSerializationQuirks(uint64_t data) {
    return _config == nullptr ? false : _config->setSerializationQuirks(data);
}

bool lrcpp::Frontend::setHwSharedContext() {
    return _video == nullptr ? false : _video->setHwSharedContext();
}

bool lrcpp::Frontend::getVfsInterface(struct retro_vfs_interface_info* data) {
    if (_virtualFileSystem == nullptr) {
        return false;
    }

    const unsigned version = _virtualFileSystem->getVirtualFileSystemInterfaceVersion();

    if (version < data->required_interface_version) {
        return false;
    }

    _virtualFileSystemInterface.get_path = virtualFileSystemGetPath;
    _virtualFileSystemInterface.open = virtualFileSystemOpen;
    _virtualFileSystemInterface.close = virtualFileSystemClose;
    _virtualFileSystemInterface.size = virtualFileSystemSize;
    _virtualFileSystemInterface.tell = virtualFileSystemTell;
    _virtualFileSystemInterface.seek = virtualFileSystemSeek;
    _virtualFileSystemInterface.read = virtualFileSystemRead;
    _virtualFileSystemInterface.write = virtualFileSystemWrite;
    _virtualFileSystemInterface.flush = virtualFileSystemFlush;
    _virtualFileSystemInterface.remove = virtualFileSystemRemove;
    _virtualFileSystemInterface.rename = virtualFileSystemRename;
    _virtualFileSystemInterface.truncate = virtualFileSystemTruncate;
    _virtualFileSystemInterface.stat = virtualFileSystemStat;
    _virtualFileSystemInterface.mkdir = virtualFileSystemMkDir;
    _virtualFileSystemInterface.opendir = virtualFileSystemOpenDir;
    _virtualFileSystemInterface.readdir = virtualFileSystemReadDir;
    _virtualFileSystemInterface.dirent_get_name = virtualFileSystemDirentGetName;
    _virtualFileSystemInterface.dirent_is_dir = virtualFileSystemDirentIsDir;
    _virtualFileSystemInterface.closedir = virtualFileSystemCloseDir;

    data->required_interface_version = version;
    data->iface = &_virtualFileSystemInterface;
    return true;
}

bool lrcpp::Frontend::getLedInterface(struct retro_led_interface* data) {
    if (_led == nullptr) {
        return false;
    }

    data->set_led_state = ledSetState;
    return true;
}

bool lrcpp::Frontend::getAudioVideoEnable(int* data) {
    return _config == nullptr ? false : _config->getAudioVideoEnable(data);
}

bool lrcpp::Frontend::getMidiInterface(struct retro_midi_interface** data) {
    if (_midi == nullptr) {
        return false;
    }

    _midiInterface.input_enabled = midiInputEnabled;
    _midiInterface.output_enabled = midiOutputEnabled;
    _midiInterface.read = midiRead;
    _midiInterface.write = midiWrite;
    _midiInterface.flush = midiFlush;

    *data = &_midiInterface;
    return true;
}

bool lrcpp::Frontend::getFastForwarding(bool* data) {
    return _config == nullptr ? false : _config->getFastForwarding(data);
}

bool lrcpp::Frontend::getTargetRefreshRate(float* data) {
    return _video == nullptr ? false : _video->getTargetRefreshRate(data);
}

bool lrcpp::Frontend::getInputBitmasks(bool* data) {
    return _input == nullptr ? false : _input->getInputBitmasks(data);
}

bool lrcpp::Frontend::getCoreOptionsVersion(unsigned* data) {
    return _config == nullptr ? false : _config->getCoreOptionsVersion(data);
}

bool lrcpp::Frontend::setCoreOptions(struct retro_core_option_definition const** data) {
    return _config == nullptr ? false : _config->setCoreOptions(data);
}

bool lrcpp::Frontend::setCoreOptionsIntl(struct retro_core_options_intl const* data) {
    return _config == nullptr ? false : _config->setCoreOptionsIntl(data);
}

bool lrcpp::Frontend::setCoreOptionsDisplay(struct retro_core_option_display const* data) {
    return _config == nullptr ? false : _config->setCoreOptionsDisplay(data);
}

bool lrcpp::Frontend::getPreferredHwRender(unsigned* data) {
    return _video == nullptr ? false : _video->getPreferredHwRender(data);
}

bool lrcpp::Frontend::getDiskControlInterfaceVersion(unsigned* data) {
    return _diskControl == nullptr ? false : _diskControl->getDiskControlInterfaceVersion(data);
}

bool lrcpp::Frontend::setDiskControlExtInterface(struct retro_disk_control_ext_callback const* data) {
    return _diskControl == nullptr ? false : _diskControl->setDiskControlExtInterface(data);
}

bool lrcpp::Frontend::environmentCallback(unsigned cmd, void* data) {
    switch (cmd) {
        case RETRO_ENVIRONMENT_SET_ROTATION:
            return setRotation(*(unsigned*)data);

        case RETRO_ENVIRONMENT_GET_OVERSCAN:
            return getOverscan((bool*)data);

        case RETRO_ENVIRONMENT_GET_CAN_DUPE:
            return getCanDupe((bool*)data);

        case RETRO_ENVIRONMENT_SET_MESSAGE:
            return showMessage((struct retro_message const*)data);

        case RETRO_ENVIRONMENT_SHUTDOWN:
            return shutdown();

        case RETRO_ENVIRONMENT_SET_PERFORMANCE_LEVEL:
            return setPerformanceLevel(*(unsigned*)data);

        case RETRO_ENVIRONMENT_GET_SYSTEM_DIRECTORY:
            return getSystemDirectory((char const**)data);

        case RETRO_ENVIRONMENT_SET_PIXEL_FORMAT:
            return setPixelFormat(*(enum retro_pixel_format*)data);

        case RETRO_ENVIRONMENT_SET_INPUT_DESCRIPTORS:
            return setInputDescriptors((struct retro_input_descriptor const*)data);

        case RETRO_ENVIRONMENT_SET_KEYBOARD_CALLBACK:
            return setKeyboardCallback((struct retro_keyboard_callback const*)data);

        case RETRO_ENVIRONMENT_SET_DISK_CONTROL_INTERFACE:
            return setDiskControlInterface((struct retro_disk_control_callback const*)data);

        case RETRO_ENVIRONMENT_SET_HW_RENDER:
            return setHwRender((struct retro_hw_render_callback*)data);

        case RETRO_ENVIRONMENT_GET_VARIABLE:
            return getVariable((struct retro_variable*)data);

        case RETRO_ENVIRONMENT_SET_VARIABLES:
            return setVariables((struct retro_variable const*)data);

        case RETRO_ENVIRONMENT_GET_VARIABLE_UPDATE:
            return getVariableUpdate((bool*)data);

        case RETRO_ENVIRONMENT_SET_SUPPORT_NO_GAME:
            return setSupportNoGame(*(bool*)data);

        case RETRO_ENVIRONMENT_GET_LIBRETRO_PATH:
            return getLibretroPath((char const**)data);

        case RETRO_ENVIRONMENT_SET_FRAME_TIME_CALLBACK:
            return setFrameTimeCallback((struct retro_frame_time_callback const*)data);

        case RETRO_ENVIRONMENT_SET_AUDIO_CALLBACK:
            return setAudioCallback((struct retro_audio_callback const*)data);

        case RETRO_ENVIRONMENT_GET_RUMBLE_INTERFACE:
            return getRumbleInterface((struct retro_rumble_interface*)data);

        case RETRO_ENVIRONMENT_GET_INPUT_DEVICE_CAPABILITIES:
            return getInputDeviceCapabilities((uint64_t*)data);

        case RETRO_ENVIRONMENT_GET_SENSOR_INTERFACE:
            return getSensorInterface((struct retro_sensor_interface*)data);

        case RETRO_ENVIRONMENT_GET_CAMERA_INTERFACE:
            return getCameraInterface((struct retro_camera_callback*)data);

        case RETRO_ENVIRONMENT_GET_LOG_INTERFACE:
            return getLogInterface((struct retro_log_callback*)data);

        case RETRO_ENVIRONMENT_GET_PERF_INTERFACE:
            return getPerfInterface((struct retro_perf_callback*)data);

        case RETRO_ENVIRONMENT_GET_LOCATION_INTERFACE:
            return getLocationInterface((struct retro_location_callback*)data);

        case RETRO_ENVIRONMENT_GET_CORE_ASSETS_DIRECTORY:
            return getCoreAssetsDirectory((char const**)data);

        case RETRO_ENVIRONMENT_GET_SAVE_DIRECTORY:
            return getSaveDirectory((char const**)data);

        case RETRO_ENVIRONMENT_SET_SYSTEM_AV_INFO:
            return setSystemAvInfo((struct retro_system_av_info const*)data);

        case RETRO_ENVIRONMENT_SET_PROC_ADDRESS_CALLBACK:
            return setProcAddressCallback((struct retro_get_proc_address_interface const*)data);

        case RETRO_ENVIRONMENT_SET_SUBSYSTEM_INFO:
            return setSubsystemInfo((struct retro_subsystem_info const*)data);

        case RETRO_ENVIRONMENT_SET_CONTROLLER_INFO:
            return setControllerInfo((struct retro_controller_info const*)data);

        case RETRO_ENVIRONMENT_SET_MEMORY_MAPS:
            return setMemoryMaps((struct retro_memory_map const*)data);

        case RETRO_ENVIRONMENT_SET_GEOMETRY:
            return setGeometry((struct retro_game_geometry const*)data);

        case RETRO_ENVIRONMENT_GET_USERNAME:
            return getUsername((char const**)data);

        case RETRO_ENVIRONMENT_GET_LANGUAGE:
            return getLanguage((unsigned*)data);

        case RETRO_ENVIRONMENT_GET_CURRENT_SOFTWARE_FRAMEBUFFER:
            return getCurrentSoftwareFramebuffer((struct retro_framebuffer*)data);

        case RETRO_ENVIRONMENT_GET_HW_RENDER_INTERFACE:
            return getHwRenderInterface((struct retro_hw_render_interface const**)data);

        case RETRO_ENVIRONMENT_SET_SUPPORT_ACHIEVEMENTS:
            return setSupportAchievements(*(bool*)data);

        case RETRO_ENVIRONMENT_SET_HW_RENDER_CONTEXT_NEGOTIATION_INTERFACE:
            return setHwRenderContextNegotiationInterface((struct retro_hw_render_context_negotiation_interface const*)data);

        case RETRO_ENVIRONMENT_SET_SERIALIZATION_QUIRKS:
            return setSerializationQuirks(*(uint64_t*)data);

        case RETRO_ENVIRONMENT_SET_HW_SHARED_CONTEXT:
            return setHwSharedContext();

        case RETRO_ENVIRONMENT_GET_VFS_INTERFACE:
            return getVfsInterface((struct retro_vfs_interface_info*)data);

        case RETRO_ENVIRONMENT_GET_LED_INTERFACE:
            return getLedInterface((struct retro_led_interface*)data);

        case RETRO_ENVIRONMENT_GET_AUDIO_VIDEO_ENABLE:
            return getAudioVideoEnable((int*)data);

        case RETRO_ENVIRONMENT_GET_MIDI_INTERFACE:
            return getMidiInterface((struct retro_midi_interface**)data);

        case RETRO_ENVIRONMENT_GET_FASTFORWARDING:
            return getFastForwarding((bool*)data);

        case RETRO_ENVIRONMENT_GET_TARGET_REFRESH_RATE:
            return getTargetRefreshRate((float*)data);

        case RETRO_ENVIRONMENT_GET_INPUT_BITMASKS:
            return getInputBitmasks((bool*)data);

        case RETRO_ENVIRONMENT_GET_CORE_OPTIONS_VERSION:
            return getCoreOptionsVersion((unsigned*)data);

        case RETRO_ENVIRONMENT_SET_CORE_OPTIONS:
            return setCoreOptions((struct retro_core_option_definition const**)data);

        case RETRO_ENVIRONMENT_SET_CORE_OPTIONS_INTL:
            return setCoreOptionsIntl((struct retro_core_options_intl const*)data);

        case RETRO_ENVIRONMENT_SET_CORE_OPTIONS_DISPLAY:
            return setCoreOptionsDisplay((struct retro_core_option_display const*)data);

        case RETRO_ENVIRONMENT_GET_PREFERRED_HW_RENDER:
            return getPreferredHwRender((unsigned*)data);

        case RETRO_ENVIRONMENT_GET_DISK_CONTROL_INTERFACE_VERSION:
            return getDiskControlInterfaceVersion((unsigned*)data);

        case RETRO_ENVIRONMENT_SET_DISK_CONTROL_EXT_INTERFACE:
            return setDiskControlExtInterface((struct retro_disk_control_ext_callback const*)data);

        default:
            return false;
    }
}

bool lrcpp::Frontend::staticEnvironmentCallback(unsigned cmd, void* data) {
    return s_instance->environmentCallback(cmd, data);
}

void lrcpp::Frontend::staticVideoRefreshCallback(void const* data, unsigned width, unsigned height, size_t pitch) {
    s_instance->_video->refresh(data, width, height, pitch);
}

size_t lrcpp::Frontend::staticAudioSampleBatchCallback(int16_t const* data, size_t frames) {
    return s_instance->_audio->sampleBatch(data, frames);
}

void lrcpp::Frontend::staticAudioSampleCallback(int16_t left, int16_t right) {
    s_instance->_audio->sample(left, right);
}

int16_t lrcpp::Frontend::staticInputStateCallback(unsigned port, unsigned device, unsigned index, unsigned id) {
    return s_instance->_input->state(port, device, index, id);
}

void lrcpp::Frontend::staticInputPollCallback() {
    s_instance->_input->poll();
}

bool lrcpp::Frontend::rumbleSetState(unsigned port, enum retro_rumble_effect effect, uint16_t strength) {
    return s_instance->_rumble->setState(port, effect, strength);
}

bool lrcpp::Frontend::sensorSetState(unsigned port, enum retro_sensor_action action, unsigned rate) {
    return s_instance->_sensor->setState(port, action, rate);
}

float lrcpp::Frontend::sensorGetInput(unsigned port, unsigned id) {
    return s_instance->_sensor->getInput(port, id);
}

bool lrcpp::Frontend::cameraStart() {
    return s_instance->_camera->start();
}

void lrcpp::Frontend::cameraStop() {
    s_instance->_camera->stop();
}

void lrcpp::Frontend::loggerPrintf(enum retro_log_level level, char const* format, ...) {
    va_list args;
    va_start(args, format);
    s_instance->_logger->printf(level, format, args);
    va_end(args);
}

retro_time_t lrcpp::Frontend::perfGetTimeUsec() {
    return s_instance->_perf->getTimeUsec();
}

uint64_t lrcpp::Frontend::perfGetCpuFeatures() {
    return s_instance->_perf->getCpuFeatures();
}

retro_perf_tick_t lrcpp::Frontend::perfGetCounter() {
    return s_instance->_perf->getCounter();
}

void lrcpp::Frontend::perfRegister(struct retro_perf_counter* counter) {
    s_instance->_perf->register_(counter);
}

void lrcpp::Frontend::perfStart(struct retro_perf_counter* counter) {
    s_instance->_perf->start(counter);
}

void lrcpp::Frontend::perfStop(struct retro_perf_counter* counter) {
    s_instance->_perf->stop(counter);
}

void lrcpp::Frontend::perfLog() {
    s_instance->_perf->log();
}

bool lrcpp::Frontend::locationStart() {
    return s_instance->_location->start();
}

void lrcpp::Frontend::locationStop() {
    s_instance->_location->stop();
}

bool lrcpp::Frontend::locationGetPosition(double* lat, double* lon, double* horizAccuracy, double* vertAccuracy) {
    return s_instance->_location->getPosition(lat, lon, horizAccuracy, vertAccuracy);
}

void lrcpp::Frontend::locationSetInterval(unsigned intervalMs, unsigned intervalDistance) {
    s_instance->_location->setInterval(intervalMs, intervalDistance);
}

char const* lrcpp::Frontend::virtualFileSystemGetPath(struct retro_vfs_file_handle* stream) {
    return s_instance->_virtualFileSystem->getPath(stream);
}

struct retro_vfs_file_handle* lrcpp::Frontend::virtualFileSystemOpen(char const* path, unsigned mode, unsigned hints) {
    return s_instance->_virtualFileSystem->open(path, mode, hints);
}

int lrcpp::Frontend::virtualFileSystemClose(struct retro_vfs_file_handle* stream) {
    return s_instance->_virtualFileSystem->close(stream);
}

int64_t lrcpp::Frontend::virtualFileSystemSize(struct retro_vfs_file_handle* stream) {
    return s_instance->_virtualFileSystem->size(stream);
}

int64_t lrcpp::Frontend::virtualFileSystemTruncate(struct retro_vfs_file_handle* stream, int64_t length) {
    return s_instance->_virtualFileSystem->truncate(stream, length);
}

int64_t lrcpp::Frontend::virtualFileSystemTell(struct retro_vfs_file_handle* stream) {
    return s_instance->_virtualFileSystem->tell(stream);
}

int64_t lrcpp::Frontend::virtualFileSystemSeek(struct retro_vfs_file_handle* stream, int64_t offset, int seekPosition) {
    return s_instance->_virtualFileSystem->seek(stream, offset, seekPosition);
}

int64_t lrcpp::Frontend::virtualFileSystemRead(struct retro_vfs_file_handle* stream, void* s, uint64_t len) {
    return s_instance->_virtualFileSystem->read(stream, s, len);
}

int64_t lrcpp::Frontend::virtualFileSystemWrite(struct retro_vfs_file_handle* stream, void const* s, uint64_t len) {
    return s_instance->_virtualFileSystem->write(stream, s, len);
}

int lrcpp::Frontend::virtualFileSystemFlush(struct retro_vfs_file_handle* stream) {
    return s_instance->_virtualFileSystem->flush(stream);
}

int lrcpp::Frontend::virtualFileSystemRemove(char const* path) {
    return s_instance->_virtualFileSystem->remove(path);
}

int lrcpp::Frontend::virtualFileSystemRename(char const* oldPath, char const* newPath) {
    return s_instance->_virtualFileSystem->rename(oldPath, newPath);
}

int lrcpp::Frontend::virtualFileSystemStat(char const* path, int32_t* size) {
    return s_instance->_virtualFileSystem->stat(path, size);
}

int lrcpp::Frontend::virtualFileSystemMkDir(char const* dir) {
    return s_instance->_virtualFileSystem->mkDir(dir);
}

struct retro_vfs_dir_handle* lrcpp::Frontend::virtualFileSystemOpenDir(char const* dir, bool includeHidden) {
    return s_instance->_virtualFileSystem->openDir(dir, includeHidden);
}

bool lrcpp::Frontend::virtualFileSystemReadDir(struct retro_vfs_dir_handle* dirstream) {
    return s_instance->_virtualFileSystem->readDir(dirstream);
}

char const* lrcpp::Frontend::virtualFileSystemDirentGetName(struct retro_vfs_dir_handle* dirstream) {
    return s_instance->_virtualFileSystem->direntGetName(dirstream);
}

bool lrcpp::Frontend::virtualFileSystemDirentIsDir(struct retro_vfs_dir_handle* dirstream) {
    return s_instance->_virtualFileSystem->direntIsDir(dirstream);
}

int lrcpp::Frontend::virtualFileSystemCloseDir(struct retro_vfs_dir_handle* dirstream) {
    return s_instance->_virtualFileSystem->closeDir(dirstream);
}

void lrcpp::Frontend::ledSetState(int led, int state) {
    return s_instance->_led->setState(led, state);
}

bool lrcpp::Frontend::midiInputEnabled() {
    return s_instance->_midi->inputEnabled();
}

bool lrcpp::Frontend::midiOutputEnabled() {
    return s_instance->_midi->outputEnabled();
}

bool lrcpp::Frontend::midiRead(uint8_t* byte) {
    return s_instance->_midi->read(byte);
}

bool lrcpp::Frontend::midiWrite(uint8_t byte, uint32_t deltaTime) {
    return s_instance->_midi->write(byte, deltaTime);
}

bool lrcpp::Frontend::midiFlush() {
    return s_instance->_midi->flush();
}
