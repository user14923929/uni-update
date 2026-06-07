#include <iostream>
#include "cli.hpp"
#include "distro.hpp"
#include "runner.hpp"
#include "extras.hpp"

static void print_banner() {
    std::cout << "\033[1;36m"
              << "╔══════════════════════════════╗\n"
              << "║       uni-update  v1.0.0     ║\n"
              << "║  cross-distro system updater ║\n"
              << "╚══════════════════════════════╝"
              << "\033[0m\n\n";
}

int main(int argc, char* argv[]) {
    CliOptions opts = parse_args(argc, argv);

    if (opts.help)    { print_help();    return 0; }
    if (opts.version) { print_version(); return 0; }

    print_banner();

    // Детектим дистрибутив
    DistroInfo distro = detect_distro();
    std::cout << "\033[1;32m[*]\033[0m Detected: "
              << (distro.name.empty() ? "Unknown" : distro.name)
              << " (" << distro_name(distro.type) << ")\n\n";

    if (distro.type == Distro::Unknown) {
        std::cerr << "\033[1;31m[!]\033[0m Unknown distro, cannot determine package manager.\n"
                  << "    Please update uni-update or run your package manager manually.\n";
        return 1;
    }

    if (opts.dry_run) {
        std::cout << "\033[1;33m[dry-run]\033[0m Would run: ";
        auto cmd = get_update_command(distro, opts.no_confirm);
        for (const auto& c : cmd) std::cout << c << " ";
        std::cout << "\n";
        if (opts.all_extras || opts.extras.flatpak)
            std::cout << "[dry-run] Would run: flatpak update -y\n";
        if (opts.all_extras || opts.extras.snap)
            std::cout << "[dry-run] Would run: snap refresh\n";
        if (opts.all_extras || opts.extras.pip)
            std::cout << "[dry-run] Would run: pip3 upgrade (outdated user packages)\n";
        if (opts.all_extras || opts.extras.cargo)
            std::cout << "[dry-run] Would run: cargo install-update -a\n";
        if (opts.all_extras || opts.extras.npm)
            std::cout << "[dry-run] Would run: npm update -g\n";
        return 0;
    }

    // ── Системное обновление ──────────────────────────────────
    std::cout << "\033[1;33m──── System packages ────\033[0m\n";

    RunResult result;
    bool need_sudo = true;  // для NixOS/Flatpak sudo не нужен, но по умолчанию да

    if (distro.type == Distro::Debian) {
        // apt требует двух шагов
        result = run_apt_update(opts.no_confirm);
    } else {
        auto cmd = get_update_command(distro, opts.no_confirm);
        result = run_command(cmd, need_sudo);
    }

    if (!result.ok()) {
        std::cerr << "\n\033[1;31m[!]\033[0m System update failed (exit code "
                  << result.exit_code << ")\n";
        return result.exit_code;
    }

    std::cout << "\n\033[1;32m[✓]\033[0m System packages updated successfully.\n";

    // ── Extras ────────────────────────────────────────────────
    bool any_extras = opts.extras.flatpak || opts.extras.snap  ||
                      opts.extras.pip     || opts.extras.cargo || opts.extras.npm;
    if (any_extras) {
        update_extras(opts.extras);
    }

    std::cout << "\n\033[1;32m[✓]\033[0m All done!\n";
    return 0;
}
