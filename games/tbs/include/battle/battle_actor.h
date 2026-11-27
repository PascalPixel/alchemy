#ifndef ALCHEMY_BATTLE_ACTOR_H
#define ALCHEMY_BATTLE_ACTOR_H

// BATTLE_ACTOR, the view of ACTOR one battle function gives a unit its sprites through.

#include "edition.h"
#include "static_assert.h"
#include "types.h"
#include "battle/battle_action.h"
#include "stubs.h"

struct ACTOR;

// ACTOR, field for field, as btObjectSpawnObjectsForList sees a battle unit's actor while it gives
// it its sprites: the sprite word untyped (void *), and the cursor, wait and userValue unsigned.
// The plain form reads the actor through ACTOR.
typedef struct BATTLE_ACTOR
{
	const Sint32        *script;
	Uint16               scriptCursor;
	Uint16               facing;
	Sint32               x;
	Sint32               y;
	Sint32               z;
	Sint32               groundHeight;
	Sint32               scaleX;
	Sint32               scaleY;
	Uint16               radius; // half the first part's box height
	Uint8                layer;
	Uint8                priorityFlags;
	Sint32               velocityX;
	Sint32               velocityY;
	Sint32               velocityZ;
	Sint32               speedLimit;
	Sint32               acceleration;
	Sint32               targetX;
	Sint32               targetY;
	Sint32               targetZ;
	Sint32               bounce;
	Sint32               gravity;
	Sint32               sceneWord;
	void                *sprite; // one sprite, or a list of a two-part enemy's two
	Uint8                spriteStorageKind;
	Uint8                motionFlags;
	Uint8                arrivalAxis;
	Uint8                conditionResult;
	Uint8                snapToTarget;
	Uint8                collisionFlags;
	Uint8                steeringFlags;
	Uint8                paused;
	Uint8                retainSpriteCache;
	Uint8                repeatCount;
	Uint16               wait;
	Uint8                refused;
	Uint8                frozen;
	Uint8                sceneCount;
	Uint8                sceneByte;
	Sint16               timer;
	Uint16               userValue;
	struct BATTLE_ACTOR *watchedActor;
	void                 (*update)(struct ACTOR *actor);
} BATTLE_ACTOR;

// The gravity a battle unit falls under, about 0.67 pixels a tick per tick in 16.16:
// btMotionReturnToSpot sets it whenever a unit goes back to its spot, and effects that change a
// unit's gravity for a leap or a lift give it this one back.
#define BATTLE_UNIT_GRAVITY (toFIXED(67) / 100)

STATIC_ASSERT(sizeof(BATTLE_ACTOR) == 112);
STATIC_ASSERT(offsetof(BATTLE_ACTOR, scaleX) == 24);
STATIC_ASSERT(offsetof(BATTLE_ACTOR, x) == 8);
STATIC_ASSERT(offsetof(BATTLE_ACTOR, velocityX) == 36);
STATIC_ASSERT(offsetof(BATTLE_ACTOR, targetX) == 56);
STATIC_ASSERT(offsetof(BATTLE_ACTOR, motionFlags) == 85);
STATIC_ASSERT(offsetof(BATTLE_ACTOR, sprite) == 80);
STATIC_ASSERT(offsetof(BATTLE_ACTOR, spriteStorageKind) == 84);
STATIC_ASSERT(offsetof(BATTLE_ACTOR, update) == 108);

#endif
