#include "simulation/ElementCommon.h"
#include "FIRE.h"
#include "physics/pu239.h"
#include <cmath>

static int update(UPDATE_FUNC_ARGS);
static int graphics(GRAPHICS_FUNC_ARGS);
static void create(ELEMENT_CREATE_FUNC_ARGS);
static int DeutExplosion(Simulation * sim, int n, int x, int y, float temp, int t);

void Element::Element_NEUT()
{
	Identifier = "DEFAULT_PT_NEUT";
	Name = "NEUT";
	Colour = 0x20E0FF_rgb;
	MenuVisible = 1;
	MenuSection = SC_NUCLEAR;
	Enabled = 1;

	Advection = 0.0f;
	AirDrag = 0.00f * CFDS;
	AirLoss = 1.00f;
	Loss = 1.00f;
	Collision = -0.99f;
	Gravity = 0.0f;
	Diffusion = 0.01f;
	HotAir = 0.002f	* CFDS;
	Falldown = 0;

	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 0;

	Weight = -1;

	DefaultProperties.temp = R_TEMP + 4.0f + 273.15f;
	HeatConduct = 60;
	Description = "Neutrons. Interact with matter in odd ways.";

	Properties = TYPE_ENERGY|PROP_LIFE_DEC|PROP_LIFE_KILL_DEC;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = ITH;
	HighTemperatureTransition = NT;

	Update = &update;
	Graphics = &graphics;
	Create = &create;
}

