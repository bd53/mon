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
./autogen.sh build
```

### Running the tool

Assuming you are on an Arch Linux system, this tool relies on `pacman` and checks paths like `/etc/pacman.conf` and `/etc/pacman.d/`.
```bash
./build/mon
```

### Running the tool using `autogen`

```bash
./autogen.sh run
```

#### Advanced

Some additional options are available using [just](https://github.com/casey/just). You can simplify the process with `just build` / `just run`.