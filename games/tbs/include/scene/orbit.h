#ifndef ALCHEMY_ORBIT_H
#define ALCHEMY_ORBIT_H

/* @file */
/* A floating Nut's actor and sprite, seen through the fields its scenes set */
/* (shared/floating_nut.c and kalay.c): the actor's fields where the Nut keeps
 */
/* the spot it sways round and how far through the sway it is. */

#include "fixed_math.h"
#include "types.h"

/* A floating Nut's sprite, seen as the attribute bits, slot and count its */
/* scenes set: SPR read a byte at a time. */
typedef struct ORBITING_MAP_ACTOR_SPR {
	u8 link[4]; /* Sprite's queue link */
	u8 y;		/* attribute 0's y */
	/* Sprite's attribute 0 high byte. */
	/* bits: 7..6 shape | 5 full colour | 4 mosaic | 3..2 sprite mode | */
	/* 1..0 affine */
	/* @{ */
	u8 affineModeMosaic : 5; /* the scenes leave these */
	u8 fullCol : 1;			 /* the Nut's scenes clear it */
	u8 shape : 2;			 /* the scenes leave it */
	/* @} */
	u8 attr1[2];
	u8 tileLow; /* attribute 2's low byte */
	/* Sprite's attribute 2 high byte. */
	/* bits: 7..4 palette | 3..2 priority | 1..0 tile bits 9..8 */
	/* @{ */
	u8 tileHigh : 2; /* tile number bits 9..8; the scenes leave them */
	u8 pri : 2;		 /* the Nut's scenes set 1 */
	u8 palNum : 4;	 /* the Nut's scenes set 0 */
	/* @} */
	u8 attr3[2];
	u8 shadow[12]; /* Sprite's shadow part */
	u8 scale[4];
	/* Sprite's slot: the VRAM cache entry the Nut's icon is loaded */
	/* into. */
	u8 slot;
	u8 cacheFlg; /* retainCacheEntry and useOwnScale */
	u8 rotation[2];
	u8 width;
	u8 height;
	u8 ofsX;
	u8 ofsY;
	u8 lastNo;
	u8 dirty;
	u8 flg;
	u8 cnt; /* Sprite's count of layers; the Nut's scenes zero it */
} ORBITING_MAP_ACTOR_SPR;

/* A floating Nut's actor: its position, the spot it sways around and */
/* how far through the sway it is, its sprite and its update function. */
typedef struct ORBITING_MAP_ACTOR {
	const s32 *script;
	s16 scriptCursor;
	u16 facing;
	FIXED x; /* 16.16 fixed-point pixels */
	FIXED y; /* 16.16 fixed-point pixels */
	FIXED z; /* 16.16 fixed-point pixels */
	s32 groundHeight;
	s32 scaleX;
	s32 scaleY;
	u16 radius;
	u8 layer;
	/* Actor's priorityFlags; the Nut's scenes clear bit 0, */
	/* ACTOR_PRIORITY_AUTOMATIC. */
	u8 priFlg;
	s32 velX;
	s32 velY;
	s32 velZ;
	/* ACTOR.speedLimit: 1/65536 turns; advanced by TRIG_TURN / 64 plus two */
	/* random steps each frame. */
	s32 orbitAng;
	s32 acceleration;
	FIXED orbitCenterX; /* ACTOR.tgtX: 16.16 fixed-point pixels */
	FIXED orbitCenterY; /* ACTOR.tgtY: 16.16 fixed-point pixels */
	FIXED orbitCenterZ; /* ACTOR.tgtZ: 16.16 fixed-point pixels */
	s32 bounce;
	s32 gravity;
	s32 sceneWord;
	ORBITING_MAP_ACTOR_SPR *spr;
	u8 sprStorageKind;
	u8 motionFlg; /* the Nut's scenes zero it */
	/* ACTOR.arrivalAxis, the axis watched for arrival; scFloatNut */
	/* zeroes it. */
	u8 arrivalAxis;
	u8 conditionResult;
	u8 snapToTgt;
	u8 collisionFlg;
	u8 steeringFlg;
	u8 paused;
	/* ACTOR.retainSprCache: the Nut's scenes keep its sprite's VRAM */
	/* cache entry. */
	u8 retainSprCache;
	u8 repeatCnt;
	s16 wait;
	u8 refused;
	/* Actor's frozen, which skips steering, terrain, gravity and floating; */
	/* the Nut's scenes set it. */
	u8 frozen;
	u8 sceneCnt;
	u8 sceneByte;
	s16 tmr;
	s16 userValue;
	struct ACTOR *watchedActor;
	/* Actor's update function; scFloatNut sets */
	/* scUpdateFloatingNut. */
	u32 func;
} ORBITING_MAP_ACTOR;

#endif
