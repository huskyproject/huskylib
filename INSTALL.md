
Please read the files (especially README.Makefiles) in the
husky-common (huskybse) package FIRST!

Instruction to build huskylib
-----------------------------

1. Put the huskylib package in the directory where the other packages of fido
   husky reside:
   1. unix, beos, possible cygwin:
      ```text
      /usr/src/packages/        -> huskybse/
                                -> huskylib/
                                -> smapi/
                                -> fidoconfig/
                                -> hpt/
                                -> htick/
                                ...some other
      ```
   1. windows, dos, os/2 & etc:
      ```text
         d:\husky\              -> huskylib\
                                -> smapi\
                                -> fidoconf\
                                -> hpt\
                                -> htick\
                                ...some other
      ```
1. Building 
   1. Compiling and installing with the standard Makefile and huskymak.cfg

      require huskymak.cfg (see huskybse), unixes only:
      ```sh
        $ make
        $ make install
      ```
   1. Compiling with the Legacy Makefiles

      unix:
      ```sh
	    $ make -f makefile.lnx
	    $ make -f makefile.lnx install
      ```
      dos:
      ```sh
        d:\husky\huskylib>make -f makefile.djg
      ```
   1. Compiling and installing using Cmake
      ```
        $ mkdir build`
        $ cd build
        $ cmake ..
        $ cmake --build .
        $ make install
      ```
      make distrib rpm, deb, tgz:
      ```sh
         $ cpack -G RPM
         $ cpack -G DEB
         $ cpack -G TGZ
      ```
1. (For UNIXes only) Ensure /usr/local/lib/ is in /etc/ld.so.conf
1. (For UNIXes only) Execute ldconfig as root

You're ready. Now you can install software which uses huskylib.

