#ifndef ALCHEMY_FIELD_OBJ_H
#define ALCHEMY_FIELD_OBJ_H

/// @file
/// Field objects and actors, and the scene transition styles.

#include "types.h"
#include "static_assert.h"

#include "event_work.h"

extern struct EventWork *gEventWork;

enum SceneTransitionStyle {
  TRANSITION_FADE = 0,
  TRANSITION_BACKDROP_FADE = 1,
  TRANSITION_WINDOW = 2
};

#define SCENE_TRANSITION(style, variant) ((style) << 8 | (variant))

#include "field_coordinate.h"

#include "animspr.h"

union FieldObject;

/// A field actor: a person or thing on the field with a position, motion
/// toward a target, sprites and an update callback.
struct FieldActor {
  u8 unk00[6];
  u16 facing;              ///< 1/65536 turns
  union FieldCoordinate x; ///< 16.16 fixed-point pixels
  union FieldCoordinate y; ///< 16.16 fixed-point pixels
  union FieldCoordinate z; ///< 16.16 fixed-point pixels
  s32 terrainHeight;       ///< 16.16 fixed-point pixels
  s32 scaleX;              ///< 16.16 fixed point, 0x10000 full size
  s32 scaleY;              ///< 16.16 fixed point, 0x10000 full size
  u16 radius;              ///< pixels
  u8 terrainId;
  u8 priorityFlags;
  s32 velocityX; ///< 16.16 fixed-point pixels per frame
  s32 velocityY; ///< 16.16 fixed-point pixels per frame
  s32 velocityZ; ///< 16.16 fixed-point pixels per frame
  s32 speed;     ///< 16.16 fixed-point pixels per frame
  s32 acceleration;
  s32 targetX; ///< 16.16 fixed-point pixels
  s32 targetY; ///< 16.16 fixed-point pixels
  s32 targetZ; ///< 16.16 fixed-point pixels
  s32 bounce;
  s32 gravity;
  s32 riseBaseY;
  union {
    struct AnimationObject *sprite;
    void **sprites;
  };
  /// Zero hides sprites; one draws a single sprite, two draws a group.
  u8 spriteStorageKind;
  u8 motionFlags;
  u8 unk56[3];
  u8 collisionFlags;
  u8 steeringFlags;
  u8 paused;
  u8 unk5c;
  u8 unk5d;
  u16 unk5e;
  u8 unk60;
  u8 unk61;
  u8 riseCounter;
  u8 riseEnabled;
  s16 timer;
  u16 work;
  struct FieldActor *linkedObject; ///< Linked object
  void (*update)(union FieldObject *object);
};

STATIC_ASSERT(offsetof(struct FieldActor, facing) == 0x06);
STATIC_ASSERT(offsetof(struct FieldActor, x) == 0x08);
STATIC_ASSERT(offsetof(struct FieldActor, z) == 0x10);
STATIC_ASSERT(offsetof(struct FieldActor, terrainHeight) == 0x14);
STATIC_ASSERT(offsetof(struct FieldActor, scaleY) == 0x1c);
STATIC_ASSERT(offsetof(struct FieldActor, priorityFlags) == 0x23);
STATIC_ASSERT(offsetof(struct FieldActor, motionFlags) == 0x55);
STATIC_ASSERT(offsetof(struct FieldActor, collisionFlags) == 0x59);
STATIC_ASSERT(offsetof(struct FieldActor, steeringFlags) == 0x5a);
STATIC_ASSERT(offsetof(struct FieldActor, paused) == 0x5b);
STATIC_ASSERT(offsetof(struct FieldActor, unk5e) == 0x5e);
STATIC_ASSERT(offsetof(struct FieldActor, riseEnabled) == 0x63);
STATIC_ASSERT(offsetof(struct FieldActor, bounce) == 0x44);
STATIC_ASSERT(offsetof(struct FieldActor, gravity) == 0x48);
STATIC_ASSERT(offsetof(struct FieldActor, riseBaseY) == 0x4c);
STATIC_ASSERT(offsetof(struct FieldActor, velocityY) == 0x28);
STATIC_ASSERT(offsetof(struct FieldActor, speed) == 0x30);
STATIC_ASSERT(offsetof(struct FieldActor, targetX) == 0x38);
STATIC_ASSERT(offsetof(struct FieldActor, sprite) == 0x50);
STATIC_ASSERT(offsetof(struct FieldActor, update) == 0x6c);

#define ACTOR_NO_TARGET ((s32)0x80000000)

/// Preserves the current action and motion when assigning a linked target.
enum ActorLinkFlag { ACTOR_LINK_KEEP_MOTION = 0x10000 };

enum ActorPriorityFlag {
  ACTOR_PRIORITY_AUTOMATIC = 0x01,
  ACTOR_PRIORITY_UNDERFOOT = 0x02
};

enum ActorMotionFlag { ACTOR_FOLLOWS_TERRAIN = 0x01, ACTOR_FALLS = 0x02 };

enum ActorSteeringFlag { ACTOR_FACES_MOTION = 0x01 };

#endif
