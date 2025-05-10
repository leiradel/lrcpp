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

#include <lrcpp/Frontend.h>

#include <new>
#include <string.h>
#include <stdlib.h>

namespace {
    class DummyLogger : public lrcpp::Logger {
    public:
        virtual void vprintf(retro_log_level level, char const* format, va_list args) override {
            (void)level;
            (void)format;
            (void)args;
        }
    };
}

// Storage for the current Frontend instance
static thread_local lrcpp::Frontend* s_frontend;
// Dummy logger to use if the caller doesn't set one
static DummyLogger s_logger;

lrcpp::Frontend::Frontend()
    : _logger(&s_logger)
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
    , _supportsNoGame(false)
    , _shutdownRequested(false)
{
    CoreFsm_Init(&_fsm, &_core, this, nullptr);
}

lrcpp::Frontend::~Frontend() {
    CoreFsm_Transition_unset(&_fsm);
}

lrcpp::Frontend* lrcpp::Frontend::getCurrent() {
    return s_frontend;
}

void lrcpp::Frontend::setCurrent(Frontend* frontend) {
    s_frontend = frontend;
}

lrcpp::Logger* lrcpp::Frontend::getLogger() {
    return _logger;
}

lrcpp::Config* lrcpp::Frontend::getConfig() {
    return _config;
}

lrcpp::Video* lrcpp::Frontend::getVideo() {
    return _video;
}

lrcpp::Led* lrcpp::Frontend::getLed() {
    return _led;
}

lrcpp::Audio* lrcpp::Frontend::getAudio() {
    return _audio;
}

lrcpp::Midi* lrcpp::Frontend::getMidi() {
    return _midi;
}

lrcpp::Input* lrcpp::Frontend::getInput() {
    return _input;
}

lrcpp::Rumble* lrcpp::Frontend::getRumble() {
    return _rumble;
}

lrcpp::Sensor* lrcpp::Frontend::getSensor() {
    return _sensor;
}

lrcpp::Camera* lrcpp::Frontend::getCamera() {
    return _camera;
}

lrcpp::Location* lrcpp::Frontend::getLocation() {
    return _location;
}

lrcpp::VirtualFileSystem* lrcpp::Frontend::getVirtualFileSystem() {
    return _virtualFileSystem;
}

lrcpp::DiskControl* lrcpp::Frontend::getDiskControl() {
    return _diskControl;
}

lrcpp::Perf* lrcpp::Frontend::getPerf() {
    return _perf;
}

bool lrcpp::Frontend::setLogger(Logger* logger) {
    if (CoreFsm_CurrentState(&_fsm) == CoreFsm_State_Start) {
        _logger = logger;
        return true;
    }

    return false;
}

bool lrcpp::Frontend::setConfig(Config* config) {
    if (CoreFsm_CurrentState(&_fsm) == CoreFsm_State_Start) {
        _config = config;
        return true;
    }

    return false;
}

bool lrcpp::Frontend::setVideo(Video* video) {
    if (CoreFsm_CurrentState(&_fsm) == CoreFsm_State_Start) {
        _video = video;
        return true;
    }

    return false;
}

bool lrcpp::Frontend::setLed(Led* led) {
    if (CoreFsm_CurrentState(&_fsm) == CoreFsm_State_Start) {
        _led = led;
        return true;
    }

    return false;
}

bool lrcpp::Frontend::setAudio(Audio* audio) {
    if (CoreFsm_CurrentState(&_fsm) == CoreFsm_State_Start) {
        _audio = audio;
        return true;
    }

    return false;
}

bool lrcpp::Frontend::setMidi(Midi* midi) {
    if (CoreFsm_CurrentState(&_fsm) == CoreFsm_State_Start) {
        _midi = midi;
        return true;
    }

    return false;
}

bool lrcpp::Frontend::setInput(Input* input) {
    if (CoreFsm_CurrentState(&_fsm) == CoreFsm_State_Start) {
        _input = input;
        return true;
    }

    return false;
}

