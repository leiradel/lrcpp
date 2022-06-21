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
#include "CoreFsm.h"

#ifdef __circle__
#include <circle/new.h>
#else
#include <new>
#endif

#include <string.h>
#include <stdlib.h>

lrcpp::Frontend::Frontend(void* fsmMemory)
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
    , _fsm(new (fsmMemory) CoreFsm(_core))
    , _supportsNoGame(false)
    , _shutdownRequested(false)
{}

lrcpp::Frontend::~Frontend() {
    if (_fsm != nullptr) {
        _fsm->unload();
        _fsm->~CoreFsm();
    }
}

#ifdef __circle__
static uint8_t fsmMemory[sizeof(CoreFsm)] alignas(CoreFsm);
static lrcpp::Frontend singleton(fsmMemory);
#endif

lrcpp::Frontend& lrcpp::Frontend::getInstance() {
#ifndef __circle__
    static uint8_t fsmMemory[sizeof(CoreFsm)] alignas(CoreFsm);
    static Frontend singleton(fsmMemory);
#endif

    return singleton;
}

bool lrcpp::Frontend::setLogger(Logger* logger) {
    if (_fsm->currentState() == CoreFsm::State::Start) {
        _logger = logger;
        return true;
    }

    return false;
}

bool lrcpp::Frontend::setConfig(Config* config) {
    if (_fsm->currentState() == CoreFsm::State::Start) {
        _config = config;
        return true;
    }

    return false;
}

bool lrcpp::Frontend::setVideo(Video* video) {
    if (_fsm->currentState() == CoreFsm::State::Start) {
        _video = video;
        return true;
    }

    return false;
}

bool lrcpp::Frontend::setLed(Led* led) {
    if (_fsm->currentState() == CoreFsm::State::Start) {
        _led = led;
        return true;
    }

    return false;
}

bool lrcpp::Frontend::setAudio(Audio* audio) {
    if (_fsm->currentState() == CoreFsm::State::Start) {
        _audio = audio;
        return true;
    }

    return false;
}

bool lrcpp::Frontend::setMidi(Midi* midi) {
    if (_fsm->currentState() == CoreFsm::State::Start) {
        _midi = midi;
        return true;
    }

    return false;
}

bool lrcpp::Frontend::setInput(Input* input) {
    if (_fsm->currentState() == CoreFsm::State::Start) {
        _input = input;
        return true;
    }

    return false;
}

bool lrcpp::Frontend::setRumble(Rumble* rumble) {
    if (_fsm->currentState() == CoreFsm::State::Start) {
        _rumble = rumble;
        return true;
    }

    return false;
}

bool lrcpp::Frontend::setSensor(Sensor* sensor) {
    if (_fsm->currentState() == CoreFsm::State::Start) {
        _sensor = sensor;
        return true;
    }

    return false;
}

bool lrcpp::Frontend::setCamera(Camera* camera) {
    if (_fsm->currentState() == CoreFsm::State::Start) {
        _camera = camera;
        return true;
    }

    return false;
}

bool lrcpp::Frontend::setLocation(Location* location) {
    if (_fsm->currentState() == CoreFsm::State::Start) {
        _location = location;
        return true;
    }

    return false;
}

bool lrcpp::Frontend::setVirtualFileSystem(VirtualFileSystem* virtualFileSystem) {
    if (_fsm->currentState() == CoreFsm::State::Start) {
        _virtualFileSystem = virtualFileSystem;
        return true;
    }

    return false;
}

bool lrcpp::Frontend::setDiskControl(DiskControl* diskControl) {
    if (_fsm->currentState() == CoreFsm::State::Start) {
        _diskControl = diskControl;
        return true;
    }

    return false;
}

bool lrcpp::Frontend::setPerf(Perf* perf) {
    if (_fsm->currentState() == CoreFsm::State::Start) {
        _perf = perf;
        return true;
    }

    return false;
}

bool lrcpp::Frontend::load(char const* corePath) {
    if (_fsm == nullptr) {
        return false;
    }

    bool ok = _fsm->load(corePath);
    ok = ok && _fsm->setEnvironment(staticEnvironmentCallback);
    ok = ok && _fsm->init();

    return ok;
}

