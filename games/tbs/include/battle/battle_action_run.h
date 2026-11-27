#ifndef ALCHEMY_BATTLE_ACTION_RUN_H
#define ALCHEMY_BATTLE_ACTION_RUN_H

/* BATTLE_ACTION_RUN, an action planned for its targets: the hits, criticals and
   multipliers rolled. */

#include "static_assert.h"
#include "battle/battle_unit.h"

enum {
	/* Targets an action can hold: as many units as BATTLE_STATE.objs holds, 8
	   party places and 6 enemies. */
	BATTLE_TARGET_CAPACITY = 14,
	BATTLE_ACTION_RUN_SIZE = 100
};

/* BATTLE_ACTION_RUN.tgtCritical: the critical hit rolled for each target. */
typedef enum BATTLE_CRITICAL {
	CRITICAL_NONE,
	CRITICAL_HIT,	/* an attack's: a quarter more damage */
	CRITICAL_STRONG /* an ability's (effects 36 to 40): half as much again */
} BATTLE_CRITICAL;

/* BATTLE_ACTION_RUN.tgtHit while the hit is still to be rolled, as the ability
   lands; btActionRunBuild rolls it ahead only for Annihilation. */
#define HIT_NOT_ROLLED -1

/* BATTLE_ACTION_RUN.failure: why a cast plays no effect, which the staging
   shows. */
typedef enum BATTLE_ACTION_FAILURE {
	ACTION_FAILURE_NONE,
	ACTION_FAILURE_SEALED, /* "blocked": the user's Psynergy is sealed */
	ACTION_FAILURE_NO_PP   /* the user has too little PP */
} BATTLE_ACTION_FAILURE;

/* One action, as btActionRunBuild lays it out for btAbilityApplyToTarget to
   apply target by target. */
typedef struct BATTLE_ACTION_RUN {
	Uint8 unitId;						  /* the acting unit */
	Sint8 tgtCnt;						  /* targets in tgtIds */
	Uint8 tgtIds[BATTLE_TARGET_CAPACITY]; /* in battlefield order */
	/* Each target's place relative to the aimed one: negative to one side. */
	Sint8 tgtOfs[BATTLE_TARGET_CAPACITY];
	/* The damage multiplier: 0 a miss, 1 a blow, 2 or 3 a double or triple. */
	Sint8  tgtMultiplier[BATTLE_TARGET_CAPACITY];
	Sint8  tgtCritical[BATTLE_TARGET_CAPACITY]; /* BATTLE_CRITICAL */
	Sint8  tgtHit[BATTLE_TARGET_CAPACITY];		/* 1, 0 or HIT_NOT_ROLLED */
	Sint16 command;								/* BATTLE_COMMAND */
	Uint8  padding[2]; /* up to the word abilityId starts on */
	Sint32 abilityId;
	Sint32 element; /* ELEMENT */
	Sint32 staging; /* BATTLE_STAGING: how the action is shown */
	/* The effect word: how it is staged (STAGING_ in battle_internal.h). bits:
	   19 view reversed | 18 view across | 17 other side | 16 on downed | 15
	   large effect | 14 by id range | 13..12 variant | 11..0 effect kind */
	Uint32 battleEff;
	Sint32 failure; /* BATTLE_ACTION_FAILURE */
	/* The damage the evil spirit deals a haunted unit at the end of its
	   action. */
	Uint32 hauntDamage;
} BATTLE_ACTION_RUN;

STATIC_ASSERT(sizeof(BATTLE_ACTION_RUN) == BATTLE_ACTION_RUN_SIZE);
STATIC_ASSERT(offsetof(BATTLE_ACTION_RUN, tgtIds) == 2);
STATIC_ASSERT(offsetof(BATTLE_ACTION_RUN, tgtMultiplier) == 30);
STATIC_ASSERT(offsetof(BATTLE_ACTION_RUN, abilityId) == 76);
STATIC_ASSERT(offsetof(BATTLE_ACTION_RUN, hauntDamage) == 96);

#endif
