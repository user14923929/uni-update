#pragma once

struct ExtrasOptions {
    bool flatpak = false;
    bool snap    = false;
    bool pip     = false;
    bool cargo   = false;
    bool npm     = false;
};

void update_extras(const ExtrasOptions& opts);
