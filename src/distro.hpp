#pragma once
#include <string>
#include <vector>

enum class Distro {
    Arch,       // Arch, CachyOS, Manjaro
    Debian,     // Debian, Ubuntu, Mint, Pop!_OS
    Fedora,     // Fedora, RHEL, CentOS
    OpenSUSE,
    Void,
    Alpine,
    NixOS,
    Gentoo,
    Unknown
};

struct DistroInfo {
    Distro      type;
    std::string id;         // из /etc/os-release: ID=arch
    std::string name;       // PRETTY_NAME=...
    std::string id_like;    // ID_LIKE=arch (для производных)
};

DistroInfo detect_distro();
std::string distro_name(Distro d);
std::vector<std::string> get_update_command(const DistroInfo& info, bool confirm);
