Name:           f
Version:        1.0
Release:        1%{?dist}
Summary:        A minimalist fetch 

License:        MIT
URL:            https://github.com/xfgusta/%{name}
Source0:        https://github.com/xfgusta/%{name}/archive/v%{version}/%{name}-%{version}.tar.gz

BuildRequires:  gcc make procps-ng-devel

%description
Fast and simple system info (for Linux) written in POSIX compliant C99.

%global debug_package %{nil}

%prep
%autosetup


%build
%make_build

%install
%make_install


%files
%license LICENSE
%{_bindir}/%{name}



%changelog
* Tue May 11 2021 Gustavo Costa <xfgusta@gmail.com>
- First f package

