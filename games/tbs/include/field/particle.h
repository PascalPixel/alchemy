#ifndef ALCHEMY_PARTICLE_H
#define ALCHEMY_PARTICLE_H

// Particle and SCREEN_POINT, the positions animations move through.

#include "edition.h"
#include "fixed_math.h"
#include "static_assert.h"
#include "types.h"

// A particle of an animation: its position, its velocity and a value each animation uses its own
// way (a size, a timer, -1 for idle).
typedef struct PARTICLE
{
#if FIX_POINTER_OUTSIDE_OBJECT
	union
	{
		struct
		{
			FIXED x; // 16.16 fixed-point pixels
			FIXED y; // 16.16 fixed-point pixels
			FIXED z; // 16.16 fixed-point pixels
		};

		Sint32 coordinates[3];
	};
#else
	FIXED x; // 16.16 fixed-point pixels
	FIXED y; // 16.16 fixed-point pixels
	FIXED z; // 16.16 fixed-point pixels
#endif
	FIXED  velocityX; // 16.16 fixed-point pixels per tick
	FIXED  velocityY; // 16.16 fixed-point pixels per tick
	FIXED  velocityZ; // 16.16 fixed-point pixels per tick
	Sint32 variant;   // each animation's own: a size, a countdown, -1 for idle
} PARTICLE;

// Returns the particle's three coordinates as an array. UB: indexing from &x crosses the particle's
// separate scalar members. UBFIX supplies a real coordinate array without including motion state.
static inline const Sint32 *btParticleCoordinates(const PARTICLE *step)
{
#if FIX_POINTER_OUTSIDE_OBJECT
	return step->coordinates;
#else
	return &step->x;
#endif
}

// A point projected onto the screen, with its depth.
typedef struct SCREEN_POINT
{
#if FIX_POINTER_OUTSIDE_OBJECT
	// Projection writes three coordinates; animations read their named components.
	union
	{
		struct
		{
			Sint32 x;     // screen pixels
			Sint32 y;     // screen pixels
			Sint32 depth; // projected depth, larger further away
		};

		Sint32 coordinates[3];
	};
#else
	Sint32 x;     // screen pixels
	Sint32 y;     // screen pixels
	Sint32 depth; // projected depth, larger further away
#endif
} SCREEN_POINT;

// Returns the position's three coordinates as an array. UB: indexing from &x crosses separate
// scalar members of the point. UBFIX provides a real array while keeping the animations' named
// coordinates.
static inline Sint32 *syScreenPointCoordinates(SCREEN_POINT *pos)
{
#if FIX_POINTER_OUTSIDE_OBJECT
	return pos->coordinates;
#else
	return &pos->x;
#endif
}

STATIC_ASSERT(offsetof(PARTICLE, x) == 0);
STATIC_ASSERT(offsetof(PARTICLE, y) == 4);
STATIC_ASSERT(offsetof(PARTICLE, z) == 8);
STATIC_ASSERT(offsetof(PARTICLE, velocityX) == 12);
STATIC_ASSERT(offsetof(PARTICLE, velocityY) == 16);
STATIC_ASSERT(offsetof(PARTICLE, velocityZ) == 20);
STATIC_ASSERT(offsetof(PARTICLE, variant) == 24);
STATIC_ASSERT(sizeof(PARTICLE) == 28);
STATIC_ASSERT(sizeof(SCREEN_POINT) == 12);

#endif
