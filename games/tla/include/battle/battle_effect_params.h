#ifndef ALCHEMY_BATTLE_EFFECT_PARAMS_H
#define ALCHEMY_BATTLE_EFFECT_PARAMS_H

/* @file */
/* The command a battle effect is run with: its kind, its user and its targets.
 */

#include "types.h"
#include "static_assert.h"

/* BATTLE_EFFECT_PARM.side: the side the targets are on. */
typedef enum BATTLE_EFFECT_SIDE {
	EFFECT_SIDE_ENEMIES,
	EFFECT_SIDE_PARTY
} BATTLE_EFFECT_SIDE;

/* What a critical hit adds to an attack's effect kind: btStagingRunAttack */
/* plays the critical version of the blow, as The Broken Seal's does. */
#define EFFECT_ID_CRITICAL_OFFSET 200

/* The command shared by an effect's caller, dispatcher and handlers. */
typedef struct BATTLE_EFFECT_PARM {
	s32 kind;	  /* the effect's index in the handler table */
	s32 side;	  /* 1 when the targets are the party, 0 for enemies */
	s32 user;	  /* the user's unit id */
	s32 firstTgt; /* the action's first target */
	s32 range;	  /* forwarded action range, otherwise initialized to 1 */
	s32 tgtCnt;	  /* how many of targets are in use */
	s32 variant;  /* the ability's size, from 0 */
	s32 prelude;  /* 1 plays a prelude: in some effects a Djinni appears */
	s32 unused;	  /* named by no code in either game */
	s16 tgts[8];  /* the targets' unit ids, targetCount of them */
} BATTLE_EFFECT_PARM;

STATIC_ASSERT(sizeof(BATTLE_EFFECT_PARM) == 52);
STATIC_ASSERT(offsetof(BATTLE_EFFECT_PARM, tgtCnt) == 20);
STATIC_ASSERT(offsetof(BATTLE_EFFECT_PARM, tgts) == 36);

/* Loops `i` over the effect's targets, effect->tgts[0] to */
/* effect->tgts[effect->tgtCnt - 1]: the loops that flash, shake, */
/* strike or aim at each target in turn. */
#define EFFECT_FOR_EACH_TARGET(i, effect)                                      \
	for((i) = 0; (i) != (effect)->tgtCnt; (i)++)

#endif
