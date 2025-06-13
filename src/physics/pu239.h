#pragma once
#include <cstddef>

struct Pu239NeutronState {
    int energy_group; // 0 thermal, 1 fast
    float weight;     // macro neutron weight
};

struct XSBin {
    float E_min;
    float E_max;
    float sigma_f;
    float sigma_g;
    float nu;
};

extern const XSBin PU239_XS[128];

// simple fission counter helpers
void pu239_increment_fissions(int count = 1);
void pu239_tick_fission_counter();
extern int lastSecondFissions;
