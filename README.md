# mon

Very wip, basically a tech demo.

[test5.webm](https://github.com/user-attachments/assets/888cc17e-aa9b-4caa-ac7d-4dee5a6d671c)

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
./build.sh
```

### Running the tool

Assuming you are on an Arch Linux system, this tool relies on `pacman` and checks paths like `/etc/pacman.d/`.
```bash
./build
```

### Running the tool using `python`

```bash
python run.py
```

#### Advanced

Some additional options are available using [make](https://www.gnu.org/software/make) and [just](https://github.com/casey/just).