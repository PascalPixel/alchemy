#ifndef ALCHEMY_MACROS_H
#define ALCHEMY_MACROS_H

/* Small statement macros both games use for positions, velocities and
   bounds. */

/* Sets the x, y and z of a three-part vector: a position about to be projected,
   a per-frame step, a camera point or a scale. */
#define SET_VECTOR(v, x, y, z) ((v)[0] = (x), (v)[1] = (y), (v)[2] = (z))

/* Sets the position or the velocity of a particle, a piece of debris or
   anything else with plain x, y and z in one statement (SET_ACTOR_POSITION sets
   an actor's). */
#define SET_POSITION(p, newX, newY, newZ) \
	((p)->x = (newX), (p)->y = (newY), (p)->z = (newZ))
#define SET_VELOCITY(p, vx, vy, vz) \
	((p)->velX = (vx), (p)->velY = (vy), (p)->velZ = (vz))

/* Keeps `v` within low to high, the low bound tested first. A plain block, so
   it cannot stand before an `else`. */
#define CLAMP(v, low, high) \
	{ \
		if((v) < (low)) \
			(v) = (low); \
		if((v) > (high)) \
			(v) = (high); \
	}

/* The magnitude of v. v is read twice. */
#define ABS(v) ((v) < 0 ? -(v) : (v))

/* The entries of an array whose size is declared, as pret's global.h has it. */
#define ARRAY_COUNT(array) (sizeof(array) / sizeof((array)[0]))

/* Data aligned as the ROM places it, where its own type would not. */
#define ALIGNED(n) __attribute__((aligned(n)))

/* A global kept in its object's .bss in the order written, where a plain one
   would be a common symbol the linker places in an order of its own. */
#define BSS_DATA __attribute__((section(".bss")))

#endif
