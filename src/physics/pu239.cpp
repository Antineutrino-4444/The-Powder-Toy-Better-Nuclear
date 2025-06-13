#include "pu239.h"
#include "pu239_xs.h"
#include <cmath>

// Example function using table (not yet integrated)
float SampleFissionProbability(float energy)
{
    // naive search
    for (const auto &bin : PU239_XS)
    {
        if (energy >= bin.E_min && energy < bin.E_max)
            return bin.sigma_f / (bin.sigma_f + bin.sigma_g + 1e-6f);
    }
    return 0.0f;
}
