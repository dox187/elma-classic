#!/bin/bash
# PortMaster launcher for Elasto Mania. The game data (elma.res, lgr, lev, ...)
# goes into the elastomania folder next to this script.

XDG_DATA_HOME=${XDG_DATA_HOME:-$HOME/.local/share}

if [ -d "/opt/system/Tools/PortMaster/" ]; then
  controlfolder="/opt/system/Tools/PortMaster"
elif [ -d "/opt/tools/PortMaster/" ]; then
  controlfolder="/opt/tools/PortMaster"
elif [ -d "$XDG_DATA_HOME/PortMaster/" ]; then
  controlfolder="$XDG_DATA_HOME/PortMaster"
else
  controlfolder="/roms/ports/PortMaster"
fi

source $controlfolder/control.txt
[ -f "${controlfolder}/mod_${CFW_NAME}.txt" ] && source "${controlfolder}/mod_${CFW_NAME}.txt"
get_controls

GAMEDIR="/$directory/ports/elastomania"
cd "$GAMEDIR"
> "$GAMEDIR/log.txt" && exec > >(tee "$GAMEDIR/log.txt") 2>&1

# The game finds its files case-insensitively; only create the folders that
# are missing in every spelling.
for folder in lev rec snaps; do
  ls | grep -qix "$folder" || mkdir "$folder"
done

$ESUDO chmod +x "$GAMEDIR/elma" 2>/dev/null

# The game reads the pad itself; gptokeyb only provides the exit hotkey.
export SDL_GAMECONTROLLERCONFIG="$sdl_controllerconfig"

$GPTOKEYB "elma" &
pm_platform_helper "$GAMEDIR/elma"
./elma

pm_finish
