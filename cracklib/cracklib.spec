%define root crack
%define maj 2
%define libname %mklibname %root %maj
%define libnamedev %libname-devel

Summary:	A password-checking library.
Name:		cracklib
Version:	2.7
Release:	18mdk
Group:		System/Libraries
Source:		ftp://coast.cs.purdue.edu/pub/tools/unix/libs/cracklib/cracklib_%{version}.tar.bz2
BuildRequires:	words
URL:		ftp://coast.cs.purdue.edu/pub/tools/unix/libs/cracklib/
License:	Artistic
Patch0:		cracklib-2.7-redhat.patch.bz2
Patch1:		cracklib-2.7-makevars.patch.bz2
Patch2:		cracklib-2.7-includes.patch.bz2
Buildroot:	%{_tmppath}/%{name}-root

%description
CrackLib tests passwords to determine whether they match certain
security-oriented characteristics. You can use CrackLib to stop
users from choosing passwords which would be easy to guess. CrackLib
performs certain tests: 

* It tries to generate words from a username and gecos entry and 
  checks those words against the password;
* It checks for simplistic patterns in passwords;
* It checks for the password in a dictionary.

CrackLib is actually a library containing a particular
C function which is used to check the password, as well as
other C functions. CrackLib is not a replacement for a passwd
program; it must be used in conjunction with an existing passwd
program.

Install the cracklib package if you need a program to check users'
passwords to see if they are at least minimally secure. If you
install CrackLib, you'll also want to install the cracklib-dicts
package.

%package -n	%libname
Summary:	A password-checking library.
Group:		System/Libraries
Provides:	lib%{root}-devel %{root}-devel = %{version}-%{release}
Obsoletes:	cracklib

%description -n %libname

%{see_base}

%package	dicts
Summary:	The standard CrackLib dictionaries.
Group:		System/Libraries

%description	dicts
The cracklib-dicts package includes the CrackLib dictionaries.
CrackLib will need to use the dictionary appropriate to your system,
which is normally put in /usr/share/dict/words.  Cracklib-dicts also contains
the utilities necessary for the creation of new dictionaries.

If you are installing CrackLib, you should also install cracklib-dicts.

%package -n	%libnamedev
Summary:	Cracklib link library & header file
Group:		Development/C
Provides:	lib%{root}-devel %{root}-devel = %{version}-%{release}
Provides:	%{root}lib-devel = %{version}-%{release}
Requires:	%{libname} = %{version}-%{release}
Obsoletes:	cracklib-devel

%description -n	%libnamedev
The cracklib devel package include the needed library link and
header files for development.

%prep

%setup -q -n cracklib,2.7
%patch0 -p1 -b .rh
%patch1 -p1 -b .makevars
%patch2 -p1 -b .includes
perl -p -i -e "s/\) -g/\)/" cracklib/Makefile
chmod -R og+rX .

%build
make all RPM_OPT_FLAGS="$RPM_OPT_FLAGS" \
	libdir=%{_libdir} datadir=%{_datadir}

%install
rm -rf $RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT{%{_sbindir},%{_libdir},%{_includedir}}
make install \
	ROOT=$RPM_BUILD_ROOT \
	sbindir=%{_sbindir} \
	libdir=%{_libdir} \
	includedir=%{_includedir}
ln -sf libcrack.so.%{version} $RPM_BUILD_ROOT%{_libdir}/libcrack.so.%{maj}

install -m644 cracklib/packer.h $RPM_BUILD_ROOT%{_includedir}/

%clean
rm -rf $RPM_BUILD_ROOT

%post -n %libname -p /sbin/ldconfig

%postun -n %libname -p /sbin/ldconfig

%files -n %{libname}
%defattr(-,root,root)
%doc README MANIFEST LICENCE HISTORY POSTER
%{_libdir}/libcrack.so.*

%files -n %{libnamedev}
%defattr(-,root,root)
%{_includedir}/*
%{_libdir}/libcrack.so

%files dicts
%defattr(-,root,root)
%{_sbindir}/*
%{_libdir}/cracklib_dict*

%changelog
* Wed May 26 2004 Oden Eriksson <oeriksson@mandrakesoft.com> 2.7-18mdk
- add one missing header file
- misc spec file fixes

* Fri Jul 18 2003 Warly <warly@mandrakesoft.com> 2.7-17mdk
- libification

* Tue Jun 25 2002 Gwenole Beauchesne <gbeauchesne@mandrakesoft.com> 2.7-16mdk
- Rpmlint fixes: hardcoded-library-path (Patch1)
- Patch2: Add missing includes

* Fri Nov  2 2001 Jeff Garzik <jgarzik@mandrakesoft.com> 2.7-15mdk
- Rebuild.
- Update URL.

* Mon Oct  2 2000 Frederic Lepied <flepied@mandrakesoft.com> 2.7-14mdk
- removed build requires on cracklib-devel.
- added build requires on words and chage the path of the dicts to /usr/share/dict.

* Mon Aug 07 2000 Frederic Lepied <flepied@mandrakesoft.com> 2.7-13mdk
- automatically added BuildRequires

* Fri Jul 21 2000 Thierry Vignaud <tvignaud@mandrakesoft.com> 2.7-12mdk
- BM

* Fri May 19 2000 Pixel <pixel@mandrakesoft.com> 2.7-11mdk
- add soname

* Thu Apr 13 2000 Yoann Vandoorselaere <yoann@mandrakesoft.com> 2.7-10mdk
- Devel package.

* Tue Mar 21 2000 Yoann Vandoorselaere <yoann@mandrakesoft.com> 2.7-9mdk
- Fix group.

* Wed Oct 20 1999 Chmouel Boudjnah <chmouel@mandrakesoft.com>

- Strip binaries.
- Add %defattr

* Sun May  2 1999 Bernhard Rosenkränzer <bero@mandrakesoft.com>
- s/V'erification/Verification in french translation - I know it's a
  spelling mistake, but rpm 3.0 doesn't like accents in Summary: lines. :/

* Thu Apr 10 1999 Alexandre Dussart <adussart@mandrakesoft.com>
- French Translation

* Fri Apr  9 1999 Bernhard Rosenkraenzer <bero@linux-mandrake.com>
- Mandrake adaptions
- handle RPM_OPT_FLAGS
- add de locale

* Wed Jan 06 1999 Cristian Gafton <gafton@redhat.com>
- build for glibc 2.1

* Sat May 09 1998 Prospector System <bugs@redhat.com>
- translations modified for de, fr, tr

* Tue Mar 10 1998 Cristian Gafton <gafton@redhat.com>
- updated to 2.7
- build shared libraries

* Mon Nov 03 1997 Donnie Barnes <djb@redhat.com>
- added -fPIC

* Mon Oct 13 1997 Donnie Barnes <djb@redhat.com>
- basic spec file cleanups

* Mon Jun 02 1997 Erik Troan <ewt@redhat.com>
- built against glibc

