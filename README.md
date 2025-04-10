# Learning OpenGL

## Usage

Building Project
-------------

To check out project together with all dependencies, be sure to use the
`--recursive` flag when cloning the repository, i.e.
```bash
$ git clone --recursive https://github.com/mmp/pbrt-v3/
```
If you accidentally already cloned pbrt without this flag (or to update an
project source tree after a new submodule has been added, run the following
command to also fetch the dependencies:
```bash
$ git submodule update --init --recursive
```

This project uses [cmake](http://www.cmake.org/) for its build system.  On Linux
and OS X, cmake is available via most package management systems.  To get
cmake for Windows, or to build it from source, see the [cmake downloads
page](http://www.cmake.org/download/).  Once you have cmake, the next step
depends on your operating system.
