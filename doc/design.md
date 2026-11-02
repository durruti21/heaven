# Overall Design

The design is composed of the following main blocks:

1. [Config](#1---config)
2. [Data Store](#2---data-store)
3. [Engine](#3---engine)
4. [GUI](#4---gui)
5. [AI Agents](#5---ai-agents)
6. [Support](#6---support)

For build instructions and a quick overview of the project, see the
[README](../README.md).

---

## 1 - Config

The component that stores all the game configuration parameters. The parameters
control the behaviour of the rest of the components.

The config component reads the configuration parameters from the file system
configuration files and loads them on initialization.

The config dataset is split into two subsets:

- **System config dataset** — composed of parameters always read from file and
  not modifiable by the user (only by mods).
- **User config dataset** — composed of parameters modifiable by the user.

---

## 2 - Data Store

The component that stores all the information related to a specific game run.
The component provides structured access to all the information related to a
specific game and also provides functions to save or load games into/from the
hard disk.

This information is provided to the active components of the application (game
engine and game GUI).

---

## 3 - Engine

The game engine is in charge of running the game simulation by inspecting the
data provided by the data store and updating it.

The game engine uses the system config dataset with the rules and parameters for
executing the simulation. The game engine is driven by the game graphical user
interface. The engine receives the actions/decisions the player takes in the game
run and updates the data store state accordingly.

---

## 4 - GUI

The graphical user interface is the main interface with the user, providing the
information of the game and capturing the interactions of the user. Those
interactions are grouped into:

- **Config management** — modify the user config dataset to be used.
- **Game management** — load or save a game run into/from the data store object.
- **Engine management** — control the execution of the game engine.
- **Display management** — show all the information related to the data store to
  the user.
- **Control management** — translate the interactions of the player into
  actions/decisions for the game engine.

---

## 5 - AI Agents

The AI agents component is intended to store a set of agents that allows the
manipulation of the data store in the same way that the user interacts with the
GUI to control the simulation data.

The AI agent is intended to provide:

- Adversaries to the player.
- Generation of scenarios on the game setup (i.e. system creation).
- Generation of random simulation data during the simulation (i.e. images).

The AI agents will be created/deleted by the game engine when the appropriate
conditions occur.

---

## 6 - Support

This component stores all the infrastructure classes used by the rest of the
components, providing cross-component services.

---

# Config

The config object is composed of a parent object that composes the different
configuration files.

The overall architecture is to have a separate class to parse each file and
provide methods to get or set these parameters. The main access point to the
component is the `ConfigData` class, which exposes its children as public
methods.

To ensure robustness, all the parameters inside a class have a default value, so
if the file is missing, corrupt, or does not include the specific parameter, the
default value is used.

The configuration files are JSON files parsed by the
[nlohmann/json](https://github.com/nlohmann/json) library. The files are stored
in the `./config` directory of the game.

The identified files are:

| File | Purpose |
| --- | --- |
| `application.json` | Game application configuration (i.e. language, video, sound, etc). |
| `setup.json` | Simulation run initial conditions (i.e. start date). |

---

# Data Store

The data store is the main component that stores the simulation data. The main
operations that the data store provides to its users are:

| Operation | Description |
| --- | --- |
| **Create New Game** | Generates a new initial data set based on the config data and any associated generation algorithm. |
| **Load Game** | Loads a game from a saved simulation file in a predefined folder. |
| **Save Game** | Stores the simulation data in a file on a predefined folder. |
| **Create Parameter(s)** | Create function for some parameters that can have a limited life span on the simulation. |
| **Set Parameter(s)** | Set functions for each specific parameter that is modified by the users of the component. |
| **Get Parameter(s)** | Get functions for each specific parameter that is required by the user. |
| **Delete Parameter(s)** | Delete function for some parameters that can have a limited life span on the simulation. |

The dataset of each game is composed of:

- Time data.
- System body info data.

---

# Engine

The engine is the main component in charge of executing the simulation. It has
all the logic necessary to process and update all the data related to the
simulation.

The engine is composed of several subcomponents, each one in charge of updating
one aspect of the data. Special caution must be kept on the engine to ensure that
the data is updated consistently so no closed loops of information are
established.

The simulation is driven by two parameters:

- The **tick**, which is the minimum amount of time simulated.
- The **increment**, which is a multiple of ticks.

This approach is used to provide flexibility on the execution.

The engine is driven by the GUI, which tells the engine when it has to run and
how many steps it must run. In that sense, the commands it receives from the GUI
are:

| Command | Description |
| --- | --- |
| **Set Simulation Time** | Establishes the time increment in ticks. |
| **Set Execution Mode** | Single tick or continuous. |
| **Execute** | Executes the simulation in the mode and time established. |
| **Stop** | Stops the simulation. |

Not all aspects of the simulation are executed on each increment. Depending on
the aspect, some fixed ticks are used for each aspect. The tick and the
associated aspect updates are driven by configuration.

As the engine must run in decoupled mode from the GUI, an internal thread is used
to run the engine.

---

# GUI

The GUI component is the interface with the user. It relies on the SDL3 and
Dear ImGui libraries to perform the graphical output and capture user inputs.

The component is divided into:

| Subcomponent | Description |
| --- | --- |
| `gui_console` | A debug console activated by the `TAB` key. |
| `gui_icons` | Definition of icons used on the GUI interface. |
| `gui_main` | The object that stores the main loop interaction with the SDL and ImGui initialization/shutdown and loop main procedures. |
| `gui_map` | The simulation main map. |
| `gui_map_camera` | The control of the map camera. |
| `gui_sim_interface` | The simulation interface menus. Control of simulation time. |
| `gui_item_display` | The component to display item information. |

Each component has the proper draw and event handling components.

Other components will be identified in the future.

---

# Support

The support component is a set of classes with cross-functional infrastructure
used by the rest of the main components. The list of services it provides is:

- **Logger** — provides logger services for the rest of the components.

The policy to use the log shall be:

| Level | Policy |
| --- | --- |
| **Critical** | Errors that prevent normal working of the application and end in termination. All conditions must be logged. |
| **Warning** | Messages that do not prevent normal working but should indicate a failure. All conditions must be logged. |
| **Info** | Messages of normal application run that are significant (i.e. load data, initialization, user actions). All must be logged. |
| **Debug** | Messages inserted only for debugging purposes. Normally only inserted to test problems on the application. |
