# Pu-239 Chain Reaction Mod

This fork rewrites the plutonium element (`PLUT`) into a toy model of Pu‑239 metal.  Each pixel represents a microscopic unit of fissile material and interacts with neutrons on a per‑particle basis.

## Overview of changes
- **Solid plutonium:** `PLUT` behaves as a floating solid metal.  Its previous powder behaviour has been removed.
- **Probabilistic fission:** Each time a neutron touches a Pu‑239 atom there is a 35 % probability that it will react; otherwise the neutron continues unaffected.
- **Local density effect:** The number of emitted neutrons depends on how many Pu‑239 atoms surround the reaction site within a 9×9 area.  Denser regions release up to five neutrons in random directions, while sparse regions emit only two.
- **Energy deposition:** Fission events add heat and pressure to the local cell and then remove the consumed Pu‑239.
- **Statistics:** The HUD displays a cumulative fission counter which can be reset with `!reset fissions`.  Values update once per second.
- **Particle fields:** Neutrons now contain `energy_group` and `weight` members in preparation for a multi‑group transport model.  An example cross‑section table (`pu239_xs.h`) and a generator script illustrate how real data could be added.

The simulation therefore mimics real behaviour at a tiny scale: dense clusters of Pu‑239 tend to release more neutrons per fission and are more likely to sustain a chain reaction, while sparse deposits fizzle out quickly.
