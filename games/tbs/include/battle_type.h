#ifndef ALCHEMY_BATTLE_TYPE_H
#define ALCHEMY_BATTLE_TYPE_H

/// @file
/// BattleTurn, an action planned for its targets: the hits, criticals and
/// multipliers rolled.

#include "static_assert.h"
#include "battle_unit.h"

enum {
  /// Targets a turn can hold: as many units as BattleWork.objects holds, 8
  /// party places and 6 enemies.
  BATTLE_TARGET_CAPACITY = 14,
  BATTLE_TURN_SIZE = 0x64
};

/// BattleTurn.targetCritical: the critical hit rolled for each target.
enum BattleCritical {
  CRITICAL_NONE,
  CRITICAL_HIT,   ///< an attack's: a quarter more damage
  CRITICAL_STRONG ///< an ability's (effects 36 to 40): half as much again
};

/// BattleTurn.targetHit while the hit is still to be rolled, as the ability
/// lands; BattleTurn_Build rolls it ahead only for Annihilation.
#define HIT_NOT_ROLLED -1

/// BattleTurn.failure: why a cast plays no animation, which the staging shows.
enum BattleTurnFailure {
  TURN_FAILURE_NONE,
  TURN_FAILURE_SEALED, ///< "blocked": the user's Psynergy is sealed
  TURN_FAILURE_NO_PP   ///< the user has too little PP
};

/// One action, as BattleTurn_Build lays it out for BattleAbility_ApplyToTarget
/// to apply target by target.
struct BattleTurn {
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
  /// The animation word: how it is staged (STAGING_ in battle_internal.h).
  /// bits: 19 view reversed | 18 view across | 17 other side | 16 on downed |
  /// 15 large animation | 14 by id range | 13..12 variant | 11..0 animation
  /// kind
  u32 animation;
  s32 failure; ///< enum BattleTurnFailure
  /// The damage the evil spirit deals a haunted actor at the end of its turn.
  u32 hauntDamage;
};

STATIC_ASSERT(sizeof(struct BattleTurn) == BATTLE_TURN_SIZE);
STATIC_ASSERT(offsetof(struct BattleTurn, targetIds) == 0x02);
STATIC_ASSERT(offsetof(struct BattleTurn, targetMultiplier) == 0x1e);
STATIC_ASSERT(offsetof(struct BattleTurn, abilityId) == 0x4c);
STATIC_ASSERT(offsetof(struct BattleTurn, hauntDamage) == 0x60);

#endif
