#ifndef ALCHEMY_MERCURY_LIGHTHOUSE_LIGHTS_H
#define ALCHEMY_MERCURY_LIGHTHOUSE_LIGHTS_H

/* The lights the pool of Mercury Lighthouse and its entrance give the leader,
   which hold him up as he hops across the water
   (shared/mercury_lighthouse_lights.c and
   shared/mercury_lighthouse_pillar.c). */

#include "fixed_math.h"
#include "static_assert.h"
#include "types.h"

struct ACTOR;

/* The metatile behaviors Ply gives the pool's metatiles in
   MercuryLighthouseEntrance and MercuryLighthouseLower, which the hops and the
   lights answer. */
typedef enum MERCURY_LIGHTHOUSE_POOL_BEHAVIOR {
	/* The pool's heart, which gives the leader lights. */
	METATILE_BEHAVIOR_MERCURY_LIGHTHOUSE_POOL_HEART = 11,
	/* Out in the water, where ripples spread from the leader. */
	METATILE_BEHAVIOR_MERCURY_LIGHTHOUSE_POOL_WATER = 21,
	/* A plain stepping square of the pool or the floor at its edge, which takes
	   the leader's lights. */
	METATILE_BEHAVIOR_MERCURY_LIGHTHOUSE_POOL_STEP = 31
} MERCURY_LIGHTHOUSE_POOL_BEHAVIOR;

/* Published in the scene's slot for the leader's lights while the pillar is
   active. */
typedef struct MERCURY_LIGHTHOUSE_LIGHTS {
	/* The leader's lights, 0 to 3: the heart gives three and each hop into the
	   pool spends one. */
	Sint32 levels;
	Sint32 poolBehavior; /* the pool's metatile behavior, the floor's region */
	/* Ticks to the next ripple in
	   METATILE_BEHAVIOR_MERCURY_LIGHTHOUSE_POOL_WATER, 10 to 39; a hop zeroes
	   it. */
	Sint32 rippleTmr;
	/* 16.16 fixed-point pixels: the centre of the column's metatile, where a
	   sinking leader is put back. */
	FIXED		  tgtX;
	FIXED		  tgtZ;
	struct ACTOR *pillar; /* the column of light over the leader, or null */
	/* The light holding the leader up where they landed in the pool, or
	   null. */
	struct ACTOR *stepEff;
} MERCURY_LIGHTHOUSE_LIGHTS;

STATIC_ASSERT(sizeof(MERCURY_LIGHTHOUSE_LIGHTS) == 28);

/* Keeps the lights' state for the floor's region; returning to where the party
   was, the column and the light under the leader are made again. */
void scMercuryLighthouseRestoreLights(
	Sint32 region, MERCURY_LIGHTHOUSE_LIGHTS *st);

/* The pool's shimmer, one counter for each colour channel
   (shared/mercury_lighthouse_palette_cycle.c), which each scene defines. */
extern Sint32 PaletteCycleRed;
extern Sint32 PaletteCycleGreen;
extern Sint32 PaletteCycleBlue;

#endif
