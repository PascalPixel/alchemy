#ifndef ALCHEMY_BATTLE_ACTOR_H
#define ALCHEMY_BATTLE_ACTOR_H

/// @file
/// BattleActor, the view of struct Actor one battle function gives a
/// unit its sprites through, and SpriteAnimationHeadView, the start of a sprite
/// sprite animation.

#include "types.h"
#include "static_assert.h"
#include "battle_session.h"
#include "edition.h"
#include "farcall.h"

struct Actor;

/// struct Actor, field for field, as BattleObject_SpawnObjectsForList gives a
/// battle unit its sprites: the sprite word untyped (void *), and the cursor,
/// wait and userValue unsigned. Through struct Actor, whose sprite is a
/// struct Sprite pointer, the store of a two-part enemy's sprite list moves
/// after the next store, and a view that pads the fields it does not read
/// with bytes moves it too (MATCHING.md).
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

/// A sprite animation, as battle objects set its palette and
/// drawing priority: the start of a SpriteAnimation.
struct SpriteAnimationHeadView {
  /// SpriteAnimation's animationId, timer and kind; not named through this
  /// view.
  ///@{
  s16 animationId;
  u16 timer;
  u8 kind;
  ///@}
  u8 palette; ///< SpriteAnimation.colorMode, the SpriteColorMode;
              ///< BattleObject.palette
  /// SpriteAnimation.priority: the body and weapon 1, the status icon 3, and
  /// the effect sprite 0, or 9 to hide it while the unit is off the ground.
  u8 priority;
};

STATIC_ASSERT(sizeof(struct BattleActor) == 0x70);
STATIC_ASSERT(offsetof(struct BattleActor, scaleX) == 0x18);
STATIC_ASSERT(offsetof(struct BattleActor, x) == 0x08);
STATIC_ASSERT(offsetof(struct BattleActor, velocityX) == 0x24);
STATIC_ASSERT(offsetof(struct BattleActor, targetX) == 0x38);
STATIC_ASSERT(offsetof(struct BattleActor, motionFlags) == 0x55);
STATIC_ASSERT(offsetof(struct BattleActor, sprite) == 0x50);
STATIC_ASSERT(offsetof(struct BattleActor, spriteStorageKind) == 0x54);
STATIC_ASSERT(offsetof(struct BattleActor, update) == 0x6c);
STATIC_ASSERT(sizeof(struct SpriteAnimationHeadView) == 0x08);
STATIC_ASSERT(offsetof(struct SpriteAnimationHeadView, palette) == 0x05);
STATIC_ASSERT(offsetof(struct SpriteAnimationHeadView, priority) == 0x06);

#endif
