#pragma once
#include "extras.hpp"

struct CliOptions {
    bool          help        = false;
    bool          version     = false;
    bool          dry_run     = false;
    bool          no_confirm  = false;
    bool          all_extras  = false;
    ExtrasOptions extras;
};

CliOptions parse_args(int argc, char* argv[]);
void       print_help();
void       print_version();
