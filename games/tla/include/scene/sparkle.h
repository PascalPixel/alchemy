#ifndef ALCHEMY_SPARKLE_H
#define ALCHEMY_SPARKLE_H

/* @file */
/* The sparkle that rises round an object while it uses Psynergy in an event */
/* (Contigo, Mars Lighthouse's aerie, the Psynergy test): each scene keeps its
 */
/* own copy of the functions. */

#include "types.h"
#include "field/effect_sprite.h"

struct ACTOR;

/* The sparkle over an object: its two pairs of sprites, how far each pair has
 */
/* risen (from 0 to RING_CLIMB - 1; RING_NOT_STARTED before it starts) and the
 */
/* pair to start next. */
typedef struct SPARKLE_STATE {
	EFFECT_SPR sprs[4];
	s16 heights[2];
	s16 vramEntry;
	s16 nextPair;
	struct ACTOR *actor;
} SPARKLE_STATE;

/* How far a ring climbs, in pixels, how many frames apart the rings start, */
/* and the height of one not yet started. */
#define RING_CLIMB		 32
#define RING_INTERVAL	 16
#define RING_NOT_STARTED 999

/* The sparkle's frame counter in ACTOR.sceneByte: bit 0 alternates, the */
/* rest picks the colour. */
typedef struct COUNTER_BITS {
	u8 low : 1;
	u8 rest : 7;
} COUNTER_BITS;

#endif
