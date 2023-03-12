# Binfetch

A neofetch inspired fetching utility for executables, written in [ib](https://github.com/Nik-Nothing/ibranching) C

![image](https://socki.moe/binfetch.png "img")

### Packages

- gentoo https://github.com/Nik-Nothing/niki-gentoo

- nix this flake

### Build

#### Depends on:

- zlib
- [ib](https://github.com/Nik-Nothing/ibranching)

if you are too lazy to install ib run 'make bootstrap' instead of 'make'

~~~
$ make CONFIG=<config folder of choice, I suggest $HOME/.config/binfetch> 
~~~
~~~
$ (root) make install CONFIG=<config>
~~~

### Configuration

by default the config is stored and read from /usr/share/binfetch/binfetch.cfg

if you want to set a custom config or use one of the other provided schemes simply copy them to .config/binfetch/binfetch.cfg
