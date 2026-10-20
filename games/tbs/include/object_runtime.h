#ifndef ALCHEMY_OBJECT_RUNTIME_H
#define ALCHEMY_OBJECT_RUNTIME_H

/// @file
/// ObjectRuntime, a field object's runtime state, and the object snapshots.

#include "unit/field/field.h"
#include "object_lookup.h"
#include "types.h"
#include "static_assert.h"

/// One of the 64 field objects in the object pool (heap slot 5): its script,
/// position, motion toward a target, animation and update callback.
struct ObjectRuntime {
  s32 *script; ///< the object script it runs; null when the object is free
  s16 step;    ///< the script cursor, in words
  u16 angle;   ///< 1/65536 turns
  s32 x;       ///< 16.16 fixed-point pixels
  s32 y;       ///< 16.16 fixed-point pixels
  s32 z;       ///< 16.16 fixed-point pixels
  s32 terrainHeight; ///< 16.16 fixed-point pixels
  u8 unk18[0x08];    ///< the x and y scales, FieldActor.scaleX and scaleY
  u16 radius;        ///< collision radius in pixels
  u8 terrainId;      ///< the map layer its ground height is read from
  /// FieldActor.priorityFlags: 1 for placed actors, 2 for footprints.
  u8 unk23;
  s32 velocityX;  ///< 16.16 fixed-point pixels per frame
  s32 velocityY;  ///< 16.16 fixed-point pixels per frame
  s32 velocityZ;  ///< 16.16 fixed-point pixels per frame
  s32 speedLimit; ///< 16.16 fixed-point pixels per frame
  /// Added toward the target each frame, or taken off the speed when there is
  /// none, in 16.16 fixed-point pixels per frame; 0x10000 when created.
  s32 acceleration;
  s32 targetX; ///< 16.16 fixed-point pixels; 0x80000000 when unset
  s32 targetY; ///< 16.16 fixed-point pixels; 0x80000000 when unset
  s32 targetZ; ///< 16.16 fixed-point pixels; 0x80000000 when unset
  /// FieldActor's bounce and gravity, and a word FieldObject_Create zeroes.
  u8 unk44[0x0c];
  void *animation; ///< its sprite, or its list of four, by animationKind
  /// Low four bits: 0 no sprite, 1 one sprite, 2 a group of up to four.
  u8 animationKind;
  /// FieldActor.motionFlags: 3 when created, 4 for a placed actor above the
  /// ground; zero is free flight in 3D.
  u8 flags;
  u8 arrivalAxis;  ///< the ARRIVAL_AXIS_* that decides arrival
  u8 unk57;        ///< the object script's last flag test
  u8 snapToTarget; ///< nonzero lands it exactly on its target
  /// FieldActor.collisionFlags: bit 0 solid, bit 1 the player pushes it,
  /// bits 2 to 4 for talk and Psynergy targets; placed actors take 1.
  u8 unk59;
  u8 actionFlags;   ///< FieldActor.steeringFlags: bit 0 faces its motion
  u8 movementState; ///< nonzero skips its script and motion (paused)
  /// Byte 0, when nonzero, keeps its sprite's VRAM cache entry while sprites
  /// are suspended or it is off screen; byte 1 is the object script's
  /// SC_LOOP count.
  u8 unk5c[0x02];
  u16 unk5e; ///< the object script's wait, in frames
  /// Not named by TBS code through this struct. Its bytes are the frames a
  /// solid object has refused the object's move and the frozen flag
  /// (ScriptMotionObject's refused and frozen), then two bytes scene code
  /// uses.
  void *callback;
  /// The parameter its action script reads (a follow distance or an angle),
  /// or the player's frames to its next footprint.
  s16 action;
  s16 unk66; ///< the side of the player's next footprint (2 after a stop)
  /// The object its action script follows or faces.
  struct ObjectRuntime *linkedObject;
  /// The update hook (FieldActor.update), which the unused
  /// ObjectDispatch_SetUpdateValue sets as a word.
  u8 unk6c[0x04];
};

STATIC_ASSERT(sizeof(struct ObjectRuntime) == 0x70);
STATIC_ASSERT(offsetof(struct ObjectRuntime, arrivalAxis) == 0x56);
STATIC_ASSERT(offsetof(struct ObjectRuntime, snapToTarget) == 0x58);

enum { OBJECT_SNAPSHOT_CAPACITY = 32, OBJECT_SNAPSHOT_EMPTY = 255 };

/// Actors kept across a battle or a saved scene, with parallel sprite state.
/// The slot list ends in OBJECT_SNAPSHOT_EMPTY unless every entry is filled.
struct ObjectTableSnapshot {
  u8 slots[OBJECT_SNAPSHOT_CAPACITY]; ///< actor numbers, or
                                      ///< OBJECT_SNAPSHOT_EMPTY
  struct ObjectRuntime actors[OBJECT_SNAPSHOT_CAPACITY]; ///< copied by DMA
  u8 animations[OBJECT_SNAPSHOT_CAPACITY];  ///< each sprite's lastNo
  u8 spriteFlags[OBJECT_SNAPSHOT_CAPACITY]; ///< restored through the dispatcher
  u8 priorities[OBJECT_SNAPSHOT_CAPACITY];  ///< both image and shadow priority
};

STATIC_ASSERT(offsetof(struct ObjectTableSnapshot, actors) == 0x20);
STATIC_ASSERT(offsetof(struct ObjectTableSnapshot, animations) == 0xe20);
STATIC_ASSERT(offsetof(struct ObjectTableSnapshot, spriteFlags) == 0xe40);
STATIC_ASSERT(offsetof(struct ObjectTableSnapshot, priorities) == 0xe60);
STATIC_ASSERT(sizeof(struct ObjectTableSnapshot) == 0xe80);

extern struct ObjectTableSnapshot gObjectSnapshots;

#endif
