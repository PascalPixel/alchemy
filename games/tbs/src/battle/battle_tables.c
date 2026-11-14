/// @file
/// The battle's small tables: where a formation's places fall, the party's
/// weapon sprites, the camera's command matrix, how an ability weakens with
/// distance, and the enemies' action weights.

#include "macros.h"
#include "types.h"
#include "battle/battle.h"
#include "battle/range_falloff.h"

// Unused: a word no code names, at the head of the tables.
const u32 gBattleTables_Unk_0 = 16;

const s8 gBattleParty_CenterOrderOffsets[] = {0, -1, 1, -2, 2, -3, 3, -4, 4};

// Each by enum BattleWeaponKind.
const u16 gBattleUnit_WeaponAnimationsIsaac[] ALIGNED(4) = {
    320, 334, 326, 327, 320, 320, 320,
};
const u16 gBattleUnit_WeaponAnimationsGaret[] = {
    329, 329, 328, 321, 321, 321, 321,
};
const u16 gBattleUnit_WeaponAnimationsIvan[] = {
    331, 331, 331, 331, 322, 330, 322,
};
const u16 gBattleUnit_WeaponAnimationsMia[] = {
    323, 323, 333, 333, 332, 323, 323,
};
const u16 gBattleUnit_WeaponAnimationsJenna[] = {
    336, 336, 325, 325, 325, 325, 325,
};

// clang-format off
const s8 gBattlePlacement_StepPairs[] = {
    80, 48,
    80, 16,
    80, -16,
    80, -48,
    0, 0,
    -80, 50,
    -80, 30,
    -80, 10,
    -80, -10,
    -80, -30,
    -80, -50,
    0, 0,
    0, 0,
};

const s32 gBattleCamera_CommandMatrix[] = {
    1 << 16, 0, 0, -(4 << 16) / 5,
    1 << 16, 0, 0, 0,
    1 << 16, 0, 0, 0,
    80 << 16, 0, 0,
};
// clang-format on

// The same in both games.
const u8 gRangeFalloff_EffectChance[] = {100, 60, 30, 30, 30, 30};
const s32 gRangeFalloff_PpLoss[] = {100, 50, 30, 10, 10, 10};
const s32 gRangeFalloff_HealHp[] = {100, 80, 60, 40, 20, 10};
const s32 gRangeFalloff_PpDamage[] = {100, 50, 30, 10, 10, 10};
const s32 gRangeFalloff_PsynergyDamage[] = {100, 80, 60, 40, 20, 10};
const s32 gRangeFalloff_SummonDamage[] = {100, 70, 40, 30, 20, 10};
const s32 gRangeFalloff_EnemyDamage[] = {100, 50, 30, 10, 10, 10};
const s32 gRangeFalloff_HealPp[] = {100, 80, 60, 40, 20, 10};
const s32 gRangeFalloff_EffectDamage[] = {100, 50, 30, 10, 10, 10};

const u8 gBattleAI_ActionWeights[3][8] = {
    {32, 32, 32, 32, 32, 32, 32, 32},
    {53, 47, 41, 35, 29, 23, 17, 11},
    {113, 56, 31, 20, 14, 10, 7, 5},
};
