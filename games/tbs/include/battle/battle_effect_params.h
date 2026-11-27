#ifndef ALCHEMY_BATTLE_EFFECT_PARAMS_H
#define ALCHEMY_BATTLE_EFFECT_PARAMS_H

/* BATTLE_EFFECT_PARM, the command an effect's caller, dispatcher and handlers
   share, and the macros that define effect kinds. */

#include "static_assert.h"
#include "types.h"

/* BATTLE_EFFECT_PARM.side: the side the targets are on. */
typedef enum BATTLE_EFFECT_SIDE {
	EFFECT_SIDE_ENEMIES,
	EFFECT_SIDE_PARTY
} BATTLE_EFFECT_SIDE;

/* What a critical hit adds to an attack's effect kind:
   btStagingRunAttack plays the critical version of the blow. Only plain attacks
   score critical hits, and their kinds are 1 to 5, all dispatched by id range,
   so it reaches kinds 201 to 205: the ring and spark effect, the same for
   each. */
#define EFFECT_ID_CRITICAL_OFFSET 200

/* The command shared by an effect's caller, dispatcher and handlers. */
typedef struct BATTLE_EFFECT_PARM {
	/* handler table index; a critical hit adds EFFECT_ID_CRITICAL_OFFSET */
	Sint32 kind;
	Sint32 side;		/* BATTLE_EFFECT_SIDE */
	Sint32 user;		/* the user's unit id */
	Sint32 firstTarget; /* the action's first target, standing or not */
	Sint32 range;		/* forwarded action range, otherwise initialized to 1 */
	Sint32 targetCount; /* entries in targets, at least 1 */
	Sint32 variant;	   /* the ability's size, from 0 (Quake, then Earthquake) */
	Sint32 prelude;	   /* 1 plays a prelude: in some effects a Djinni appears */
	Sint32 unused;	   /* no code names it */
	Sint16 targets[8]; /* the targets' unit ids, count of them */
} BATTLE_EFFECT_PARM;

STATIC_ASSERT(sizeof(BATTLE_EFFECT_PARM) == 52);
STATIC_ASSERT(offsetof(BATTLE_EFFECT_PARM, targetCount) == 20);
STATIC_ASSERT(offsetof(BATTLE_EFFECT_PARM, targets) == 36);

/* Loops `i` over the effect's targets, effect->targets[0] to
   effect->targets[effect->targetCount - 1]: the loops that flash, shake, strike
   or aim at each target in turn. */
#define EFFECT_FOR_EACH_TARGET(i, effect) \
	for((i) = 0; (i) != (effect)->targetCount; (i)++)

#endif
