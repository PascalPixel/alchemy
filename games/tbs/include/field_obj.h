#ifndef ALCHEMY_FIELD_OBJ_H
#define ALCHEMY_FIELD_OBJ_H

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

#include "field_sprite.h"

union FieldObject;

struct FieldActor {
  u8 unk00[6];
  u16 facing;
  union FieldCoordinate x;
  union FieldCoordinate y;
  union FieldCoordinate z;
  s32 terrainHeight;
  s32 scaleX;
  s32 scaleY;
  u16 radius;
  u8 terrainId;
  u8 priorityFlags;
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  s32 speed;
  s32 acceleration;
  s32 targetX;
  s32 targetY;
  s32 targetZ;
  s32 bounce;
  s32 gravity;
  s32 riseBaseY;
  struct FieldSprite *sprite;
  u8 active;
  u8 motionFlags;
  u8 unk56[3];
  u8 collisionFlags;
  u8 steeringFlags;
  u8 paused;
  u8 unk5c;
  u8 unk5d[5];
  u8 riseCounter;
  u8 riseEnabled;
  s16 timer;
  u16 work;
  struct FieldActor *linkedObject; // Linked object
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

enum ActorPriorityFlag {
  ACTOR_PRIORITY_AUTOMATIC = 0x01,
  ACTOR_PRIORITY_UNDERFOOT = 0x02
};

enum ActorMotionFlag { ACTOR_FOLLOWS_TERRAIN = 0x01, ACTOR_FALLS = 0x02 };

enum ActorSteeringFlag { ACTOR_FACES_MOTION = 0x01 };

#endif
