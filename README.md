# iLCUtil

ILCUTIL is a utility package for the iLCSoft software framework. It is intended to be a "meta-package" which packages together a set of independent utility packages living in separate sub-directories.

iLCUtil is distributed under the [GPLv3 License](http://www.gnu.org/licenses/gpl-3.0.en.html)

[![License](https://www.gnu.org/graphics/gplv3-127x51.png)](https://www.gnu.org/licenses/gpl-3.0.en.html)

## Usage

It should be possible to configure, build and use all of the sub-packages independently of ILCUTIL.

Currently ILCUTIL contains the following sub-packages:

ILCSOFT_CMAKE_MODULES (Previously known as CMakeModules) This package contains a set of cmake modules and utility macros. Almost all iLCSoft packages depend on this sub-package for the cmake configure/build step.

There are 3 possible ways of using the ILCSOFT_CMAKE_MODULES:

1. (Preferred ) Using the ILCUTIL package, i.e.:
```
FIND_PACKAGE( ILCUTIL COMPONENTS ILCSOFT_CMAKE_MODULES )

ILCUTIL_DIR or CMAKE_PREFIX_PATH needs to be set to find the configuration file ILCUTILConfig.cmake.

FIND_PACKAGE( ILCUTIL COMPONENTS ILCSOFT_CMAKE_MODULES streamlog )
```


2. Directly using the ILCSOFT_CMAKE_MODULES package, e.g.:
```
FIND_PACKAGE( ILCSOFT_CMAKE_MODULES )

ILCSOFT_CMAKE_MODULES_DIR or CMAKE_PREFIX_PATH need to be
set in order to find ILCSOFT_CMAKE_MODULESConfig.cmake.
```
This method should be used in packages living inside ILCUTIL,
e.g. ILCTEST

3. By setting CMAKE_MODULE_PATH directly to where the cmake modules and macros live.

This method should be used with care or for debugging purposes.

### ILCTEST

This package provides utilities to help developers write new C++ tests in their packages. It consists of a C++ utility header and a set of cmake macros (living inside ILCSOFT_CMAKE_MODULES) to facilitate adding new tests in a straight-forward manner.

### streamlog
This package provides a set of logging classes for C++ applications.

### example
This example shows how to create a new package in ILCUTIL which depends on other sub-packages from ILCUTIL



## License and Copyright
Copyright (C), iLCUtil Authors

iLCUtil is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License long with this program.  If not, see <http://www.gnu.org/licenses/>.
