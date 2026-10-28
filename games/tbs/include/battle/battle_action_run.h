#ifndef ALCHEMY_BATTLE_ACTION_RUN_H
#define ALCHEMY_BATTLE_ACTION_RUN_H

/// @file
/// BattleActionRun, an action planned for its targets: the hits, criticals and
/// multipliers rolled.

#include "static_assert.h"
#include "battle_unit.h"

enum {
  /// Targets an action can hold: as many units as BattleWork.objects holds, 8
  /// party places and 6 enemies.
  BATTLE_TARGET_CAPACITY = 14,
  BATTLE_ACTION_RUN_SIZE = 0x64
};

/// BattleActionRun.targetCritical: the critical hit rolled for each target.
enum BattleCritical {
  CRITICAL_NONE,
  CRITICAL_HIT,   ///< an attack's: a quarter more damage
  CRITICAL_STRONG ///< an ability's (effects 36 to 40): half as much again
};

/// BattleActionRun.targetHit while the hit is still to be rolled, as the
/// ability lands; BattleActionRun_Build rolls it ahead only for Annihilation.
#define HIT_NOT_ROLLED -1

/// BattleActionRun.failure: why a cast plays no effect, which the staging
/// shows.
enum BattleActionFailure {
  ACTION_FAILURE_NONE,
  ACTION_FAILURE_SEALED, ///< "blocked": the user's Psynergy is sealed
  ACTION_FAILURE_NO_PP   ///< the user has too little PP
};

/// One action, as BattleActionRun_Build lays it out for
/// BattleAbility_ApplyToTarget to apply target by target.
struct BattleActionRun {
  u8 unitId;                            ///< the acting unit
  s8 targetCount;                       ///< targets in targetIds
  u8 targetIds[BATTLE_TARGET_CAPACITY]; ///< in battlefield order
  /// Each target's place relative to the aimed one: negative to one side.
  s8 targetOffset[BATTLE_TARGET_CAPACITY];
  /// The damage multiplier: 0 a miss, 1 a blow, 2 or 3 a double or triple.
  s8 targetMultiplier[BATTLE_TARGET_CAPACITY];
  s8 targetCritical[BATTLE_TARGET_CAPACITY]; ///< enum BattleCritical
  s8 targetHit[BATTLE_TARGET_CAPACITY];      ///< 1, 0 or HIT_NOT_ROLLED
  s16 command;                               ///< enum BattleCommand
  u8 padding[2]; ///< up to the word abilityId starts on
  s32 abilityId;
  s32 element; ///< enum Element
  s32 staging; ///< enum BattleStaging: how the action is shown
  /// The effect word: how it is staged (STAGING_ in battle_internal.h).
  /// bits: 19 view reversed | 18 view across | 17 other side | 16 on downed |
  /// 15 large effect | 14 by id range | 13..12 variant | 11..0 effect
  /// kind
  u32 battleEffect;
  s32 failure; ///< enum BattleActionFailure
  /// The damage the evil spirit deals a haunted unit at the end of its action.
  u32 hauntDamage;
};

STATIC_ASSERT(sizeof(struct BattleActionRun) == BATTLE_ACTION_RUN_SIZE);
STATIC_ASSERT(offsetof(struct BattleActionRun, targetIds) == 0x02);
STATIC_ASSERT(offsetof(struct BattleActionRun, targetMultiplier) == 0x1e);
STATIC_ASSERT(offsetof(struct BattleActionRun, abilityId) == 0x4c);
STATIC_ASSERT(offsetof(struct BattleActionRun, hauntDamage) == 0x60);

#endif