static int update(UPDATE_FUNC_ARGS)
{
	auto &sd = SimulationData::CRef();
	auto &elements = sd.elements;
	unsigned int pressureFactor = 3 + (int)sim->pv[y/CELL][x/CELL];
        int targetIndex = -1;
        int targetX = 0, targetY = 0;
        for (int rx = -1; rx <= 1; rx++)
        {
                for (int ry = -1; ry <= 1; ry++)
                {
                        auto r = pmap[y+ry][x+rx];
                        switch (TYP(r))
                        {
                        case PT_WATR:
                                if (sim->rng.chance(3, 20))
                                        sim->part_change_type(ID(r),x+rx,y+ry,PT_DSTW);
                        case PT_ICEI:
                        case PT_SNOW:
                                parts[i].vx *= 0.995f;
                                parts[i].vy *= 0.995f;
                                break;
                        case PT_PLUT:
                                if (targetIndex == -1)
                                {
                                        targetIndex = ID(r);
                                        targetX = x + rx;
                                        targetY = y + ry;
                                }
                                break;
			case PT_DEUT:
				if (sim->rng.chance(pressureFactor + 1 + (parts[ID(r)].life/100), 1000))
				{
					DeutExplosion(sim, parts[ID(r)].life, x+rx, y+ry, restrict_flt(parts[ID(r)].temp + parts[ID(r)].life*500.0f, MIN_TEMP, MAX_TEMP), PT_NEUT);
					sim->kill_part(ID(r));
				}
				break;
			case PT_GUNP:
				if (sim->rng.chance(3, 200))
					sim->part_change_type(ID(r),x+rx,y+ry,PT_DUST);
				break;
			case PT_DYST:
				if (sim->rng.chance(3, 200))
					sim->part_change_type(ID(r),x+rx,y+ry,PT_YEST);
				break;
			case PT_YEST:
				sim->part_change_type(ID(r),x+rx,y+ry,PT_DYST);
				break;
			case PT_PLEX:
				if (sim->rng.chance(3, 200))
					sim->part_change_type(ID(r),x+rx,y+ry,PT_GOO);
				break;
			case PT_NITR:
				if (sim->rng.chance(3, 200))
					sim->part_change_type(ID(r),x+rx,y+ry,PT_DESL);
				break;
			case PT_PLNT:
				if (sim->rng.chance(1, 20))
					sim->create_part(ID(r), x+rx, y+ry, PT_WOOD);
				break;
			case PT_DESL:
			case PT_OIL:
				if (sim->rng.chance(3, 200))
					sim->part_change_type(ID(r),x+rx,y+ry,PT_GAS);
				break;
			case PT_COAL:
				if (sim->rng.chance(1, 20))
					sim->create_part(ID(r), x+rx, y+ry, PT_WOOD);
				break;
			case PT_BCOL:
				if (sim->rng.chance(1, 20))
					sim->create_part(ID(r), x+rx, y+ry, PT_SAWD);
				break;
			case PT_DUST:
				if (sim->rng.chance(1, 20))
					sim->part_change_type(ID(r), x+rx, y+ry, PT_FWRK);
				break;
			case PT_FWRK:
				if (sim->rng.chance(1, 20))
					parts[ID(r)].ctype = PT_DUST;
				break;
			case PT_ACID:
				if (sim->rng.chance(1, 20))
					sim->create_part(ID(r), x+rx, y+ry, PT_ISOZ);
				break;
			case PT_TTAN:
				if (sim->rng.chance(1, 20))
				{
					sim->kill_part(i);
					return 1;
				}
				break;
			case PT_EXOT:
				if (sim->rng.chance(1, 20))
					parts[ID(r)].life = 1500;
				break;
			case PT_RFRG:
				if (sim->rng.chance(1, 2))
					sim->create_part(ID(r), x+rx, y+ry, PT_GAS);
				else
					sim->create_part(ID(r), x+rx, y+ry, PT_CAUS);
				break;
			case PT_RSSS:
				if(!rx && !ry)
				{
					int ct_under, tmp_under;

					ct_under = parts[ID(r)].ctype;
					tmp_under = parts[ID(r)].tmp;

					//If there's a correct ctype set, liquefy into it
					if(ct_under > 0 && ct_under < PT_NUM)
					{
						sim->create_part(ID(r), x, y, ct_under);

						//If there's a correct tmp set, use it for ctype
						if((tmp_under > 0) && (tmp_under < PT_NUM) && (elements[ct_under].CarriesTypeIn & (1U << FIELD_CTYPE)))
							parts[ID(r)].ctype = tmp_under;
					}
					else
						sim->part_change_type(ID(r), x, y, PT_RSST); //Default to RSST if no ctype

					sim->kill_part(i);

					return 1;
				}
				break;
			default:
				break;
			}
		}
        }
        if (targetIndex != -1 && sim->rng.chance(35, 100))
        {
                int count = 0;
                for (int ax = -4; ax <= 4; ++ax)
                        for (int ay = -4; ay <= 4; ++ay)
                        {
                                int nx = targetX + ax;
                                int ny = targetY + ay;
                                if ((unsigned)nx < XRES && (unsigned)ny < YRES && TYP(pmap[ny][nx]) == PT_PLUT)
                                        count++;
                        }

                int neutrons = 2 + count / 25;
                if (neutrons > 5)
                        neutrons = 5;
                for (int n = 0; n < neutrons; ++n)
                {
                        int s = sim->create_part(-1, targetX, targetY, PT_NEUT);
                        if (s >= 0)
                        {
                                float ang = sim->rng.uniform01() * 6.2831853f;
                                float speed = 2.f;
                                parts[s].vx = cosf(ang) * speed;
                                parts[s].vy = sinf(ang) * speed;
                        }
                }

                pu239_increment_fissions();
                sim->pv[targetY/CELL][targetX/CELL] += neutrons * 5.f * CFDS;
                Element_FIRE_update(UPDATE_FUNC_SUBCALL_ARGS);
                sim->kill_part(targetIndex);
                sim->kill_part(i);
                return 1;
        }
        return 0;
}

static int graphics(GRAPHICS_FUNC_ARGS)
{
	*firea = 120;
	*firer = 10;
	*fireg = 80;
	*fireb = 120;

	*pixel_mode |= FIRE_ADD;
	return 1;
}

static void create(ELEMENT_CREATE_FUNC_ARGS)
{
	float r = sim->rng.between(128, 255) / 127.0f;
	float a = sim->rng.between(0, 359) * 3.14159f / 180.0f;
	sim->parts[i].life = sim->rng.between(480, 959);
	sim->parts[i].vx = r * cosf(a);
	sim->parts[i].vy = r * sinf(a);
}

static int DeutExplosion(Simulation * sim, int n, int x, int y, float temp, int t)//testing a new deut create part
{
	int i;
	n = (n/50);
	if (n < 1)
		n = 1;
	else if (n > 340)
		n = 340;

	for (int c = 0; c < n; c++)
	{
		i = sim->create_part(-3, x, y, t);
		if (i >= 0)
			sim->parts[i].temp = temp;
		else if (sim->MaxPartsReached())
			break;
	}
	sim->pv[y/CELL][x/CELL] += (6.0f * CFDS)*n;
	return 0;
}
