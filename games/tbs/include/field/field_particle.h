#ifndef ALCHEMY_FIELD_PARTICLE_H
#define ALCHEMY_FIELD_PARTICLE_H

/* @file */
/* Field effect sprites: the particles field Psynergy moves and draws. */

#include "edition.h"
#include "fixed_math.h"
#include "static_assert.h"
#include "types.h"

/* FIELD_PARTICLE.tgtX and targetZ while the sprite has no target, at */
/* rest. */
#define FIELD_EFFECT_SPRITE_NO_TARGET ((s32)0x80000000)

struct FIELD_PARTICLE;
/* A field effect sprite's per-frame function (FIELD_PARTICLE.func). */
typedef void (*EFFECT_CALLBACK)(struct FIELD_PARTICLE *);

/* A particle a field Psynergy moves and draws: its sprite, its position and */
/* target, its speed, scale and heading, and its per-frame function. */
typedef struct FIELD_PARTICLE {
	void *spr;	   /* the sprite it draws, released with it */
	FIXED x;	   /* 16.16 fixed-point pixels */
	FIXED z;	   /* 16.16 fixed-point pixels */
	FIXED tgtX;	   /* 16.16 fixed-point pixels; FIELD_EFFECT_SPRITE_NO_TARGET at
					*/
				   /* rest */
	FIXED tgtZ;	   /* 16.16 fixed-point pixels; FIELD_EFFECT_SPRITE_NO_TARGET at
					*/
				   /* rest */
	FIXED originX; /* 16.16 fixed-point pixels; where the sprite started */
	FIXED originZ; /* 16.16 fixed-point pixels; where the sprite started */
	FIXED speed;   /* 16.16 fixed-point pixels per frame */
	FIXED speedMax;		  /* 16.16 fixed-point pixels per frame */
	FIXED acceleration;	  /* 16.16 fixed-point pixels per frame, each frame */
	FIXED scaleX;		  /* 16.16 fixed point, FIXED_ONE full size */
	FIXED scaleY;		  /* 16.16 fixed point, FIXED_ONE full size */
	u16 heading;		  /* 1/65536 turns */
	s16 turnStepMax;	  /* 1/65536 turns a frame */
	EFFECT_CALLBACK func; /* run each frame once funcDelay is 0 */
	s16 age;			  /* frames */
	s16 funcDelay;		  /* frames */
	s16 orbitRadius;	  /* pixels */
	s16 orbitAng;		  /* 1/32 turns */
	s8 state;			  /* the function's own step; 0 when it is given */
	s8 stopAtTgt;		  /* nonzero: lands on the target within 8 pixels */
	/* Nonzero turns the heading toward the target by at most turnStepMax a */
	/* frame; zero faces the target at once. */
	s8 turnStepLimited;
	s8 updateMotion; /* nonzero: fdParticleUpdateMotion moves it each */
					 /* frame */
	s8 render;		 /* nonzero: drawn each frame */
	s8 active;		 /* nonzero while the sprite is in use */
	u8 randomValue;	 /* random when filled */
	u8 flg;			 /* FIELD_PARTICLE_FLAG bits */
} FIELD_PARTICLE;

/* FIELD_PARTICLE.flg bits. */
typedef enum FIELD_PARTICLE_FLAG {
	/* Its z also lifts the sprite up the screen, from 508 pixels down; set when
	 */
	/* it is filled (fdParticleInit). */
	FIELD_PARTICLE_LIFTED_BY_Z = 0x04
} FIELD_PARTICLE_FLAG;

STATIC_ASSERT(sizeof(FIELD_PARTICLE) == 72);
STATIC_ASSERT(offsetof(FIELD_PARTICLE, orbitRadius) == 60);
STATIC_ASSERT(offsetof(FIELD_PARTICLE, state) == 64);

#endif
