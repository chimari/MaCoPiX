Summary: MaCoPiX = Mascot Constructive Pilot for X
Name: macopix
Version: 3.4.0
Release: 1%{?_dist_release}
License: GPL3
Group: Amusements/Games
Packager: Kurumi Chimari <chimari@rosegray.sakura.ne.jp>
URL: http://rosegray.sakura.ne.jp/MaCoPiX/
Source: %{name}-%{version}.tar.gz
BuildRoot: %{_tmppath}/%{name}-%{version}-buildroot
Vendor: RoseGray Society
Requires:	gtk3 
Requires:	cairo
Requires:       openssl
BuildRequires:	gtk3-devel
BuildRequires:	cairo-devel
BuildRequires:  openssl-devel

%description
MaCoPiX is a desktop mascot program running under UNIX / X window systems,
macOS, and Microsoft Windows.
Users can make Focus follower (window sitters) or Fixed style (Desktop wappen)
mascots using MaCoPiX. Furthermore, users can select with or without a digital
clock for each types of mascots. 
The biff function is also available for POP/APOP/UNIX local spool/qmail Maildir
environments.
Uers can create mascots and change their settings from GUI instead of editing
mascot files directly.
						
%prep
%setup -q -n macopix-%{version}

%build
./configure

make

%install
rm -rf $RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT/usr/local
make DESTDIR=$RPM_BUILD_ROOT install

%clean
rm -rf %{buildroot}

%files
/usr/local/bin/macopix
/usr/local/share/locale/ja/LC_MESSAGES/macopix.mo
/usr/local/share/man/man1/macopix.1

%changelog
* Mon Jun 17 2019 Kurumi Chimari <chimari@rosegray.sakura.ne.jp>
- Supported macOS.

* Wed May 22 2019 Kurumi Chimari <chimari@rosegray.sakura.ne.jp>
- first release for version 3.0.0
