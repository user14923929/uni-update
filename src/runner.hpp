#pragma once
#include <string>
#include <vector>

struct RunResult {
    int  exit_code;
    bool ok() const { return exit_code == 0; }
};

// Запустить команду, унаследовав stdin/stdout/stderr (интерактивный режим)
RunResult run_command(const std::vector<std::string>& args, bool need_sudo = false);

// Для Debian нужны два отдельных шага: update + upgrade
RunResult run_apt_update(bool confirm);

// Проверить наличие бинаря в PATH
bool has_binary(const std::string& name);
