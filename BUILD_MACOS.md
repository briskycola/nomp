# Building on macOS
To build the New Old Music Player on macOS, you will need to
first install the Xcode Command Line tools. To do that,
run the following command:
```bash
xcode-select --install
```

Once the tools are installed, you will then need to install
the Homebrew package manager. Refer [here](https://brew.sh/)
for instructions on installing Homebrew. Once Homebrew is
installed, you will now need to install the necessary
dependencies:
- **libmpv**
- **FFmpeg**
- **FluidSynth**

To install the necessary dependencies, run the
following commands:

## macOS
```bash
brew install mpv ffmpeg pkg-config
```

If `pkg-config` fails to detect the mpv library,
you might need to run the following:
```bash
brew link --overwrite mpv
```

Next you will need to clone the repo and
run the Makefile.
```bash
git clone https://github.com/briskycola/nomp
cd nomp
make -f Makefile.macos -j$(nproc)
```
You should now have a binary called `nomp` in
the current directory.
