#ifndef ALCHEMY_SOL_SANCTUM_ELEMENTAL_STAR_LIGHTS_H
#define ALCHEMY_SOL_SANCTUM_ELEMENTAL_STAR_LIGHTS_H

// The lights that float in Sol Sanctum's Elemental Star room, before and during the eruption
// (shared/sol_sanctum_elemental_star_lights.c).

#include "fixed_math.h"
#include "types.h"

// Starts count lights floating, actors from first (at most 10).
void         scSolSanctumStartFloatingLights(Sint32 first, Uint32 count);

// Each light's drift, swing and direction on x, y and z, then its largest scale and how fast it
// pulses, from shared/sol_sanctum_elemental_star_light_tables.c.
extern Uint8 SolSanctumLightDrift[][3];
extern Uint8 SolSanctumLightSwing[][3];
extern Sint8 SolSanctumLightDirection[][3];
extern FIXED SolSanctumLightMaxScales[];
extern FIXED SolSanctumLightScaleSteps[];

#endif
