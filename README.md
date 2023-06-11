# Binfetch

A neofetch inspired fetching utility for executables, written in [ib](https://github.com/Im-0xea/ibranching) C

![image](https://socki.moe/binfetch.png "img")

### Packages

- gentoo [my ebuild repo](https://github.com/Im-0xea/xea-ebuilds)
- nix [this repo is a flake](https://github.com/Im-0xea/binfetch)
- arch [AUR](https://aur.archlinux.org/packages/binfetch-git) [my pkgbuild repo](https://github.com/Im-0xea/xea-pkgbuilds)

### Build

#### Depends on:

- libc (manic laughter)
- zlib
- [ib](https://github.com/Im-0xea/ibranching)

if you are too lazy to install ib run 'make bootstrap' instead of 'make'

~~~
$ make (bootstrap)
~~~
~~~
$ (root) make install
~~~

### Configuration

by default the config is stored and read from /usr/share/binfetch/binfetch.cfg

if you want to set a custom config or use one of the other provided schemes simply copy them to .config/binfetch/binfetch.cfg
