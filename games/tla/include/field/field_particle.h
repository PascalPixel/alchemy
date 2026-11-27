#ifndef ALCHEMY_FIELD_PARTICLE_H
#define ALCHEMY_FIELD_PARTICLE_H

/* @file */
/* The field effect sprites the field Psynergy keeps (FIELD_PARTICLE). */

#include "edition.h"
#include "fixed_math.h"
#include "types.h"
#include "static_assert.h"

/* A targetX that marks an effect with no target to steer toward. */
#define EFFECT_NO_TARGET ((s32)0x80000000)

struct FIELD_PARTICLE;
typedef void (*EFFECT_CALLBACK)(struct FIELD_PARTICLE *);

/* A particle a Psynergy effect moves and draws: its sprite, its position and */
/* target, its speed, scale and heading, and its per-frame callback. */
typedef struct FIELD_PARTICLE {
	void *spr;			/* the sprite it draws (SPR), released with it */
	FIXED x;			/* 16.16 fixed-point pixels */
	FIXED z;			/* 16.16 fixed-point pixels */
	FIXED tgtX;			/* 16.16 fixed-point pixels; EFFECT_NO_TARGET at rest */
	FIXED tgtZ;			/* 16.16 fixed-point pixels; EFFECT_NO_TARGET at rest */
	FIXED originX;		/* 16.16 fixed-point pixels */
	FIXED originZ;		/* 16.16 fixed-point pixels */
	FIXED speed;		/* 16.16 fixed-point pixels per frame */
	FIXED speedMax;		/* 16.16 fixed-point pixels per frame */
	FIXED acceleration; /* 16.16 fixed-point pixels per frame, each frame */
	FIXED scaleX;		/* 16.16 fixed point, FIXED_ONE full size */
	FIXED scaleY;		/* 16.16 fixed point, FIXED_ONE full size */
	u16 heading;		/* 1/65536 turns */
	s16 turnStepMax;	/* 1/65536 turns a frame */
	EFFECT_CALLBACK func; /* run each frame once funcDelay is 0 */
	u16 age;			  /* frames */
	s16 funcDelay;		  /* frames */
	s16 orbitRadius;	  /* pixels */
	s16 orbitAng;		  /* 1/32 turns */
	s8 state;			  /* the callback's own step */
	s8 stopAtTgt;		  /* nonzero: lands on the target within 8 pixels */
	/* Nonzero turns it toward the target at most turnStepMax a frame; zero */
	/* faces it straight at the target. */
	s8 turnStepLimited;
	s8 updateMotion; /* nonzero: fdParticleUpdateMotion moves it each frame */
	s8 render;		 /* nonzero: drawn each frame */
	s8 active;		 /* nonzero while the slot is in use */
	u8 randomValue;	 /* not read by name */
	u8 flg;			 /* FIELD_PARTICLE_LIFTED_BY_Z */
} FIELD_PARTICLE;

/* FIELD_PARTICLE.flg: z also lifts the sprite off the ground. */
#define FIELD_PARTICLE_LIFTED_BY_Z 0x04

STATIC_ASSERT(sizeof(FIELD_PARTICLE) == 72);
STATIC_ASSERT(offsetof(FIELD_PARTICLE, orbitRadius) == 60);
STATIC_ASSERT(offsetof(FIELD_PARTICLE, state) == 64);

/* The states of a particle that flies out and back: Halt's and Force's */
/* (fdEffectUpdateHaltParticle, fdEffectUpdateRadialBurst). */
typedef enum OUT_AND_BACK_STATE {
	OUT_AND_BACK_AIM_OUT = 0,	 /* picks the point it flies out to */
	OUT_AND_BACK_FLYING_OUT = 1, /* waits to arrive there */
	OUT_AND_BACK_AIM_BACK = 2,	 /* picks the point it comes back to */
	OUT_AND_BACK_FLYING_BACK = 3 /* frees its slot once it arrives */
} OUT_AND_BACK_STATE;

#endif
