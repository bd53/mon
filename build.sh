#!/usr/bin/env bash

set -e

c="gcc"
cxx="g++"
cflags="-Wall -Wextra -O2 -fPIC"
cxxflags="-Wall -Wextra -O2 -std=c++17 -fPIC"
target="mon"
lib="libsysinfo.a"
c_sources=("sysinfo.c")
cxx_sources=(
    "Monitor.cpp"
    "main.cpp"
    "ui/Terminal.cpp"
    "ui/Menu.cpp"
    "ui/Display.cpp"
    "screens/Memory.cpp"
    "screens/Package.cpp"
    "screens/Process.cpp"
    "screens/Profile.cpp"
    "screens/Runner.cpp"
    "screens/System.cpp"
    "screens/Timezone.cpp"
    "screens/CPU.cpp"
    "screens/Kernel.cpp"
    "screens/Mirrors.cpp"
    "screens/Hostname.cpp"
    "screens/Network.cpp"
)
output="build"
c_objs=("$output/sysinfo.o")
cxx_objs=(
    "$output/Monitor.o"
    "$output/main.o"
    "$output/Terminal.o"
    "$output/Menu.o"
    "$output/Display.o"
    "$output/Memory.o"
    "$output/Package.o"
    "$output/Process.o"
    "$output/Profile.o"
    "$output/Runner.o"
    "$output/System.o"
    "$output/Timezone.o"
    "$output/CPU.o"
    "$output/Kernel.o"
    "$output/Mirrors.o"
    "$output/Hostname.o"
    "$output/Network.o"
)

setup() {
    mkdir -p "$output"
    echo "Created build directory: $output"
}

compile() {
    echo "Compiling C sources..."
    for src in "${c_sources[@]}"; do
        obj="$output/$(basename "${src%.*}").o"
        $c $cflags -c "$src" -o "$obj"
    done
    echo "Compiling C++ sources..."
    for src in "${cxx_sources[@]}"; do
        obj="$output/$(basename "${src%.*}").o"
        $cxx $cxxflags -c "$src" -o "$obj"
    done
}

build() {
    setup
    compile
    echo "Building static library..."
    ar rcs "$output/$lib" "${c_objs[@]}"
    echo "Static library created: $output/$lib"
    echo "Linking final binary..."
    $cxx $cxxflags -o "$output/$target" "${cxx_objs[@]}" -L"$output" -lsysinfo -pthread
    echo "Binary created: $output/$target"
}

build
