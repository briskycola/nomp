# Building on Windows
To build the New Old Music Player on Windows, you will need to
install MSYS2 to get a Unix-like environment on Windows. To
install MSYS2, refer to the instructions [here](https://www.msys2.org/).
Once you install MSYS2, you will need to use the `MSYS MINGW64` shell.
This will be the main shell where you compile NOMP. Finally,
you will need the following dependencies installed:
- **gcc**
- **libmpv**
- **FFmpeg**
- **FluidSynth**

To install the necessary dependencies, run the
following command in the MSYS MINGW64 shell:

## MSYS MINGW64
```bash
pacman -S base-devel mingw-w64-x86_64-toolchain mingw-w64-x86_64-mpv mingw-w64-x86_64-fluidsynth
```

Next you will need to clone the repo and
run the Makefile.
```bash
git clone https://github.com/briskycola/nomp
cd nomp
make -f Makefile.windows -j$(nproc)
```
You should now have a binary called `nomp` in
the current directory.
