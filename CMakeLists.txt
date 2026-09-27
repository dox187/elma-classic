cmake_minimum_required(VERSION 3.13)
project(elma_classic LANGUAGES CXX)

option(ELMA_REGISTERED "Build the registered version; needs the registered game data" OFF)
set(ELMA_PLATFORM "desktop" CACHE STRING
    "Target device: desktop, handheld (SDL 2 handhelds such as the RGB30) or miyoomini")
set_property(CACHE ELMA_PLATFORM PROPERTY STRINGS desktop handheld miyoomini)

# The Miyoo Mini only ships SDL 1.2, everything else uses SDL 2.
if(ELMA_PLATFORM STREQUAL "miyoomini")
    set(ELMA_SDL_MODULE sdl)
elseif(ELMA_PLATFORM STREQUAL "desktop" OR ELMA_PLATFORM STREQUAL "handheld")
    set(ELMA_SDL_MODULE sdl2)
else()
    message(FATAL_ERROR "Unknown ELMA_PLATFORM: ${ELMA_PLATFORM}")
endif()

# Handheld systems ship different C++ runtimes; linking it in lets one binary
# run on all of them.
if(ELMA_PLATFORM STREQUAL "desktop")
    set(ELMA_STATIC_RUNTIME_DEFAULT OFF)
else()
    set(ELMA_STATIC_RUNTIME_DEFAULT ON)
endif()
option(ELMA_STATIC_RUNTIME "Link the C++ runtime statically" ${ELMA_STATIC_RUNTIME_DEFAULT})

find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL REQUIRED IMPORTED_TARGET ${ELMA_SDL_MODULE})

set(ELMA_CORE
    ABC8.CPP ADATOK.CPP ANIM.CPP BEALLIT.CPP BESTTIME.CPP
    CUSTOM.CPP DIALOG.CPP DIALOG32.CPP DIGIT.CPP DISPLAY.CPP DOBOZ.CPP
    D_PIC.CPP ECSET.CPP EDITHELP.CPP EDITMENU.CPP EDITPLAY.CPP EDITTOLT.CPP
    EDITTOOL.CPP EDITUJ.CPP ED_CHECK.CPP FOGOCSKA.CPP GOLYKISZ.CPP
    GOLYOK.CPP GOLYUTK.CPP GRAPH.CPP GYURU.CPP H_WAV.CPP JATEKOS.CPP
    KIRAJ320.CPP KISKEP.CPP KOVETO.CPP KOZOS.CPP LEJATSZO.CPP LEPTET.CPP
    LGRFILE.CPP LOAD.CPP MAIN.CPP MAINMENU.CPP MENUKEP.CPP NEWUJ.CPP
    OPTIONS.CPP PEXTMENU.CPP PIC8.CPP PICLIST.CPP PLAY.CPP
    QOPEN.CPP README.CPP RECORDER.CPP SKIP.CPP SPRITE.CPP
    STATE.CPP SZAKASZ.CPP TELJES.CPP TOPOL.CPP UPGRADE.CPP UTKOZES.CPP
    UTKOZES2.CPP VALASZT2.CPP VEKT2.CPP
)

# The sources include headers by lowercase names (all.h) while the files are
# upper case (ALL.H). Generate lowercase forwarding headers so the build also
# works on case-sensitive file systems such as Linux. They are only rewritten
# when their content changes, so reconfiguring does not rebuild everything.
set(ELMA_HEADER_DIR ${CMAKE_CURRENT_BINARY_DIR}/lowercase_headers)
file(GLOB ELMA_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/*.H)
foreach(header ${ELMA_HEADERS})
    string(TOLOWER ${header} lower)
    set(forward "#include \"${CMAKE_CURRENT_SOURCE_DIR}/${header}\"\n")
    set(current "")
    if(EXISTS ${ELMA_HEADER_DIR}/${lower})
        file(READ ${ELMA_HEADER_DIR}/${lower} current)
    endif()
    if(NOT current STREQUAL forward)
        file(WRITE ${ELMA_HEADER_DIR}/${lower} "${forward}")
    endif()
endforeach()

add_executable(elma ${ELMA_CORE}
    F_SDL/SDL_PLATFORM.CPP F_SDL/SDL_SOUND.CPP F_SDL/SDL_HHIGH.CPP)
target_include_directories(elma PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${ELMA_HEADER_DIR})
target_link_libraries(elma PRIVATE PkgConfig::SDL)
# The code was written for x86, where char is signed, and for compilers that
# let signed integers wrap around instead of optimizing on their overflow.
# Floating point contraction (fused multiply-add on ARM) would change the
# physics results, so replays stay the same on every platform.
target_compile_options(elma PRIVATE -fpermissive -fsigned-char -fwrapv -ffp-contract=off)
if(ELMA_REGISTERED)
    target_compile_definitions(elma PRIVATE ELMA_REGISTERED)
endif()
if(NOT ELMA_PLATFORM STREQUAL "desktop")
    target_compile_definitions(elma PRIVATE ELMA_HANDHELD)
endif()
if(ELMA_STATIC_RUNTIME)
    target_link_libraries(elma PRIVATE -static-libstdc++ -static-libgcc)
endif()
if(ELMA_PLATFORM STREQUAL "miyoomini")
    target_compile_definitions(elma PRIVATE ELMA_MIYOO)
endif()
set_target_properties(elma PROPERTIES CXX_STANDARD 11 CXX_STANDARD_REQUIRED ON)
