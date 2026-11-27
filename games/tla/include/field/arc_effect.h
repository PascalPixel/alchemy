#ifndef ALCHEMY_ARC_EFFECT_H
#define ALCHEMY_ARC_EFFECT_H

/* The swelling arcs a field Psynergy shows over its target. */

#include "types.h"
#include "field/effect_sprite.h"

struct ACTOR;

/* Ticks a pair of swelling arcs shows for, rising a pixel a tick. */
#define ARC_GROUP_TICKS 32

/* A pair's age before it is first spawned: past ARC_GROUP_TICKS, so not
   drawn. */
#define ARC_GROUP_UNSPAWNED 999

/* A field Psynergy's arc state, the block of heap slot HEAP_SLOT_ARC_EFFECT:
   two groups of two arc sprites, their ages, and the VRAM cache entry, group
   and target they were spawned with (fdBattleEffectSpawnScaledArcObjects). */
typedef struct ARC_EFFECT_STATE {
	EFFECT_SPR sprs[2][2];
	/* ticks since each group was spawned; ARC_GROUP_UNSPAWNED before */
	Sint16		  age[2];
	Sint16		  resource; /* VRAM cache entry holding the arcs' tiles */
	Sint16		  group;	/* the group spawned next */
	struct ACTOR *tgt;		/* the actor the last group was spawned over */
} ARC_EFFECT_STATE;

void fdBattleEffectSpawnScaledArcObjects(struct ACTOR *tgt);

#endif
