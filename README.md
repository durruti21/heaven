# HEAVEN

**HEAVEN** is a space-simulation game built in C++ around a modular, component-based
architecture. The simulation is driven by a graphical interface (SDL3 + Dear ImGui),
runs independently in its own thread, and keeps all game state in a dedicated data
store that can be saved and loaded.

The project is currently at version **0.02**, which implements planetary orbital
mechanics, generation/loading of the Sol system, a 2D map projection with camera
controls, a debug console, and system-body information display.

---

## Table of Contents

- [Architecture](#architecture)
- [Project Layout](#project-layout)
- [Components](#components)
  - [Config](#1-config)
  - [Data Store](#2-data-store)
  - [Engine](#3-engine)
  - [GUI](#4-gui)
  - [AI Agents (planned)](#5-ai-agents-planned)
  - [Support](#6-support)
- [Configuration Files](#configuration-files)
- [Simulation Model](#simulation-model)
- [Controls](#controls)
- [Logging Policy](#logging-policy)
- [Building and Running](#building-and-running)
- [Roadmap](#roadmap)
- [Documentation Sources](#documentation-sources)

---

## Architecture

The application is split into independent components that communicate through
well-defined interfaces. The current implementation is composed of five active
components plus one planned component:

```
                    +-------------------+
                    |       GUI         |
                    |  (SDL3 + ImGui)   |
                    +----+---------+----+
                         |         |
             user input  |         |  display data
                         v         ^
+------------+     +-------------------+     +------------------+
|   Config   |---->|    Data Store     |<--->|      Engine      |
| (JSON)     |     | (game run state)  |     | (simulation loop)|
+------------+     +-------------------+     +------------------+
                         ^
                         |  cross-component services
                    +-------------------+
                    |      Support      |
                    | (logger, types)   |
                    +-------------------+
```

- The **GUI** is the only interface with the player. It displays data from the
  data store and translates user actions into engine commands.
- The **Engine** reads from and writes to the **Data Store**, and is driven by
  the GUI. It runs in a separate thread so it stays decoupled from rendering.
- The **Config** component provides the immutable system rules and the
  user-modifiable settings to the rest of the application.
- The **Support** component supplies infrastructure shared by every component,
  such as logging and common unit types.

---

## Project Layout

```
heaven/
├── CMakeLists.txt          # Top-level build definition
├── CMakePresets.json       # CMake configure presets
├── config/                 # Runtime configuration (JSON)
│   ├── application.json    # Application/game settings
│   └── setup.json          # Simulation initial conditions
├── data/                   # Runtime data assets
│   ├── fonts/              # Fonts (e.g. typicons.ttf)
│   └── reference/          # Reference data (stellar_info.db, body images)
├── doc/                    # Design notes, controls and roadmap
├── lib/                    # Vendored dependencies
│   ├── SDL3/               # SDL3 headers and shared libraries
│   ├── SDL3_image/         # SDL3_image headers and shared library
│   ├── imgui/              # Dear ImGui
│   ├── nlohmann_json/      # JSON parser
│   ├── spdlog/             # Logging library
│   ├── sqlite/             # SQLite (sources + prebuilt library)
│   └── typicons.font/      # Icon font sources
├── src/
│   ├── heaven.cpp          # Application entry point (SDL3 callbacks)
│   ├── config/             # Configuration component
│   ├── data_store/         # Game-state component
│   ├── engine/             # Simulation engine
│   ├── gui/                # Graphical interface
│   └── support/            # Shared infrastructure
└── build/                  # Generated build output (not tracked)
```

---

## Components

### 1. Config

Stores every game configuration parameter. Parameters are loaded from JSON files
in `./config` during initialization, and control the behaviour of all other
components.

The configuration dataset is split into two subsets:

| Dataset | Description |
| --- | --- |
| **System config** | Always read from file and not modifiable by the user (only by mods). |
| **User config** | Parameters that can be modified by the user. |

`ConfigData` is the main access point and exposes its child configuration
objects as public members (`gameApp`, `gameSetup`). Each configuration file has
its own parser class with get/set methods.

To ensure robustness, every parameter has a default value: if a file is missing,
corrupt, or lacks a specific parameter, the default is used instead.

**Files**

| File | Purpose |
| --- | --- |
| `config/application.json` | Application configuration (logging, paths, assets). |
| `config/setup.json` | Simulation run initial conditions (start date, increments). |

**Implementation:** `src/config/`

---

### 2. Data Store

The central repository for all information related to a specific game run. It
provides structured access to the game state and the operations required to
persist it.

Main operations:

- **Create New Game** — builds an initial dataset from the config and the
  associated generation algorithms.
- **Load Game** — loads a simulation from a file in the configured save folder.
- **Save Game** — stores the simulation to a file in the configured save folder.
- **Create Parameter(s)** — creates parameters with a limited lifetime in the
  simulation.
- **Set Parameter(s)** — mutates parameters on behalf of the active components.
- **Get Parameter(s)** — exposes parameters required by consumers.
- **Delete Parameter(s)** — removes parameters with a limited lifetime.

The dataset of each game currently includes:

- **Time data** — simulation clock and tick counters.
- **System body data** — celestial bodies, their orbital parameters and minerals.

**Implementation:** `src/data_store/` (`DataStore`, `DataTime`,
`SystemBody`, `SystemBodyFactory`).

---

### 3. Engine

The simulation engine executes the game logic and updates every aspect of the
simulation data. It is composed of several subcomponents, each responsible for
one aspect of the data. Care is taken to keep updates consistent and to avoid
closed information loops.

The simulation is driven by two parameters:

- **Tick** — the minimum amount of simulated time.
- **Increment** — a multiple of ticks.

This separation provides flexibility in how the simulation is executed. Not
every aspect of the simulation is updated on every increment: each aspect runs on
its own fixed tick cadence, configured through the configuration files.

The engine runs in its own internal thread so it is decoupled from the GUI. The
GUI commands it through:

- **Set Simulation Time** — establishes the time increment in ticks.
- **Set Execution Mode** — single tick or continuous.
- **Execute** — runs the simulation with the established mode and time.
- **Stop** — stops the simulation.

`SimEngine` exposes this as `Step()`, `Execute()`, `Pause()`, `Stop()`,
`SetExecutionTime()`, and `SetExecutionMode()` / `GetExecutionMode()`.

**Implementation:** `src/engine/sim_engine.{hpp,cpp}`.

---

### 4. GUI

The main interface with the user: it provides the game information and captures
player interactions. It relies on **SDL3** for windows/input and **Dear ImGui**
for the interface.

Interaction groups:

- **Config management** — modify the user config dataset.
- **Game management** — load/save a game run through the data store.
- **Engine management** — control execution of the engine.
- **Display management** — show all data-store information to the player.
- **Control management** — translate player interactions into engine actions.

Subcomponents:

| Component | Role |
| --- | --- |
| `gui_console` | Debug console, opened with `TAB`. |
| `gui_icons` | Icon definitions used by the interface. |
| `gui_main` | SDL/ImGui initialization, shutdown, main loop and event dispatch. |
| `gui_map` | The simulation map. |
| `gui_map_camera` | Map camera control (pan/zoom). |
| `gui_sim_interface` | Simulation menus and time control. |
| `gui_item_display` | Item/system-body information display. |

Each subcomponent implements its own drawing and event handling. More components
will be added in the future.

**Implementation:** `src/gui/`.

---

### 5. AI Agents (planned)

A future component that will hold agents able to manipulate the data store in the
same way the player does through the GUI. Intended uses:

- Adversaries for the player.
- Scenario generation during game setup (for example, system creation).
- Generation of random simulation data during the simulation (for example,
  images).

Agents will be created and deleted by the engine when the appropriate conditions
occur. This component is **not yet implemented**.

---

### 6. Support

Cross-cutting infrastructure used by every component. Services include:

- **Logger** — shared logging service (singleton) used across the application.
- **Unit types** — common unit definitions used by the simulation.

**Implementation:** `src/support/`.

---

## Configuration Files

### `config/application.json`

```json
{
    "log_level"           : "debug",
    "save_directory"      : "./save/",
    "data_directory"      : "./data/",
    "sbody_img_directory" : "./data/reference/images/sbodies",
    "stellar_info_file"   : "./data/reference/stellar_info.db",
    "fonts_file"          : "./data/fonts/typicons.ttf"
}
```

| Key | Description |
| --- | --- |
| `log_level` | Logging threshold (`critical`, `warning`, `info`, `debug`). |
| `save_directory` | Folder where saved games are written/read. |
| `data_directory` | Root folder for runtime data. |
| `sbody_img_directory` | Folder containing system-body images. |
| `stellar_info_file` | SQLite database with reference stellar information. |
| `fonts_file` | Icon font used by the interface. |

### `config/setup.json`

```json
{
    "start_date" : {
        "date" : "2050-01-01",
        "time" : "00:00:00"
    },
    "increments" : {
        "tick" : 1
    }
}
```

| Key | Description |
| --- | --- |
| `start_date.date` | Initial simulation calendar date. |
| `start_date.time` | Initial simulation time of day. |
| `increments.tick` | Base simulation tick increment. |

---

## Simulation Model

- The simulation advances in **ticks**, the smallest unit of simulated time.
- An **increment** is a multiple of ticks and defines how much time a single
  execution step covers.
- The engine can run in two modes:
  - **Single step** (`EXEC_SIM_STEP`) — advance exactly one increment.
  - **Continuous** (`EXEC_SIM_CONTINUOUS`) — keep advancing until paused/stopped.
- Different simulation aspects update at different tick cadences, configured
  through the configuration files.
- The engine executes on its own thread; the GUI only sends commands and reads
  the resulting state.

---

## Controls

| Input | Action |
| --- | --- |
| `ESC` | Exit the application immediately. |
| `TAB` | Open the debug console. |
| `Z` / Mouse wheel up | Zoom in. |
| `X` / Mouse wheel down | Zoom out. |
| `W` | Move up. |
| `S` | Move down. |
| `A` | Move left. |
| `D` | Move right. |

---

## Logging Policy

Logging is provided by the **Support** component through `spdlog`. The active
level is read from `log_level` in `config/application.json` and defaults to a
safe value if the file is missing or invalid.

| Level | Usage |
| --- | --- |
| **Critical** | Errors that prevent normal operation and terminate the application. All such conditions must be logged. |
| **Warning** | Conditions that do not prevent operation but indicate a failure. All such conditions must be logged. |
| **Info** | Significant normal events (data loading, initialization, user actions). All must be logged. |
| **Debug** | Messages used only while debugging, typically to investigate problems. |

---

## Building and Running

### Prerequisites

- A C++ compiler (C++17 or later)
- **CMake 3.28** or later
- Linux (the vendored SDL3 and SDL3_image libraries are Linux `.so` builds)

All other dependencies are vendored under `lib/`: SDL3, SDL3_image, Dear ImGui,
nlohmann/json, spdlog, and SQLite.

### Build

```bash
cmake --preset default
cmake --build build -j
```

The `default` preset configures a `Debug` build in `build/` with
`compile_commands.json` enabled.

### Run

Run from the project root so the relative paths in `config/application.json`
resolve correctly:

```bash
./build/heaven
```

The application reads `./config/application.json` and `./config/setup.json`,
writes its log to `./log.txt`, and loads data from `./data/`.

---

## Roadmap

| Version | Scope |
| --- | --- |
| **0.01** | Initial version validating the architecture and integrating the main libraries (SDL, ImGui, JSON, spdlog). Basic time control to validate simulation time. |
| **0.02** | Planetary mechanics: orbital mechanics for planets, creation/reading of Sol system bodies from a data file, basic 2D projection of the Sol system, debug console, basic map controls (move, zoom). |
| **0.03** | Planetary information display: selection of a system body on the map, display of body information, inclusion and display of body images for the Sol system, generation of minerals for the Sol system, display of minerals. |

---

## Documentation Sources

This README is derived from the project notes in `doc/`:

- [`doc/design.txt`](doc/design.txt) — overall design and component descriptions.
- [`doc/keys.txt`](doc/keys.txt) — application controls.
- [`doc/roadmap.txt`](doc/roadmap.txt) — development roadmap.
