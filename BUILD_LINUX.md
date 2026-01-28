# Building on Linux
To build the New Old Music Player on Linux, you will need a C++
compiler that supports C++14 (GCC 5 or Clang 3.4). As
long as you don't have something super ancient, you'll
be fine. Additionally, you will need the following
dependencies installed:
- **gcc**
- **libmpv**
- **FFmpeg**
- **FluidSynth**

To install the necessary dependencies, run the
following command that corresponds to your
Linux distribution:

## Arch Linux
```bash
sudo pacman -S base-devel mpv ffmpeg fluidsynth
```

## Debian/Ubuntu/Linux Mint
```bash
sudo apt install build-essential libmpv-dev libavcodec-dev libavformat-dev libavutil-dev libfluidsynth-dev
```

## Fedora/RHEL
```bash
sudo dnf install @development-tools mpv-devel ffmpeg-devel fluidsynth-devel
```

## openSUSE
```bash
sudo zypper install pattern devel_basis mpv-devel ffmpeg-devel fluidsynth-devel
```

## Alpine Linux
```bash
doas apk add build-base mpv-dev ffmpeg-dev fluidsynth-dev
```

## Void Linux
```bash
sudo xbps-install base-devel mpv-devel ffmpeg-devel fluidsynth-devel
```

Next you will need to clone the repo and
run the Makefile.
```bash
git clone https://github.com/briskycola/nomp
cd nomp
make -f Makefile.linux -j$(nproc)
```
You should now have a binary called `nomp` in
the current directory.