bool lrcpp::Frontend::use(CoreFunctions const* coreFunctions) {
    if (_fsm == nullptr) {
        return false;
    }

    bool ok = _fsm->use(coreFunctions);
    ok = ok && _fsm->setEnvironment(staticEnvironmentCallback);
    ok = ok && _fsm->init();

    return ok;
}

bool lrcpp::Frontend::loadGame() {
    return _supportsNoGame ? loadGame(nullptr, nullptr, 0) : false;
}

bool lrcpp::Frontend::loadGame(char const* gamePath) {
    retro_system_info info;

    if (!_fsm->getSystemInfo(&info)) {
        return false;
    }

    return info.need_fullpath ? loadGame(gamePath, nullptr, 0) : false;
}

bool lrcpp::Frontend::loadGame(char const* gamePath, void const* data, size_t size) {
    retro_game_info game;
    game.path = gamePath;
    game.data = data;
    game.size = size;
    game.meta = nullptr;

    bool ok = _fsm->loadGame(&game);
    ok = ok && _fsm->setCallbacks(videoRefresh,
                                 audioSample,
                                 audioSampleBatch,
                                 inputPoll,
                                 inputState);

    retro_system_av_info avinfo;
    ok = ok && _fsm->getSystemAvInfo(&avinfo);
    ok = ok && setSystemAvInfo(&avinfo);

    return ok;
}

bool lrcpp::Frontend::loadGameSpecial(unsigned gameType, struct retro_game_info const* info, size_t numInfo) {
    bool ok = _fsm->loadGameSpecial(gameType, info, numInfo);
    ok = ok && _fsm->setCallbacks(videoRefresh,
                                 audioSample,
                                 audioSampleBatch,
                                 inputPoll,
                                 inputState);

    retro_system_av_info avinfo;
    ok = ok && _fsm->getSystemAvInfo(&avinfo);
    ok = ok && setSystemAvInfo(&avinfo);

    return ok;
}

bool lrcpp::Frontend::run() {
    _shutdownRequested = false;
    bool const ok = _fsm->run();

    if (ok && _shutdownRequested) {
        return _fsm->unload();
    }

    return ok;
}

bool lrcpp::Frontend::reset() {
    return _fsm->reset();
}

bool lrcpp::Frontend::unloadGame() {
    return _fsm->unloadGame();
}

bool lrcpp::Frontend::unload() {
    return _fsm->unload();
}

bool lrcpp::Frontend::apiVersion(unsigned* version) {
    return _fsm->apiVersion(version);
}

bool lrcpp::Frontend::getSystemInfo(struct retro_system_info* info) {
    return _fsm->getSystemInfo(info);
}

bool lrcpp::Frontend::getSystemAvInfo(struct retro_system_av_info* info) {
    return _fsm->getSystemAvInfo(info);
}

bool lrcpp::Frontend::serializeSize(size_t* size) {
    return _fsm->serializeSize(size);
}

bool lrcpp::Frontend::serialize(void* data, size_t size) {
    return _fsm->serialize(data, size);
}

bool lrcpp::Frontend::unserialize(void const* data, size_t size) {
    return _fsm->unserialize(data, size);
}

bool lrcpp::Frontend::cheatReset() {
    return _fsm->cheatReset();
}

bool lrcpp::Frontend::cheatSet(unsigned index, bool enabled, char const* code) {
    return _fsm->cheatSet(index, enabled, code);
}

bool lrcpp::Frontend::getRegion(unsigned* region) {
    return _fsm->getRegion(region);
}

bool lrcpp::Frontend::getMemoryData(unsigned id, void** data) {
    return _fsm->getMemoryData(id, data);
}

bool lrcpp::Frontend::getMemorySize(unsigned id, size_t* size) {
    return _fsm->getMemorySize(id, size);
}

bool lrcpp::Frontend::setControllerPortDevice(unsigned port, unsigned device) {
    return _fsm->setControllerPortDevice(port, device);
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
    return getInstance().environmentCallback(cmd, data);
}

