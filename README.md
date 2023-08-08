# Binfetch

A neofetch inspired fetching utility for executables.

![image](https://socki.moe/binfetch.png "img")

### Packages

- gentoo [my ebuild repo](https://github.com/Im-0xea/xea-ebuilds)
- arch [AUR](https://aur.archlinux.org/packages/binfetch-git) [my pkgbuild repo](https://github.com/Im-0xea/xea-pkgbuilds)

### Build

#### Depends on:

- libc
- libelf 
- zlib

~~~
$ meson setup build
$ ninja -C build
$ ninja -C build install
~~~

### Configuration

~~~
# - ~/.config/binfetch/binfetch.cfg

# Array of colors for data labels
[label_colors]
    #F5A9B8

# Array of colors for flag
[art_colors]
    #5BCEFA
    #F5A9B8
    #FFFFFF
    #F5A9B8
    #5BCEFA

# Order and whitelist for data labels
[order]
    Name
    Header
    Type
    Version
    Arch
    Class
    Endian
    OS ABI
    ABI Version
    Entry
    Table
    Interpreter
    Size
    Arches
    Binary count

# Coloring pattern for flag
[pattern]
    value

# Text attributes
[attributes]
    bold
~~~
