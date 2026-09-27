#!/bin/sh
# Starts Elasto Mania on the Miyoo Mini, as an Onion OS port or App
# (Roms/PORTS/Games/ElastoMania, App/ElastoMania) or as a MinUI pak
# (Tools/miyoomini/Elasto Mania.pak). These launchers provide the device's
# SDL, which turns the picture right side up, and preload the sound library.
# The game data (elma.res, lgr, lev, ...) goes into this folder.
cd "$(dirname "$0")"

# The game finds its files case-insensitively; only create the folders that
# are missing in every spelling.
for folder in lev rec snaps; do
    ls | grep -qix "$folder" || mkdir "$folder"
done

./elma > log.txt 2>&1