bool lrcpp::Frontend::rumbleSetState(unsigned port, enum retro_rumble_effect effect, uint16_t strength) {
    return getInstance()._rumble->setState(port, effect, strength);
}

bool lrcpp::Frontend::sensorSetState(unsigned port, enum retro_sensor_action action, unsigned rate) {
    return getInstance()._sensor->setState(port, action, rate);
}

float lrcpp::Frontend::sensorGetInput(unsigned port, unsigned id) {
    return getInstance()._sensor->getInput(port, id);
}

bool lrcpp::Frontend::cameraStart() {
    return getInstance()._camera->start();
}

void lrcpp::Frontend::cameraStop() {
    getInstance()._camera->stop();
}

void lrcpp::Frontend::loggerVprintf(enum retro_log_level level, char const* format, ...) {
    va_list args;
    va_start(args, format);
    getInstance()._logger->vprintf(level, format, args);
    va_end(args);
}

retro_time_t lrcpp::Frontend::perfGetTimeUsec() {
    return getInstance()._perf->getTimeUsec();
}

uint64_t lrcpp::Frontend::perfGetCpuFeatures() {
    return getInstance()._perf->getCpuFeatures();
}

retro_perf_tick_t lrcpp::Frontend::perfGetCounter() {
    return getInstance()._perf->getCounter();
}

void lrcpp::Frontend::perfRegister(struct retro_perf_counter* counter) {
    getInstance()._perf->register_(counter);
}

void lrcpp::Frontend::perfStart(struct retro_perf_counter* counter) {
    getInstance()._perf->start(counter);
}

void lrcpp::Frontend::perfStop(struct retro_perf_counter* counter) {
    getInstance()._perf->stop(counter);
}

void lrcpp::Frontend::perfLog() {
    getInstance()._perf->log();
}

bool lrcpp::Frontend::locationStart() {
    return getInstance()._location->start();
}

void lrcpp::Frontend::locationStop() {
    getInstance()._location->stop();
}

bool lrcpp::Frontend::locationGetPosition(double* lat, double* lon, double* horizAccuracy, double* vertAccuracy) {
    return getInstance()._location->getPosition(lat, lon, horizAccuracy, vertAccuracy);
}

void lrcpp::Frontend::locationSetInterval(unsigned intervalMs, unsigned intervalDistance) {
    getInstance()._location->setInterval(intervalMs, intervalDistance);
}

char const* lrcpp::Frontend::virtualFileSystemGetPath(struct retro_vfs_file_handle* stream) {
    return getInstance()._virtualFileSystem->getPath(stream);
}

struct retro_vfs_file_handle* lrcpp::Frontend::virtualFileSystemOpen(char const* path, unsigned mode, unsigned hints) {
    return getInstance()._virtualFileSystem->open(path, mode, hints);
}

int lrcpp::Frontend::virtualFileSystemClose(struct retro_vfs_file_handle* stream) {
    return getInstance()._virtualFileSystem->close(stream);
}

int64_t lrcpp::Frontend::virtualFileSystemSize(struct retro_vfs_file_handle* stream) {
    return getInstance()._virtualFileSystem->size(stream);
}

int64_t lrcpp::Frontend::virtualFileSystemTruncate(struct retro_vfs_file_handle* stream, int64_t length) {
    return getInstance()._virtualFileSystem->truncate(stream, length);
}

int64_t lrcpp::Frontend::virtualFileSystemTell(struct retro_vfs_file_handle* stream) {
    return getInstance()._virtualFileSystem->tell(stream);
}

int64_t lrcpp::Frontend::virtualFileSystemSeek(struct retro_vfs_file_handle* stream, int64_t offset, int seekPosition) {
    return getInstance()._virtualFileSystem->seek(stream, offset, seekPosition);
}

int64_t lrcpp::Frontend::virtualFileSystemRead(struct retro_vfs_file_handle* stream, void* s, uint64_t len) {
    return getInstance()._virtualFileSystem->read(stream, s, len);
}

