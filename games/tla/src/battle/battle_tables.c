// The battle's small tables: where a formation's enemies stand, the party's weapon sprites, the
// camera's command matrix, how an ability weakens with distance, and the forms Dullahan takes.

#include "enemy_ids.h"
#include "fixed_math.h"
#include "macros.h"
#include "sprite_ids.h"
#include "battle/weapon_kind.h"

// Unused: a word no code names, at the head of the tables; The Broken Seal's is 16.
const Uint32 BattleTablesUnusedWord           = 17;

// Where a centred formation's enemies stand, as places from the middle of the line: the first in
// the middle, the rest either side in turn.
const Sint8  BattleFormationCenteredOffsets[] = {0, -1, 1, -2, 2, -3, 3, -4, 4};

// clang-format off
// Each by BATTLE_WEAPON_KIND. Isaac's battle sprite for each kind of weapon he holds.
const Uint16 BattleUnitWeaponAnimationsIsaac[] ALIGNED(4) = {
    [WEAPON_KIND_LONG_SWORD]  = SPRITE_WEAPON_ISAAC_SWORD,
    [WEAPON_KIND_LIGHT_BLADE] = SPRITE_WEAPON_ISAAC_LIGHT_BLADE,
    [WEAPON_KIND_AXE]         = SPRITE_WEAPON_ISAAC_AXE,
    [WEAPON_KIND_MACE]        = SPRITE_WEAPON_ISAAC_MACE,
    [WEAPON_KIND_STAFF]       = SPRITE_WEAPON_ISAAC_SWORD,
    [WEAPON_KIND_ANKH]        = SPRITE_WEAPON_ISAAC_SWORD,
    [WEAPON_KIND_NONE]        = SPRITE_WEAPON_ISAAC_SWORD,
};
// Garet's battle sprite for each kind of weapon he holds.
const Uint16 BattleUnitWeaponAnimationsGaret[] = {
    [WEAPON_KIND_LONG_SWORD]  = SPRITE_WEAPON_GARET_SWORD,
    [WEAPON_KIND_LIGHT_BLADE] = SPRITE_WEAPON_GARET_SWORD,
    [WEAPON_KIND_AXE]         = SPRITE_WEAPON_GARET_AXE,
    [WEAPON_KIND_MACE]        = SPRITE_WEAPON_GARET_MACE,
    [WEAPON_KIND_STAFF]       = SPRITE_WEAPON_GARET_MACE,
    [WEAPON_KIND_ANKH]        = SPRITE_WEAPON_GARET_MACE,
    [WEAPON_KIND_NONE]        = SPRITE_WEAPON_GARET_MACE,
};
// Ivan's battle sprite for each kind of weapon he holds.
const Uint16 BattleUnitWeaponAnimationsIvan[] = {
    [WEAPON_KIND_LONG_SWORD]  = SPRITE_WEAPON_IVAN_BLADE,
    [WEAPON_KIND_LIGHT_BLADE] = SPRITE_WEAPON_IVAN_BLADE,
    [WEAPON_KIND_AXE]         = SPRITE_WEAPON_IVAN_BLADE,
    [WEAPON_KIND_MACE]        = SPRITE_WEAPON_IVAN_BLADE,
    [WEAPON_KIND_STAFF]       = SPRITE_WEAPON_IVAN_STAFF,
    [WEAPON_KIND_ANKH]        = SPRITE_WEAPON_IVAN_ANKH,
    [WEAPON_KIND_NONE]        = SPRITE_WEAPON_IVAN_STAFF,
};
// Mia's battle sprite for each kind of weapon she holds.
const Uint16 BattleUnitWeaponAnimationsMia[] = {
    [WEAPON_KIND_LONG_SWORD]  = SPRITE_WEAPON_MIA_ANKH,
    [WEAPON_KIND_LIGHT_BLADE] = SPRITE_WEAPON_MIA_ANKH,
    [WEAPON_KIND_AXE]         = SPRITE_WEAPON_MIA_MACE,
    [WEAPON_KIND_MACE]        = SPRITE_WEAPON_MIA_MACE,
    [WEAPON_KIND_STAFF]       = SPRITE_WEAPON_MIA_STAFF,
    [WEAPON_KIND_ANKH]        = SPRITE_WEAPON_MIA_ANKH,
    [WEAPON_KIND_NONE]        = SPRITE_WEAPON_MIA_ANKH,
};
// Jenna's battle sprite for each kind of weapon she holds.
const Uint16 BattleUnitWeaponAnimationsJenna[] = {
    [WEAPON_KIND_LONG_SWORD]  = SPRITE_WEAPON_JENNA_BLADE,
    [WEAPON_KIND_LIGHT_BLADE] = SPRITE_WEAPON_JENNA_BLADE,
    [WEAPON_KIND_AXE]         = SPRITE_WEAPON_JENNA_BLADE,
    [WEAPON_KIND_MACE]        = SPRITE_WEAPON_JENNA_BLADE,
    [WEAPON_KIND_STAFF]       = SPRITE_WEAPON_JENNA_STAFF,
    [WEAPON_KIND_ANKH]        = SPRITE_WEAPON_JENNA_ANKH,
    [WEAPON_KIND_NONE]        = SPRITE_WEAPON_JENNA_STAFF,
};
// Felix's battle sprite for each kind of weapon he holds.
const Uint16 BattleUnitWeaponAnimationsFelix[] = {
    [WEAPON_KIND_LONG_SWORD]  = SPRITE_WEAPON_FELIX_SWORD,
    [WEAPON_KIND_LIGHT_BLADE] = SPRITE_WEAPON_FELIX_LIGHT_BLADE,
    [WEAPON_KIND_AXE]         = SPRITE_WEAPON_FELIX_AXE,
    [WEAPON_KIND_MACE]        = SPRITE_WEAPON_FELIX_MACE,
    [WEAPON_KIND_STAFF]       = SPRITE_WEAPON_FELIX_SWORD,
    [WEAPON_KIND_ANKH]        = SPRITE_WEAPON_FELIX_SWORD,
    [WEAPON_KIND_NONE]        = SPRITE_WEAPON_FELIX_SWORD,
};
// Sheba's battle sprite for each kind of weapon she holds.
const Uint16 BattleUnitWeaponAnimationsSheba[] = {
    [WEAPON_KIND_LONG_SWORD]  = SPRITE_WEAPON_SHEBA_MACE,
    [WEAPON_KIND_LIGHT_BLADE] = SPRITE_WEAPON_SHEBA_MACE,
    [WEAPON_KIND_AXE]         = SPRITE_WEAPON_SHEBA_MACE,
    [WEAPON_KIND_MACE]        = SPRITE_WEAPON_SHEBA_MACE,
    [WEAPON_KIND_STAFF]       = SPRITE_WEAPON_SHEBA_STAFF,
    [WEAPON_KIND_ANKH]        = SPRITE_WEAPON_SHEBA_ANKH,
    [WEAPON_KIND_NONE]        = SPRITE_WEAPON_SHEBA_STAFF,
};
// Piers's battle sprite for each kind of weapon he holds.
const Uint16 BattleUnitWeaponAnimationsPiers[] = {
    [WEAPON_KIND_LONG_SWORD]  = SPRITE_WEAPON_PIERS_SWORD,
    [WEAPON_KIND_LIGHT_BLADE] = SPRITE_WEAPON_PIERS_LIGHT_BLADE,
    [WEAPON_KIND_AXE]         = SPRITE_WEAPON_PIERS_AXE,
    [WEAPON_KIND_MACE]        = SPRITE_WEAPON_PIERS_MACE,
    [WEAPON_KIND_STAFF]       = SPRITE_WEAPON_PIERS_SWORD,
    [WEAPON_KIND_ANKH]        = SPRITE_WEAPON_PIERS_SWORD,
    [WEAPON_KIND_NONE]        = SPRITE_WEAPON_PIERS_SWORD,
};

