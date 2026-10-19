#ifndef ALCHEMY_BATTLE_TYPE_H
#define ALCHEMY_BATTLE_TYPE_H

/// @file
/// BattlePlan, an action planned for its targets: the hits, criticals and
/// multipliers rolled.

#include "static_assert.h"
#include "battle_unit.h"

enum { BATTLE_TARGET_CAPACITY = 14, BATTLE_PLAN_SIZE = 0x64 };

/// BattlePlan.targetCritical: the critical hit rolled for each target.
enum BattleCritical {
  CRITICAL_NONE,
  CRITICAL_HIT,   ///< an attack's: a quarter more damage
  CRITICAL_STRONG ///< an ability's (effects 36 to 40): half as much again
};

/// BattlePlan.targetHit while the hit is still to be rolled, as the ability
/// lands; BattleCommand_BuildPlan rolls it ahead only for Annihilation.
#define HIT_NOT_ROLLED -1

/// One action, as BattleCommand_BuildPlan lays it out for BattleTargetExec to
/// apply target by target.
struct BattlePlan {
  u8 actorId;
  s8 targetCount;
  u8 targetIds[BATTLE_TARGET_CAPACITY];
  /// Each target's place relative to the aimed one: negative to one side.
  s8 targetOffset[BATTLE_TARGET_CAPACITY];
  /// The damage multiplier: 0 a miss, 1 a blow, 2 or 3 a double or triple.
  s8 targetMultiplier[BATTLE_TARGET_CAPACITY];
  s8 targetCritical[BATTLE_TARGET_CAPACITY]; ///< enum BattleCritical
  s8 targetHit[BATTLE_TARGET_CAPACITY];      ///< 1, 0 or HIT_NOT_ROLLED
  s16 command;                               ///< enum BattleCommand
  u8 unk4a[2];
  s32 abilityId;
  s32 element; ///< enum Element
  s32 outcome;
  u32 presentationFlags;
  s32 failure;
  /// The damage the evil spirit deals a haunted actor at the end of its turn.
  u32 hauntDamage;
};

STATIC_ASSERT(sizeof(struct BattlePlan) == BATTLE_PLAN_SIZE);
STATIC_ASSERT(offsetof(struct BattlePlan, targetIds) == 0x02);
STATIC_ASSERT(offsetof(struct BattlePlan, targetMultiplier) == 0x1e);
STATIC_ASSERT(offsetof(struct BattlePlan, abilityId) == 0x4c);
STATIC_ASSERT(offsetof(struct BattlePlan, hauntDamage) == 0x60);

#endif
