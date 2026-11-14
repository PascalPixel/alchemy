#ifndef ALCHEMY_ACTOR_H
#define ALCHEMY_ACTOR_H

#include "field/field_coordinate.h"
#include "types.h"

/// Actor.targetX, targetY and targetZ when it has no movement target.
#define TARGET_UNSET ((s32)0x80000000)

/// A field object's sprite animation, which Actor.sprite points to
/// when animationKind is 1.
/// Sets an actor's position in one statement, as SET_POSITION does for a
/// particle.
#define SET_ACTOR_POSITION(p, fixedX, fixedY, fixedZ)                          \
  ((p)->x.fixed = (fixedX), (p)->y.fixed = (fixedY), (p)->z.fixed = (fixedZ))

struct ObjectAnimationState {
  u8 unk00[0x28];
  s16 *value28;
};

/// A field object: its script, position, motion toward a target, sprites and
/// update hook.
struct Actor {
  const s32 *script;        ///< the object script it runs; null when it is free
  s16 scriptCursor;         ///< the script cursor, in words
  u16 facing;               ///< 1/65536 turns
  struct FieldCoordinate x; ///< 16.16 fixed-point pixels
  struct FieldCoordinate y; ///< 16.16 fixed-point pixels
  struct FieldCoordinate z; ///< 16.16 fixed-point pixels
  s32 groundHeight;         ///< 16.16 fixed-point pixels
  s32 scaleX;               ///< 16.16 fixed point, 0x10000 full size
  s32 scaleY;               ///< 16.16 fixed point, 0x10000 full size
  u16 radius;               ///< collision radius in pixels
  u8 layer;                 ///< the map layer its ground height is read from
  /// Bit 0 is cleared when Actor_SetSpritePriorityDirect fixes its
  /// sprite's priority.
  u8 priorityFlags;
  s32 velocityX;  ///< 16.16 fixed-point pixels per frame
  s32 velocityY;  ///< 16.16 fixed-point pixels per frame
  s32 velocityZ;  ///< 16.16 fixed-point pixels per frame
  s32 speedLimit; ///< 16.16 fixed-point pixels per frame
  /// Added toward the target each frame, or taken off the speed when there is
  /// none, in 16.16 fixed-point pixels per frame.
  s32 acceleration;
  s32 targetX; ///< 16.16 fixed-point pixels; 0x80000000 when unset
  s32 targetY; ///< 16.16 fixed-point pixels; 0x80000000 when unset
  s32 targetZ; ///< 16.16 fixed-point pixels; 0x80000000 when unset
  /// The share of its falling speed kept as it bounces on landing, 16.16
  /// fixed point.
  s32 bounce;
  /// Taken off the vertical velocity each frame while it is above the
  /// ground, in 16.16 fixed-point pixels per frame.
  s32 gravity;
  /// Not read by the motion update; field code keeps its own values here (a
  /// drifting speed, a 0x31415927 marker), and scene code a resting height.
  s32 riseBaseY;
  /// Its sprite (a struct FieldSprite) when spriteStorageKind is 1, or its list
  /// of sprites when it is 2. Code that reads one kind converts it: typed, or
  /// as a union of the two, this pointer compiles differently where a store
  /// comes between two of its reads.
  void *sprite;
  /// 1 one sprite, 2 a group of sprites, in the low four bits, which is all
  /// the field and the scenes test.
  u8 spriteStorageKind;
  u8 motionFlags; ///< bit 1: falls by its gravity and bounces
  /// The axis watched for arrival at the target; zero once it arrives, and
  /// cleared with the target.
  u8 arrivalAxis;
  u8 conditionResult; ///< the actor script's last flag test
  u8 snapToTarget;    ///< nonzero lands it exactly on its target
  /// bits: 4 targeted from half as far again | 3 skipped by the target
  /// searches | 2 counts as 10/13 as far in the nearest search | 0 solid
  u8 collisionFlags;
  u8 steeringFlags; ///< bit 0: turns toward its velocity, 1/16 turn a frame
  u8 paused;
  /// Keeps its sprite's VRAM cache entry (Actor_SetItemIcon sets it).
  u8 retainSpriteCache;
  u8 repeatCount; ///< the actor script's loop count
  s16 wait;       ///< the actor script's wait, in frames
  /// The field's effects keep a hook here; a scene whose actor bobs keeps its
  /// own frame count in the third byte instead.
  union {
    void *callback;
    struct {
      u8 refused; ///< frames in a row a solid actor has refused its move
      u8 frozen;
      u8 riseCounter; ///< frames into the bob, its low six bits a cycle
      /// A block's: nonzero while it blocks the cells ahead of and behind it.
      u8 blockMarked;
    };
  };
  /// The parameter its action script reads (a follow distance or an angle to
  /// face).
  s16 timer;
  s16 userValue; ///< the side of the player's next footprint, or scene scratch
  /// The object its action script follows or faces.
  struct Actor *watchedActor;
  void (*update)(void); ///< the object's per-frame hook
  u8 unk70[0x10];
};

typedef char Actor_size[sizeof(struct Actor) == 0x80 ? 1 : -1];

typedef char Actor_collision_flags_offset
    [(u32)&((struct Actor *)0)->collisionFlags == 0x59 ? 1 : -1];

void Actor_SetPositionAndResetMotion(struct Actor *object, s32 x, s32 y, s32 z);
void Actor_MoveHalfwayTowardTarget(struct Actor *object);
void Actor_MoveHalfwayTowardTargetCopy(struct Actor *object);

struct Actor *ObjectTable_Get(u32 objectId);
void Actor_StartIdleFar(struct Actor *);
void Actor_SetMoveTargetFar(struct Actor *, s32, s32, s32);
void Actor_WaitForMoveFar(struct Actor *);
void Actor_SelectAnimationFar(struct Actor *, s32);
void Actor_WaitForCommandEndFar(struct Actor *);
void Actor_SetActionCallback(struct Actor *, s32);
void Actor_SetSpritePriority(u32, s32);
void OverlayObject_WaitUntilIdle(struct Actor *object);
void Actor_WaitForAnimationChange(u32);
void Actor_SetModeById(u32, s32);
void Actor_RefreshSelectorById(u32);
void Sound_PlayFar(s32);

extern const u8 gActor_FaceWatchedScript[];
extern const u8 gActor_OffsetScript[];
extern const u8 gActor_VariantScripts[];

#endif