bool lrcpp::Frontend::setRumble(Rumble* rumble) {
    if (CoreFsm_CurrentState(&_fsm) == CoreFsm_State_Start) {
        _rumble = rumble;
        return true;
    }

    return false;
}

bool lrcpp::Frontend::setSensor(Sensor* sensor) {
    if (CoreFsm_CurrentState(&_fsm) == CoreFsm_State_Start) {
        _sensor = sensor;
        return true;
    }

    return false;
}

bool lrcpp::Frontend::setCamera(Camera* camera) {
    if (CoreFsm_CurrentState(&_fsm) == CoreFsm_State_Start) {
        _camera = camera;
        return true;
    }

    return false;
}

bool lrcpp::Frontend::setLocation(Location* location) {
    if (CoreFsm_CurrentState(&_fsm) == CoreFsm_State_Start) {
        _location = location;
        return true;
    }

    return false;
}

bool lrcpp::Frontend::setVirtualFileSystem(VirtualFileSystem* virtualFileSystem) {
    if (CoreFsm_CurrentState(&_fsm) == CoreFsm_State_Start) {
        _virtualFileSystem = virtualFileSystem;
        return true;
    }

    return false;
}

bool lrcpp::Frontend::setDiskControl(DiskControl* diskControl) {
    if (CoreFsm_CurrentState(&_fsm) == CoreFsm_State_Start) {
        _diskControl = diskControl;
        return true;
    }

    return false;
}

bool lrcpp::Frontend::setPerf(Perf* perf) {
    if (CoreFsm_CurrentState(&_fsm) == CoreFsm_State_Start) {
        _perf = perf;
        return true;
    }

    return false;
}

bool lrcpp::Frontend::setCore(Core const* core) {
    _core = *core;

    if (!CoreFsm_Transition_coreSet(&_fsm)) {
        _logger->error("Error in coreSet transition\n");
        return false;
    }

    if (!CoreFsm_Transition_setEnvironment(&_fsm, staticEnvironmentCallback)) {
        _logger->error("Error in setEnvironment transition\n");
        return false;
    }

    if (!CoreFsm_Transition_init(&_fsm)) {
        _logger->error("Error in init transition\n");
        return false;
    }

    return true;
}

bool lrcpp::Frontend::loadGame() {
    _logger->debug("Starting core without a content\n");

    if (!_supportsNoGame) {
        _logger->error("Core does not support being started without a content\n");
        return false;
    }

    return loadGame(nullptr, nullptr, 0);
}

bool lrcpp::Frontend::loadGame(char const* gamePath) {
    _logger->debug("Asking core to load \"%s\"\n", gamePath);

    retro_system_info info;

    if (!CoreFsm_Transition_getSystemInfo(&_fsm, &info)) {
        _logger->error("Error in getSystemInfo transition\n");
        return false;
    }

    if (!info.need_fullpath) {
        _logger->error("Core needs the content handed by the frontend\n");
        return false;
    }

    return loadGame(gamePath, nullptr, 0);
}

bool lrcpp::Frontend::loadGame(char const* gamePath, void const* data, size_t size) {
    _logger->debug("Asking core to load \"%s\" (%p and %zu)\n", gamePath, data, size);

    retro_game_info game;
    game.path = gamePath;
    game.data = data;
    game.size = size;
    game.meta = nullptr;

    if (!CoreFsm_Transition_loadGame(&_fsm, &game)) {
        _logger->error("Error in loadGame transition\n");
        return false;
    }

    int const ok = CoreFsm_Transition_setCallbacks(
        &_fsm,
        videoRefresh,
        audioSample,
        audioSampleBatch,
        inputPoll,
        inputState
    );

    if (!ok) {
        _logger->error("Error in setCallbacks transition\n");
        return false;
    }

    retro_system_av_info avinfo;

    if (!CoreFsm_Transition_getSystemAvInfo(&_fsm, &avinfo)) {
        _logger->error("Error in getSystemAvInfo transition\n");
        return false;
    }

    _shutdownRequested = false;
    return setSystemAvInfo(&avinfo);
}

