Summary: Subaru HDS++ OPE file Editor
Name: macopix
Version: 3.0.0
Release: 1%{?_dist_release}
License: GPL3
Group: Applications/Engineering
Packager: Kurumi Chimari <chimari@rosegray.sakura.ne.jp>
URL: http://rosegray.sakura.ne.jp/MaCoPiX/
Source: %{name}-%{version}.tar.gz
BuildRoot: %{_tmppath}/%{name}-%{version}-buildroot
Vendor: RoseGray Society
Requires:	gtk3 
Requires:	cairo
Requires:       libssh2
Requires:       openssl
BuildRequires:	gtk3-devel
BuildRequires:	cairo-devel
BuildRequires:  libssh2-devel
BuildRequires:  openssl-devel

%description
MaCoPiX is a desktop mascot program running under UNIX / X window systems
or Microsoft Windows.
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
/usr/local/bin/hoe
/usr/local/share/man/man1/macopix.1

%changelog
* Fri May 21 2019 Kurumi Chimari <chimari@rosegray.sakura.ne.jp>
- first release for version 3.0.0
