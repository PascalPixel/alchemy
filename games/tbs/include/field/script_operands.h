#ifndef ALCHEMY_SCRIPT_OPERANDS_H
#define ALCHEMY_SCRIPT_OPERANDS_H

/* The actor script interpreter and the operands its commands read. */

#include "edition.h"
#include "static_assert.h"
#include "types.h"

struct ACTOR;

/* An actor script command's handler: it reads its operands at the actor's
   script cursor, moves the cursor past them and returns 1 to run the next
   command this tick or 0 to wait. */
typedef Sint32 (*SCRIPT_COMMAND)(struct ACTOR *);

/* The whole 112-byte slot as SC_SET, SC_ADD and SC_CMP see it: each field is
   one state value (ACTOR_SCRIPT_VALUE in actor_script.h), reached through
   ActorScriptOperandHandlerTable, and named for what the field keeps there. */
typedef struct SCRIPT_OPERANDS {
	Uint32 scriptAdr;	 /* SW_SCRIPT; an add steps it by whole words */
	Uint16 cursor;		 /* SW_CURSOR, compared as signed */
	Uint16 facing;		 /* SW_FACING, and the state values either side */
	Uint32 x;			 /* SW_X, 16.16 fixed-point pixels */
	Uint32 y;			 /* SW_Y */
	Uint32 z;			 /* SW_Z */
	Uint32 groundHeight; /* SW_GROUND */
	Uint32 scaleX;		 /* SW_SCALE_X, 16.16 fixed point */
	Uint32 scaleY;		 /* SW_SCALE_Y, and the state value after it */
	Uint16 radius;		 /* SW_RADIUS, in pixels; compared as signed */
	Uint8  layer;		 /* no state value */
	Uint8  priFlg;		 /* no state value */
	Uint32 velX;		 /* SW_VELOCITY_X, 16.16 fixed-point pixels per tick */
	Uint32 velY;		 /* SW_VELOCITY_Y */
	Uint32 velZ;		 /* SW_VELOCITY_Z */
	Uint32 speedLimit;	 /* SW_SPEED */
	Uint32 acceleration; /* SW_ACCELERATION */
	Uint32 tgtX;		 /* SW_TARGET_X; ACTOR_NO_TARGET when unset */
	Uint32 tgtY;		 /* SW_TARGET_Y */
	Uint32 tgtZ;		 /* SW_TARGET_Z */
	Uint32 bounce;		 /* SW_BOUNCE, or a floating actor's phase */
	Uint32 gravity;		 /* SW_GRAVITY */
	/* SW_SCENE_WORD: the word fdActorCreate zeroes and only scene code
	   reads. */
	Uint32 sceneWord;
	Uint32 spr;			   /* SW_SPRITE: the sprite, or its list of four */
	Uint8  sprStorageKind; /* SW_SPRITE_KIND, an ACTOR_SPR_KIND */
	Uint8  motionFlg;	   /* SW_MOTION, ACTOR_MOTION_FLAG bits */
	Uint8  arrivalAxis;	   /* SW_ARRIVAL */
	Uint8 conditionResult; /* SW_RESULT, the last test; every compare sets it */
	Uint8 snapToTgt;	   /* SW_SNAP */
	Uint8 collisionFlg;	   /* SW_COLLISION */
	Uint8 steeringFlg;	   /* SW_STEERING */
	Uint8 paused;		   /* SW_PAUSED */
	Uint8 retainSprCache;  /* keeps the sprite's VRAM; no state value */
	Uint8 repeatCnt;	   /* SW_REPEAT, SC_LOOP's count */
	Sint16 wait;		   /* SW_WAIT */
	Uint8  refused;		   /* ACTOR.refused; no state value */
	Uint8  frozen;		   /* ACTOR_ARM_VIEW.frozen; no state value */
	Uint8  sceneCnt;	   /* SW_SCENE_COUNTER, kept by scene code */
	Uint8  sceneByte;	   /* SW_SCENE_BYTE, kept by scene code */
	Sint16 tmr;			   /* SW_TIMER, what its actor script reads */
	Sint16 userValue;	   /* SW_USER_VALUE, or scene scratch */
	Uint32 watchedActor;   /* SW_WATCHED_ACTOR, the actor it follows or faces */
	Uint32 update;		   /* SW_HOOK, the function run each tick */
} SCRIPT_OPERANDS;

STATIC_ASSERT(offsetof(SCRIPT_OPERANDS, scriptAdr) == 0);
STATIC_ASSERT(offsetof(SCRIPT_OPERANDS, cursor) == 4);
STATIC_ASSERT(offsetof(SCRIPT_OPERANDS, facing) == 6);
STATIC_ASSERT(offsetof(SCRIPT_OPERANDS, radius) == 32);
STATIC_ASSERT(offsetof(SCRIPT_OPERANDS, sceneWord) == 76);
STATIC_ASSERT(offsetof(SCRIPT_OPERANDS, sprStorageKind) == 84);
STATIC_ASSERT(offsetof(SCRIPT_OPERANDS, repeatCnt) == 93);
STATIC_ASSERT(offsetof(SCRIPT_OPERANDS, refused) == 96);
STATIC_ASSERT(offsetof(SCRIPT_OPERANDS, conditionResult) == 87);
STATIC_ASSERT(offsetof(SCRIPT_OPERANDS, wait) == 94);
STATIC_ASSERT(offsetof(SCRIPT_OPERANDS, tmr) == 100);
STATIC_ASSERT(offsetof(SCRIPT_OPERANDS, watchedActor) == 104);
STATIC_ASSERT(offsetof(SCRIPT_OPERANDS, update) == 108);
STATIC_ASSERT(sizeof(SCRIPT_OPERANDS) == 112);

#endif
