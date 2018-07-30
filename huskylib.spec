%define reldate 20160316
%define reltype C
# may be one of: C (current), R (release), S (stable)

# on default static build is made but using 'rpmbuild --without static'
# produces the binary that uses dynamic C library
%bcond_without static

# if you use 'rpmbuild --with debug' then debug binary is produced
%bcond_with debug

Name: huskylib
Version: 1.9.%{reldate}%{reltype}
Release: 3
Group: Libraries/FTN
Summary: Library for the Husky Project applications
URL: https://github.com/huskyproject/%{name}
License: GPL
Source: %{name}.tar.gz
%if %{with static}
Provides: %{name}-static-%{version}
%endif
BuildRoot: %{_tmppath}/%{name}-%{version}-root

%description
This package contains common library for the Husky Project
FTN software.

%prep
%setup -q -n %{name}

%build
%if %{with static}
    %if %{with debug}
        make DEBUG:=1
    %endif
%else
    %if %{with debug}
        make DYNLIBS:=1 DEBUG:=1
    %else
        make DYNLIBS:=1
    %endif
%endif

# macro 'install' is omitted for debug build bacause it strips the library
%if ! %{with debug}
%install
%endif
rm -rf %{buildroot}
make DESTDIR=%{buildroot} install
chmod -R a+rX,u+w,go-w %{buildroot}

%clean
rm -rf %{buildroot}

%if ! %{with static}
%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig
%endif

%files
%defattr(-,root,root)
%{_bindir}/*
%{_libdir}/*
%if %{with static}
%{_includedir}/%{name}/*

%else

%package devel
Summary: Header files for the Husky Project applications

%description devel
Header files for the Husky Project applications

%files devel
%defattr(-,root,root)
%{_includedir}/%{name}/*
%endif
