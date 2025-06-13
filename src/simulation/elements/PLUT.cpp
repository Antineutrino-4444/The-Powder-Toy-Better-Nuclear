#include "simulation/ElementCommon.h"
#include "physics/pu239.h"

static int update(UPDATE_FUNC_ARGS);

void Element::Element_PLUT()
{
	Identifier = "DEFAULT_PT_PLUT";
	Name = "PLUT";
	Colour = 0x407020_rgb;
	MenuVisible = 1;
	MenuSection = SC_NUCLEAR;
	Enabled = 1;

	Advection = 0.0f;
	AirDrag = 0.00f * CFDS;
	AirLoss = 1.00f;
	Loss = 1.00f;
	Collision = 0.0f;
	Gravity = 0.0f;
	Diffusion = 0.00f;
	HotAir = 0.000f	* CFDS;
	Falldown = 0;

	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 0;
	PhotonReflectWavelengths = 0x001FCE00;

	Weight = 100;

	DefaultProperties.temp = R_TEMP + 4.0f + 273.15f;
       HeatConduct = 251;
       Description = "Plutonium metal. Fissile solid used for chain reactions.";

       Properties = TYPE_SOLID|PROP_NEUTPASS|PROP_RADIOACTIVE;

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
       // PLUT no longer emits neutrons on its own
       return 0;

}
