#include "extras.hpp"
#include "runner.hpp"
#include <iostream>

static void section(const std::string& name) {
    std::cout << "\n\033[1;33m──── " << name << " ────\033[0m\n";
}

void update_extras(const ExtrasOptions& opts) {
    if (opts.flatpak) {
        section("Flatpak");
        if (has_binary("flatpak")) {
            // flatpak update не требует sudo
            run_command({"flatpak", "update", "-y"}, false);
        } else {
            std::cout << "  [skip] flatpak not installed\n";
        }
    }

    if (opts.snap) {
        section("Snap");
        if (has_binary("snap")) {
            run_command({"snap", "refresh"}, true);
        } else {
            std::cout << "  [skip] snap not installed\n";
        }
    }

    if (opts.pip) {
        section("pip (user packages)");
        if (has_binary("pip3")) {
            // обновляем все устаревшие pip пакеты текущего юзера
            run_command({
                "bash", "-c",
                "pip3 list --outdated --format=freeze 2>/dev/null "
                "| cut -d= -f1 "
                "| xargs -r pip3 install --upgrade --user"
            }, false);
        } else {
            std::cout << "  [skip] pip3 not installed\n";
        }
    }

    if (opts.cargo) {
        section("cargo-install-update");
        if (has_binary("cargo-install-update")) {
            run_command({"cargo", "install-update", "-a"}, false);
        } else if (has_binary("cargo")) {
            std::cout << "  [hint] Install cargo-update: cargo install cargo-update\n";
        } else {
            std::cout << "  [skip] cargo not installed\n";
        }
    }

    if (opts.npm) {
        section("npm global packages");
        if (has_binary("npm")) {
            run_command({"npm", "update", "-g"}, false);
        } else {
            std::cout << "  [skip] npm not installed\n";
        }
    }
}
