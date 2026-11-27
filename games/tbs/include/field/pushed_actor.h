#ifndef ALCHEMY_PUSHED_ACTOR_H
#define ALCHEMY_PUSHED_ACTOR_H

/* The pushed actors' movement probes and rectangles. */

#include "static_assert.h"
#include "types.h"
#include "field/actor.h"
#include "field/scene.h"

/* A pixel step packed as one word: X in the upper halfword, Z in the lower.
   Packed through Uint32, so a negative step is its two's-complement bits. */
#define DIRECTION_STEP(dx, dz)		 (((Uint32)(dx) << 16) | ((Uint32)(dz) & 0xffff))
/* A DIRECTION_STEP's X, in pixels. */
#define DIRECTION_STEP_X(step)		 ((Sint32)(step) >> 16)
/* A DIRECTION_STEP's Z, in pixels. */
#define DIRECTION_STEP_Z(step)		 ((Sint16)(step))
/* A DIRECTION_STEP's X as a 16.16 offset: its upper halfword, kept in place. */
#define DIRECTION_STEP_X_FIXED(step) ((step) & 0xffff0000)
/* A DIRECTION_STEP's Z as a 16.16 offset: its lower halfword, shifted up. */
#define DIRECTION_STEP_Z_FIXED(step) INTtoFIXED(step)

/* PUSHED_ACTOR_PROBE.rectangleIdx of an actor whose sprite is none of the six
   large things, past their rows. */
#define PUSHED_ACTOR_NO_RECTANGLE 7

/* Where a pushed actor ahead of the leader would roll to: its rectangle, its
   actor, that position, and a function run during the leader's
   follow-through. */
typedef struct PUSHED_ACTOR_PROBE {
	/* The large thing's row in PushedActor_RectangleKinds and
	   PushedActor_Rectangles, 0 to 5. */
	Sint32 rectangleIdx;
	Sint32 actorSlot; /* the large thing's actor id, 8 to 65 */
	/* 16.16 pixels while moving; converted to metatiles by rectangle
	   redraws. */
	FIELD_POS pos;
	/* Run as the leader follows the rolling thing, when not null; the probe
	   clears it. */
	void (*func)(void);
} PUSHED_ACTOR_PROBE;

/* A large thing's edges, a row of PushedActor_Rectangles. */
typedef struct PUSHED_ACTOR_RECTANGLE {
	Sint32 x0; /* left edge, pixels from the thing's position */
	Sint32 z0; /* top edge, pixels from the thing's position */
	Sint32 x1; /* right edge, pixels from the thing's position */
	Sint32 z1; /* bottom edge, pixels from the thing's position */
} PUSHED_ACTOR_RECTANGLE;

STATIC_ASSERT(sizeof(PUSHED_ACTOR_PROBE) == 24);
STATIC_ASSERT(offsetof(PUSHED_ACTOR_PROBE, pos.x) == 8);
STATIC_ASSERT(offsetof(PUSHED_ACTOR_PROBE, pos.z) == 16);

STATIC_ASSERT(sizeof(PUSHED_ACTOR_RECTANGLE) == 16);

/* The pixel step of each direction, X in the upper halfword and Z in the lower
   (DIRECTION_STEP); each scene that pushes a large thing includes it
   (shared/pushed_actor_direction_steps.c). */
extern Sint32 PushedActor_DirectionSteps[];

#endif
