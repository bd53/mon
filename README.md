# mon

Very wip, basically a tech demo.

[test.webm](https://github.com/user-attachments/assets/05915675-e43b-495c-ae64-933190386ff6)

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

Some additional options are available using [just](https://github.com/casey/just). You can build and run with `just build` and `just run`.