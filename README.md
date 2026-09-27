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
