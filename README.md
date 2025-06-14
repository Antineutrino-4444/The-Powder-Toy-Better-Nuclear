# Pu-239 Chain Reaction Mod

This fork rewrites the plutonium element (`PLUT`) into a toy model of Pu‑239 metal.  Each pixel represents a microscopic unit of fissile material and interacts with neutrons on a per‑particle basis.

## Overview of changes
- **Solid plutonium:** `PLUT` behaves as a floating solid metal.  Its previous powder behaviour has been removed.
- **Probabilistic fission:** When a neutron enters a Pu‑239 cell there is a 35 % chance of interaction.  Otherwise it passes straight through.
- **Criticality check:** Neutron interactions count neighbouring Pu‑239 cells in a 9×9 window.  If over one thousand atoms are present the mass is treated as supercritical; otherwise it is subcritical.
- **Neutron yield:** Subcritical fissions emit two neutrons, while supercritical ones emit five.  Emission directions are isotropic.
- **Energy deposition:** Fission events add heat and pressure to the local cell and then remove the consumed Pu‑239.
- **Statistics:** The HUD displays a cumulative fission counter which can be reset with `!reset fissions`.  Values update once per second.
- **Particle fields:** Neutrons now contain `energy_group` and `weight` members in preparation for a multi‑group transport model.  An example cross‑section table (`pu239_xs.h`) and a generator script illustrate how real data could be added.

The logic is intentionally simplistic but tries to echo real behaviour: a sufficiently large mass will undergo a runaway chain reaction when struck by a neutron, whereas smaller samples merely release a few particles before stopping.

