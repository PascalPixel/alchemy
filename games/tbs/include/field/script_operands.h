#ifndef ALCHEMY_SCRIPT_OPERANDS_H
#define ALCHEMY_SCRIPT_OPERANDS_H

/* @file */
/* The actor script interpreter and the operands its commands read. */

#include "edition.h"
#include "static_assert.h"
#include "types.h"

struct ACTOR;

/* An actor script command's handler: it reads its operands at the actor's */
/* script cursor, moves the cursor past them and returns 1 to run the next */
/* command this frame or 0 to wait. */
typedef s32 (*SCRIPT_COMMAND)(struct ACTOR *);

/* The whole 112-byte slot as SC_SET, SC_ADD and SC_CMP see it: each field is */
/* one state value (ACTOR_SCRIPT_VALUE in actor_script.h), reached through */
/* ActorScript_OperandHandlerTable, and named for what the field keeps there. */
typedef struct SCRIPT_OPERANDS {
	u32 scriptAdr;	  /* SW_SCRIPT; an add steps it by whole words */
	u16 cursor;		  /* SW_CURSOR, compared as signed */
	u16 facing;		  /* SW_FACING, and the state values either side */
	u32 x;			  /* SW_X, 16.16 fixed-point pixels */
	u32 y;			  /* SW_Y */
	u32 z;			  /* SW_Z */
	u32 groundHeight; /* SW_GROUND */
	u32 scaleX;		  /* SW_SCALE_X, 16.16 fixed point */
	u32 scaleY;		  /* SW_SCALE_Y, and the state value after it */
	u16 radius;		  /* SW_RADIUS, in pixels; compared as signed */
	u8 layer;		  /* no state value */
	u8 priFlg;		  /* no state value */
	u32 velX;		  /* SW_VELOCITY_X, 16.16 fixed-point pixels per frame */
	u32 velY;		  /* SW_VELOCITY_Y */
	u32 velZ;		  /* SW_VELOCITY_Z */
	u32 speedLimit;	  /* SW_SPEED */
	u32 acceleration; /* SW_ACCELERATION */
	u32 tgtX;		  /* SW_TARGET_X; ACTOR_NO_TARGET when unset */
	u32 tgtY;		  /* SW_TARGET_Y */
	u32 tgtZ;		  /* SW_TARGET_Z */
	u32 bounce;		  /* SW_BOUNCE, or a floating actor's phase */
	u32 gravity;	  /* SW_GRAVITY */
	/* SW_SCENE_WORD: the word fdActorCreate zeroes and only scene code */
	/* reads. */
	u32 sceneWord;
	u32 spr;			/* SW_SPRITE: the sprite, or its list of four */
	u8 sprStorageKind;	/* SW_SPRITE_KIND, an ACTOR_SPR_KIND */
	u8 motionFlg;		/* SW_MOTION, ACTOR_MOTION_FLAG bits */
	u8 arrivalAxis;		/* SW_ARRIVAL */
	u8 conditionResult; /* SW_RESULT, the last test; every compare sets it */
	u8 snapToTgt;		/* SW_SNAP */
	u8 collisionFlg;	/* SW_COLLISION */
	u8 steeringFlg;		/* SW_STEERING */
	u8 paused;			/* SW_PAUSED */
	u8 retainSprCache;	/* keeps the sprite's VRAM; no state value */
	u8 repeatCnt;		/* SW_REPEAT, SC_LOOP's count */
	s16 wait;			/* SW_WAIT */
	u8 refused;			/* ACTOR.refused; no state value */
	u8 frozen;			/* ACTOR_ARM_VIEW.frozen; no state value */
	u8 sceneCnt;		/* SW_SCENE_COUNTER, kept by scene code */
	u8 sceneByte;		/* SW_SCENE_BYTE, kept by scene code */
	s16 tmr;			/* SW_TIMER, what its actor script reads */
	s16 userValue;		/* SW_USER_VALUE, or scene scratch */
	u32 watchedActor;	/* SW_WATCHED_ACTOR, the actor it follows or faces */
	u32 update;			/* SW_HOOK, the function run each frame */
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