bool lrcpp::Frontend::loadGameSpecial(unsigned gameType, struct retro_game_info const* info, size_t numInfo) {
    bool ok = CoreFsm_Transition_loadGameSpecial(&_fsm, gameType, info, numInfo);

    ok = ok && CoreFsm_Transition_setCallbacks(
        &_fsm,
        videoRefresh,
        audioSample,
        audioSampleBatch,
        inputPoll,
        inputState
    );

    retro_system_av_info avinfo;
    ok = ok && CoreFsm_Transition_getSystemAvInfo(&_fsm, &avinfo);
    ok = ok && setSystemAvInfo(&avinfo);

    _shutdownRequested = false;
    return ok;
}

bool lrcpp::Frontend::run() {
    return CoreFsm_Transition_run(&_fsm);
}

bool lrcpp::Frontend::reset() {
    return CoreFsm_Transition_reset(&_fsm);
}

bool lrcpp::Frontend::unloadGame() {
    return CoreFsm_Transition_unloadGame(&_fsm);
}

bool lrcpp::Frontend::unset() {
    return CoreFsm_Transition_unset(&_fsm);
}

bool lrcpp::Frontend::apiVersion(unsigned* version) {
    return CoreFsm_Transition_apiVersion(&_fsm, version);
}

bool lrcpp::Frontend::getSystemInfo(struct retro_system_info* info) {
    return CoreFsm_Transition_getSystemInfo(&_fsm, info);
}

bool lrcpp::Frontend::getSystemAvInfo(struct retro_system_av_info* info) {
    return CoreFsm_Transition_getSystemAvInfo(&_fsm, info);
}

bool lrcpp::Frontend::serializeSize(size_t* size) {
    return CoreFsm_Transition_serializeSize(&_fsm, size);
}

bool lrcpp::Frontend::serialize(void* data, size_t size) {
    return CoreFsm_Transition_serialize(&_fsm, data, size);
}

bool lrcpp::Frontend::unserialize(void const* data, size_t size) {
    return CoreFsm_Transition_unserialize(&_fsm, data, size);
}

bool lrcpp::Frontend::cheatReset() {
    return CoreFsm_Transition_cheatReset(&_fsm);
}

bool lrcpp::Frontend::cheatSet(unsigned index, bool enabled, char const* code) {
    return CoreFsm_Transition_cheatSet(&_fsm, index, enabled, code);
}

bool lrcpp::Frontend::getRegion(unsigned* region) {
    return CoreFsm_Transition_getRegion(&_fsm, region);
}

bool lrcpp::Frontend::getMemoryData(unsigned id, void** data) {
    return CoreFsm_Transition_getMemoryData(&_fsm, id, data);
}

bool lrcpp::Frontend::getMemorySize(unsigned id, size_t* size) {
    return CoreFsm_Transition_getMemorySize(&_fsm, id, size);
}

bool lrcpp::Frontend::shutdownRequested() const {
    return _shutdownRequested;
}


bool lrcpp::Frontend::setControllerPortDevice(unsigned port, unsigned device) {
    return CoreFsm_Transition_setControllerPortDevice(&_fsm, port, device);
}

bool lrcpp::Frontend::setRotation(unsigned data) {
    return _video != nullptr && _video->setRotation(data);
}

bool lrcpp::Frontend::getOverscan(bool* data) {
    return _video != nullptr && _video->getOverscan(data);
}

bool lrcpp::Frontend::getCanDupe(bool* data) {
    return _video != nullptr && _video->getCanDupe(data);
}

bool lrcpp::Frontend::showMessage(struct retro_message const* data) {
    return _video != nullptr && _video->showMessage(data);
}

bool lrcpp::Frontend::shutdown() {
    _shutdownRequested = true;
    return true;
}

bool lrcpp::Frontend::setPerformanceLevel(unsigned data) {
    return _config != nullptr && _config->setPerformanceLevel(data);
}

