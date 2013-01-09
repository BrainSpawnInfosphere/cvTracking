# cvBlob

**Maintainer:** Kevin Walchko

**License:** LGPL

cvBlob is written for OpenCV 2.x library which uses modern C++ concepts. 

cvBlob used parts and ideas from Cristóbal Carnero Liñán's (grendel.ccl@gmail.com) 
[cvBlob](http://code.google.com/p/cvblob/). 

---------------------------------------------------------------------------------------

## To Do

* [done] Change all `structs` to `classes`
* [done] Change `IplImage` to `Mat`
* Optimize code 
* Leverage OpenCV 2.x API, remove unnecessary old C code
* Document code for doxygen output
* Build and install documentation to share/docs
* Make better name than cvBlob so as not to add more confusion with all of the cvBlob libraries

## Source Code

The git repository is located at [cvBlob](http://github.com/walcko/cvBlob). The code can be checked out:

	git clone http://github.com/walcko/cvBlob

## Compiling and Installation

### Linux

You will need to install openvcv and cmake using whatever package manager (e.g., apt-get,
pacman) your linux distribution uses. Once you have unpacked the source in $CVBLOB, enter 
in a console:

	cd $CVBLOB
	mkdir build
	cd build
	cmake ..
	make
	make install

### Apple

Use the [homebrew](http://mxcl.github.com/homebrew/) formula to install or follow the 
linux example.

    brew update
    brew install $CVBLOB/homebrew/cvblob.rb

### Useful CMake Options

##### OpenCV Path

You can tell CMake where OpenCV is installed:

	cmake .. -DOpenCV_DIR=<path_to_OpenCV>

#### Install Path

To change the destination path for the installation, set "CMAKE_INSTALL_PREFIX" variable:

	cmake .. -DCMAKE_INSTALL_PREFIX=<installation_path>

## Documentation

To build the documentation using [Doxygen](), just type:

    make doc

The documentation will get built in the doc folder in your build location. Just click
on the index.html.

## Example Usage

blah ...
