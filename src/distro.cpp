#include "distro.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>

static std::string parse_os_release_field(const std::string& content, const std::string& key) {
    std::istringstream ss(content);
    std::string line;
    while (std::getline(ss, line)) {
        if (line.rfind(key + "=", 0) == 0) {
            std::string val = line.substr(key.size() + 1);
            // убираем кавычки если есть
            if (!val.empty() && val.front() == '"') val = val.substr(1);
            if (!val.empty() && val.back()  == '"') val.pop_back();
            return val;
        }
    }
    return "";
}

static Distro classify(const std::string& id, const std::string& id_like) {
    auto contains = [](const std::string& s, const std::string& sub) {
        return s.find(sub) != std::string::npos;
    };

    // Сначала проверяем id
    if (id == "arch" || id == "cachyos" || id == "manjaro" || id == "endeavouros" ||
        id == "garuda" || id == "artix")
        return Distro::Arch;

    if (id == "ubuntu" || id == "debian" || id == "linuxmint" || id == "pop" ||
        id == "elementary" || id == "kali" || id == "parrot" || id == "zorin")
        return Distro::Debian;

    if (id == "fedora" || id == "rhel" || id == "centos" || id == "almalinux" ||
        id == "rocky" || id == "nobara")
        return Distro::Fedora;

    if (id == "opensuse-leap" || id == "opensuse-tumbleweed" || id == "sles")
        return Distro::OpenSUSE;

    if (id == "void")   return Distro::Void;
    if (id == "alpine") return Distro::Alpine;
    if (id == "nixos")  return Distro::NixOS;
    if (id == "gentoo") return Distro::Gentoo;

    // Фоллбек на ID_LIKE (для производных дистров)
    if (contains(id_like, "arch"))   return Distro::Arch;
    if (contains(id_like, "debian") || contains(id_like, "ubuntu")) return Distro::Debian;
    if (contains(id_like, "fedora") || contains(id_like, "rhel"))   return Distro::Fedora;
    if (contains(id_like, "suse"))   return Distro::OpenSUSE;

    return Distro::Unknown;
}

DistroInfo detect_distro() {
    DistroInfo info;

    std::ifstream f("/etc/os-release");
    if (!f.is_open()) {
        // fallback: /usr/lib/os-release
        f.open("/usr/lib/os-release");
    }
    if (!f.is_open()) {
        std::cerr << "[warn] Cannot open /etc/os-release\n";
        info.type = Distro::Unknown;
        return info;
    }

    std::string content((std::istreambuf_iterator<char>(f)),
                         std::istreambuf_iterator<char>());

    info.id      = parse_os_release_field(content, "ID");
    info.name    = parse_os_release_field(content, "PRETTY_NAME");
    info.id_like = parse_os_release_field(content, "ID_LIKE");

    // приводим к нижнему регистру
    std::transform(info.id.begin(),      info.id.end(),      info.id.begin(),      ::tolower);
    std::transform(info.id_like.begin(), info.id_like.end(), info.id_like.begin(), ::tolower);

    info.type = classify(info.id, info.id_like);
    return info;
}

std::string distro_name(Distro d) {
    switch (d) {
        case Distro::Arch:     return "Arch Linux (and derivatives)";
        case Distro::Debian:   return "Debian/Ubuntu (and derivatives)";
        case Distro::Fedora:   return "Fedora/RHEL (and derivatives)";
        case Distro::OpenSUSE: return "openSUSE";
        case Distro::Void:     return "Void Linux";
        case Distro::Alpine:   return "Alpine Linux";
        case Distro::NixOS:    return "NixOS";
        case Distro::Gentoo:   return "Gentoo";
        default:               return "Unknown";
    }
}

std::vector<std::string> get_update_command(const DistroInfo& info, bool confirm) {
    switch (info.type) {
        case Distro::Arch:
            return confirm
                ? std::vector<std::string>{"pacman", "-Syu", "--noconfirm"}
                : std::vector<std::string>{"pacman", "-Syu"};

        case Distro::Debian:
            // apt не поддерживает один вызов, runner сам обработает два шага
            return confirm
                ? std::vector<std::string>{"apt-get", "dist-upgrade", "-y"}
                : std::vector<std::string>{"apt-get", "dist-upgrade"};

        case Distro::Fedora:
            return confirm
                ? std::vector<std::string>{"dnf", "upgrade", "-y"}
                : std::vector<std::string>{"dnf", "upgrade"};

        case Distro::OpenSUSE:
            return confirm
                ? std::vector<std::string>{"zypper", "--non-interactive", "up"}
                : std::vector<std::string>{"zypper", "up"};

        case Distro::Void:
            return confirm
                ? std::vector<std::string>{"xbps-install", "-Suy"}
                : std::vector<std::string>{"xbps-install", "-Su"};

        case Distro::Alpine:
            return std::vector<std::string>{"apk", "upgrade"};

        case Distro::NixOS:
            return std::vector<std::string>{"nixos-rebuild", "switch", "--upgrade"};

        case Distro::Gentoo:
            return std::vector<std::string>{"emerge", "--sync"};  // упрощённо

        default:
            return {};
    }
}
