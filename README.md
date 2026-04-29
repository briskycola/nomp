<!-- # New Old Music Player (NOMP) -->
<!-- This is the New Old Music Player :). To build -->
<!-- NOMP, refer to the following build instructions -->
<!-- depending on your platform. -->



<!-- Improved compatibility of back to top link: See: https://github.com/othneildrew/Best-README-Template/pull/73 -->
<a id="readme-top"></a>

<!-- PROJECT SHIELDS -->
[![Contributors][contributors-shield]][contributors-url]
[![Forks][forks-shield]][forks-url]
[![Stargazers][stars-shield]][stars-url]
[![Issues][issues-shield]][issues-url]
[![project_license][license-shield]][license-url]
[![LinkedIn][linkedin-shield]][linkedin-url]



<!-- PROJECT LOGO -->
<br />
<div align="center">
  <a href="https://github.com/briskycola/nomp">
    <img src="images/logo.png" alt="Logo" width="80" height="80">
  </a>

<h3 align="center">NOMP — New Old Music Player</h3>

  <p align="center">
    A terminal-based music player that brings the retro feel of 60s–80s terminal programs into the modern age.
    <br />
    <a href="https://github.com/briskycola/nomp"><strong>Explore the docs »</strong></a>
    <br />
    <br />
    <a href="https://github.com/briskycola/nomp">View Demo</a>
    &middot;
    <a href="https://github.com/briskycola/nomp/issues/new?labels=bug&template=bug-report---.md">Report Bug</a>
    &middot;
    <a href="https://github.com/briskycola/nomp/issues/new?labels=enhancement&template=feature-request---.md">Request Feature</a>
  </p>
</div>



<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li><a href="#usage">Usage</a></li>
    <li><a href="#roadmap">Roadmap</a></li>
    <li><a href="#contributing">Contributing</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#contact">Contact</a></li>
    <li><a href="#acknowledgments">Acknowledgments</a></li>
  </ol>
</details>



<!-- ABOUT THE PROJECT -->
## About The Project

[![Product Name Screen Shot][product-screenshot]](https://github.com/briskycola/nomp)

NOMP (New Old Music Player) is a terminal-based music player written in C++, designed to run in UNIX terminals. It emulates the retro aesthetic of terminal programs from the 60s–80s while incorporating modern features like MIDI playback and SoundFont support.

Whether you're a fan of terminal-based environments, running a low-resource system, or just want a music player that runs on your toaster — NOMP has you covered.

<p align="right">(<a href="#readme-top">back to top</a>)</p>



### Built With

* [![C++][CPP-badge]][CPP-url]
* [![libmpv][MPV-badge]][MPV-url]
* [![FluidSynth][FluidSynth-badge]][FluidSynth-url]

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- GETTING STARTED -->
## Getting Started

To get a local copy up and running, follow the steps below depending on your platform.

- [Linux](docs/BUILD_LINUX.md)
- [macOS](docs/BUILD_MACOS.md)
- [Windows](docs/BUILD_WINDOWS.md)

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- USAGE EXAMPLES -->
## Usage

Place your audio files (`.mp3`, `.wav`, `.flac`, `.aac`, `.ogg`) and MIDI files (`.mid`, `.midi`) along with any SoundFonts (`.sf2`, `.sf3`) into the `NompSongs` folder located in the project root directory. NOMP will automatically detect and load them on startup.

```
nomp/
└── NompSongs/
    ├── song.mp3
    ├── beethoven.mid
    └── instruments.sf2
```

_For more details, refer to the [Documentation](https://github.com/briskycola/nomp)_

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- ROADMAP -->
## Roadmap

- [ ] Terminal UI (TUI)
- [ ] Standard audio playback (MP3, WAV, FLAC) via libmpv
- [ ] MIDI playback via libfluidsynth
- [ ] SoundFont support for custom MIDI instruments
- [ ] Audio filters
- [ ] Future features

See the [open issues](https://github.com/briskycola/nomp/issues) for a full list of proposed features and known issues.

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- CONTRIBUTING -->
## Contributing

Contributions are what make the open source community such an amazing place to learn, inspire, and create. Any contributions you make are **greatly appreciated**.

If you have a suggestion that would make this better, please fork the repo and create a pull request. You can also simply open an issue with the tag "enhancement".
Don't forget to give the project a star! Thanks again!

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

<p align="right">(<a href="#readme-top">back to top</a>)</p>

### Top contributors:

<a href="https://github.com/briskycola/nomp/graphs/contributors">
  <img src="https://contrib.rocks/image?repo=briskycola/nomp" alt="contrib.rocks image" />
</a>



<!-- LICENSE -->
## License

Distributed under the MIT License. See `LICENSE.txt` for more information.

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- CONTACT -->
## Contact

Project Link: [https://github.com/briskycola/nomp](https://github.com/briskycola/nomp)

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- ACKNOWLEDGMENTS -->
## Acknowledgments

* [libmpv](https://mpv.io/) — audio playback engine
* [libfluidsynth](https://www.fluidsynth.org/) — MIDI synthesizer
* [Best-README-Template](https://github.com/othneildrew/Best-README-Template)

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- MARKDOWN LINKS & IMAGES -->
[contributors-shield]: https://img.shields.io/github/contributors/briskycola/nomp.svg?style=for-the-badge
[contributors-url]: https://github.com/briskycola/nomp/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/briskycola/nomp.svg?style=for-the-badge
[forks-url]: https://github.com/briskycola/nomp/network/members
[stars-shield]: https://img.shields.io/github/stars/briskycola/nomp.svg?style=for-the-badge
[stars-url]: https://github.com/briskycola/nomp/stargazers
[issues-shield]: https://img.shields.io/github/issues/briskycola/nomp.svg?style=for-the-badge
[issues-url]: https://github.com/briskycola/nomp/issues
[license-shield]: https://img.shields.io/github/license/briskycola/nomp.svg?style=for-the-badge
[license-url]: https://github.com/briskycola/nomp/blob/master/LICENSE.txt
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=for-the-badge&logo=linkedin&colorB=555
[linkedin-url]: https://linkedin.com/in/linkedin_username
[product-screenshot]: images/screenshot.png
[CPP-badge]: https://img.shields.io/badge/C++-00599C?style=for-the-badge&logo=cplusplus&logoColor=white
[CPP-url]: https://isocpp.org/
[MPV-badge]: https://img.shields.io/badge/libmpv-672168?style=for-the-badge&logo=mpv&logoColor=white
[MPV-url]: https://mpv.io/
[FluidSynth-badge]: https://img.shields.io/badge/FluidSynth-000000?style=for-the-badge&logoColor=white
[FluidSynth-url]: https://www.fluidsynth.org/






<!-- - [Linux](docs/BUILD_LINUX.md) -->
<!-- - [macOS](docs/BUILD_MACOS.md) -->
<!-- - [Windows](docs/BUILD_WINDOWS.md) -->
