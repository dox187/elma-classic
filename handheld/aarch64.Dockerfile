# Cross toolchain for 64-bit ARM handhelds (RGB30, RG353, RG35XX H/Plus/SP, ...).
# Debian bullseye keeps the glibc requirement low enough for ArkOS, ROCKNIX,
# muOS, Knulli and AmberELEC. Packages come from the snapshot the base image
# was built from, so they match its libc and stay available after bullseye's
# end of life.
FROM docker.io/library/debian:bullseye
RUN sed -i -e 's/^deb /# deb /' -e 's/^# deb http:\/\/snapshot/deb [check-valid-until=no] http:\/\/snapshot/' /etc/apt/sources.list \
 && dpkg --add-architecture arm64 \
 && apt-get update \
 && apt-get install -y --no-install-recommends \
        cmake make pkg-config g++-aarch64-linux-gnu libsdl2-dev:arm64 \
 && rm -rf /var/lib/apt/lists/*
