# Elgato Lights Manager for Linux

A lightweight alternative to Elgato Control Center (Windows-only) for Linux.

Built with *QML*, so it has a minimal footprint compared to Electron/WebView projects.

---

## TODO

- Add support for more light types (e.g., RGB)
- Build binaries for each important distro's package manager

---

## Dependencies

You need **Qt6** and **Avahi** libraries installed on your system.

### Ubuntu / Debian
```bash
sudo apt update
sudo apt install -y qt6-base-dev qt6-declarative-dev libavahi-client-dev libavahi-common-dev
```

### Arch Linux / Manjaro
```bash
sudo pacman -Syu
sudo pacman -S qt6-base qt6-declarative avahi
```

---

## Build & Run
```bash
git clone https://github.com/maduranma/elgato-lights-manager-linux.git
cd elgato-lights-manager-linux
mkdir build && cd build
echo "*" > .gitignore
cmake ..
make -j$(nproc)
./elgato_lights_manager_linux
```

---

## Installation

Currently, no prebuilt binaries or packages are available.

You can build it yourself as shown above.

Future plans include adding an AUR package, and a couple more package managers.