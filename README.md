Do you hate the annoyance when you just want to play an internet radio stream and you have to either install bloatware browser plugins for playing them or hack in bloated HTML code and search for stream URLs, which could be played in your favorite player? *Yes, me too.* And that's why HeyTrack is here.

HeyTrack is simple utility which resides in tray (and optionally small window) and has two simple functions:

- display currently played track (a baloon notification pops out after song change)
- play your favourite radio stream in your favourite audio player

If you are using KDE4, there is also a (very simple) plasmoid, which displays current track. It's so simple and dummy that it cannot generate its own configuration file, but requires existing configuration file, which can be generated in the Settings dialog of the tray app. Controlling the player via plasmoid is currently impossible too.

Currently supported stream servers
----------------------------------

- ABradio.cz - http://www.abradio.cz/
- Rockmax.cz - http://www.rockmax.cz/
- SomaFM.com - http://somafm.com/

Currently supported players
---------------------------

- VLC (via DBUS)
- internal Phonon player

Installation
============

Build dependencies
------------------

- Qt >= 4.6
- QJSON >= 0.7.1
- CMake >= 2.6

Three++ easy steps
------------------

    mkdir -p build && cd build
    cmake ../build
    make
    make install
