#ifndef ALCHEMY_FIELD_PARTICLE_H
#define ALCHEMY_FIELD_PARTICLE_H

/* Field effect sprites: the particles field Psynergy moves and draws. */

#include "edition.h"
#include "fixed_math.h"
#include "static_assert.h"
#include "types.h"

/* FIELD_PARTICLE.tgtX and targetZ while the sprite has no target, at rest. */
#define FIELD_EFFECT_SPRITE_NO_TARGET ((Sint32)0x80000000)

struct FIELD_PARTICLE;
/* A field effect sprite's per-frame function (FIELD_PARTICLE.func). */
typedef void (*EFFECT_CALLBACK)(struct FIELD_PARTICLE *);

/* A particle a field Psynergy moves and draws: its sprite, its position and
   target, its speed, scale and heading, and its per-frame function. */
typedef struct FIELD_PARTICLE {
	void *spr; /* the sprite it draws, released with it */
	FIXED x;   /* 16.16 fixed-point pixels */
	FIXED z;   /* 16.16 fixed-point pixels */
	/* 16.16 fixed-point pixels; FIELD_EFFECT_SPRITE_NO_TARGET at rest */
	FIXED tgtX;
	/* 16.16 fixed-point pixels; FIELD_EFFECT_SPRITE_NO_TARGET at rest */
	FIXED  tgtZ;
	FIXED  originX;	 /* 16.16 fixed-point pixels; where the sprite started */
	FIXED  originZ;	 /* 16.16 fixed-point pixels; where the sprite started */
	FIXED  speed;	 /* 16.16 fixed-point pixels per frame */
	FIXED  speedMax; /* 16.16 fixed-point pixels per frame */
	FIXED  acceleration;  /* 16.16 fixed-point pixels per frame, each frame */
	FIXED  scaleX;		  /* 16.16 fixed point, toFIXED(1) full size */
	FIXED  scaleY;		  /* 16.16 fixed point, toFIXED(1) full size */
	Uint16 heading;		  /* 1/65536 turns */
	Sint16 turnStepMax;	  /* 1/65536 turns a frame */
	EFFECT_CALLBACK func; /* run each frame once funcDelay is 0 */
	Sint16			age;  /* frames */
	Sint16			funcDelay;	 /* frames */
	Sint16			orbitRadius; /* pixels */
	Sint16			orbitAng;	 /* 1/32 turns */
	Sint8			state; /* the function's own step; 0 when it is given */
	Sint8 stopAtTgt;	   /* nonzero: lands on the target within 8 pixels */
	/* Nonzero turns the heading toward the target by at most turnStepMax a
	   frame; zero faces the target at once. */
	Sint8 turnStepLimited;
	/* nonzero: fdParticleUpdateMotion moves it each frame */
	Sint8 updateMotion;
	Sint8 render;	   /* nonzero: drawn each frame */
	Sint8 active;	   /* nonzero while the sprite is in use */
	Uint8 randomValue; /* random when filled */
	Uint8 flg;		   /* FIELD_PARTICLE_FLAG bits */
} FIELD_PARTICLE;

/* FIELD_PARTICLE.flg bits. */
typedef enum FIELD_PARTICLE_FLAG {
	/* Its z also lifts the sprite up the screen, from 508 pixels down; set when
	   it is filled (fdParticleInit). */
	FIELD_PARTICLE_LIFTED_BY_Z = 0x04
} FIELD_PARTICLE_FLAG;

STATIC_ASSERT(sizeof(FIELD_PARTICLE) == 72);
STATIC_ASSERT(offsetof(FIELD_PARTICLE, orbitRadius) == 60);
STATIC_ASSERT(offsetof(FIELD_PARTICLE, state) == 64);

#endif
