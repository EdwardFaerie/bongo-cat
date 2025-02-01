
# Bongo Cat

Bongo Cat is a simple and easy to configure software to use in streaming!

How to Use: To change any possible configuration theres an file called "config.json" this file has everything you need to change from configurations. About custom images, just change the images inside each folder of "img" folder.

Features:

- Simple images to change!
- Easy to set up!
- Gamer mode with lots of customizations!
- More to come!

#

Bongo Cat an overlay with smooth paw movement and simple skinning ability, written in C++. Originally created by [HamishDuncanson](https://github.com/HamishDuncanson).

This bongo cat software is originally from [here](https://github.com/kuroni/bongocat-osu).

Download the "Ready to go" software [here on Itch.Io](https://edwardfaerie.itch.io/bongo-cat) please to make this program be updated frequently consider donating or just download from here in [Github Releases](https://github.com/EdwardFaerie/bongo-cat/releases)!

## Further information
In order to play with fullscreen on Windows 10, run both bongo cat! and this application in Windows 7 compability mode.

Press Ctrl + R to reload configuration and images (will only reload configurations when the window is focused).

Supported operating system:
* Windows
* Linux (tested with Arch Linux with WINE Staging 5). Note: You **must** use WINE Staging, because for whatever reason on stable WINE bongocat-osu doesn't register keyboard input from other windows.

_Notice_: If you're using WINE on Linux, make sure that osu! and this application run in the same `WINEPREFIX`.

## For developers
This project uses [SFML](https://www.sfml-dev.org/index.php) and [JsonCpp](https://github.com/open-source-parsers/jsoncpp). JsonCpp libraries are directly included in the source using the provided `amalgamation.py` from the developers.

### Libraries and dependency

#### Windows and MinGW
To build the source, download the SFML libraries [here](https://www.sfml-dev.org/index.php), edit file `Makefile.windows` then replace *`<SFML-folder>`* with your **SFML 2.5.1** folder.

#### Linux (NOT TESTED FOR NOW!)
You need to have these dependencies installed. Check with your package manager for the exact name of these dependencies on your distro:
- g++
- libxdo
- sdl2
- sfml
- x11
- xrandr

### Building and testing
To build, run this command from the base directory:

```sh
make
```

To test the program, run this from the base directory:

```sh
make test
```

Alternatively, you can copy the newly-compiled `bin/bongo.exe` or `bin/bongo` into the base directory and execute it.