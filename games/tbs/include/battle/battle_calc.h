#ifndef ALCHEMY_BATTLE_CALC_H
#define ALCHEMY_BATTLE_CALC_H

/// @file
/// The battle calculation tables: hit chances, HP and PP damage and recovery.

#include "types.h"
#include "edition.h"
#include "farcall.h"

// Percentages by a target's distance from the aimed position, 0 for the aimed
// target to 5 for the farthest: BattleTargetExec scales an amount by the entry
// and divides by 100, so an ability spread over a group weakens step by step.
// gHitTable instead scales the effect's chance, through BattleHitCheck.
/// The share of an effect's chance to land (BattleFx_RollSuccess's
/// successScale): 100, 60, then 30.
extern u8 gHitTable[];
/// PP loss (ABILITY_KIND_PP_DAMAGE), Crazy Voice's and Psy Drain's: 100, 50,
/// 30, then 10.
extern s32 gPpLossTable[];
/// HP healing (ABILITY_KIND_HEAL_HP): 100 down to 20 by 20, then 10.
extern s32 gHpRecoveryTable[];
/// PP damage of the never-reached ability kind -1: 100, 50, 30, then 10.
extern s32 gPpDmgTable[];
/// Psynergy damage (ABILITY_KIND_DAMAGE): 100 down to 20 by 20, then 10.
extern s32 gHpDmgTable5[];
/// Summon damage (ABILITY_KIND_SUMMON): 100, 70, 40, 30, 20, then 10.
extern s32 gHpDmgTable8[];
/// An enemy's unleashed damage (ABILITY_KIND_UNLEASHED_DAMAGE): 100, 50, 30,
/// then 10.
extern s32 gHpDmgTable6[];
/// PP healing (ABILITY_KIND_HEAL_PP): 100 down to 20 by 20, then 10.
extern s32 gPpRecoveryTable[];
/// The damage of an effect ability whose effect lands (ABILITY_KIND_EFFECT):
/// 100, 50, 30, then 10.
extern s32 gHpDmgTable[];

#endif
