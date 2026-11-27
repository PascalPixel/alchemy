#ifndef ALCHEMY_ACTOR_ARM_VIEW_H
#define ALCHEMY_ACTOR_ARM_VIEW_H

/* What the actors' ARM update can read: the words of an actor's script as the
   motion loops index them, the target value that is no coordinate, the motion
   and steering flags, and ACTOR_ARM_VIEW, ACTOR as actor_update_arm.c reads
   it. */

#include "fixed_math.h"
#include "static_assert.h"
#include "types.h"

struct ACTOR;

/* The values of ACTOR.targetX, targetY and targetZ (and ACTOR_ARM_VIEW's) that
   are not coordinates. */
typedef enum ACTOR_TARGET {
	/* A target coordinate that is not set: the actor has no target. */
	ACTOR_NO_TARGET = (Sint32)0x80000000
} ACTOR_TARGET;

/* ACTOR.motionFlags bits. */
typedef enum ACTOR_MOTION_FLAG {
	ACTOR_FOLLOWS_TERRAIN = 0x01, /* moves with the ground height under it */
	ACTOR_FALLS = 0x02,			  /* falls by its gravity and bounces */
	ACTOR_FLOATS = 0x04,		  /* bobs up and down by its gravity */
	ACTOR_FLOATS_HIGH = 0x08,	  /* with ACTOR_FLOATS, four times as far */
	/* Keeps its speed on slopes, which slow others down. */
	ACTOR_KEEPS_SPEED_ON_SLOPES = 0x10,
	/* Unused: nothing sets or reads it; a jump clears it with
	   ACTOR_FOLLOWS_TERRAIN. */
	ACTOR_MOTION_SPARE = 0x80,
	/* The motion flags a jump keeps as the actor leaves the ground: all but
	   ACTOR_FOLLOWS_TERRAIN and the spare bit. */
	ACTOR_JUMP_KEEPS_MOTION =
		(Uint8)~(ACTOR_FOLLOWS_TERRAIN | ACTOR_MOTION_SPARE)
} ACTOR_MOTION_FLAG;

/* ACTOR.steeringFlags bits. */
typedef enum ACTOR_STEERING_FLAG {
	ACTOR_FACES_MOTION = 0x01 /* turns toward its velocity, 1/16 turn a tick */
} ACTOR_STEERING_FLAG;

/* UB: scripts have variable lengths, so a one-word member cannot describe
   them. */
#if FIX_POINTER_OUTSIDE_OBJECT
typedef Sint32 ACTOR_SCRIPT_WORDS;
#else
/* The words of an actor's script, as its script pointer reads them. */
typedef struct ACTOR_SCRIPT_WORDS {
	Sint32 word[1];
} ACTOR_SCRIPT_WORDS;
#endif

/* ACTOR, field for field, as fdActorUpdateAll (actor_update_arm.c) reads it.
   actor_update_arm.c is ARM code built with GCC 2.95.2, which cannot read
   ACTOR's anonymous unions, so it keeps this copy of the layout with the script
   as words and the bounce as a union. */
typedef struct ACTOR_ARM_VIEW {
	const ACTOR_SCRIPT_WORDS *script;		/* null when its slot is released */
	Sint16					  scriptCursor; /* in words */
	Uint16					  facing;		/* 1/65536 turns */
	FIXED					  x;			/* 16.16 fixed-point pixels */
	FIXED					  y;			/* 16.16 fixed-point pixels */
	FIXED					  z;			/* 16.16 fixed-point pixels */
	FIXED					  groundHeight; /* 16.16 fixed-point pixels */
	/* Actor's scales and radius; not named here. */
	Sint32 scaleX;
	Sint32 scaleY;
	Uint16 radius;
	Uint8  layer;		  /* the map layer whose ground height it follows */
	Uint8  priorityFlags; /* Actor's; not named here */
	FIXED  velocityX;	  /* 16.16 fixed-point pixels per tick */
	FIXED  velocityY;	  /* 16.16 fixed-point pixels per tick */
	FIXED  velocityZ;	  /* 16.16 fixed-point pixels per tick */
	FIXED  speedLimit;	  /* 16.16 fixed-point pixels per tick */
	/* Added toward the target each tick, or taken off the speed when there is
	   none, in 16.16 fixed-point pixels per tick. */
	FIXED acceleration;
	FIXED targetX; /* 16.16 fixed-point pixels; ACTOR_NO_TARGET when unset */
	FIXED targetY; /* 16.16 fixed-point pixels; ACTOR_NO_TARGET when unset */
	FIXED targetZ; /* 16.16 fixed-point pixels; ACTOR_NO_TARGET when unset */

	union {
		/* The share of the falling speed kept on landing, 16.16 fixed point. */
		FIXED bounce;
		/* A floating actor's step along ActorScriptCycleTable, low six
		   bits. */
		Sint32 phase;
	} vertical;

	/* Taken off velocityY each tick above the ground, 16.16 fixed-point pixels
	   per tick; also scales a floating actor's bob. */
	FIXED gravity;
	/* Actor's sceneWord, sprite and sprite kind; not named here. */
	Sint32 sceneWord;
	void  *sprite;
	Uint8  spriteStorageKind;
	/* ACTOR_MOTION_FLAG bits; zero is free flight in 3D. */
	Uint8 motionFlags;
	Uint8 arrivalAxis; /* the ARRIVAL_AXIS_* that decides arrival */
	/* the actor script's last flag test; not named here */
	Uint8 conditionResult;
	Uint8 snapToTarget;	  /* nonzero lands it exactly on its target */
	Uint8 collisionFlags; /* bit 7: a solid actor refuses its move */
	Uint8 steeringFlags;  /* ACTOR_STEERING_FLAG bits */
	/* nonzero skips its script and motion, not its update function */
	Uint8 paused;
	/* Actor's flag that keeps its sprite's VRAM, and the actor script's SC_LOOP
	   count; not named here. */
	Uint8  retainSpriteCache;
	Uint8  repeatCount;
	Sint16 wait;	/* ticks left before its script runs again */
	Uint8  refused; /* ticks in a row a solid actor refused its move */
	Uint8  frozen;	/* nonzero skips steering, terrain, gravity and floating */
	/* Actor's two bytes scene code uses, and its timer (the actor script's
	   parameter), footprint side and watched actor; not named here. */
	Uint8				   sceneCount;
	Uint8				   sceneByte;
	Sint16				   timer;
	Sint16				   userValue;
	struct ACTOR_ARM_VIEW *watchedActor;
	/* Called each tick before its script. */
	void (*update)(struct ACTOR *actor);
} ACTOR_ARM_VIEW;

STATIC_ASSERT(sizeof(ACTOR_ARM_VIEW) == 112);
STATIC_ASSERT(offsetof(ACTOR_ARM_VIEW, script) == 0);
STATIC_ASSERT(offsetof(ACTOR_ARM_VIEW, scriptCursor) == 4);
STATIC_ASSERT(offsetof(ACTOR_ARM_VIEW, facing) == 6);
STATIC_ASSERT(offsetof(ACTOR_ARM_VIEW, vertical) == 68);
STATIC_ASSERT(offsetof(ACTOR_ARM_VIEW, arrivalAxis) == 86);
STATIC_ASSERT(offsetof(ACTOR_ARM_VIEW, wait) == 94);
STATIC_ASSERT(offsetof(ACTOR_ARM_VIEW, frozen) == 97);
STATIC_ASSERT(offsetof(ACTOR_ARM_VIEW, update) == 108);

#endif
