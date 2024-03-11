***********************************************************************************************************************

Naval Observatory Vector Astronomy Software (NOVAS)

C Edition 

Version C3.1 - March 2011

************************************************************************************************************************

Fork from: https://github.com/indigo-astronomy/novas

Adapted and refactorized to be used with C++ modern compilers and CMake. 

This fork has been created in order to facilitate the integration of NOVAS into the modern DegorasSLR (DPSLR) library for calculations related to Satellite Laser Ranging. This library, based on C++17, is used to control the SFEL SLR station of the Royal Institute and Observatory of the Spanish Navy (ROA).

Ángel Vera Herrera - avera@roa.es - Computer Engineer - ROA.

November - 2023

************************************************************************************************************************

IMPORTANT NOTES:

- Usage.

This project is prepared to be used with CMake >= 3.21 and to be compiled with modern C++ compilers. 

The CMakeList is a little complex but it is prepared to be configured in several aspect, for example the build configuration or the install directory (read the comments in the file). Remember changue the build configuration to release.

- About compilers.

At this momment, this fork has been tested in the following platforms:

-- Windows 10 with MinGW 8.1 for 64 bits.
-- Windows 10 with MinGW 13.1 for 64 bits.
-- Windows 10 with MSVC2019 for 64 bits.
-- Ubuntu 22.04 with GCC 11.4.

- Compiling the library.

In Windows I recommend use a prepared IDE for compiling, for example VSCode or QTCreator. Then, open the project in the IDE (the CMakeList) and compile the library. Also you can deploy (install) the library.

In Linux, you can create a build directory (for example "build_debug_gcc64"). Then, call to CMake and then, "make install" to build and install the library.
  
- About solarsystem versions.

You must select in the CMakeLists the <SOLSYS_SOURCE> specific file to be used for compiling, between <solsys1.c>, <solsys2.c> and <solsys3.c> source files. 

By default <solsys3.c> is used, so only reduced-accuracy mode can be used for calculations. However, this mode is enought for a lot of engineering applications. Remember that using the reduced-accuracy mode with this version of solarsystem  for the in the computation of apparent places of stars, it should contribute less than 1.5 milliarcseconds of error (this assessment applies to the interval 1800–2050). If you want to use the full-accuracy mode, remember that you must compile the Fortran routines.

See the NOVAS_C3.1_Guide.pdf for more detailed explanation.

- About Fortran routines.

The Fortran routines must be compiled externally. I recommend use IntelFortran with Visual Studio (increases efficiency noticeably) but GFortran can also be used.

- About original examples.

The original examples has been replaced with modified C++ versions. Not all examples has been ported yet. 

- About utilities.

Some utilities has been added to the project as a more complex examples for testing.

- TODOs.

It would be interesting to create a complete wrapper of the library in C++ within a namespace, so that it is not necessary to use C syntax externally when calling functions.

************************************************************************************************************************