bool lrcpp::Frontend::getSystemDirectory(char const** data) {
    return _config != nullptr && _config->getSystemDirectory(data);
}

bool lrcpp::Frontend::setPixelFormat(enum retro_pixel_format data) {
    return _video != nullptr && _video->setPixelFormat(data);
}

bool lrcpp::Frontend::setInputDescriptors(struct retro_input_descriptor const* data) {
    return _input != nullptr && _input->setInputDescriptors(data);
}

bool lrcpp::Frontend::setKeyboardCallback(struct retro_keyboard_callback const* data) {
    return _input != nullptr && _input->setKeyboardCallback(data);
}

bool lrcpp::Frontend::setDiskControlInterface(struct retro_disk_control_callback const* data) {
    return _diskControl != nullptr && _diskControl->setDiskControlInterface(data);
}

bool lrcpp::Frontend::setHwRender(struct retro_hw_render_callback* data) {
    return _video != nullptr && _video->setHwRender(data);
}

bool lrcpp::Frontend::getVariable(struct retro_variable* data) {
    return _config != nullptr && _config->getVariable(data);
}

bool lrcpp::Frontend::setVariables(struct retro_variable const* data) {
    return _config != nullptr && _config->setVariables(data);
}

bool lrcpp::Frontend::getVariableUpdate(bool* data) {
    return _config != nullptr && _config->getVariableUpdate(data);
}

bool lrcpp::Frontend::setSupportNoGame(bool data) {
    _supportsNoGame = data;
    return _config != nullptr && _config->setSupportNoGame(data);
}

bool lrcpp::Frontend::getLibretroPath(char const** data) {
    return _config != nullptr && _config->getLibretroPath(data);
}

bool lrcpp::Frontend::setFrameTimeCallback(struct retro_frame_time_callback const* data) {
    return _video != nullptr && _video->setFrameTimeCallback(data);
}

bool lrcpp::Frontend::setAudioCallback(struct retro_audio_callback const* data) {
    return _audio != nullptr && _audio->setAudioCallback(data);
}

bool lrcpp::Frontend::getRumbleInterface(struct retro_rumble_interface* data) {
    if (_rumble == nullptr) {
        return false;
    }

    data->set_rumble_state = rumbleSetState;
    return true;
}

