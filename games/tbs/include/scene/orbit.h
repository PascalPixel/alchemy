#ifndef ALCHEMY_ORBIT_H
#define ALCHEMY_ORBIT_H

/* A floating Nut's actor and sprite, seen through the fields its scenes set
   (shared/floating_nut.c and kalay.c): the actor's fields where the Nut keeps
   the spot it sways round and how far through the sway it is. */

#include "fixed_math.h"
#include "types.h"

/* A floating Nut's sprite, seen as the attribute bits, slot and count its
   scenes set: SPR read a byte at a time. */
typedef struct ORBITING_MAP_ACTOR_SPR {
	Uint8 link[4]; /* Sprite's queue link */
	Uint8 y;	   /* attribute 0's y */
	/* Sprite's attribute 0 high byte. bits: 7..6 shape | 5 full colour | 4
	   mosaic | 3..2 sprite mode | 1..0 affine */
	Uint8 affineModeMosaic : 5; /* the scenes leave these */
	Uint8 fullCol : 1;			/* the Nut's scenes clear it */
	Uint8 shape : 2;			/* the scenes leave it */
	Uint8 attr1[2];
	Uint8 tileLow; /* attribute 2's low byte */
	/* Sprite's attribute 2 high byte. bits: 7..4 palette | 3..2 priority | 1..0
	   tile bits 9..8 */
	Uint8 tileHigh : 2; /* tile number bits 9..8; the scenes leave them */
	Uint8 pri : 2;		/* the Nut's scenes set 1 */
	Uint8 palNum : 4;	/* the Nut's scenes set 0 */
	Uint8 attr3[2];
	Uint8 shadow[12]; /* Sprite's shadow part */
	Uint8 scale[4];
	/* Sprite's slot: the VRAM cache entry the Nut's icon is loaded into. */
	Uint8 slot;
	Uint8 cacheFlg; /* retainCacheEntry and useOwnScale */
	Uint8 rotation[2];
	Uint8 width;
	Uint8 height;
	Uint8 ofsX;
	Uint8 ofsY;
	Uint8 lastNo;
	Uint8 dirty;
	Uint8 flg;
	Uint8 cnt; /* Sprite's count of layers; the Nut's scenes zero it */
} ORBITING_MAP_ACTOR_SPR;

/* A floating Nut's actor: its position, the spot it sways around and how far
   through the sway it is, its sprite and its update function. */
typedef struct ORBITING_MAP_ACTOR {
	const Sint32 *script;
	Sint16		  scriptCursor;
	Uint16		  facing;
	FIXED		  x; /* 16.16 fixed-point pixels */
	FIXED		  y; /* 16.16 fixed-point pixels */
	FIXED		  z; /* 16.16 fixed-point pixels */
	Sint32		  groundHeight;
	Sint32		  scaleX;
	Sint32		  scaleY;
	Uint16		  radius;
	Uint8		  layer;
	/* Actor's priorityFlags; the Nut's scenes clear bit 0,
	   ACTOR_PRIORITY_AUTOMATIC. */
	Uint8  priFlg;
	Sint32 velX;
	Sint32 velY;
	Sint32 velZ;
	/* ACTOR.speedLimit: 1/65536 turns; advanced by TRIG_TURN / 64 plus two
	   random steps each tick. */
	Sint32 orbitAng;
	Sint32 acceleration;
	FIXED  orbitCenterX; /* ACTOR.tgtX: 16.16 fixed-point pixels */
	FIXED  orbitCenterY; /* ACTOR.tgtY: 16.16 fixed-point pixels */
	FIXED  orbitCenterZ; /* ACTOR.tgtZ: 16.16 fixed-point pixels */
	Sint32 bounce;
	Sint32 gravity;
	Sint32 sceneWord;
	ORBITING_MAP_ACTOR_SPR *spr;
	Uint8					sprStorageKind;
	Uint8					motionFlg; /* the Nut's scenes zero it */
	/* ACTOR.arrivalAxis, the axis watched for arrival; scFloatNut zeroes it. */
	Uint8 arrivalAxis;
	Uint8 conditionResult;
	Uint8 snapToTgt;
	Uint8 collisionFlg;
	Uint8 steeringFlg;
	Uint8 paused;
	/* ACTOR.retainSprCache: the Nut's scenes keep its sprite's VRAM cache
	   entry. */
	Uint8  retainSprCache;
	Uint8  repeatCnt;
	Sint16 wait;
	Uint8  refused;
	/* Actor's frozen, which skips steering, terrain, gravity and floating; the
	   Nut's scenes set it. */
	Uint8		  frozen;
	Uint8		  sceneCnt;
	Uint8		  sceneByte;
	Sint16		  tmr;
	Sint16		  userValue;
	struct ACTOR *watchedActor;
	/* Actor's update function; scFloatNut sets scUpdateFloatingNut. */
	Uint32 func;
} ORBITING_MAP_ACTOR;

#endif
