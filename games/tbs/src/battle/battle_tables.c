/// @file
/// The battle's small tables: where a formation's places fall, the party's
/// weapon sprites, the camera's command matrix, how an ability weakens with
/// distance, and the enemies' action weights.

#include "fixed_math.h"
#include "macros.h"
#include "sprite_ids.h"
#include "types.h"
#include "battle/battle.h"
#include "battle/range_falloff.h"
#include "battle/weapon_kind.h"
#include "game/character.h"

/// Unused: a word no code names, at the head of the tables; The Lost Age's is
/// 17.
const u32 gBattleTables_UnusedWord = 16;

const s8 gBattleFormation_CenteredOffsets[] = {0, -1, 1, -2, 2, -3, 3, -4, 4};

// clang-format off
/// Isaac's battle sprite for each kind of weapon he holds.
const u16 gBattleUnit_WeaponAnimationsIsaac[] ALIGNED(4) = {
    [WEAPON_KIND_LONG_SWORD]  = SPRITE_WEAPON_ISAAC_SWORD,
    [WEAPON_KIND_LIGHT_BLADE] = SPRITE_WEAPON_ISAAC_LIGHT_BLADE,
    [WEAPON_KIND_AXE]         = SPRITE_WEAPON_ISAAC_AXE,
    [WEAPON_KIND_MACE]        = SPRITE_WEAPON_ISAAC_MACE,
    [WEAPON_KIND_STAFF]       = SPRITE_WEAPON_ISAAC_SWORD,
    [WEAPON_KIND_ANKH]        = SPRITE_WEAPON_ISAAC_SWORD,
    [WEAPON_KIND_NONE]        = SPRITE_WEAPON_ISAAC_SWORD,
};
/// Garet's battle sprite for each kind of weapon he holds.
const u16 gBattleUnit_WeaponAnimationsGaret[] = {
    [WEAPON_KIND_LONG_SWORD]  = SPRITE_WEAPON_GARET_SWORD,
    [WEAPON_KIND_LIGHT_BLADE] = SPRITE_WEAPON_GARET_SWORD,
    [WEAPON_KIND_AXE]         = SPRITE_WEAPON_GARET_AXE,
    [WEAPON_KIND_MACE]        = SPRITE_WEAPON_GARET_MACE,
    [WEAPON_KIND_STAFF]       = SPRITE_WEAPON_GARET_MACE,
    [WEAPON_KIND_ANKH]        = SPRITE_WEAPON_GARET_MACE,
    [WEAPON_KIND_NONE]        = SPRITE_WEAPON_GARET_MACE,
};
/// Ivan's battle sprite for each kind of weapon he holds.
const u16 gBattleUnit_WeaponAnimationsIvan[] = {
    [WEAPON_KIND_LONG_SWORD]  = SPRITE_WEAPON_IVAN_BLADE,
    [WEAPON_KIND_LIGHT_BLADE] = SPRITE_WEAPON_IVAN_BLADE,
    [WEAPON_KIND_AXE]         = SPRITE_WEAPON_IVAN_BLADE,
    [WEAPON_KIND_MACE]        = SPRITE_WEAPON_IVAN_BLADE,
    [WEAPON_KIND_STAFF]       = SPRITE_WEAPON_IVAN_STAFF,
    [WEAPON_KIND_ANKH]        = SPRITE_WEAPON_IVAN_ANKH,
    [WEAPON_KIND_NONE]        = SPRITE_WEAPON_IVAN_STAFF,
};
/// Mia's battle sprite for each kind of weapon she holds.
const u16 gBattleUnit_WeaponAnimationsMia[] = {
    [WEAPON_KIND_LONG_SWORD]  = SPRITE_WEAPON_MIA_ANKH,
    [WEAPON_KIND_LIGHT_BLADE] = SPRITE_WEAPON_MIA_ANKH,
    [WEAPON_KIND_AXE]         = SPRITE_WEAPON_MIA_MACE,
    [WEAPON_KIND_MACE]        = SPRITE_WEAPON_MIA_MACE,
    [WEAPON_KIND_STAFF]       = SPRITE_WEAPON_MIA_STAFF,
    [WEAPON_KIND_ANKH]        = SPRITE_WEAPON_MIA_ANKH,
    [WEAPON_KIND_NONE]        = SPRITE_WEAPON_MIA_ANKH,
};
/// Jenna's battle sprite for each kind of weapon she holds.
const u16 gBattleUnit_WeaponAnimationsJenna[] = {
    [WEAPON_KIND_LONG_SWORD]  = SPRITE_WEAPON_JENNA_BLADE,
    [WEAPON_KIND_LIGHT_BLADE] = SPRITE_WEAPON_JENNA_BLADE,
    [WEAPON_KIND_AXE]         = SPRITE_WEAPON_JENNA_STAFF,
    [WEAPON_KIND_MACE]        = SPRITE_WEAPON_JENNA_STAFF,
    [WEAPON_KIND_STAFF]       = SPRITE_WEAPON_JENNA_STAFF,
    [WEAPON_KIND_ANKH]        = SPRITE_WEAPON_JENNA_STAFF,
    [WEAPON_KIND_NONE]        = SPRITE_WEAPON_JENNA_STAFF,
};

const s8 gBattlePlacement_Spots[] = {
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

/// A shear: x gains -0.8 of y. No code reads the three words after the
/// matrix.
const FIXED gBattleCamera_CommandMatrix[] = {
    FIXED_ONE,     0,         0,
    toFIXED(-0.8), FIXED_ONE, 0,
    0,             0,         FIXED_ONE,
    0,             0,         0,
    toFIXED(80),   0,         0,
};
// clang-format on

// Percentages by a target's distance from the aimed one, 0 to 5
// (range_falloff.h).
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
    [ACTION_STRATEGY_EVEN] = {32, 32, 32, 32, 32, 32, 32, 32},
    [ACTION_STRATEGY_FALLING] = {53, 47, 41, 35, 29, 23, 17, 11},
    [ACTION_STRATEGY_FIRST_FAVOURED] = {113, 56, 31, 20, 14, 10, 7, 5},
};
