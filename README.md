# huskylib
[![Build Status](https://travis-ci.org/huskyproject/huskylib.svg?branch=master)](https://travis-ci.org/huskyproject/huskylib)
[![Build status](https://ci.appveyor.com/api/projects/status/ey5avxi0qb71eoqp/branch/master?svg=true)](https://ci.appveyor.com/project/dukelsky/huskylib/branch/master)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/e45137d7095f4a0ab5ef53f7f7751269)](https://www.codacy.com/app/dukelsky/huskylib?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=huskyproject/huskylib&amp;utm_campaign=Badge_Grade)

**huskylib** is a support library of Husky project, containing essential project functionality

This fork allows cross-compilation from Linux to Windows NT & OS/2 using Open Watcom 2.0:
```console
$ git clone https://github.com/zoomosis/huskylib
$ cd huskylib/make
```
Windows NT build:
```console
$ wmake -f makefile.watcom NT=1
```
OS/2 build:
```console
$ wmake -f makefile.watcom OS2=1
```
Where 'wmake' is Open Watcom 2.0's WMAKE.
