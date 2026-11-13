/// @file
/// Sol Sanctum's Elemental Star room and its eruption: each floating light's
/// drift, swing, direction and pulse, by its index, as
/// sol_sanctum_elemental_star_ring_setup.c reads them.

#include "types.h"

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

s32 gSolSanctum_RingMaxScales[] = {
    0x9999, 0xcccc, 0xb333, 0x9999, 0xcccc,
    0x9999, 0xb333, 0x9999, 0x9999, 0xb333,
};

s32 gSolSanctum_RingScaleSteps[] = {
    0x28f, 0x1ca, 0x28f, 0x1ca, 0x28f,
    0x1ca, 0x28f, 0x28f, 0x20c, 0x28f,
};
// clang-format on
