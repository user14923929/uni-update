#include "cli.hpp"
#include <iostream>
#include <cstring>

CliOptions parse_args(int argc, char* argv[]) {
    CliOptions opts;
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if      (arg == "-h" || arg == "--help")       opts.help       = true;
        else if (arg == "-v" || arg == "--version")    opts.version    = true;
        else if (arg == "-n" || arg == "--dry-run")    opts.dry_run    = true;
        else if (arg == "-y" || arg == "--no-confirm") opts.no_confirm = true;
        else if (arg == "--all") {
            opts.all_extras          = true;
            opts.extras.flatpak      = true;
            opts.extras.snap         = true;
            opts.extras.pip          = true;
            opts.extras.cargo        = true;
            opts.extras.npm          = true;
        }
        else if (arg == "--flatpak") opts.extras.flatpak = true;
        else if (arg == "--snap")    opts.extras.snap    = true;
        else if (arg == "--pip")     opts.extras.pip     = true;
        else if (arg == "--cargo")   opts.extras.cargo   = true;
        else if (arg == "--npm")     opts.extras.npm     = true;
        else {
            std::cerr << "[error] Unknown option: " << arg << "\n";
            opts.help = true;
        }
    }
    return opts;
}

void print_help() {
    std::cout <<
        "uni-update — cross-distro system updater\n\n"
        "Usage:\n"
        "  uni-update [options]\n\n"
        "Options:\n"
        "  -h, --help         Show this help\n"
        "  -v, --version      Show version\n"
        "  -y, --no-confirm   Pass -y / --noconfirm to package manager\n"
        "  -n, --dry-run      Show what would be done, but don't run\n"
        "      --all          Also update flatpak, snap, pip, cargo, npm\n"
        "      --flatpak      Update Flatpak packages\n"
        "      --snap         Update Snap packages\n"
        "      --pip          Update pip user packages\n"
        "      --cargo        Update cargo global installs\n"
        "      --npm          Update npm global packages\n\n"
        "Supported distros:\n"
        "  Arch/CachyOS/Manjaro, Debian/Ubuntu, Fedora/RHEL,\n"
        "  openSUSE, Void, Alpine, NixOS, Gentoo\n";
}

void print_version() {
    std::cout << "uni-update 1.0.0\n";
}
