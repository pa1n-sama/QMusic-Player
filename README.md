# Music Player Qt Application

This is a simple music player application built using Qt. The application allows users to browse folders, select music files (such as `.mp3` and `.m4a`), and play them with functionalities like pause, mute, and next/previous song controls. The UI dynamically updates as you navigate through your file system and switch between different layers.

## Features

- Browse folders and list songs
- Play, pause, and skip songs
- Volume control and mute option
- Shuffle and repeat modes
- Displays song metadata and album art (with default image if not available)

## Requirements

- **Qt**: 6.6 or later
- **qmake** or **CMake** (depending on your build system)
- **Audio output** capability (like speakers or headphones)
- The project supports `mp3` and `m4a` files.

## Installation

1. **Clone the Repository:**
   ```bash
   
   git clone https://github.com/your-username/music-player.git
   cd music-player
   mkdir build
   cd build
   qmake ../Music-Player.pro -o ./build
   make
