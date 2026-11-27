#ifndef ALCHEMY_PARTICLE_H
#define ALCHEMY_PARTICLE_H

/* @file */
/* Particle and SCR_POINT, the positions animations move through. */

#include "edition.h"
#include "fixed_math.h"
#include "static_assert.h"
#include "types.h"

/* A particle of an animation: its position, its velocity and a value each */
/* animation uses its own way (a size, a timer, -1 for idle). */
typedef struct PARTICLE {
#ifdef UBFIX
	union {
		struct {
			FIXED x; /* 16.16 fixed-point pixels */
			FIXED y; /* 16.16 fixed-point pixels */
			FIXED z; /* 16.16 fixed-point pixels */
		};
		s32 coordinates[3];
	};
#else
	FIXED x; /* 16.16 fixed-point pixels */
	FIXED y; /* 16.16 fixed-point pixels */
	FIXED z; /* 16.16 fixed-point pixels */
#endif
	FIXED velX;	 /* 16.16 fixed-point pixels per frame */
	FIXED velY;	 /* 16.16 fixed-point pixels per frame */
	FIXED velZ;	 /* 16.16 fixed-point pixels per frame */
	s32 variant; /* each animation's own: a size, a countdown, -1 for idle */
} PARTICLE;

/* Returns the particle's three coordinates as an array. */
/* UB: indexing from &x crosses the particle's separate scalar members. UBFIX */
/* supplies a real coordinate array without including motion state. */
static inline const s32 *btParticleCoordinates(const PARTICLE *step) {
#ifdef UBFIX
	return step->coordinates;
#else
	return &step->x;
#endif
}

/* A point projected onto the screen, with its depth. */
typedef struct SCR_POINT {
#ifdef UBFIX
	/* Projection writes three coordinates; animations read their named */
	/* components. */
	union {
		struct {
			s32 x;	   /* screen pixels */
			s32 y;	   /* screen pixels */
			s32 depth; /* projected depth, larger further away */
		};
		s32 coordinates[3];
	};
#else
	s32 x;	   /* screen pixels */
	s32 y;	   /* screen pixels */
	s32 depth; /* projected depth, larger further away */
#endif
} SCR_POINT;

/* Returns the position's three coordinates as an array. */
/* UB: indexing from &x crosses separate scalar members of the point. UBFIX */
/* provides a real array while keeping the animations' named coordinates. */
static inline s32 *syScrPointCoordinates(SCR_POINT *position) {
#ifdef UBFIX
	return position->coordinates;
#else
	return &position->x;
#endif
}

STATIC_ASSERT(offsetof(PARTICLE, x) == 0);
STATIC_ASSERT(offsetof(PARTICLE, y) == 4);
STATIC_ASSERT(offsetof(PARTICLE, z) == 8);
STATIC_ASSERT(offsetof(PARTICLE, velX) == 12);
STATIC_ASSERT(offsetof(PARTICLE, velY) == 16);
STATIC_ASSERT(offsetof(PARTICLE, velZ) == 20);
STATIC_ASSERT(offsetof(PARTICLE, variant) == 24);
STATIC_ASSERT(sizeof(PARTICLE) == 28);
STATIC_ASSERT(sizeof(SCR_POINT) == 12);

#endif
