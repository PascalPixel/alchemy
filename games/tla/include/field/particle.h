#ifndef ALCHEMY_PARTICLE_H
#define ALCHEMY_PARTICLE_H

/* A particle's position and the screen points the effects draw at. */

#include "edition.h"
#include "fixed_math.h"
#include "types.h"
#include "static_assert.h"

/* A moving point of an effect: its position, its velocity and a value each
   effect uses its own way. */
typedef struct PARTICLE {
#if FIX_POINTER_OUTSIDE_OBJECT
	union {
		struct {
			FIXED x; /* 16.16 fixed-point pixels */
			FIXED y; /* 16.16 fixed-point pixels */
			FIXED z; /* 16.16 fixed-point pixels */
		};

		Sint32 coordinates[3];
	};
#else
	FIXED x; /* 16.16 fixed-point pixels */
	FIXED y; /* 16.16 fixed-point pixels */
	FIXED z; /* 16.16 fixed-point pixels */
#endif
	FIXED  velocityX; /* 16.16 fixed-point pixels per tick */
	FIXED  velocityY; /* 16.16 fixed-point pixels per tick */
	FIXED  velocityZ; /* 16.16 fixed-point pixels per tick */
	Sint32 variant;	  /* each effect's own */
} PARTICLE;

/* Returns the step's three coordinates as an array. */
/* UB: indexing from &x crosses the step's separate scalar members. UBFIX
   supplies a real coordinate array without including motion state. */
static inline const Sint32 *btParticleCoordinates(const PARTICLE *step)
{
#if FIX_POINTER_OUTSIDE_OBJECT
	return step->coordinates;
#else
	return &step->x;
#endif
}

/* A point projected onto the screen, with its depth. */
typedef struct SCREEN_POINT {
#if FIX_POINTER_OUTSIDE_OBJECT
	/* Projection writes three coordinates; effects read their named
	   components. */
	union {
		struct {
			Sint32 x; /* screen pixels */
			Sint32 y; /* screen pixels */
			Sint32 depth;
		};

		Sint32 coordinates[3];
	};
#else
	Sint32 x; /* screen pixels */
	Sint32 y; /* screen pixels */
	Sint32 depth;
#endif
} SCREEN_POINT;

/* Returns the position's three coordinates as an array. */
/* UB: indexing from &x crosses separate scalar members in the legacy record.
   UBFIX provides a real array while keeping the effects' named coordinates. An
   always-on union changes native alias analysis in the effect/window code. */
static inline Sint32 *syScreenPointCoordinates(SCREEN_POINT *pos)
{
#if FIX_POINTER_OUTSIDE_OBJECT
	return pos->coordinates;
#else
	return &pos->x;
#endif
}

void btEffectPosAdvanceWithGravity3D(
	PARTICLE *step, Sint32 damping, Sint32 gravity);
Sint32 btEffectPosProjectPoint(const void *point, SCREEN_POINT *pos);
void   btEffectPosProjectScaledBattler(Sint32 id, SCREEN_POINT *pos);

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
