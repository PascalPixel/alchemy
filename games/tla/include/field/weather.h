#ifndef ALCHEMY_WEATHER_H
#define ALCHEMY_WEATHER_H

/* @file */
/* The ground particles and the rain that the weather state holds. */

#include "fixed_math.h"
#include "types.h"

/* A ground particle: an OAM entry linked by depth, its place (16.16 pixels), */
/* its delay and its frame count. */
typedef struct WEATHER_PARTICLE {
	u8 unused[4]; /* no code names it */
	u16 y : 8;
	u16 affine : 2;
	u16 objMode : 2;
	u16 mosaic : 1;
	u16 fullCol : 1;
	u16 shape : 2;
	u16 x : 9;
	u16 affineIdx : 5;
	u16 size : 2;
	u16 tile : 10;
	u16 pri : 2;
	u16 pal : 4;
	u8 unusedAfterPal[2]; /* no code names it */
	FIXED posX;
	FIXED posY;
	FIXED posZ;
	s32 delay;
	u16 tmr;
	u8 unusedAfterTmr[2]; /* no code names it */
} WEATHER_PARTICLE;

/* WEATHER_PARTICLE.tmr after the decrement that counts a live particle down,
 */
/* when it was already free (0): the u16 wraps. */
#define GROUND_PARTICLE_FREED 65535
/* The frames of a ground particle's splash (Weather_RainSplashFrames, */
/* or a scene's own GroundParticleFrames, five halfwords each), played from */
/* the last down to the first. */
#define RAIN_SPLASH_FRAMES 16

/* WEATHER_STATE.particles. */
#define WEATHER_PARTICLE_COUNT 64
/* A raindrop's frames (fdWeatherUpdateRain). */
#define RAIN_DROP_FRAMES 12

/* The heap slot weather while the ground particles run: their tiles' VRAM */
/* cache entry and first tile, and the particles. */
typedef struct WEATHER_STATE {
	s32 entry;	   /* the VRAM cache entry holding the particles' tiles */
	long tileBase; /* their first tile in VRAM */
	WEATHER_PARTICLE particles[WEATHER_PARTICLE_COUNT];
	u8 unused[8]; /* no code reads them */
	s32 stopped;  /* nonzero stops new sparkle bursts */
} WEATHER_STATE;

#endif