// Where the party's places stand, x and y pixel pairs from the battlefield's centre, a 0, 0 pair after each layout; only the unused btPlacementGetStepPair reads it.
const Sint8 BattlePlacementSpots[] = {
    80, 48,
    80, 16,
    80, -16,
    80, -48,
    0, 0,
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

// The tilt the camera adds while commands are chosen: a shear that moves x by -0.8 of y.
const FIXED BattleCameraCommandMatrix[] = {
    toFIXED(1),     0,         0,
    toFIXED(-0.8), toFIXED(1), 0,
    0,             0,         toFIXED(1),
    0,             0,         0,
};
// clang-format on

// The same in both games: each a percent by the target's distance from the unit aimed at
// (battle_targeting.c). The chance an effect lands.
Uint8        RangeFalloffEffectChance[]       = {100, 60, 30, 30, 30, 30};
// The PP an effect drains.
Sint32       RangeFalloffPpLoss[]             = {100, 50, 30, 10, 10, 10};
// The HP healed.
Sint32       RangeFalloffHealHp[]             = {100, 80, 60, 40, 20, 10};
// The PP damage done.
Sint32       RangeFalloffPpDamage[]           = {100, 50, 30, 10, 10, 10};
// Psynergy's damage.
Sint32       RangeFalloffPsynergyDamage[]     = {100, 80, 60, 40, 20, 10};
// A summon's damage.
Sint32       RangeFalloffSummonDamage[]       = {100, 70, 40, 30, 20, 10};
// An enemy ability's damage.
Sint32       RangeFalloffEnemyDamage[]        = {100, 50, 30, 10, 10, 10};
// The PP restored.
Sint32       RangeFalloffHealPp[]             = {100, 80, 60, 40, 20, 10};
// An effect's own damage.
Sint32       RangeFalloffEffectDamage[]       = {100, 50, 30, 10, 10, 10};

// The enemy definitions Dullahan acts as, one drawn at random.
Uint16       BattleAIDullahanForms[]       = {ENEMY_DULLAHAN, ENEMY_DULLAHAN_B, ENEMY_DULLAHAN_C};

// Unused: three zeros after Dullahan's forms that no code reads.
Uint16       BattleAIUnusedDullahanForms[] = {0, 0, 0};
