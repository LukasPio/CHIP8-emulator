#!/usr/bin/env bash
set -euo pipefail
root=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)
cd "$root"
platform=${1:-all}
case "$platform" in
    linux|windows|all) ;;
    *) echo "Usage: $0 [linux|windows|all]" >&2; exit 2 ;;
esac
if [[ $(uname -m) != x86_64 ]]; then
    echo 'Release builds require an x86_64 Linux build host.' >&2
    exit 1
fi
version=$(sed -n 's/^project(chip8 VERSION \([0-9.]*\).*/\1/p' CMakeLists.txt)
archive="$root/build/downloads/SDL2-2.32.10.tar.gz"
mkdir -p build/downloads dist
if [[ ! -f "$archive" ]]; then
    curl --fail --location --retry 3 https://www.libsdl.org/release/SDL2-2.32.10.tar.gz -o "$archive.tmp"
    mv "$archive.tmp" "$archive"
fi
echo "5f5993c530f084535c65a6879e9b26ad441169b3e25d789d83287040a9ca5165  $archive" | sha256sum --check

build_release() {
    local target=$1
    local name="chip8-${version}-${target}-x86_64"
    local build_dir="${CHIP8_BUILD_ROOT:-$root/build}/release-$target"
    local options=()
    if [[ $target == windows ]]; then
        options+=("-DCMAKE_TOOLCHAIN_FILE=$root/cmake/mingw-x86_64.cmake")
    fi
    cmake -S "$root" -B "$build_dir" -G Ninja \
        -DCMAKE_BUILD_TYPE=Release -DCHIP8_SDL_ARCHIVE="$archive" "${options[@]}"
    cmake --build "$build_dir" --parallel "${JOBS:-2}"
    if [[ $target == linux ]]; then
        ctest --test-dir "$build_dir" --output-on-failure
        # A portable build must only link directly to glibc and its loader.
        local needed
        needed=$(readelf -d "$build_dir/chip8" | sed -n 's/.*Shared library: \[\(.*\)\]/\1/p')
        if echo "$needed" | grep -Ev '^(libc\.so\.6|libm\.so\.6|libdl\.so\.2|libpthread\.so\.0|librt\.so\.1)$'; then
            echo 'Unexpected runtime dependency in Linux release.' >&2
            exit 1
        fi
    else
        if x86_64-w64-mingw32-objdump -p "$build_dir/chip8.exe" | grep -Ei 'DLL Name:.*(SDL|libgcc|libwinpthread|libstdc\+\+)'; then
            echo 'Unexpected runtime dependency in Windows release.' >&2
            exit 1
        fi
    fi
    cmake --install "$build_dir" --prefix "$root/dist/$name" --component Runtime --strip
    if [[ $target == windows ]]; then
        # Recreate the archive so files removed in later builds cannot linger.
        rm -f "dist/$name.zip"
        (cd dist && zip -q -r "$name.zip" "$name")
    else
        tar -C dist -czf "dist/$name.tar.gz" "$name"
    fi
}
if [[ $platform == linux || $platform == all ]]; then build_release linux; fi
if [[ $platform == windows || $platform == all ]]; then build_release windows; fi
(cd dist && find . -maxdepth 1 -type f \( -name 'chip8-*.zip' -o -name 'chip8-*.tar.gz' \) -print0 | sort -z | xargs -0 sha256sum > SHA256SUMS)
