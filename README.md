<!-- Improved compatibility of back to top link: See: https://github.com/othneildrew/Best-README-Template/pull/73 -->
<a name="readme-top"></a>

<!-- PROJECT SHIELDS -->
[![Cplusplus][cplusplus-shield]][cplusplus-url]
[![LinkedIn][linkedin-shield]][linkedin-url]

<!-- PROJECT LOGO -->
  <h1 align="center">LibNovasCPP - Naval Observatory Vector Astronomy Software For C++ Compilers</h1>

  <p align="center">
    <br />
    A NOVAS (Naval Observatory Vector Astronomy Software) library fork adapted and refactorized to be compiled and installed easily with modern C++ compilers together with CMake. 
    <br />
    <br />
    <br />
  </p>
</div>

<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li><a href="#original-novas-project">Original NOVAS Project</a></li>
    <li><a href="#about-libnovascpp-project">About LibNOVASCpp Project</a></li>
    <li><a href="#build-and-installation">Build And Installation</a></li>
    <li><a href="#external-library-usage">External Library Usage</a></li>
    <li><a href="#important-notes">Important Notes</a></li>
    <li><a href="#todos">TODOs</a></li>
  </ol>
</details>

<!-- ORIGINAL NOVAS PROJECT -->
## Original NOVAS Project

Naval Observatory Vector Astronomy Software (NOVAS) - C Edition

Fork from: https://github.com/indigo-astronomy/novas

Version C3.1 - March 2011

NOVAS is an integrated package of ANSI C functions for computing many commonly needed quantities in positional astronomy.  The package can supply, in one or two function calls, the instantaneous coordinates of any star or solar system body in a variety of coordinate systems.  At a lower level, NOVAS also supplies astrometric utility transformations, such as those for precession, nutation, aberration, parallax, and the gravitational deflection of light. The computations are accurate to better than one milliarcsecond. 

To learn more about NOVAS, visit its original GitHub page.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- ABOUT THE PROJECT -->
## About LibNOVASCpp Project

This project is a fork ok NOVAS (Naval Observatory Vector Astronomy Software) library (C Edition). 

Internally, the behavior of this adaptation is similar to the original NOVAS library, but it has been adapted and refactorized (minimally) to be compiled and installed easily with C++17 modern compilers together with CMake. The most relevant addition has been a series of CMake modules that facilitate the configuration for the compilation and subsequent installation of the library.

The original motivation for developing this fork was to facilitate integration the integration of NOVAS into the modern LibDegorasSLR (Degoras Project Satellite Laser Ranging Library) library for calculations related to stars and the SLR technique. 

The LibDegorasSLR library, based on C++17 and currently under development, is used to control the SFEL SLR station of the Royal Institute and Observatory of the Spanish Navy (ROA), located in San Fernando, Cádiz, Spain. To learn more about LibDegorasSLR and related projects, you can visit its GitHub page: https://github.com/DegorasProjectTeam/LibDegorasSLR

Ángel Vera Herrera - avera@roa.es - Computer Engineer - ROA.

March - 2024

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## Build And Installation

This project is prepared to be used with CMake >= 3.21 and to be compiled with modern C++17 compilers. The "CMakeLists.txt" and the related CMake modules are a little complex but they are ready to be configured in many aspects (for example the build configuration or the install directory). To know more aspects of the configuration, read directly the comments in the CMake modules.

Remember changue the build configuration to release and the default installation directory for final deployment environments.

### About compilers.

At this momment, this fork has been tested in the following platforms:

- Ubuntu 22.04 with GCC 11.4.
- Windows 10 with MinGW 8.1 for 64 bits.
- Windows 10 with MinGW 13.1 for 64 bits.
- Windows 10 with MSVC2019 for 64 bits.

### Compiling and installing the library.

On Windows platforms I recommend use a development IDE for compiling, for example VSCode or QTCreator. The easiest way is to open the project in the IDE we want (using the "CMakeLists.txt") and configure the IDE project to use the corresponding compiler (for example MinGW). Finally, we can compile the library and even deploy (install) it in the configured install folder.

In Linux platforms the process is very simple if you already have the GCC compiler and CMake installed and configured. You can create a build directory (for example "build_debug_gcc64") in the root of the LibNovasCpp project. Inside the build directory, we can call to "cmake" and then, "make install" to build and install the library in the configured install folder.

```
git clone https://github.com/DegorasProjectTeam/LibNovasCpp
cd LibNovasCpp/
mkdir build_debug_gnu
cd build_debug_gnu/
cmake ../LibNovasCpp
make install
```

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## External Library Usage

Once the library has been compiled and installed, a product folder will be created with everything necessary to use the library externally, for example in another project.

It is important to mention that when installing the library, a series of CMake configuration files are generated (located in the "share" folder within the installation folder), so it is not possible to move the installation folder to another place once created. If you want to change the location of the installation, we must delete the current folder and perform a new installation configuring the desired path.

### Linking LibNovasCpp in another project

If we have installed the library in the default path or a common or standard path (for example "Program Files" on Windows platforms), we can use CMake's automatic system to easily include and link LibNovasCpp in another project.

To use the automatic system, we will copy the "ConfigLibNovasCpp.cmake" module found in the "CMakeLibsConfig" folder in the root of our new project (or inside a folder if we prefer). Then, in the "CMakeLists.txt" file of our new project, we must include the following instructions (you must replace what is between <> with the correct data). 

```
# Set path to additional CMake modules.
set(CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH}
    ${CMAKE_SOURCE_DIR}/< ConfigLibNovasCpp.cmake FOLDER PATH >)

# [ETC MORE PROJECT CONFIGURATIONS]...

# Configure the LibNovasCPP package.
include(ConfigLibNovasCpp)
macro_configure_libnovascpp_default("3.1" EXACT "" "")

# [ETC MORE PROJECT CONFIGURATIONS] 
# At this point we must have already created a target (library or executable).

# Link with LibNovasCPP. 
macro_link_libnovascpp_default(${< NAME OF THE TARGET >})
```

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## Important Notes

### About solarsystem versions.

You must select in the "CMakeLists.txt" the <SOLSYS_SOURCE> specific file to be used for compiling, between <solsys1.c>, <solsys2.c> and <solsys3.c> source files. 

By default <solsys3.c> is used, so only reduced-accuracy mode can be used for calculations. However, this mode is enought for a lot of engineering applications. Remember that using the reduced-accuracy mode with this version of solarsystem  for the in the computation of apparent places of stars, it should contribute less than 1.5 milliarcseconds of error (this assessment applies to the interval 1800–2050). If you want to use the full-accuracy mode, remember that you must compile the Fortran routines.

See the NOVAS_C3.1_Guide.pdf for more detailed explanation.

### About Fortran routines.

The Fortran routines must be compiled externally. I recommend use IntelFortran with Visual Studio (increases efficiency noticeably) but GFortran can also be used.

### About original examples.

The original examples has been replaced with modified C++ versions. Not all examples has been ported yet. 

### About utilities.

Some utilities has been added to the project as a more complex examples for testing.

## TODOs.

- It would be interesting to create a complete wrapper of the library in C++ within a namespace to avoid using C syntax.
- Port all the examples.
- Do more complex examples.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->
[cplusplus-shield]: https://img.shields.io/badge/-C++17-black?style=for-the-badge&logo=cplusplus&colorB=555
[cplusplus-url]: https://en.cppreference.com/w/cpp/17
[linkedin-shield]: https://img.shields.io/badge/LinkedIn-blue?style=for-the-badge&logo=linkedin
[linkedin-url]: https://www.linkedin.com/in/angelveraherrera/
