
Debian
====================
This directory contains files used to package taustraliacashd/taustraliacash-qt
for Debian-based Linux systems. If you compile taustraliacashd/taustraliacash-qt yourself, there are some useful files here.

## taustraliacash: URI support ##


taustraliacash-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install taustraliacash-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your taustraliacash-qt binary to `/usr/bin`
and the `../../share/pixmaps/taustraliacash128.png` to `/usr/share/pixmaps`

taustraliacash-qt.protocol (KDE)

