#ifndef ALCHEMY_FALLING_ROCKS_H
#define ALCHEMY_FALLING_ROCKS_H

/* Falling rocks and particles: the effects Prox, Magma Rock, Eastern Alhafra
   and Tundaria Tower each keep a copy of the code for. */

#include "fixed_math.h"
#include "types.h"
#include "static_assert.h"
#include "field/effect_sprite.h"
#include "field/particle.h"

/* The falling rocks in flight at once, the ring a scene's rock state keeps. */
#define ROCK_COUNT		 64
/* The falling rock's pictures, decompressed, in bytes. */
#define ROCK_SHEET_BYTES 1280

/* One falling rock: where it is, how it moves and how long it has fallen. */
typedef struct ROCK {
	Sint32 pos[3];
	Sint32 vel[3];
	Sint32 tmr;
} ROCK;

/* The falling rocks' state, in the menu state's heap slot: their tiles, the
   rocks and their sprites, in a ring that next walks round, the first sprite
   tile their tiles were loaded to and the VRAM cache entry holding them. */
typedef struct ROCK_STATE {
	Uint8	   tiles[ROCK_SHEET_BYTES];
	ROCK	   rocks[ROCK_COUNT];
	EFFECT_SPR sprs[ROCK_COUNT];
	Sint16	   next;
	Uint16	   tileBase;
	Sint16	   resource;
} ROCK_STATE;

STATIC_ASSERT(offsetof(ROCK_STATE, resource) == 5636);

/* A falling particle's sprite: EFFECT_SPR with OAM attribute 2 read whole, as
   the particles change its tile. */
typedef struct PARTICLE_SPR {
	EFFECT_SPR *next;
	Uint16		attr0;
	Uint16		attr1;
	Uint16		attr2; /* bits: 15..12 palette | 11..10 priority | 9..0 tile */
	Uint16		attr3;
	FIXED		x;
	FIXED		y;
	FIXED		scaleX;
	FIXED		scaleY;
	Uint16		ang;
	Sint16		depth;
	Uint16		width;
	Uint16		height;
	Sint16		pivotX;
	Sint16		pivotY;
} PARTICLE_SPR;

/* The falling particles' state, in the menu state's heap slot: their tiles, 64
   particles and their sprites, the next particle to start and the first sprite
   tile their tiles were loaded to. */
typedef struct PARTICLE_STATE {
	Uint8		 tiles[ROCK_SHEET_BYTES];
	PARTICLE	 particles[64];
	PARTICLE_SPR sprs[64];
	Sint16		 next;
	Uint16		 tile;
} PARTICLE_STATE;

#endif
