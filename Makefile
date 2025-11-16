C := gcc
CXX := g++
CFLAGS := -Wall -Wextra -O2 -fPIC
CXXFLAGS := -Wall -Wextra -O2 -std=c++17 -fPIC
TARGET := mon
LIB := libsysinfo.a
C_SOURCES := sysinfo.c
CXX_SOURCES := Monitor.cpp main.cpp ui/Terminal.cpp ui/Menu.cpp ui/Display.cpp screens/Memory.cpp screens/Package.cpp screens/Process.cpp screens/Profile.cpp screens/Runner.cpp screens/System.cpp screens/Timezone.cpp screens/CPU.cpp screens/Kernel.cpp screens/Mirrors.cpp screens/Hostname.cpp screens/Network.cpp
OUTPUT := build
C_OBJS := $(OUTPUT)/sysinfo.o
CXX_OBJS := $(OUTPUT)/Monitor.o $(OUTPUT)/main.o $(OUTPUT)/Terminal.o $(OUTPUT)/Menu.o $(OUTPUT)/Display.o $(OUTPUT)/Memory.o $(OUTPUT)/Package.o $(OUTPUT)/Process.o $(OUTPUT)/Runner.o $(OUTPUT)/System.o $(OUTPUT)/Profile.o $(OUTPUT)/Timezone.o $(OUTPUT)/CPU.o $(OUTPUT)/Kernel.o $(OUTPUT)/Mirrors.o $(OUTPUT)/Hostname.o $(OUTPUT)/Network.o

.PHONY: all setup compile build run clean

all: clean build

setup:
	mkdir -p $(OUTPUT)

compile: setup
	$(C) $(CFLAGS) -c $(C_SOURCES) -o $(OUTPUT)/sysinfo.o
	$(CXX) $(CXXFLAGS) -c Monitor.cpp -o $(OUTPUT)/Monitor.o
	$(CXX) $(CXXFLAGS) -c main.cpp -o $(OUTPUT)/main.o
	$(CXX) $(CXXFLAGS) -c ui/Terminal.cpp -o $(OUTPUT)/Terminal.o
	$(CXX) $(CXXFLAGS) -c ui/Menu.cpp -o $(OUTPUT)/Menu.o
	$(CXX) $(CXXFLAGS) -c ui/Display.cpp -o $(OUTPUT)/Display.o
	$(CXX) $(CXXFLAGS) -c screens/Memory.cpp -o $(OUTPUT)/Memory.o
	$(CXX) $(CXXFLAGS) -c screens/Package.cpp -o $(OUTPUT)/Package.o
	$(CXX) $(CXXFLAGS) -c screens/Process.cpp -o $(OUTPUT)/Process.o
	$(CXX) $(CXXFLAGS) -c screens/Profile.cpp -o $(OUTPUT)/Profile.o
	$(CXX) $(CXXFLAGS) -c screens/Runner.cpp -o $(OUTPUT)/Runner.o
	$(CXX) $(CXXFLAGS) -c screens/System.cpp -o $(OUTPUT)/System.o
	$(CXX) $(CXXFLAGS) -c screens/Timezone.cpp -o $(OUTPUT)/Timezone.o
	$(CXX) $(CXXFLAGS) -c screens/CPU.cpp -o $(OUTPUT)/CPU.o
	$(CXX) $(CXXFLAGS) -c screens/Kernel.cpp -o $(OUTPUT)/Kernel.o
	$(CXX) $(CXXFLAGS) -c screens/Mirrors.cpp -o $(OUTPUT)/Mirrors.o
	$(CXX) $(CXXFLAGS) -c screens/Hostname.cpp -o $(OUTPUT)/Hostname.o
	$(CXX) $(CXXFLAGS) -c screens/Network.cpp -o $(OUTPUT)/Network.o

build: setup compile
	ar rcs $(OUTPUT)/$(LIB) $(C_OBJS)
	@echo "Static library created: $(OUTPUT)/$(LIB)"
	$(CXX) $(CXXFLAGS) -o $(OUTPUT)/$(TARGET) $(CXX_OBJS) -L$(OUTPUT) -lsysinfo -pthread
	@echo "Binary created: $(OUTPUT)/$(TARGET)"

run:
	$(OUTPUT)/$(TARGET)

clean:
	rm -rf $(OUTPUT)
	@echo "Cleaned build artifacts"