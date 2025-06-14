# Pu-239 Chain Reaction Mod

This fork replaces the game's old plutonium behavior with a rudimentary simulation of Pu‑239 criticality.

## Changes
- **Solid plutonium:** `PLUT` is now a floating solid rather than a powder.
- **Chain reactions:** A neutron hitting a cluster larger than about 40 cells triggers a runaway reaction releasing five neutrons. Smaller masses emit two neutrons. The struck plutonium cell disappears.
- **Random neutron emission:** Fissioned cells spawn neutrons in random directions and deposit heat and pressure.
- **Fission statistics:** The HUD shows cumulative and per‑second fission counts. They can be cleared with `!reset fissions`.
- **Neutron metadata:** Particles store an energy group and macro weight for later physics extensions.
- **Cross‑section data:** Placeholder lookup tables live in `src/physics/` along with a generator script.

The simulation uses a simple Monte-Carlo rule. Each neutron has a 50% chance to interact when crossing a plutonium cell. Nearby plutonium is counted to estimate whether the mass is supercritical. Fission events deposit heat, spawn neutrons in random directions, and remove the source cell so reactions do not repeat indefinitely.