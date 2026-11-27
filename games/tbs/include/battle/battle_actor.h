#ifndef ALCHEMY_BATTLE_ACTOR_H
#define ALCHEMY_BATTLE_ACTOR_H

/* @file */
/* BATTLE_ACTOR, the view of ACTOR one battle function gives a */
/* unit its sprites through. */

#include "edition.h"
#include "static_assert.h"
#include "types.h"
#include "battle/battle_action.h"
#include "stubs.h"

struct ACTOR;

/* ACTOR, field for field, as btObjectSpawnObjectsForList sees a */
/* battle unit's actor while it gives it its sprites: the sprite word untyped */
/* (void *), and the cursor, wait and userValue unsigned. The plain form reads
 */
/* the actor through ACTOR. */
typedef struct BATTLE_ACTOR {
	const s32 *script;
	u16 scriptCursor;
	u16 facing;
	s32 x;
	s32 y;
	s32 z;
	s32 groundHeight;
	s32 scaleX;
	s32 scaleY;
	u16 radius; /* half the first part's box height */
	u8 layer;
	u8 priFlg;
	s32 velX;
	s32 velY;
	s32 velZ;
	s32 speedLimit;
	s32 acceleration;
	s32 tgtX;
	s32 tgtY;
	s32 tgtZ;
	s32 bounce;
	s32 gravity;
	s32 sceneWord;
	void *spr; /* one sprite, or a list of a two-part enemy's two */
	u8 sprStorageKind;
	u8 motionFlg;
	u8 arrivalAxis;
	u8 conditionResult;
	u8 snapToTgt;
	u8 collisionFlg;
	u8 steeringFlg;
	u8 paused;
	u8 retainSprCache;
	u8 repeatCnt;
	u16 wait;
	u8 refused;
	u8 frozen;
	u8 sceneCnt;
	u8 sceneByte;
	s16 tmr;
	u16 userValue;
	struct BATTLE_ACTOR *watchedActor;
	void (*update)(struct ACTOR *actor);
} BATTLE_ACTOR;

/* The gravity a battle unit falls under, about 0.67 pixels a frame per frame */
/* in 16.16: btMotionReturnToSpot sets it whenever a unit goes back to its */
/* spot, and effects that change a unit's gravity for a leap or a lift give it
 */
/* this one back. */
#define BATTLE_UNIT_GRAVITY (toFIXED(67) / 100)

STATIC_ASSERT(sizeof(BATTLE_ACTOR) == 112);
STATIC_ASSERT(offsetof(BATTLE_ACTOR, scaleX) == 24);
STATIC_ASSERT(offsetof(BATTLE_ACTOR, x) == 8);
STATIC_ASSERT(offsetof(BATTLE_ACTOR, velX) == 36);
STATIC_ASSERT(offsetof(BATTLE_ACTOR, tgtX) == 56);
STATIC_ASSERT(offsetof(BATTLE_ACTOR, motionFlg) == 85);
STATIC_ASSERT(offsetof(BATTLE_ACTOR, spr) == 80);
STATIC_ASSERT(offsetof(BATTLE_ACTOR, sprStorageKind) == 84);
STATIC_ASSERT(offsetof(BATTLE_ACTOR, update) == 108);

#endif
