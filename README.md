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

The current macOS backend provides native video, keyboard, mouse and timing.
Audio is temporarily disabled.
