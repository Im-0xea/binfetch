# Binfetch

A neofetch inspired fetching utility for executables.

![image](https://socki.moe/binfetch.png "img")

### Packages

- gentoo [my ebuild repo](https://github.com/Im-0xea/xea-ebuilds)
- arch [AUR](https://aur.archlinux.org/packages/binfetch-git) [my pkgbuild repo](https://github.com/Im-0xea/xea-pkgbuilds)

### Build

##### Depends on:

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

##### Colors

If you are in a graphical virtual terminal and you are getting rough colors, you might be using a termial that simiar to mine does not set "COLORTERM" nor have a termcap entry with color256 at the end, to fix this for binfetch simply add the following to any config which sets envvars for your shell, such as .{ash,bash,zsh}rc.
~~~
[ "$TERM" = "<name of term>" ] && export COLORTERM=""
~~~
