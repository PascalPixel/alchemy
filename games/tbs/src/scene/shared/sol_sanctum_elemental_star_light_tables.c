/* @file */
/* Sol Sanctum's Elemental Star room and its eruption: each floating light's */
/* drift, swing, direction and pulse, by its index, as */
/* sol_sanctum_elemental_star_lights.c reads them. */

#include "fixed_math.h"
#include "types.h"

/* clang-format off */
/* Each light, on x, y and z: the most its drift speeds up a frame (thousandths */
/* of a pixel), how fast it swings, and which way it drifts (1, -1, or 0 for */
/* not at all); then its largest scale and how fast it pulses. */
u8 SolSanctum_LightDrift[][3] = {
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
u8 SolSanctum_LightSwing[][3] = {
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
s8 SolSanctum_LightDirection[][3] = {
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
FIXED SolSanctum_LightMaxScales[] = {
    toFIXED(0.6), toFIXED(0.8), toFIXED(0.7), toFIXED(0.6), toFIXED(0.8),
    toFIXED(0.6), toFIXED(0.7), toFIXED(0.6), toFIXED(0.6), toFIXED(0.7),
};
FIXED SolSanctum_LightScaleSteps[] = {
    toFIXED(0.01), toFIXED(0.007), toFIXED(0.01), toFIXED(0.007), toFIXED(0.01),
    toFIXED(0.007), toFIXED(0.01), toFIXED(0.01), toFIXED(0.008), toFIXED(0.01),
};
/* clang-format on */
