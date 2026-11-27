#ifndef ALCHEMY_BATTLE_TYPE_H
#define ALCHEMY_BATTLE_TYPE_H

/* @file */
/* BATTLE_PLAN: one action laid out target by target for its staging. */

#include "static_assert.h"
#include "battle/battle_unit.h"

/* The most targets one action has (BATTLE_PLAN), and a plan's size in bytes. */
enum { BATTLE_TARGET_CAPACITY = 14, BATTLE_PLAN_SIZE = 100 };

/* One action, laid out for btAbilityApplyToTarget to apply target by */
/* target. */
typedef struct BATTLE_PLAN {
	u8 unitId; /* the acting unit */
	s8 tgtCnt; /* targets in tgtIds */
	/* The other unit of Meld and Mold: the strongest ally Meld joins, whose */
	/* Attack adds a quarter, or the unit Mold takes the place of. */
	u8 partnerId;
	u8 tgtIds[BATTLE_TARGET_CAPACITY];
	/* Each target's place from the aimed unit's, which sets the range falloff.
	 */
	s8 tgtOfs[BATTLE_TARGET_CAPACITY];
	/* The blows that land on each target: 0 a miss, 2 or 3 a double or triple.
	 */
	s8 tgtMultiplier[BATTLE_TARGET_CAPACITY];
	s8 tgtCritical[BATTLE_TARGET_CAPACITY]; /* BATTLE_CRITICAL */
	/* Whether the ability's effect takes on each target, or HIT_NOT_ROLLED. */
	s8 tgtHit[BATTLE_TARGET_CAPACITY];
	u8 paddingBeforeCommand;

	s16 command; /* BATTLE_COMMAND */
	s32 abilityId;
	s32 element; /* ELEMENT */
	s32 staging; /* BATTLE_STAGING */
	/* How the effect is staged. */
	/* bits: 16 on downed | 14 by id range | 11..0 effect kind */
	u32 battleEff;

	s32 failure; /* 0 cast; 1 the Psynergy is blocked, 2 too little PP */
	/* The damage the evil spirit deals a haunted actor at the end of its turn.
	 */
	u32 hauntDamage;
} BATTLE_PLAN;

STATIC_ASSERT(sizeof(BATTLE_PLAN) == BATTLE_PLAN_SIZE);
STATIC_ASSERT(offsetof(BATTLE_PLAN, partnerId) == 2);
STATIC_ASSERT(offsetof(BATTLE_PLAN, tgtIds) == 3);
STATIC_ASSERT(offsetof(BATTLE_PLAN, tgtMultiplier) == 31);
STATIC_ASSERT(offsetof(BATTLE_PLAN, command) == 74);
STATIC_ASSERT(offsetof(BATTLE_PLAN, abilityId) == 76);
STATIC_ASSERT(offsetof(BATTLE_PLAN, hauntDamage) == 96);

#endif
