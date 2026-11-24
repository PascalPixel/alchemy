#ifndef ALCHEMY_MOTION_OBJECT_H
#define ALCHEMY_MOTION_OBJECT_H

/// @file
/// The motion object: a battle unit's actor as the battle moves it, and the
/// sprites it draws.

#include "fixed_math.h"
#include "types.h"
#include "static_assert.h"
#include "battle_action.h"
#include "edition.h"

union FieldObject;
struct Actor;

/// An effect object that moves and draws one sprite: its position, scale,
/// velocity and target, and its per-frame update. It is one of the object
/// slots the field's FieldActor also describes.
struct MotionObject {
  const s32 *script;  ///< the object script it runs; null when it is free
  s16 scriptCursor;   ///< the script cursor, in words
  u16 angle;          ///< 1/65536 turns
  FIXED x;            ///< 16.16 fixed-point pixels
  FIXED y;            ///< 16.16 fixed-point pixels
  FIXED z;            ///< 16.16 fixed-point pixels
  FIXED groundHeight; ///< 16.16 fixed-point pixels
  FIXED scaleX;       ///< 16.16 fixed point, FIXED_ONE full size
  FIXED scaleY;       ///< 16.16 fixed point, FIXED_ONE full size
  u16 height;         ///< no code names it through this view
  u8 layer;           ///< the map layer its ground height is read from
  u8 priorityFlags;
  FIXED velocityX;  ///< 16.16 fixed-point pixels per frame
  FIXED velocityY;  ///< 16.16 fixed-point pixels per frame
  FIXED velocityZ;  ///< 16.16 fixed-point pixels per frame
  FIXED speedLimit; ///< 16.16 fixed-point pixels per frame
  /// Horizontal speed gained each frame towards the target, or lost braking
  /// without one, in 16.16 fixed-point pixels per frame.
  FIXED acceleration;
  FIXED targetX; ///< 16.16 fixed-point pixels; 0x80000000 for no target
  FIXED targetY; ///< 16.16 fixed-point pixels
  FIXED targetZ; ///< 16.16 fixed-point pixels
  /// With motionFlags bit 1, the 16.16 share of velocityY kept on a bounce.
  FIXED verticalMotionPhase;
  /// Gravity: taken from velocityY each frame above the ground with
  /// motionFlags bit 1, in 16.16 fixed-point pixels per frame.
  FIXED verticalMotionStrength;
  s32 riseBaseY;
  void *records; ///< one sprite, or a table of them (recordStorageKind)
  /// bits: 3..0 how records holds the sprites
  u8 recordStorageKind;
  /// bits: 1 falls under gravity and bounces | 0 follows the terrain
  u8 motionFlags;
  u8 arrivalAxis;
  u8 conditionResult; ///< the object script's last flag test
  u8 snapToTarget;    ///< nonzero: lands exactly on the target
  u8 collisionFlags;
  u8 autoFaceMotion; ///< bit 0 turns the object to face where it moves
  u8 paused;
  u8 retainSpriteCache;
  u8 repeatCount; ///< the object script's loop count
  s16 wait;       ///< the object script's wait, in frames
  void *callback;
  s32 timer;
  struct Actor *watchedActor;
  void (*update)(union FieldObject *object);
};

/// The gravity a battle unit falls under, about 0.67 pixels a frame per frame
/// in 16.16, as verticalMotionStrength: BattleMotion_ReturnToSpot sets it
/// whenever a unit goes back to its spot, and effects that change a unit's
/// gravity for a leap or a lift give it this one back.
#define BATTLE_UNIT_GRAVITY (toFIXED(67) / 100)

void BattleObject_DestroySprites(s32 objectId);
void Sprite_DetachScripts(void *recordGroup);
void Actor_DetachSpriteScripts(struct MotionObject *object);

STATIC_ASSERT(sizeof(struct MotionObject) == 112);
STATIC_ASSERT(offsetof(struct MotionObject, scaleX) == 24);
STATIC_ASSERT(offsetof(struct MotionObject, x) == 8);
STATIC_ASSERT(offsetof(struct MotionObject, velocityX) == 36);
STATIC_ASSERT(offsetof(struct MotionObject, targetX) == 56);
STATIC_ASSERT(offsetof(struct MotionObject, motionFlags) == 85);
STATIC_ASSERT(offsetof(struct MotionObject, records) == 80);
STATIC_ASSERT(offsetof(struct MotionObject, recordStorageKind) == 84);
STATIC_ASSERT(offsetof(struct MotionObject, update) == 108);

s32 BattleObject_MarkForRelease(s32 objectId);
struct BattleObject *BattleObject_Get(s32 objectId);
/// Returns a unit's battle object, or null for a unit with no place
/// (BattleObject_Get).
struct BattleObject *BattleObject_GetFar(s32 objectId);
void *Actor_GetSprite(struct MotionObject *object, s32 recordIndex);

#endif
