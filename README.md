# mon

Very wip, basically a tech demo.

[test2.webm](https://github.com/user-attachments/assets/98f004ee-a697-453a-a57b-c487bcb731b9)

[![](https://img.shields.io/badge/License-MIT-blue?logo=opensource)](./LICENSE)
[![](https://img.shields.io/github/contributors/bd53/mon?logo=github)](https://github.com/bd53/mon/graphs/contributors)
[![](https://img.shields.io/github/last-commit/bd53/mon?logo=github)](https://github.com/bd53/mon/commits/main)

For additional legal notices, refer to [NOTICE.md](./NOTICE.md).

## Installation & Usage

```bash
wget https://github.com/bd53/mon/archive/refs/heads/main.zip -O mon.zip
```

Alternative ways to install are `git clone` the repository.

### Build

```bash
# Create build directory
mkdir -p build

# Compile C source
gcc -Wall -Wextra -O2 -fPIC -c sysinfo.c -o build/sysinfo.o

# Compile C++ sources
g++ -Wall -Wextra -O2 -std=c++17 -fPIC -c Monitor.cpp -o build/Monitor.o
g++ -Wall -Wextra -O2 -std=c++17 -fPIC -c main.cpp -o build/main.o
g++ -Wall -Wextra -O2 -std=c++17 -fPIC -c ui/Terminal.cpp -o build/Terminal.o
g++ -Wall -Wextra -O2 -std=c++17 -fPIC -c ui/Menu.cpp -o build/Menu.o
g++ -Wall -Wextra -O2 -std=c++17 -fPIC -c ui/Display.cpp -o build/Display.o
g++ -Wall -Wextra -O2 -std=c++17 -fPIC -c screens/Memory.cpp -o build/Memory.o
g++ -Wall -Wextra -O2 -std=c++17 -fPIC -c screens/Package.cpp -o build/Package.o
g++ -Wall -Wextra -O2 -std=c++17 -fPIC -c screens/Process.cpp -o build/Process.o
g++ -Wall -Wextra -O2 -std=c++17 -fPIC -c screens/Profile.cpp -o build/Profile.o
g++ -Wall -Wextra -O2 -std=c++17 -fPIC -c screens/Runner.cpp -o build/Runner.o
g++ -Wall -Wextra -O2 -std=c++17 -fPIC -c screens/System.cpp -o build/System.o
g++ -Wall -Wextra -O2 -std=c++17 -fPIC -c screens/Timezone.cpp -o build/Timezone.o
g++ -Wall -Wextra -O2 -std=c++17 -fPIC -c screens/CPU.cpp -o build/CPU.o
g++ -Wall -Wextra -O2 -std=c++17 -fPIC -c screens/Kernel.cpp -o build/Kernel.o

# Create static library
ar rcs build/libsysinfo.a build/sysinfo.o

# Link final binary
g++ -Wall -Wextra -O2 -std=c++17 -fPIC \
  -o build/mon \
  build/Monitor.o build/main.o build/Terminal.o build/Menu.o build/Display.o \
  build/Memory.o build/Package.o build/Process.o build/Profile.o build/Runner.o \
  build/System.o build/Timezone.o build/CPU.o build/Kernel.o \
  -Lbuild -lsysinfo -pthread
```

### Running the tool

Assuming you are on an Arch Linux system, this will work; if not, it won't, as it fetches packages via `pacman`.
```bash
./build/mon
```

#### Advanced

Some additional options are available using [just](https://github.com/casey/just). You can simplify the process with `just build` / `just run`.