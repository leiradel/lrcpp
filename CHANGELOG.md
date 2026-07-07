# Changelog

All notable changes to **lrcpp** are recorded here, loosely following
[Keep a Changelog](https://keepachangelog.com/). The project has no tagged
releases, so changes are grouped into dated milestones, newest first.

Unless noted otherwise, entries are about the library. Changes to the bundled
SDL2 example front-end are marked *(example)*.

## 2026-07 — SDL2 example overhaul and cross-platform build

### Added
- *(example)* Audio-driven emulation on a dedicated thread, clocked by the sound
  card via a ring buffer, replacing the wall-clock frame timer that drifted
  against the audio hardware. Video is handed to the main thread through a
  double-buffered CPU frame and drawn vsync-paced.
- *(example)* Mouse support (relative deltas, buttons, wheel) and pointer support
  (absolute coordinates mapped into the displayed game image). Video exposes its
  viewport so Input can map pointer coordinates.
- *(example)* A VFS v3 component backed by the host filesystem (files and
  directories).
- *(example)* Fully configuration-driven operation. Command-line arguments are
  now a list of config files (later files override earlier ones). Keys:
  `sdl2lrcpp_core_path`, `sdl2lrcpp_content_path`, `sdl2lrcpp_state_path`,
  `sdl2lrcpp_system_path`, `sdl2lrcpp_assets_path`, `sdl2lrcpp_save_path`,
  `sdl2lrcpp_log_path`, and `sdl2lrcpp_log_level`; any other key is passed to the
  core as a core option.
- *(example)* File logging via `sdl2lrcpp_log_path`, automatic save-state loading
  after content is loaded, and graceful handling of `RETRO_ENVIRONMENT_SHUTDOWN`.
- Handling of `RETRO_ENVIRONMENT_SET_CORE_OPTIONS_V2` in the environment dispatch.

### Changed
- *(example)* Consumes lrcpp's V2 core-options interface
  (`Config::setCoreOptionsV2Intl`) instead of the legacy calls.
- Build: honor `CC`/`CXX` in the Makefiles; timing uses `std::chrono` so the
  example builds and runs on both Windows (MSYS2) and Linux.
- Documentation: added a Concurrency section explaining the thread-local
  "current frontend", corrected the stale singleton/`getInstance` claim, and
  synced the API reference with the code.

### Fixed
- *(example)* Mouse deltas now reset every poll; keyboard and mouse buttons
  report the libretro-canonical `0`/`1`.

## 2026-01 – 2026-06 — Core options v2, multi-threaded cores, new FSM

### Added
- Core options v2 with backward compatibility: front-ends implement only
  `Config::setCoreOptionsV2Intl`, and the base class converts `SET_VARIABLES`,
  `SET_CORE_OPTIONS`, and the internationalized variants into it.
- Support for cores that call the front-end from their own threads (one such core
  at a time), via a thread-local current front-end with a global fallback.
- Methods to query which API calls are allowed in the core's current state.
- `Frontend::getExtension` (later fixed to be a self-transition).
- Routing of hardware-render `get_current_framebuffer` / `get_proc_address` to the
  Video component.
- A default implementation of `lrcpp::Audio::sample`.

### Changed
- New finite state machine, wrapped by a C++ class and regenerated with the
  latest ddlt.
- Renamed `interface` to `ifc` to avoid a Windows macro clash.

## 2025 — API updates and fixes

### Added
- `RETRO_ENVIRONMENT_GET_SAVESTATE_CONTEXT` support; `serialize` and
  `serializeSize` take a savestate context.
- Default implementations for every component method.

### Changed
- `retro_vfs_interface_info` is no longer const, since the front-end fills it in.
- Libretro log messages are terminated with an end-of-line.
- Dynamic-library handling moved out of lrcpp into the example, dropping the extra
  abstraction; updated `libretro.h`.

### Fixed
- Correct unloading of the core library, dynamic-library loading, and the perf
  clock.

### Removed
- The abandoned Android example.

## 2023 — Multiple front-ends, no more singleton

### Added
- Getters for all components.
- Re-entrant calls from different front-ends, tracked with a current-front-end
  pointer set around each core call (`getCurrent` / `setCurrent`).

### Changed
- `Frontend` is no longer a singleton, so multiple front-ends can run again.
- The front-end logs through the provided logger.
- Fixed how core functions are declared and initialized.

### Removed
- The ability to load core shared libraries from within lrcpp (moved to the
  example).

## 2022 — Android and shutdown handling

### Added
- An Android build (removed again in 2025).
- `RETRO_ENVIRONMENT_SHUTDOWN`: the front-end records the request and lets the
  caller shut down gracefully instead of unloading the core immediately.

## 2021-10 — SDL2 example front-end

### Added
- *(example)* A complete SDL2 front-end: the player loop plus Audio, Video, Input,
  Config, Logger, and Perf components.
- *(example)* Configuration files (comments, quoted and unquoted values, multiple
  files) with options for the audio device, video renderer, smoothing, and log
  level; software-framebuffer rendering; and precise frame timing.

## 2021-01 – 2021-03 — Library hardening

### Added
- Support for statically-linked cores and Circle baremetal.
- Logger convenience functions and `setControllerPortDevice` on the front-end.

### Changed
- The system AV info is set after the game is loaded; temporary strings are
  allocated on the heap; headers were reorganized; the singleton was made
  non-copyable.

### Removed
- Concurrent multi-core support — the front-end became a singleton because some
  cores call back from other threads (multiple front-ends returned in 2023).

## 2020-05 – 2020-12 — Initial library

### Added
- The core architecture: the `Frontend` class, the component classes, and the
  finite state machine that prevents the core from entering invalid states (such
  as running a frame before initialization).
- Component setters that validate the core's state, virtual destructors on the
  components, and the README documenting each component's environment calls.
