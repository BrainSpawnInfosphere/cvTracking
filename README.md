# cvTracking Library

**Maintainer:** Kevin Walchko

**License:** LGPL

cvTracking is written for OpenCV 2.x library which uses modern C++ concepts. 

---------------------------------------------------------------------------------------

## To Do

* Optimize code 
* Leverage OpenCV 2.x API, remove unnecessary old C code
* Document code for doxygen output
* Build and install documentation to share/docs

## Source Code

The git repository is located at [cvTracking](http://github.com/walcko/cvTracking). The code can be checked out:

	git clone http://github.com/walcko/cvTracking

## Compiling and Installation

### Linux

You will need to install openvcv and cmake using whatever package manager (e.g., apt-get,
pacman) your linux distribution uses. Once you have unpacked the source in $CVT, enter 
in a console:

	cd $CVT
	mkdir build
	cd build
	cmake ..
	make
	make install

### Apple

Use the [homebrew](http://mxcl.github.com/homebrew/) formula to install or follow the 
linux example.

    brew update
    brew install $CVT/homebrew/cvtracking.rb

### Useful CMake Options

##### OpenCV Path

You can tell CMake where OpenCV is installed:

	cmake .. -DOpenCV_DIR=<path_to_OpenCV>

#### Install Path

To change the destination path for the installation, set "CMAKE_INSTALL_PREFIX" variable:

	cmake .. -DCMAKE_INSTALL_PREFIX=<installation_path>

## Documentation

To build the documentation using [Doxygen](www.doxygen.org), just type:

    make doc

The documentation will get built in the doc folder in your build location. Just click
on the index.html.

## Example Usage

blah ...