int64_t lrcpp::Frontend::virtualFileSystemWrite(struct retro_vfs_file_handle* stream, void const* s, uint64_t len) {
    return getInstance()._virtualFileSystem->write(stream, s, len);
}

int lrcpp::Frontend::virtualFileSystemFlush(struct retro_vfs_file_handle* stream) {
    return getInstance()._virtualFileSystem->flush(stream);
}

int lrcpp::Frontend::virtualFileSystemRemove(char const* path) {
    return getInstance()._virtualFileSystem->remove(path);
}

int lrcpp::Frontend::virtualFileSystemRename(char const* oldPath, char const* newPath) {
    return getInstance()._virtualFileSystem->rename(oldPath, newPath);
}

int lrcpp::Frontend::virtualFileSystemStat(char const* path, int32_t* size) {
    return getInstance()._virtualFileSystem->stat(path, size);
}

int lrcpp::Frontend::virtualFileSystemMkDir(char const* dir) {
    return getInstance()._virtualFileSystem->mkDir(dir);
}

struct retro_vfs_dir_handle* lrcpp::Frontend::virtualFileSystemOpenDir(char const* dir, bool includeHidden) {
    return getInstance()._virtualFileSystem->openDir(dir, includeHidden);
}

bool lrcpp::Frontend::virtualFileSystemReadDir(struct retro_vfs_dir_handle* dirstream) {
    return getInstance()._virtualFileSystem->readDir(dirstream);
}

char const* lrcpp::Frontend::virtualFileSystemDirentGetName(struct retro_vfs_dir_handle* dirstream) {
    return getInstance()._virtualFileSystem->direntGetName(dirstream);
}

bool lrcpp::Frontend::virtualFileSystemDirentIsDir(struct retro_vfs_dir_handle* dirstream) {
    return getInstance()._virtualFileSystem->direntIsDir(dirstream);
}

int lrcpp::Frontend::virtualFileSystemCloseDir(struct retro_vfs_dir_handle* dirstream) {
    return getInstance()._virtualFileSystem->closeDir(dirstream);
}

void lrcpp::Frontend::ledSetState(int led, int state) {
    return getInstance()._led->setState(led, state);
}

bool lrcpp::Frontend::midiInputEnabled() {
    return getInstance()._midi->inputEnabled();
}

bool lrcpp::Frontend::midiOutputEnabled() {
    return getInstance()._midi->outputEnabled();
}

bool lrcpp::Frontend::midiRead(uint8_t* byte) {
    return getInstance()._midi->read(byte);
}

bool lrcpp::Frontend::midiWrite(uint8_t byte, uint32_t deltaTime) {
    return getInstance()._midi->write(byte, deltaTime);
}

bool lrcpp::Frontend::midiFlush() {
    return getInstance()._midi->flush();
}

uintptr_t lrcpp::Frontend::videoGetCurrentFramebuffer() {
    return getInstance()._video->getCurrentFramebuffer();
}

retro_proc_address_t lrcpp::Frontend::videoGetProcAddress(char const* symbol) {
    return getInstance()._video->getProcAddress(symbol);
}

void lrcpp::Frontend::videoRefresh(void const* data, unsigned width, unsigned height, size_t pitch) {
    if (getInstance()._video != nullptr) {
        getInstance()._video->refresh(data, width, height, pitch);
    }
}

size_t lrcpp::Frontend::audioSampleBatch(int16_t const* data, size_t frames) {
    if (getInstance()._audio != nullptr) {
        return getInstance()._audio->sampleBatch(data, frames);
    }

    return 0;
}

void lrcpp::Frontend::audioSample(int16_t left, int16_t right) {
    if (getInstance()._audio != nullptr) {
        getInstance()._audio->sample(left, right);
    }
}

int16_t lrcpp::Frontend::inputState(unsigned port, unsigned device, unsigned index, unsigned id) {
    if (getInstance()._input != nullptr) {
        return getInstance()._input->state(port, device, index, id);
    }

    return 0;
}

void lrcpp::Frontend::inputPoll() {
    if (getInstance()._input != nullptr) {
        getInstance()._input->poll();
    }
}
