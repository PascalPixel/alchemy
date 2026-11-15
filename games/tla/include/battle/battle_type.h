#ifndef ALCHEMY_BATTLE_TYPE_H
#define ALCHEMY_BATTLE_TYPE_H

#include "static_assert.h"
#include "battle/battle_unit.h"

enum { BATTLE_TARGET_CAPACITY = 14, BATTLE_PLAN_SIZE = 0x64 };

/// One action, laid out for BattleAbility_ApplyToTarget to apply target by
/// target.
struct BattlePlan {
  u8 unitId;      ///< the acting unit
  s8 targetCount; ///< targets in targetIds
  /// The other unit of Meld and Mold: the strongest ally Meld joins, whose
  /// Attack adds a quarter, or the unit Mold takes the place of.
  u8 partnerId;
  u8 targetIds[BATTLE_TARGET_CAPACITY];
  s8 targetOffset[BATTLE_TARGET_CAPACITY];
  s8 targetMultiplier[BATTLE_TARGET_CAPACITY];
  s8 targetCritical[BATTLE_TARGET_CAPACITY];
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
STATIC_ASSERT(offsetof(struct BattlePlan, partnerId) == 0x02);
STATIC_ASSERT(offsetof(struct BattlePlan, targetIds) == 0x03);
STATIC_ASSERT(offsetof(struct BattlePlan, targetMultiplier) == 0x1f);
STATIC_ASSERT(offsetof(struct BattlePlan, command) == 0x4a);
STATIC_ASSERT(offsetof(struct BattlePlan, abilityId) == 0x4c);
STATIC_ASSERT(offsetof(struct BattlePlan, hauntDamage) == 0x60);

#endif