bool lrcpp::Frontend::getInputDeviceCapabilities(uint64_t* data) {
    return _input != nullptr && _input->getInputDeviceCapabilities(data);
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

    if (!_camera->getCameraInterface(data)) {
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

    data->log = loggerVprintf;
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
    return _location != nullptr && _location->getLocationInterface(data);
}

bool lrcpp::Frontend::getCoreAssetsDirectory(char const** data) {
    return _config != nullptr && _config->getCoreAssetsDirectory(data);
}

bool lrcpp::Frontend::getSaveDirectory(char const** data) {
    return _config != nullptr && _config->getSaveDirectory(data);
}

bool lrcpp::Frontend::setSystemAvInfo(struct retro_system_av_info const* data) {
    bool videoOk = _video != nullptr && _video->setSystemAvInfo(data);
    bool audioOk = _audio != nullptr && _audio->setSystemAvInfo(data);
    return videoOk || audioOk;
}

bool lrcpp::Frontend::setProcAddressCallback(struct retro_get_proc_address_interface const* data) {
    return _config != nullptr && _config->setProcAddressCallback(data);
}

bool lrcpp::Frontend::setSubsystemInfo(struct retro_subsystem_info const* data) {
    return _config != nullptr && _config->setSubsystemInfo(data);
}

bool lrcpp::Frontend::setControllerInfo(struct retro_controller_info const* data) {
    return _input != nullptr && _input->setControllerInfo(data);
}

bool lrcpp::Frontend::setMemoryMaps(struct retro_memory_map const* data) {
    return _config != nullptr && _config->setMemoryMaps(data);
}

bool lrcpp::Frontend::setGeometry(struct retro_game_geometry const* data) {
    return _video != nullptr && _video->setGeometry(data);;
}

bool lrcpp::Frontend::getUsername(char const** data) {
    return _config != nullptr && _config->getUsername(data);
}

bool lrcpp::Frontend::getLanguage(unsigned* data) {
    return _config != nullptr && _config->getLanguage(data);
}

bool lrcpp::Frontend::getCurrentSoftwareFramebuffer(struct retro_framebuffer* data) {
    return _video != nullptr && _video->getCurrentSoftwareFramebuffer(data);
}

bool lrcpp::Frontend::getHwRenderInterface(struct retro_hw_render_interface const** data) {
    return _video != nullptr && _video->getHwRenderInterface(data);
}

bool lrcpp::Frontend::setSupportAchievements(bool data) {
    return _config != nullptr && _config->setSupportAchievements(data);
}

bool lrcpp::Frontend::setHwRenderContextNegotiationInterface(struct retro_hw_render_context_negotiation_interface const* data) {
    return _video != nullptr && _video->setHwRenderContextNegotiationInterface(data);
}

bool lrcpp::Frontend::setSerializationQuirks(uint64_t data) {
    return _config != nullptr && _config->setSerializationQuirks(data);
}

bool lrcpp::Frontend::setHwSharedContext() {
    return _video != nullptr && _video->setHwSharedContext();
}

bool lrcpp::Frontend::getVfsInterface(struct retro_vfs_interface_info* data) {
    if (_virtualFileSystem == nullptr) {
        return false;
    }

    if (!_virtualFileSystem->getVfsInterface(data)) {
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
    return _config != nullptr && _config->getAudioVideoEnable(data);
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
    return _config != nullptr && _config->getFastForwarding(data);
}

bool lrcpp::Frontend::getTargetRefreshRate(float* data) {
    return _video != nullptr && _video->getTargetRefreshRate(data);
}

bool lrcpp::Frontend::getInputBitmasks(bool* data) {
    // HACK: At least the vice core calls this with a null data
    bool dummy = false;
    return _input != nullptr && _input->getInputBitmasks(data != nullptr ? data : &dummy);
}

bool lrcpp::Frontend::getCoreOptionsVersion(unsigned* data) {
    return _config != nullptr && _config->getCoreOptionsVersion(data);
}

bool lrcpp::Frontend::setCoreOptions(struct retro_core_option_definition const* data) {
    return _config != nullptr && _config->setCoreOptions(data);
}

bool lrcpp::Frontend::setCoreOptionsIntl(struct retro_core_options_intl const* data) {
    return _config != nullptr && _config->setCoreOptionsIntl(data);
}

bool lrcpp::Frontend::setCoreOptionsDisplay(struct retro_core_option_display const* data) {
    return _config != nullptr && _config->setCoreOptionsDisplay(data);
}

bool lrcpp::Frontend::getPreferredHwRender(unsigned* data) {
    return _video != nullptr && _video->getPreferredHwRender(data);
}

bool lrcpp::Frontend::getDiskControlInterfaceVersion(unsigned* data) {
    return _diskControl != nullptr && _diskControl->getDiskControlInterfaceVersion(data);
}

bool lrcpp::Frontend::setDiskControlExtInterface(struct retro_disk_control_ext_callback const* data) {
    return _diskControl != nullptr && _diskControl->setDiskControlExtInterface(data);
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
            return setCoreOptions((struct retro_core_option_definition const*)data);

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
    return getCurrent()->environmentCallback(cmd, data);
}

bool lrcpp::Frontend::rumbleSetState(unsigned port, enum retro_rumble_effect effect, uint16_t strength) {
    return getCurrent()->_rumble->setState(port, effect, strength);
}

bool lrcpp::Frontend::sensorSetState(unsigned port, enum retro_sensor_action action, unsigned rate) {
    return getCurrent()->_sensor->setState(port, action, rate);
}

float lrcpp::Frontend::sensorGetInput(unsigned port, unsigned id) {
    return getCurrent()->_sensor->getInput(port, id);
}

bool lrcpp::Frontend::cameraStart() {
    return getCurrent()->_camera->start();
}

void lrcpp::Frontend::cameraStop() {
    getCurrent()->_camera->stop();
}

void lrcpp::Frontend::loggerVprintf(enum retro_log_level level, char const* format, ...) {
    va_list args;
    va_start(args, format);
    getCurrent()->_logger->vprintf(level, format, args);
    va_end(args);
}

retro_time_t lrcpp::Frontend::perfGetTimeUsec() {
    return getCurrent()->_perf->getTimeUsec();
}

uint64_t lrcpp::Frontend::perfGetCpuFeatures() {
    return getCurrent()->_perf->getCpuFeatures();
}

retro_perf_tick_t lrcpp::Frontend::perfGetCounter() {
    return getCurrent()->_perf->getCounter();
}

void lrcpp::Frontend::perfRegister(struct retro_perf_counter* counter) {
    getCurrent()->_perf->register_(counter);
}

void lrcpp::Frontend::perfStart(struct retro_perf_counter* counter) {
    getCurrent()->_perf->start(counter);
}

void lrcpp::Frontend::perfStop(struct retro_perf_counter* counter) {
    getCurrent()->_perf->stop(counter);
}

void lrcpp::Frontend::perfLog() {
    getCurrent()->_perf->log();
}

bool lrcpp::Frontend::locationStart() {
    return getCurrent()->_location->start();
}

void lrcpp::Frontend::locationStop() {
    getCurrent()->_location->stop();
}

bool lrcpp::Frontend::locationGetPosition(double* lat, double* lon, double* horizAccuracy, double* vertAccuracy) {
    return getCurrent()->_location->getPosition(lat, lon, horizAccuracy, vertAccuracy);
}

void lrcpp::Frontend::locationSetInterval(unsigned intervalMs, unsigned intervalDistance) {
    getCurrent()->_location->setInterval(intervalMs, intervalDistance);
}

char const* lrcpp::Frontend::virtualFileSystemGetPath(struct retro_vfs_file_handle* stream) {
    return getCurrent()->_virtualFileSystem->getPath(stream);
}

struct retro_vfs_file_handle* lrcpp::Frontend::virtualFileSystemOpen(char const* path, unsigned mode, unsigned hints) {
    return getCurrent()->_virtualFileSystem->open(path, mode, hints);
}

int lrcpp::Frontend::virtualFileSystemClose(struct retro_vfs_file_handle* stream) {
    return getCurrent()->_virtualFileSystem->close(stream);
}

int64_t lrcpp::Frontend::virtualFileSystemSize(struct retro_vfs_file_handle* stream) {
    return getCurrent()->_virtualFileSystem->size(stream);
}

int64_t lrcpp::Frontend::virtualFileSystemTruncate(struct retro_vfs_file_handle* stream, int64_t length) {
    return getCurrent()->_virtualFileSystem->truncate(stream, length);
}

int64_t lrcpp::Frontend::virtualFileSystemTell(struct retro_vfs_file_handle* stream) {
    return getCurrent()->_virtualFileSystem->tell(stream);
}

int64_t lrcpp::Frontend::virtualFileSystemSeek(struct retro_vfs_file_handle* stream, int64_t offset, int seekPosition) {
    return getCurrent()->_virtualFileSystem->seek(stream, offset, seekPosition);
}

int64_t lrcpp::Frontend::virtualFileSystemRead(struct retro_vfs_file_handle* stream, void* s, uint64_t len) {
    return getCurrent()->_virtualFileSystem->read(stream, s, len);
}

int64_t lrcpp::Frontend::virtualFileSystemWrite(struct retro_vfs_file_handle* stream, void const* s, uint64_t len) {
    return getCurrent()->_virtualFileSystem->write(stream, s, len);
}

int lrcpp::Frontend::virtualFileSystemFlush(struct retro_vfs_file_handle* stream) {
    return getCurrent()->_virtualFileSystem->flush(stream);
}

int lrcpp::Frontend::virtualFileSystemRemove(char const* path) {
    return getCurrent()->_virtualFileSystem->remove(path);
}

int lrcpp::Frontend::virtualFileSystemRename(char const* oldPath, char const* newPath) {
    return getCurrent()->_virtualFileSystem->rename(oldPath, newPath);
}

int lrcpp::Frontend::virtualFileSystemStat(char const* path, int32_t* size) {
    return getCurrent()->_virtualFileSystem->stat(path, size);
}

int lrcpp::Frontend::virtualFileSystemMkDir(char const* dir) {
    return getCurrent()->_virtualFileSystem->mkDir(dir);
}

struct retro_vfs_dir_handle* lrcpp::Frontend::virtualFileSystemOpenDir(char const* dir, bool includeHidden) {
    return getCurrent()->_virtualFileSystem->openDir(dir, includeHidden);
}

bool lrcpp::Frontend::virtualFileSystemReadDir(struct retro_vfs_dir_handle* dirstream) {
    return getCurrent()->_virtualFileSystem->readDir(dirstream);
}

char const* lrcpp::Frontend::virtualFileSystemDirentGetName(struct retro_vfs_dir_handle* dirstream) {
    return getCurrent()->_virtualFileSystem->direntGetName(dirstream);
}

bool lrcpp::Frontend::virtualFileSystemDirentIsDir(struct retro_vfs_dir_handle* dirstream) {
    return getCurrent()->_virtualFileSystem->direntIsDir(dirstream);
}

int lrcpp::Frontend::virtualFileSystemCloseDir(struct retro_vfs_dir_handle* dirstream) {
    return getCurrent()->_virtualFileSystem->closeDir(dirstream);
}

void lrcpp::Frontend::ledSetState(int led, int state) {
    return getCurrent()->_led->setState(led, state);
}

bool lrcpp::Frontend::midiInputEnabled() {
    return getCurrent()->_midi->inputEnabled();
}

bool lrcpp::Frontend::midiOutputEnabled() {
    return getCurrent()->_midi->outputEnabled();
}

bool lrcpp::Frontend::midiRead(uint8_t* byte) {
    return getCurrent()->_midi->read(byte);
}

bool lrcpp::Frontend::midiWrite(uint8_t byte, uint32_t deltaTime) {
    return getCurrent()->_midi->write(byte, deltaTime);
}

bool lrcpp::Frontend::midiFlush() {
    return getCurrent()->_midi->flush();
}

uintptr_t lrcpp::Frontend::videoGetCurrentFramebuffer() {
    return getCurrent()->_video->getCurrentFramebuffer();
}

retro_proc_address_t lrcpp::Frontend::videoGetProcAddress(char const* symbol) {
    return getCurrent()->_video->getProcAddress(symbol);
}

void lrcpp::Frontend::videoRefresh(void const* data, unsigned width, unsigned height, size_t pitch) {
    if (getCurrent()->_video != nullptr) {
        getCurrent()->_video->refresh(data, width, height, pitch);
    }
}

size_t lrcpp::Frontend::audioSampleBatch(int16_t const* data, size_t frames) {
    if (getCurrent()->_audio != nullptr) {
        return getCurrent()->_audio->sampleBatch(data, frames);
    }

    return 0;
}

void lrcpp::Frontend::audioSample(int16_t left, int16_t right) {
    if (getCurrent()->_audio != nullptr) {
        getCurrent()->_audio->sample(left, right);
    }
}

int16_t lrcpp::Frontend::inputState(unsigned port, unsigned device, unsigned index, unsigned id) {
    if (getCurrent()->_input != nullptr) {
        return getCurrent()->_input->state(port, device, index, id);
    }

    return 0;
}

void lrcpp::Frontend::inputPoll() {
    if (getCurrent()->_input != nullptr) {
        getCurrent()->_input->poll();
    }
}
