cc := "gcc"
cxx := "g++"
cflags := "-Wall -Wextra -O2 -fPIC"
cxxflags := "-Wall -Wextra -O2 -std=c++17 -fPIC"
target := "mon"
lib := "libsysinfo.a"
c_sources := "sysinfo.c"
cxx_sources := "Monitor.cpp main.cpp ui/Terminal.cpp ui/Menu.cpp ui/Display.cpp screens/Memory.cpp screens/Package.cpp screens/Process.cpp screens/Profile.cpp screens/Runner.cpp screens/System.cpp screens/Timezone.cpp screens/CPU.cpp screens/Kernel.cpp"
builddir := "build"
c_objs := builddir + "/sysinfo.o"
cxx_objs := builddir + "/Monitor.o " + builddir + "/main.o " + builddir + "/Terminal.o " + builddir + "/Menu.o " + builddir + "/Display.o " + builddir + "/Memory.o " + builddir + "/Package.o " + builddir + "/Process.o " + builddir + "/Runner.o " + builddir + "/System.o " + builddir + "/Profile.o " + builddir + "/Timezone.o " + builddir + "/CPU.o " + builddir + "/Kernel.o"

all: clean build run

build: setup compile
    ar rcs {{builddir}}/{{lib}} {{c_objs}}
    @echo "Static library created: {{builddir}}/{{lib}}"
    {{cxx}} {{cxxflags}} -o {{builddir}}/{{target}} {{cxx_objs}} -L{{builddir}} -lsysinfo -pthread
    @echo "Binary created: {{builddir}}/{{target}}"

setup:
    mkdir -p {{builddir}}

compile: setup
    {{cc}} {{cflags}} -c {{c_sources}} -o {{builddir}}/sysinfo.o
    {{cxx}} {{cxxflags}} -c Monitor.cpp -o {{builddir}}/Monitor.o
    {{cxx}} {{cxxflags}} -c main.cpp -o {{builddir}}/main.o
    {{cxx}} {{cxxflags}} -c ui/Terminal.cpp -o {{builddir}}/Terminal.o
    {{cxx}} {{cxxflags}} -c ui/Menu.cpp -o {{builddir}}/Menu.o
    {{cxx}} {{cxxflags}} -c ui/Display.cpp -o {{builddir}}/Display.o
    {{cxx}} {{cxxflags}} -c screens/Memory.cpp -o {{builddir}}/Memory.o
    {{cxx}} {{cxxflags}} -c screens/Package.cpp -o {{builddir}}/Package.o
    {{cxx}} {{cxxflags}} -c screens/Process.cpp -o {{builddir}}/Process.o
    {{cxx}} {{cxxflags}} -c screens/Profile.cpp -o {{builddir}}/Profile.o
    {{cxx}} {{cxxflags}} -c screens/Runner.cpp -o {{builddir}}/Runner.o
    {{cxx}} {{cxxflags}} -c screens/System.cpp -o {{builddir}}/System.o
    {{cxx}} {{cxxflags}} -c screens/Timezone.cpp -o {{builddir}}/Timezone.o
    {{cxx}} {{cxxflags}} -c screens/CPU.cpp -o {{builddir}}/CPU.o
    {{cxx}} {{cxxflags}} -c screens/Kernel.cpp -o {{builddir}}/Kernel.o

clean:
    rm -rf {{builddir}}
    @echo "Cleaned build artifacts"

run:
    {{builddir}}/{{target}}