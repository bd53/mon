c := "gcc"
cxx := "g++"
cflags := "-Wall -Wextra -O2 -fPIC"
cxxflags := "-Wall -Wextra -O2 -std=c++17 -fPIC"
target := "mon"
lib := "libsysinfo.a"
c_sources := "sysinfo.c"
cxx_sources := "Monitor.cpp main.cpp ui/Terminal.cpp ui/Menu.cpp ui/Display.cpp screens/Memory.cpp screens/Package.cpp screens/Process.cpp screens/Profile.cpp screens/Runner.cpp screens/System.cpp screens/Timezone.cpp screens/CPU.cpp screens/Kernel.cpp screens/Mirrors.cpp screens/Hostname.cpp screens/Network.cpp"
output := "build"
c_objs := output + "/sysinfo.o"
cxx_objs := output + "/Monitor.o " + output + "/main.o " + output + "/Terminal.o " + output + "/Menu.o " + output + "/Display.o " + output + "/Memory.o " + output + "/Package.o " + output + "/Process.o " + output + "/Runner.o " + output + "/System.o " + output + "/Profile.o " + output + "/Timezone.o " + output + "/CPU.o " + output + "/Kernel.o " + output + "/Mirrors.o " + output + "/Hostname.o " + output + "/Network.o"

default: clean build

all: clean build

setup:
    mkdir -p {{output}}

compile: setup
    {{c}} {{cflags}} -c {{c_sources}} -o {{output}}/sysinfo.o
    {{cxx}} {{cxxflags}} -c Monitor.cpp -o {{output}}/Monitor.o
    {{cxx}} {{cxxflags}} -c main.cpp -o {{output}}/main.o
    {{cxx}} {{cxxflags}} -c ui/Terminal.cpp -o {{output}}/Terminal.o
    {{cxx}} {{cxxflags}} -c ui/Menu.cpp -o {{output}}/Menu.o
    {{cxx}} {{cxxflags}} -c ui/Display.cpp -o {{output}}/Display.o
    {{cxx}} {{cxxflags}} -c screens/Memory.cpp -o {{output}}/Memory.o
    {{cxx}} {{cxxflags}} -c screens/Package.cpp -o {{output}}/Package.o
    {{cxx}} {{cxxflags}} -c screens/Process.cpp -o {{output}}/Process.o
    {{cxx}} {{cxxflags}} -c screens/Profile.cpp -o {{output}}/Profile.o
    {{cxx}} {{cxxflags}} -c screens/Runner.cpp -o {{output}}/Runner.o
    {{cxx}} {{cxxflags}} -c screens/System.cpp -o {{output}}/System.o
    {{cxx}} {{cxxflags}} -c screens/Timezone.cpp -o {{output}}/Timezone.o
    {{cxx}} {{cxxflags}} -c screens/CPU.cpp -o {{output}}/CPU.o
    {{cxx}} {{cxxflags}} -c screens/Kernel.cpp -o {{output}}/Kernel.o
    {{cxx}} {{cxxflags}} -c screens/Mirrors.cpp -o {{output}}/Mirrors.o
    {{cxx}} {{cxxflags}} -c screens/Hostname.cpp -o {{output}}/Hostname.o
    {{cxx}} {{cxxflags}} -c screens/Network.cpp -o {{output}}/Network.o

build: setup compile
    ar rcs {{output}}/{{lib}} {{c_objs}}
    @echo "Static library created: {{output}}/{{lib}}"
    {{cxx}} {{cxxflags}} -o {{output}}/{{target}} {{cxx_objs}} -L{{output}} -lsysinfo -pthread
    @echo "Binary created: {{output}}/{{target}}"

run:
    {{output}}/{{target}}

clean:
    rm -rf {{output}}
    @echo "Cleaned build artifacts"