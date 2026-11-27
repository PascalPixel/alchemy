#ifndef ALCHEMY_BATTLE_TYPE_H
#define ALCHEMY_BATTLE_TYPE_H

/* BATTLE_PLAN: one action laid out target by target for its staging. */

#include "static_assert.h"
#include "battle/battle_unit.h"

/* The most targets one action has (BATTLE_PLAN), and a plan's size in bytes. */
enum { BATTLE_TARGET_CAPACITY = 14, BATTLE_PLAN_SIZE = 100 };

/* One action, laid out for btAbilityApplyToTarget to apply target by target. */
typedef struct BATTLE_PLAN {
	Uint8 unitId;	   /* the acting unit */
	Sint8 targetCount; /* targets in targetIds */
	/* The other unit of Meld and Mold: the strongest ally Meld joins, whose
	   Attack adds a quarter, or the unit Mold takes the place of. */
	Uint8 partnerId;
	Uint8 targetIds[BATTLE_TARGET_CAPACITY];
	/* Each target's place from the aimed unit's, which sets the range
	   falloff. */
	Sint8 targetOffset[BATTLE_TARGET_CAPACITY];
	/* The blows that land on each target: 0 a miss, 2 or 3 a double or
	   triple. */
	Sint8 targetMultiplier[BATTLE_TARGET_CAPACITY];
	Sint8 targetCritical[BATTLE_TARGET_CAPACITY]; /* BATTLE_CRITICAL */
	/* Whether the ability's effect takes on each target, or HIT_NOT_ROLLED. */
	Sint8 targetHit[BATTLE_TARGET_CAPACITY];
	Uint8 paddingBeforeCommand;

	Sint16 command; /* BATTLE_COMMAND */
	Sint32 abilityId;
	Sint32 element; /* ELEMENT */
	Sint32 staging; /* BATTLE_STAGING */
	/* How the effect is staged. bits: 16 on downed | 14 by id range | 11..0
	   effect kind */
	Uint32 battleEffect;

	Sint32 failure; /* 0 cast; 1 the Psynergy is blocked, 2 too little PP */
	/* The damage the evil spirit deals a haunted actor at the end of its
	   turn. */
	Uint32 hauntDamage;
} BATTLE_PLAN;

STATIC_ASSERT(sizeof(BATTLE_PLAN) == BATTLE_PLAN_SIZE);
STATIC_ASSERT(offsetof(BATTLE_PLAN, partnerId) == 2);
STATIC_ASSERT(offsetof(BATTLE_PLAN, targetIds) == 3);
STATIC_ASSERT(offsetof(BATTLE_PLAN, targetMultiplier) == 31);
STATIC_ASSERT(offsetof(BATTLE_PLAN, command) == 74);
STATIC_ASSERT(offsetof(BATTLE_PLAN, abilityId) == 76);
STATIC_ASSERT(offsetof(BATTLE_PLAN, hauntDamage) == 96);

#endif
