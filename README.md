# uni-update

[![CI](https://github.com/user14923929/uni-update/actions/workflows/ci.yml/badge.svg)](https://github.com/user14923929/uni-update/actions/workflows/ci.yml)
[![AUR version](https://img.shields.io/aur/version/uni-update)](https://aur.archlinux.org/packages/uni-update)

Cross-distro system updater — one command to update them all.

```
╔══════════════════════════════╗
║       uni-update  v1.0.0     ║
║  cross-distro system updater ║
╚══════════════════════════════╝
```

## Supported distros

| Distro family             | Package manager          |
|---------------------------|--------------------------|
| Arch / CachyOS / Manjaro  | `pacman -Syu`            |
| Debian / Ubuntu           | `apt-get update/upgrade` |
| Fedora / RHEL             | `dnf upgrade`            |
| openSUSE                  | `zypper up`              |
| Void Linux                | `xbps-install -Su`       |
| Alpine                    | `apk upgrade`            |
| NixOS                     | `nixos-rebuild switch`   |
| Gentoo                    | `emerge --sync`          |

## Install

### AUR (Arch / CachyOS / Manjaro)

```bash
yay -S uni-update
# or
paru -S uni-update
```

### Build from source

```bash
git clone https://github.com/user14923929/uni-update
cd uni-update
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
sudo cmake --install build
```

**Dependencies:** `cmake`, `g++` (C++17)

## Usage

```
uni-update [options]

Options:
  -h, --help         Show this help
  -v, --version      Show version
  -y, --no-confirm   Pass -y / --noconfirm to package manager
  -n, --dry-run      Show what would be done, but don't run
      --all          Also update flatpak, snap, pip, cargo, npm
      --flatpak      Update Flatpak packages
      --snap         Update Snap packages
      --pip          Update pip user packages
      --cargo        Update cargo global installs
      --npm          Update npm global packages
```

### Examples

```bash
# Обычное обновление системы
uni-update

# Обновить всё включая Flatpak и pip
uni-update --all

# Посмотреть что будет без запуска
uni-update --dry-run --all

# Без интерактивных вопросов (для скриптов)
uni-update -y
```

## License

MIT
