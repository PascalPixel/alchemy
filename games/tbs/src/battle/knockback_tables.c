/* @file */
/* How far, how fast and how high each knockback sends a unit, by variant */
/* (BATTLE_KNOCKBACK). */

#include "battle/battle.h"

/* clang-format off */
FIXED BattleKnockback_Acceleration[] = {
    [KNOCKBACK_NUDGE]  = toFIXED(1),
    [KNOCKBACK_HOP]    = toFIXED(1),
    [KNOCKBACK_LEAP]   = toFIXED(1),
    [KNOCKBACK_LAUNCH] = toFIXED(1),
    [KNOCKBACK_HURL]   = toFIXED(8),
    [KNOCKBACK_SKID]   = toFIXED(1),
    [KNOCKBACK_BOUNCE] = toFIXED(1),
    [KNOCKBACK_JOLT]   = toFIXED(1),
};

FIXED BattleKnockback_SpeedLimit[] = {
    [KNOCKBACK_NUDGE]  = toFIXED(4),
    [KNOCKBACK_HOP]    = toFIXED(4),
    [KNOCKBACK_LEAP]   = toFIXED(4),
    [KNOCKBACK_LAUNCH] = toFIXED(4),
    [KNOCKBACK_HURL]   = toFIXED(20),
    [KNOCKBACK_SKID]   = toFIXED(4),
    [KNOCKBACK_BOUNCE] = toFIXED(4),
    [KNOCKBACK_JOLT]   = toFIXED(4),
};

FIXED BattleKnockback_VelocityY[] = {
    [KNOCKBACK_NUDGE]  = toFIXED(3),
    [KNOCKBACK_HOP]    = toFIXED(6),
    [KNOCKBACK_LEAP]   = toFIXED(7),
    [KNOCKBACK_LAUNCH] = toFIXED(10),
    [KNOCKBACK_HURL]   = toFIXED(5),
    [KNOCKBACK_SKID]   = toFIXED(2),
    [KNOCKBACK_BOUNCE] = toFIXED(3),
    [KNOCKBACK_JOLT]   = toFIXED(11) / 2,
};

s32 BattleKnockback_DistancePercent[] = {
    [KNOCKBACK_NUDGE]  = 140,
    [KNOCKBACK_HOP]    = 140,
    [KNOCKBACK_LEAP]   = 150,
    [KNOCKBACK_LAUNCH] = 200,
    [KNOCKBACK_HURL]   = 200,
    [KNOCKBACK_SKID]   = 150,
    [KNOCKBACK_BOUNCE] = 100,
    [KNOCKBACK_JOLT]   = 100,
};
/* clang-format on */

/* Unused: three words after the knockback tables that no code names. */
s32 BattleKnockback_UnusedWords[] = {0, 20, 27};
