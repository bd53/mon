c := "gcc"
cflags := "-Wall -Wextra -O2 -Iinclude"
ldflags := "-lncurses"
target := "build"
c_sources := "main.c src/menu/menu.c src/menu/menu_items.c src/system/system_info.c src/system/hardware.c src/system/software.c src/ui/screen.c src/ui/display.c utils/command.c utils/string_utils.c"
c_objs := "main.o src/menu/menu.o src/menu/menu_items.o src/system/system_info.o src/system/hardware.o src/system/software.o src/ui/screen.o src/ui/display.o utils/command.o utils/string_utils.o"

default: clean build

all: clean build

compile:
    {{c}} {{cflags}} -c main.c -o main.o
    {{c}} {{cflags}} -c src/menu/menu.c -o src/menu/menu.o
    {{c}} {{cflags}} -c src/menu/menu_items.c -o src/menu/menu_items.o
    {{c}} {{cflags}} -c src/system/system_info.c -o src/system/system_info.o
    {{c}} {{cflags}} -c src/system/hardware.c -o src/system/hardware.o
    {{c}} {{cflags}} -c src/system/software.c -o src/system/software.o
    {{c}} {{cflags}} -c src/ui/screen.c -o src/ui/screen.o
    {{c}} {{cflags}} -c src/ui/display.c -o src/ui/display.o
    {{c}} {{cflags}} -c utils/command.c -o utils/command.o
    {{c}} {{cflags}} -c utils/string_utils.c -o utils/string_utils.o

build: compile
    {{c}} {{cflags}} -o {{target}} {{c_objs}} {{ldflags}}

run:
    ./{{target}}

clean:
    rm -f {{target}} {{c_objs}}