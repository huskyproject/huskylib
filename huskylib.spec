%define reldate 20160316
%define reltype C
# may be one of: C (current), R (release), S (stable)

Name: huskylib
Version: 1.9.%{reldate}%{reltype}
Release: 1
Group: Libraries/FTN
Summary: Libraries for the Husky Project applications
URL: http://husky.sf.net
License: GPL
Source: %{name}.tar.gz
BuildRoot: %{_tmppath}/%{name}-%{version}-root

%description
This package contains common libraries for the Husky Project
FTN software.

%prep
%setup -q -n %{name}

%build
make

%install
rm -rf %{buildroot}
make DESTDIR=%{buildroot} install
chmod -R a+rX,u+w,go-w %{buildroot}

%clean
rm -rf %{buildroot}

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files
%defattr(-,root,root)
%{_bindir}/*
%{_includedir}/%{name}/*
%{_libdir}/*
