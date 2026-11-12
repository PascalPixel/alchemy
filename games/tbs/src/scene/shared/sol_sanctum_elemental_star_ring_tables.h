/// @file
/// Sol Sanctum's Elemental Star room and its eruption: each floating light's
/// drift, swing, direction and pulse, by its index, as
/// sol_sanctum_elemental_star_ring_setup.h reads them.

#include "types.h"
#include "fixed_math.h"

// clang-format off
u8 gSolSanctum_RingDrift[][3] = {
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

u8 gSolSanctum_RingSwing[][3] = {
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

s8 gSolSanctum_RingDirection[][3] = {
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

s32 gSolSanctumRing_MaxScales[] = {
    3 * Q16_ONE / 5, 4 * Q16_ONE / 5, 7 * Q16_ONE / 10, 3 * Q16_ONE / 5, 4 * Q16_ONE / 5,
    3 * Q16_ONE / 5, 7 * Q16_ONE / 10, 3 * Q16_ONE / 5, 3 * Q16_ONE / 5, 7 * Q16_ONE / 10,
};

s32 gSolSanctumRing_ScaleSteps[] = {
    10 * Q16_ONE / 1000, 7 * Q16_ONE / 1000, 10 * Q16_ONE / 1000, 7 * Q16_ONE / 1000, 10 * Q16_ONE / 1000,
    7 * Q16_ONE / 1000, 10 * Q16_ONE / 1000, 10 * Q16_ONE / 1000, 8 * Q16_ONE / 1000, 10 * Q16_ONE / 1000,
};
// clang-format on
