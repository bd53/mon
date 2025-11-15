cc := "gcc"
cxx := "g++"
cflags := "-Wall -Wextra -O2 -fPIC"
cxxflags := "-Wall -Wextra -O2 -std=c++17 -fPIC"
target := "mon"
lib := "libsysinfo.a"
c_sources := "sysinfo.c"
cxx_sources := "Monitor.cpp main.cpp"
c_objs := "sysinfo.o"
cxx_objs := "Monitor.o main.o"
all_objs := "sysinfo.o Monitor.o main.o"

all: clean build run

build: compile
    ar rcs {{lib}} {{c_objs}}
    @echo "Static library created: {{lib}}"
    {{cxx}} {{cxxflags}} -o {{target}} {{cxx_objs}} -L. -lsysinfo -pthread
    @echo "Binary created: {{target}}"

compile:
    {{cc}} {{cflags}} -c {{c_sources}}
    {{cxx}} {{cxxflags}} -c {{cxx_sources}}

clean:
    rm -f {{all_objs}} {{target}} {{lib}}
    @echo "Cleaned build artifacts"

run:
    ./{{target}}
