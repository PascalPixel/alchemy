#ifndef ALCHEMY_BATTLE_ACTOR_H
#define ALCHEMY_BATTLE_ACTOR_H

/// @file
/// BattleActor, the view of struct Actor one battle function gives a
/// unit its sprites through.

#include "edition.h"
#include "static_assert.h"
#include "types.h"
#include "battle/battle_action.h"
#include "gamepak/far_call.h"

struct Actor;

/// struct Actor, field for field, as BattleObject_SpawnObjectsForList sees a
/// battle unit's actor while it gives it its sprites: the sprite word untyped
/// (void *), and the cursor, wait and userValue unsigned. The plain form reads
/// the actor through struct Actor.
struct BattleActor {
  const s32 *script;
  u16 scriptCursor;
  u16 facing;
  s32 x;
  s32 y;
  s32 z;
  s32 groundHeight;
  s32 scaleX;
  s32 scaleY;
  u16 radius; ///< half the first part's box height
  u8 layer;
  u8 priorityFlags;
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  s32 speedLimit;
  s32 acceleration;
  s32 targetX;
  s32 targetY;
  s32 targetZ;
  s32 bounce;
  s32 gravity;
  s32 riseBaseY;
  void *sprite; ///< one sprite, or a list of a two-part enemy's two
  u8 spriteStorageKind;
  u8 motionFlags;
  u8 arrivalAxis;
  u8 conditionResult;
  u8 snapToTarget;
  u8 collisionFlags;
  u8 steeringFlags;
  u8 paused;
  u8 retainSpriteCache;
  u8 repeatCount;
  u16 wait;
  u8 refused;
  u8 frozen;
  u8 riseCounter;
  u8 riseEnabled;
  s16 timer;
  u16 userValue;
  struct BattleActor *watchedActor;
  void (*update)(struct Actor *actor);
};

/// The gravity a battle unit falls under, about 0.67 pixels a frame per frame
/// in 16.16: BattleMotion_ReturnToSpot sets it whenever a unit goes back to its
/// spot, and effects that change a unit's gravity for a leap or a lift give it
/// this one back.
#define BATTLE_UNIT_GRAVITY (toFIXED(67) / 100)

STATIC_ASSERT(sizeof(struct BattleActor) == 112);
STATIC_ASSERT(offsetof(struct BattleActor, scaleX) == 24);
STATIC_ASSERT(offsetof(struct BattleActor, x) == 8);
STATIC_ASSERT(offsetof(struct BattleActor, velocityX) == 36);
STATIC_ASSERT(offsetof(struct BattleActor, targetX) == 56);
STATIC_ASSERT(offsetof(struct BattleActor, motionFlags) == 85);
STATIC_ASSERT(offsetof(struct BattleActor, sprite) == 80);
STATIC_ASSERT(offsetof(struct BattleActor, spriteStorageKind) == 84);
STATIC_ASSERT(offsetof(struct BattleActor, update) == 108);

#endif
