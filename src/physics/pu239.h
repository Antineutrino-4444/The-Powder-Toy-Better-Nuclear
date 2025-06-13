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

