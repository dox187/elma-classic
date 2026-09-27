cmake_minimum_required(VERSION 3.20)
project(elma_classic LANGUAGES CXX)

option(ELMA_REGISTERED "Build the registered version; needs the registered game data" OFF)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED IMPORTED_TARGET sdl2)

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
# works on case-sensitive file systems such as Linux.
set(ELMA_HEADER_DIR ${CMAKE_CURRENT_BINARY_DIR}/lowercase_headers)
file(GLOB ELMA_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/*.H)
foreach(header ${ELMA_HEADERS})
    string(TOLOWER ${header} lower)
    set(forward "#include \"${CMAKE_CURRENT_SOURCE_DIR}/${header}\"\n")
    file(CONFIGURE OUTPUT ${ELMA_HEADER_DIR}/${lower} CONTENT "${forward}" @ONLY)
endforeach()

add_executable(elma ${ELMA_CORE}
    F_SDL/SDL_PLATFORM.CPP F_SDL/SDL_SOUND.CPP F_SDL/SDL_HHIGH.CPP)
target_include_directories(elma PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${ELMA_HEADER_DIR})
target_link_libraries(elma PRIVATE PkgConfig::SDL2)
target_compile_options(elma PRIVATE -fpermissive)
if(ELMA_REGISTERED)
    target_compile_definitions(elma PRIVATE ELMA_REGISTERED)
endif()
set_target_properties(elma PROPERTIES CXX_STANDARD 11 CXX_STANDARD_REQUIRED ON)
