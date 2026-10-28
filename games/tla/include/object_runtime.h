#ifndef ALCHEMY_OBJECT_RUNTIME_H
#define ALCHEMY_OBJECT_RUNTIME_H

#include "types.h"

struct BattleAnimationState {
  u8 unk00[0x28];
  s16 *value28;
};

/// A field object: its script, position, motion toward a target, sprites and
/// update hook.
struct ObjectRuntime {
  s32 *script; ///< the object script it runs; null when the object is free
  s16 step;    ///< the script cursor, in words
  u16 angle;   ///< 1/65536 turns
  s32 x;       ///< 16.16 fixed-point pixels
  s32 y;       ///< 16.16 fixed-point pixels
  s32 z;       ///< 16.16 fixed-point pixels
  s32 terrainHeight; ///< 16.16 fixed-point pixels
  s32 scaleX;        ///< 16.16 fixed point, 0x10000 full size
  s32 scaleY;        ///< 16.16 fixed point, 0x10000 full size
  u16 radius;        ///< collision radius in pixels
  u8 terrainId;      ///< the map layer its ground height is read from
  /// Bit 0 is cleared when ObjectDispatch_SetSpritePriority fixes its
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
  /// drifting speed, a 0x31415927 marker).
  s32 unk4c;
  void *animation;  ///< its sprite, or its list of sprites, by animationKind
  u8 animationKind; ///< 1 one sprite, 2 a group of sprites
  u8 motionFlags;   ///< bit 1: falls by its gravity and bounces
  /// The axis watched for arrival at the target; zero once it arrives, and
  /// cleared with the target.
  u8 unk56;
  u8 unk57;        ///< not named by TLA code
  u8 snapToTarget; ///< nonzero lands it exactly on its target
  /// bits: 4 targeted from half as far again | 3 skipped by the target
  /// searches | 2 counts as 10/13 as far in the nearest search | 0 solid
  u8 collisionFlags;
  u8 actionFlags; ///< bit 0: turns toward its velocity, 1/16 turn a frame
  u8 movementState;
  u8 unk5c[0x04];
  void *callback;
  /// The parameter its action script reads (a follow distance or an angle to
  /// face).
  s16 action;
  u16 unk66;
  /// The object its action script follows or faces.
  struct ObjectRuntime *watchedObject;
  void (*update)(void); ///< the object's per-frame hook
  u8 unk70[0x10];
};

typedef char ObjectRuntime_size[sizeof(struct ObjectRuntime) == 0x80 ? 1 : -1];

typedef char ObjectRuntime_collision_flags_offset
    [(u32)&((struct ObjectRuntime *)0)->collisionFlags == 0x59 ? 1 : -1];

void Object_SetPositionAndResetMotion(struct ObjectRuntime *object, s32 x,
                                      s32 y, s32 z);
void ObjectMotion_MoveHalfwayTowardTarget(struct ObjectRuntime *object);
void ObjectMotion_MoveHalfwayTowardTargetCopy(struct ObjectRuntime *object);

struct ObjectRuntime *ObjectTable_Get(u32 objectId);
void Object_ResetMotion(struct ObjectRuntime *);
void Object_SetPosition(struct ObjectRuntime *, s32, s32, s32);
void Object_CommitPosition(struct ObjectRuntime *);
void Object_SetMode(struct ObjectRuntime *, s32);
void Object_RefreshSelector(struct ObjectRuntime *);
void Object_SetActionCallback(struct ObjectRuntime *, s32);
void ObjectMotion_SetActionVariant(u32, s32);
void ObjectMotion_WaitForAnimationChange(u32);
void Object_SetModeById(u32, s32);
void Object_RefreshSelectorById(u32);
void Sound_PlayFar(s32);

extern const u8 gObject_LinkedMotionScript[];
extern const u8 gObject_OffsetMotionScript[];
extern const u8 gObject_VariantMotionScripts[];

#endif
