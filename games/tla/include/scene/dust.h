#ifndef ALCHEMY_DUST_H
#define ALCHEMY_DUST_H

/* Dust and ground particles the scenes draw by hand. */

#include "fixed_math.h"
#include "types.h"
#include "field/weather.h"

/* The heap slot weather while the scenes' sparkles run (fdGroundParticlesDraw):
   The Broken Seal's WEATHER_STATE, grown to 64 particles, with the place the
   particles gather around. */
typedef struct GROUND_PARTICLE_STATE {
	Sint32 entry;	 /* the VRAM cache entry holding the particles' tiles */
	Sint32 tileBase; /* their first tile in VRAM */
	WEATHER_PARTICLE particles[64];
	/* The 16.16 position (x, height, z) new particles appear around; NULL keeps
	   them where they fall. */
	FIXED *leader;
	Uint8  unused[4]; /* no code reads it */
	Sint32 stopped; /* nonzero stops new sparkle bursts, as in WEATHER_STATE */
} GROUND_PARTICLE_STATE;

#endif
