#include "runner.hpp"
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <cstring>

// Собираем argv[] из вектора строк
static RunResult exec_argv(std::vector<std::string> args) {
    if (args.empty()) return {1};

    std::vector<char*> argv;
    argv.reserve(args.size() + 1);
    for (auto& a : args) argv.push_back(a.data());
    argv.push_back(nullptr);

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return {1};
    }
    if (pid == 0) {
        // дочерний процесс
        execvp(argv[0], argv.data());
        perror(argv[0]);
        _exit(127);
    }

    int status = 0;
    waitpid(pid, &status, 0);

    if (WIFEXITED(status))   return {WEXITSTATUS(status)};
    if (WIFSIGNALED(status)) return {128 + WTERMSIG(status)};
    return {1};
}

RunResult run_command(const std::vector<std::string>& args, bool need_sudo) {
    if (args.empty()) {
        std::cerr << "[error] Empty command\n";
        return {1};
    }

    std::vector<std::string> final_args;

    if (need_sudo && geteuid() != 0) {
        if (!has_binary("sudo")) {
            std::cerr << "[error] sudo not found, please run as root\n";
            return {1};
        }
        final_args.push_back("sudo");
    }

    for (const auto& a : args) final_args.push_back(a);

    std::cout << "\033[1;34m==>\033[0m Running:";
    for (const auto& a : final_args) std::cout << " " << a;
    std::cout << "\n";

    return exec_argv(final_args);
}

RunResult run_apt_update(bool confirm) {
    std::cout << "\033[1;34m==>\033[0m apt-get update\n";

    std::vector<std::string> update_cmd;
    if (geteuid() != 0) update_cmd.push_back("sudo");
    update_cmd.insert(update_cmd.end(), {"apt-get", "update"});

    auto r = exec_argv(update_cmd);
    if (!r.ok()) return r;

    std::vector<std::string> upgrade_cmd;
    if (geteuid() != 0) upgrade_cmd.push_back("sudo");
    upgrade_cmd.insert(upgrade_cmd.end(), {"apt-get", "dist-upgrade"});
    if (confirm) upgrade_cmd.push_back("-y");

    std::cout << "\033[1;34m==>\033[0m apt-get dist-upgrade\n";
    return exec_argv(upgrade_cmd);
}

bool has_binary(const std::string& name) {
    // Пробуем через which (проще, чем парсить PATH вручную)
    std::string cmd = "which " + name + " > /dev/null 2>&1";
    return std::system(cmd.c_str()) == 0;
}
