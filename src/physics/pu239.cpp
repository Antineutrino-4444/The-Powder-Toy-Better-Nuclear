#include "pu239.h"
#include "pu239_xs.h"
#include <cmath>
#include <atomic>

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

// --- simple fission counter implementation ---
static std::atomic<int> fissionsThisSecond{0};
int lastSecondFissions = 0;
long long totalFissions = 0;
void pu239_increment_fissions(int count)
{
    fissionsThisSecond += count;
    totalFissions += count;
}

void pu239_tick_fission_counter()
{
    static int frame = 0;
    frame++;
    if (frame >= 60)
    {
        frame = 0;
        lastSecondFissions = fissionsThisSecond.load();
        fissionsThisSecond = 0;
    }
}
