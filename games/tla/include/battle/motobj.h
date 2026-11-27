#ifndef ALCHEMY_MOTION_OBJECT_H
#define ALCHEMY_MOTION_OBJECT_H

/* The motion object: a battle unit's actor as the battle moves it, and the
   sprites it draws. */

#include "fixed_math.h"
#include "types.h"
#include "static_assert.h"
#include "battle_action.h"
#include "edition.h"

union FIELD_OBJECT;
struct ACTOR;

/* An effect object that moves and draws one sprite: its position, scale,
   velocity and target, and its per-frame update. It is one of the object slots
   the field engine's FIELD_ACTOR also describes. */
typedef struct MOTION_OBJECT {
	const Sint32 *script; /* the object script it runs; null when it is free */
	Sint16		  scriptCursor; /* the script cursor, in words */
	Uint16		  ang;			/* 1/65536 turns */
	FIXED		  x;			/* 16.16 fixed-point pixels */
	FIXED		  y;			/* 16.16 fixed-point pixels */
	FIXED		  z;			/* 16.16 fixed-point pixels */
	FIXED		  groundHeight; /* 16.16 fixed-point pixels */
	FIXED		  scaleX;		/* 16.16 fixed point, toFIXED(1) full size */
	FIXED		  scaleY;		/* 16.16 fixed point, toFIXED(1) full size */
	Uint16		  height;		/* no code names it through this view */
	Uint8		  layer; /* the map layer its ground height is read from */
	Uint8		  priFlg;
	FIXED		  velX;		  /* 16.16 fixed-point pixels per frame */
	FIXED		  velY;		  /* 16.16 fixed-point pixels per frame */
	FIXED		  velZ;		  /* 16.16 fixed-point pixels per frame */
	FIXED		  speedLimit; /* 16.16 fixed-point pixels per frame */
	/* Horizontal speed gained each frame towards the target, or lost braking
	   without one, in 16.16 fixed-point pixels per frame. */
	FIXED acceleration;
	FIXED tgtX; /* 16.16 fixed-point pixels; 0x80000000 for no target */
	FIXED tgtY; /* 16.16 fixed-point pixels */
	FIXED tgtZ; /* 16.16 fixed-point pixels */
	/* With motionFlags bit 1, the 16.16 share of velocityY kept on a bounce. */
	FIXED verticalMotionPhase;
	/* Gravity: taken from velocityY each frame above the ground with
	   motionFlags bit 1, in 16.16 fixed-point pixels per frame. */
	FIXED  verticalMotionStrength;
	Sint32 sceneWord;
	void  *records; /* one sprite, or a table of them (recordStorageKind) */
	/* bits: 3..0 how records holds the sprites */
	Uint8 recordStorageKind;
	/* bits: 1 falls under gravity and bounces | 0 follows the terrain */
	Uint8 motionFlg;
	/* The ARRIVAL_AXIS_* watched for arrival at the target, as Actor's. */
	Uint8  arrivalAxis;
	Uint8  conditionResult; /* the object script's last flag test */
	Uint8  snapToTgt;		/* nonzero: lands exactly on the target */
	Uint8  collisionFlg;	/* ACTOR_COLLISION_FLAG bits */
	Uint8  autoFaceMotion;	/* bit 0 turns the object to face where it moves */
	Uint8  paused; /* nonzero: the motion update leaves it where it is */
	Uint8  retainSprCache; /* keeps its sprite's VRAM cache entry */
	Uint8  repeatCnt;	   /* the object script's loop count */
	Sint16 wait;		   /* the object script's wait, in frames */
	void  *func;		   /* Actor's hook; no battle code names it */
	Sint32 tmr; /* Actor's timer and userValue; no battle code names it */
	struct ACTOR *watchedActor;
	void (*update)(union FIELD_OBJECT *object);
} MOTION_OBJECT;

/* The gravity a battle unit falls under, about 0.67 pixels a frame per frame in
   16.16, as verticalMotionStrength: btMotionReturnToSpot sets it whenever a
   unit goes back to its spot, and effects that change a unit's gravity for a
   leap or a lift give it this one back. */
#define BATTLE_UNIT_GRAVITY (toFIXED(67) / 100)

void btSprDetachScripts(void *recordGroup);
void btActorDetachSprScripts(MOTION_OBJECT *object);

STATIC_ASSERT(sizeof(MOTION_OBJECT) == 112);
STATIC_ASSERT(offsetof(MOTION_OBJECT, scaleX) == 24);
STATIC_ASSERT(offsetof(MOTION_OBJECT, x) == 8);
STATIC_ASSERT(offsetof(MOTION_OBJECT, velX) == 36);
STATIC_ASSERT(offsetof(MOTION_OBJECT, tgtX) == 56);
STATIC_ASSERT(offsetof(MOTION_OBJECT, motionFlg) == 85);
STATIC_ASSERT(offsetof(MOTION_OBJECT, records) == 80);
STATIC_ASSERT(offsetof(MOTION_OBJECT, recordStorageKind) == 84);
STATIC_ASSERT(offsetof(MOTION_OBJECT, update) == 108);

Sint32 btObjectMarkForRelease(Sint32 objectId);
/* Returns a unit's battle object, or null for a unit with no place
   (btObjectGet). */
BATTLE_OBJECT *btObjectGetStub(Sint32 objectId);

#endif
