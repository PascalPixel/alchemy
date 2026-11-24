#ifndef ALCHEMY_BATTLE_TYPE_H
#define ALCHEMY_BATTLE_TYPE_H

/// @file
/// BattlePlan: one action laid out target by target for its staging.

#include "static_assert.h"
#include "battle/battle_unit.h"

/// The most targets one action has (BattlePlan), and a plan's size in bytes.
enum { BATTLE_TARGET_CAPACITY = 14, BATTLE_PLAN_SIZE = 100 };

/// One action, laid out for BattleAbility_ApplyToTarget to apply target by
/// target.
struct BattlePlan {
  u8 unitId;      ///< the acting unit
  s8 targetCount; ///< targets in targetIds
  /// The other unit of Meld and Mold: the strongest ally Meld joins, whose
  /// Attack adds a quarter, or the unit Mold takes the place of.
  u8 partnerId;
  u8 targetIds[BATTLE_TARGET_CAPACITY];
  /// Each target's place from the aimed unit's, which sets the range falloff.
  s8 targetOffset[BATTLE_TARGET_CAPACITY];
  /// The blows that land on each target: 0 a miss, 2 or 3 a double or triple.
  s8 targetMultiplier[BATTLE_TARGET_CAPACITY];
  s8 targetCritical[BATTLE_TARGET_CAPACITY]; ///< enum BattleCritical
  /// Whether the ability's effect takes on each target, or HIT_NOT_ROLLED.
  s8 targetHit[BATTLE_TARGET_CAPACITY];
  u8 paddingBeforeCommand;

  s16 command; ///< enum BattleCommand
  s32 abilityId;
  s32 element; ///< enum Element
  s32 staging; ///< enum BattleStaging
  /// How the effect is staged.
  /// bits: 16 on downed | 14 by id range | 11..0 effect kind
  u32 battleEffect;

  s32 failure; ///< 0 cast; 1 the Psynergy is blocked, 2 too little PP
  /// The damage the evil spirit deals a haunted actor at the end of its turn.
  u32 hauntDamage;
};

STATIC_ASSERT(sizeof(struct BattlePlan) == BATTLE_PLAN_SIZE);
STATIC_ASSERT(offsetof(struct BattlePlan, partnerId) == 2);
STATIC_ASSERT(offsetof(struct BattlePlan, targetIds) == 3);
STATIC_ASSERT(offsetof(struct BattlePlan, targetMultiplier) == 31);
STATIC_ASSERT(offsetof(struct BattlePlan, command) == 74);
STATIC_ASSERT(offsetof(struct BattlePlan, abilityId) == 76);
STATIC_ASSERT(offsetof(struct BattlePlan, hauntDamage) == 96);

#endif
