# Game release
This is NOT a playable release of Elasto Mania. If you're looking to play any of the Elasto Mania games, visit https://elastomania.com.

# Elasto Mania (2000) Source Code
This repository was uploaded in good faith for the purpose of exploring the original source code of this classic game. Non-source-code game assets (sounds, graphics, tools, etc.) are not part of this open source release.

See LICENSE.md for license information.

If you'd like to use this source code in ways other than permitted by the license and this document, contact us at info@elastomania.com.

If you'd like to support continued development of the Elasto Mania franchise, you can do so by buying our games on any store front linked on our website.

*The Elasto Mania Team*

https://elastomania.com

## Native macOS port

This fork adds a native Apple Silicon macOS backend using SDL2. It builds the
original game and physics code as a Mach-O arm64 executable; Wine and DOSBox
are not involved.

### Dependencies

Install CMake and SDL2 compatibility libraries with Homebrew:

```sh
brew install cmake pkg-config sdl2-compat
```

### Build

```sh
cmake -S . -B build -DCMAKE_CXX_COMPILER=/usr/bin/clang++
cmake --build build -j8
```

### Game data

The upstream source release deliberately excludes the copyrighted game data.
Copy these files from a legally obtained Elasto Mania installation:

```text
elma.res
lgr/default.lgr
```

Create empty runtime directories if they do not exist:

```sh
mkdir -p lev rec snaps
```

Run the executable from the repository root so it can find the data:

```sh
./build/elma
```

The current macOS backend provides native video, keyboard, mouse, timing and
sound.

## Native Linux port

The same SDL2 backend also builds natively on Linux (tested on Fedora 44,
x86_64, GCC 16).

### Dependencies

```sh
sudo dnf install cmake gcc-c++ sdl2-compat-devel   # Fedora
sudo apt install cmake g++ libsdl2-dev             # Debian, Ubuntu
```

### Build

The default build is the shareware version, as configured in the upstream
source. To play an installation of the registered game (for example version
1.11a), build with `ELMA_REGISTERED`; that build needs the registered
`elma.res` and cannot read the shareware data.

```sh
cmake -S . -B build -DELMA_REGISTERED=ON
cmake --build build -j
```

### Running

Run the executable from the game directory, next to `elma.res`:

```sh
cd /path/to/ElastoMania
/path/to/elma-classic/build/elma
```

File names are matched case-insensitively, like on Windows, so an original
installation (`Elma.res`, `Lgr/Default.lgr`, `Lev/`, `Rec/`) works without
renaming. The on-disk `state.dat` structures use 32-bit fields, so existing
`state.dat` files keep their players and best times. Sound uses the original
mixer of the Windows version, played through SDL.

## Handheld Linux devices

The SDL backend also builds for handheld game consoles, cross-compiled in a
container (podman or docker):

- 64-bit ARM handhelds with [PortMaster](https://portmaster.games), for
  example the Powkiddy RGB30 or the Anbernic RG353 and RG35XX H/Plus/SP, on
  ArkOS, ROCKNIX, muOS, Knulli or AmberELEC (SDL 2).
- The Miyoo Mini and Mini Plus with Onion OS or MinUI (SDL 1.2, using the
  Onion toolchain image).

### Build

```sh
handheld/build.sh portmaster -DELMA_REGISTERED=ON
handheld/build.sh miyoomini -DELMA_REGISTERED=ON
```

Leave out `-DELMA_REGISTERED=ON` for the shareware data. The packages are
written to `dist/portmaster` and `dist/miyoomini`.

### Install

- PortMaster: copy `Elasto Mania.sh` and the `elastomania` folder to the
  `ports` folder of the device, then copy the game data (`elma.res`, `lgr`,
  `lev`, and `state.dat` if you want to keep your players and times) into
  `elastomania`. The game shows up among the ports.
- Onion OS, among the ports: copy the `ElastoMania` folder to
  `Roms/PORTS/Games` on the SD card and the game data into it, then copy
  `Elasto Mania.port` to `Roms/PORTS/Shortcuts`. A 256x360 picture saved as
  `Roms/PORTS/Imgs/Elasto Mania.png` is shown as its box art.
- Onion OS, among the apps: copy the `ElastoMania` folder to `App` instead,
  with the game data in it. Put an `icon.png` there for an icon in the Apps
  list.
- MinUI: copy the `ElastoMania` folder to `Tools/miyoomini` on the SD card as
  `Elasto Mania.pak`, and the game data into it.

### Controls

| Button            | In the game                          | In the menus |
|-------------------|--------------------------------------|--------------|
| D-pad, left stick | Up throttle, Down brake, Left/Right rotate | Move   |
| A                 | Throttle                             | Select       |
| B                 | Brake; leaves demos and replays      | Back         |
| X, R1             | Change direction                     | R1: page down |
| Y                 | Toggle navigator                     |              |
| L1                | Toggle time                          | Page up      |
| L2 / R2           | Smaller / larger screen              |              |
| Select            | Leave the level                      | Back         |
| Start             |                                      | Select       |

The buttons press the keys set for player A under Options, Customize
Controls, so they keep working with any key setup. Names are entered with the
d-pad: Up and Down change the last letter, Right starts a new one and Left
deletes one. The level editor needs a mouse and a keyboard, so the handheld
builds leave it out.

The game runs full screen on handhelds, and at 640x480 fills the screen of
most of them. On other screens it is scaled; set
`SDL_RENDER_SCALE_QUALITY=linear` in the launcher for smooth instead of sharp
scaling. `ELMA_FULLSCREEN=0` or `1` overrides the full screen default on any
build.
