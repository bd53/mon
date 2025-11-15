[test.webm](https://github.com/user-attachments/assets/62276890-6ed0-4138-84d0-a15791bd55b5)

For additional legal notices, refer to [NOTICE.md](./NOTICE.md).

## Installation & Usage

```bash
wget https://github.com/bd53/mon/archive/refs/heads/main.zip -O mon.zip
```

Alternative ways to install are `git clone` the repository.

### Build

```bash
gcc -Wall -Wextra -O2 -fPIC -c sysinfo.c
g++ -Wall -Wextra -O2 -std=c++17 -fPIC -c Monitor.cpp main.cpp
ar rcs libsysinfo.a sysinfo.o
g++ -Wall -Wextra -O2 -std=c++17 -fPIC -o mon Monitor.o main.o -L. -lsysinfo -pthread
```

### Running the tool

```bash
./mon
```

#### Advanced

Some additional options are available using [Just](https://github.com/casey/just). You can build and run with `just build` and `just run`.
