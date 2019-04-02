%define reldate 20190311
%define reltype C
# may be one of: C (current), R (release), S (stable)

# release number for Release: header
%define relnum 2

# for generic build; will override for some distributions
%define vendor_prefix %nil
%define vendor_suffix %nil
%define pkg_group Libraries/FTN
%define mnt_mail 2:5020/545

# for CentOS, Fedora and RHEL
%if %_vendor == "redhat"
%define vendor_suffix %dist
%endif

# for ALT Linux
%if %_vendor == "alt"
%define vendor_prefix %_vendor
%define pkg_group Networking/FTN
%define mnt_mail gremlin@altlinux.org
%endif

Name: huskylib
Version: 1.9.%{reldate}%{reltype}
Release: %{vendor_prefix}%relnum%{vendor_suffix}
Group: %pkg_group
Summary: Libraries for the Husky Project applications
URL: https://github.com/huskyproject/huskylib
License: GPL
Source: %{name}.tar.xz
BuildRoot: %{_tmppath}/%{name}-%{version}-root

%if %_vendor == "alt"
BuildRequires: glibc-devel-static
%endif
#else
%if %_vendor == "redhat"
BuildRequires: glibc-static
%endif

%description
This package contains common libraries for the Husky Project
FTN software.

%package devel
Group: %pkg_group
Summary: Development headers for %name
BuildArch: noarch
Requires: %name-devel-libs = %version-%release

%description devel
%summary


%package devel-libs-shared
Summary: Shared development libraries for %name
Group: %pkg_group
Requires: %name-devel = %version-%release
Requires: %name = %version-%release
Provides: %name-devel-libs = %version-%release

%description devel-libs-shared
%summary


%package devel-libs-static
Summary: Static development libraries for %name
Group: %pkg_group
Requires: %name-devel = %version-%release
Provides: %name-devel-libs = %version-%release

%description devel-libs-static
%summary



%prep
%setup -q -n %{name}
date '+char cvs_date[]="%%F";' > cvsdate.h

%build
# parallel build appears to be broken at least in CentOS
%if %_vendor == "redhat"
make DYNLIBS=1
make
%else
%make DYNLIBS=1
%make
%endif

%install
rm -rf -- %buildroot
umask 022
make DESTDIR=%buildroot DYNLIBS=1 install
make DESTDIR=%buildroot install
chmod -R a+rX,u+w,go-w %buildroot
# do not package headers for unsupported systems
rm -f -- \
  %buildroot%_includedir/%name/B*.h \
  %buildroot%_includedir/%name/D*.h \
  %buildroot%_includedir/%name/E*.h \
  %buildroot%_includedir/%name/H*.h \
  %buildroot%_includedir/%name/I*.h \
  %buildroot%_includedir/%name/M*.h \
  %buildroot%_includedir/%name/S*.h \
  %buildroot%_includedir/%name/W*.h \
  ;



%clean
rm -rf -- %buildroot


%files
%defattr(-,root,root)
%_bindir/*
%_libdir/*.so.*

%files devel
%dir %_includedir/%name
%_includedir/%name/*

%files devel-libs-shared
%_libdir/*.so

%files devel-libs-static
%_libdir/*.a

%changelog
* Mon Apr 01 2019 Gremlin from Kremlin <%{mnt_mail}> 1.9.20190311C-%{_vendor}2
- adjust build parameters for different distributions

* Mon Mar 11 2019 Gremlin from Kremlin <%{mnt_mail}> 1.9.20190311C-%{_vendor}1
- rewrite .spec from scratch, split to subpackages
