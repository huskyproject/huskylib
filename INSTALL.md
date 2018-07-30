# Instruction to build huskylib

Please read the files (especially README.Makefiles) in the
husky-common (huskybse) package FIRST!

## Table of Contents
- [Prebuild](#prebuild)
- [Build](#build)
  - [Compiling and installing with the standard Makefile and huskymak.cfg](#compiling-and-installing-with-the-standard-makefile-and-huskymakcfg)
  - [Compiling with the Legacy Makefiles](#compiling-with-the-legacy-makefiles)
  - [Producing rpm packages](#producing-rpm-packages)
  - [Compiling and installing using Cmake](#compiling-and-installing-using-cmake)
- [Afterbuild actions](#afterbuild-actions)

## Prebuild

- Put the huskylib package in the directory where the other packages of fido
  husky reside:
   
  - unix, beos, possible cygwin:
      ```text
      /usr/src/packages/        -> huskybse/
                                -> huskylib/
                                -> smapi/
                                -> fidoconfig/
                                -> hpt/
                                -> htick/
                                ...some other
      ```
   - windows, dos, os/2 & etc:
      ```text
         d:\husky\              -> huskylib\
                                -> smapi\
                                -> fidoconf\
                                -> hpt\
                                -> htick\
                                ...some other
      ```
## Build 

### Compiling and installing with the standard Makefile and huskymak.cfg

require huskymak.cfg (see huskybse), unixes only:
```sh
   $ make
   $ make install
```
### Compiling with the Legacy Makefiles

unix:
```sh
   $ make -f makefile.lnx
   $ make -f makefile.lnx install
```
dos:
```sh
   d:\husky\huskylib>make -f makefile.djg
```
### Producing rpm packages

It is not necessary to prepare a special huskymak.cfg since it is
already prepared and placed in the root directory.

To compile and build an rpm file first download tarball (*.tar.gz)
with the source code from 

https://github.com/huskyproject/huskylib/releases/latest

and then run the command

    rpmbuild -ta name.tar.gz

where *name* is the name of the downloaded tarball. The resulting
packages are in the ~/rpmbuild/RPMS/*arch* directory. Here *arch*
is your platform architecture, for example, x86_64.

On default static library is made. There are two build options for the
rpmbuild command: '--without static' and '--with debug'. If you want
to build dynamic library you use '--without static' and if you want to
debug the program you use '--with debug'.

    rpmbuild -ta name.tar.gz --without static

### Compiling and installing using Cmake
 
- Run CMake to configure the build tree.
   ```sh
      $ cmake -H. -Bbuild -DBUILD_SHARED_LIBS=OFF
   ```
- Afterwards, generated files can be used to compile the project.
   ```sh
      $ cmake --build build
   ```
- Make distrib rpm, deb,tgz (optional)
   ```sh
      $ cpack -G RPM --config build/CPackConfig.cmake
      $ cpack -G DEB --config build/CPackConfig.cmake
      $ cpack -G TGZ --config build/CPackConfig.cmake
   ```

- Install the built files (optional).
   ```sh
      $ cmake --build build --target install
   ```
## Afterbuild actions

- (For UNIXes only) Ensure /usr/local/lib/ is in /etc/ld.so.conf
- (For UNIXes only) Execute ldconfig as root

You're ready. Now you can install software which uses huskylib.

