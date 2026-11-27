/* Sol Sanctum's Elemental Star room and its eruption: each floating light's
   drift, swing, direction and pulse, by its index, as
   sol_sanctum_elemental_star_lights.c reads them. */

#include "fixed_math.h"
#include "types.h"

/* clang-format off */
/* Each light, on x, y and z: the most its drift speeds up a tick (thousandths */
/* of a pixel), how fast it swings, and which way it drifts (1, -1, or 0 for */
/* not at all); then its largest scale and how fast it pulses. */
Uint8 SolSanctumLightDrift[][3] = {
	{ 4, 4, 4 },
	{ 4, 0, 3 },
	{ 4, 0, 4 },
	{ 4, 0, 4 },
	{ 3, 0, 4 },
	{ 4, 6, 4 },
	{ 6, 0, 4 },
	{ 4, 0, 3 },
	{ 2, 0, 1 },
	{ 2, 0, 2 },
};
Uint8 SolSanctumLightSwing[][3] = {
	{ 1, 1, 2 },
	{ 1, 0, 2 },
	{ 1, 0, 2 },
	{ 1, 0, 1 },
	{ 1, 0, 2 },
	{ 1, 1, 2 },
	{ 1, 0, 2 },
	{ 1, 0, 2 },
	{ 1, 0, 2 },
	{ 1, 0, 2 },
};
Sint8 SolSanctumLightDirection[][3] = {
	{ 1, 1, 1 },
	{ 1, 0, 1 },
	{ 1, 0, 1 },
	{ -1, 0, 1 },
	{ -1, 0, -1 },
	{ 1, 1, 1 },
	{ -1, 0, -1 },
	{ -1, 0, -1 },
	{ -1, 0, -1 },
	{ -1, 0, -1 },
};
FIXED SolSanctumLightMaxScales[] = {
	toFIXED(0.6), toFIXED(0.8), toFIXED(0.7), toFIXED(0.6), toFIXED(0.8),
	toFIXED(0.6), toFIXED(0.7), toFIXED(0.6), toFIXED(0.6), toFIXED(0.7),
};
FIXED SolSanctumLightScaleSteps[] = {
	toFIXED(0.01), toFIXED(0.007), toFIXED(0.01), toFIXED(0.007), toFIXED(0.01),
	toFIXED(0.007), toFIXED(0.01), toFIXED(0.01), toFIXED(0.008), toFIXED(0.01),
};
/* clang-format on */
