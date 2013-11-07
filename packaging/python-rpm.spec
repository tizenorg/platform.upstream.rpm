Name:           python-rpm
Version:        4.11.0.1
Release:        0
Summary:        Python Bindings for Manipulating RPM Packages
License:        GPL-2.0+
Group:          System/Packages
Source99:       rpm.spec
BuildRequires:  gettext-tools
BuildRequires:  file-devel
BuildRequires:  libacl-devel
BuildRequires:  bzip2-devel
BuildRequires:  libcap-devel
BuildRequires:  libelf-devel
BuildRequires:  libtool
BuildRequires:  lua-devel
BuildRequires:  ncurses-devel
BuildRequires:  popt-devel
BuildRequires:  python-devel
BuildRequires:  xz-devel
BuildRequires:  zlib-devel
BuildRequires:  nss-devel
BuildRequires:  uthash-devel
BuildRequires:  libxml2-devel
BuildRequires:  libattr-devel
BuildRequires:  pkgconfig(libsmack)
Requires:       rpm = %{version}
Source0:        rpm-%{version}.tar.bz2
Source1:       	db-4.8.30.tar.bz2
Source2:	    db-4.8.30-integration.dif
Source4:        rpm-tizen_macros
Source8:        rpmconfigcheck
Source13:	    find-docs.sh
Source22:	    device-sec-policy
Source23:       find-provides.ksyms
Source1001: 	rpm.manifest
BuildRoot:      %{_tmppath}/%{name}-%{version}-build
%global with_python 2

%description
The python-rpm package contains a module that permits applications
written in the Python programming language to use the interface
supplied by RPM Package Manager libraries.

This package should be installed if you want to develop Python programs
that will manipulate RPM packages and databases.

%prep
%setup -q -n rpm-%{version}
cp %{SOURCE1001} .
rm -rf sqlite
tar xjf %{S:1}
ln -s db-4.8.30 db
chmod -R u+w db/*
# will get linked from db3
rm -f rpmdb/db.h
patch -p0 < %{S:2}

if [ -s /etc/rpm/tizen_macros ]; then
        cp -a /etc/rpm/tizen_macros %{SOURCE4}
fi
cp -a %{SOURCE4} tizen_macros
rm -f m4/libtool.m4
rm -f m4/lt*.m4

%build
CPPFLAGS="$CPPFLAGS `pkg-config --cflags nss`"
export CPPFLAGS 
export CFLAGS="%{optflags} -ffunction-sections"
export LDFLAGS="-Wl,-Bsymbolic-functions -ffunction-sections"
%ifarch armv5tel
export CFLAGS="-g -O0 -fno-strict-aliasing -ffunction-sections"
%endif

%if %{_target_cpu}
%ifarch %arm
BUILDTARGET="--build=%{_target_cpu}-tizen-linux-gnueabi"
%else
BUILDTARGET="--build=%{_target_cpu}-tizen-linux"
%endif
%endif

autoreconf -i -f
./configure --disable-dependency-tracking --prefix=%{_prefix} --mandir=%{_mandir} --infodir=%{_infodir} \
--libdir=%{_libdir} --sysconfdir=/etc --localstatedir=/var  --with-lua \
--with-acl --with-cap  --enable-shared %{?with_python: --enable-python} --with-msm $BUILDTARGET

make %{?_smp_mflags}

%install
mkdir -p %{buildroot}%{_prefix}/lib
# only installing in python/ does not work because rpm links against
# installed libs at install time
%make_install
find %{buildroot} -not -type d -and -not -path %{buildroot}%{_libdir}/python*/site-packages/rpm/\* -print0 | xargs -0 rm
pushd %{buildroot}/%{_libdir}/python*/site-packages/rpm
rm -f _rpmmodule.a _rpmmodule.la
#python %{_libdir}/python*/py_compile.py *.py
#python -O %{_libdir}/python*/py_compile.py *.py
popd

%files
%defattr(-,root,root)
%{_libdir}/python*/*/*

%changelog
