#include "simulation/ElementCommon.h"

static int update(UPDATE_FUNC_ARGS);

void Element::Element_PLUT()
{
	Identifier = "DEFAULT_PT_PLUT";
	Name = "PLUT";
	Colour = 0x407020_rgb;
	MenuVisible = 1;
	MenuSection = SC_NUCLEAR;
	Enabled = 1;

	Advection = 0.4f;
	AirDrag = 0.01f * CFDS;
	AirLoss = 0.99f;
	Loss = 0.95f;
	Collision = 0.0f;
	Gravity = 0.4f;
	Diffusion = 0.00f;
	HotAir = 0.000f	* CFDS;
	Falldown = 1;

	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 0;
	PhotonReflectWavelengths = 0x001FCE00;

	Weight = 90;

	DefaultProperties.temp = R_TEMP + 4.0f + 273.15f;
	HeatConduct = 251;
       Description = "Plutonium. Heavy, fissile particles. Rarely emits neutrons and can chain react.";

	Properties = TYPE_PART|PROP_NEUTPASS|PROP_RADIOACTIVE;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = ITH;
	HighTemperatureTransition = NT;

	Update = &update;
}

static int update(UPDATE_FUNC_ARGS)
{
       // Count neighbouring plutonium to approximate material density
       int neighbourPlut = 0;
       for (int nx = -1; nx <= 1; ++nx)
       {
               for (int ny = -1; ny <= 1; ++ny)
               {
                       if (!nx && !ny)
                               continue;
                       int ax = x + nx;
                       int ay = y + ny;
                       if (InBounds(ax, ay) && TYP(pmap[ay][ax]) == PT_PLUT)
                               neighbourPlut++;
               }
       }

       // Spontaneous fission: probability increases slightly with density
       int chance = 5000; // base 1/5000 chance each frame
       if (sim->rng.chance(neighbourPlut + 1, chance))
       {
               sim->create_part(i, x, y, PT_NEUT);
       }
       return 0;
}
