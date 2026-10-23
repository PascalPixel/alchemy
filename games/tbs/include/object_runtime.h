#ifndef ALCHEMY_OBJECT_RUNTIME_H
#define ALCHEMY_OBJECT_RUNTIME_H

/// @file
/// Object, a field object's runtime state, and the object snapshots.

#include "unit/field/field.h"
#include "object_lookup.h"
#include "types.h"
#include "static_assert.h"

union FieldObject;

/// One of the 64 field objects in the object pool (HEAP_SLOT_OBJECTS): its
/// script, position, motion toward a target, animation and update callback.
///
/// ObjectSystem_Initialize takes the pool with the object system's work
/// (HEAP_SLOT_OBJECT_SYSTEM), so it lives until the next map change. struct
/// ObjectEvent is the same 0x70 bytes as maps see them.
struct Object {
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
  /// ObjectEvent.priorityFlags: 1 for placed object events, 2 for footprints.
  u8 priorityFlags;
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
  /// ObjectEvent's bounce and gravity, and the word Object_CreateField zeroes
  /// and map code keeps a resting height in; not named here.
  ///@{
  s32 bounce;
  s32 gravity;
  s32 riseBaseY;
  ///@}
  void *animation; ///< its sprite, or its list of four, by animationKind
  /// Low four bits: 0 no sprite, 1 one sprite, 2 a group of up to four.
  u8 animationKind;
  /// ObjectEvent.motionFlags: 3 when created, 4 for a placed object event above
  /// the ground; zero is free flight in 3D.
  u8 flags;
  u8 arrivalAxis;     ///< the ARRIVAL_AXIS_* that decides arrival
  u8 conditionResult; ///< the object script's last flag test
  u8 snapToTarget;    ///< nonzero lands it exactly on its target
  /// ObjectEvent.collisionFlags: bit 0 solid, bit 1 the player pushes it,
  /// bits 2 to 4 for talk and Psynergy targets; placed object events take 1.
  u8 collisionFlags;
  u8 actionFlags;   ///< ObjectEvent.steeringFlags: bit 0 faces its motion
  u8 movementState; ///< nonzero skips its script and motion (paused)
  /// Nonzero keeps its sprite's VRAM cache entry while sprites are suspended
  /// or it is off screen.
  u8 retainSpriteCache;
  u8 repeatCount; ///< the object script's SC_LOOP count
  u16 wait;       ///< the object script's wait, in frames
  /// Not named by TBS code through this struct. Its bytes are the frames a
  /// solid object has refused the object's move and the frozen flag
  /// (ScriptMotionObject's refused and frozen), then two bytes map code
  /// uses.
  void *callback;
  /// The parameter its action script reads (a follow distance or an angle),
  /// or the player's frames to its next footprint.
  s16 action;
  s16 footprintSide; ///< the side of the player's next footprint (2 after a
                     ///< stop)
  /// The object its action script follows or faces.
  struct Object *linkedObject;
  /// The update hook (ObjectEvent.update), which the unused
  /// ObjectDispatch_SetUpdateValue sets as a word.
  void (*update)(union FieldObject *object);
};

STATIC_ASSERT(sizeof(struct Object) == 0x70);
STATIC_ASSERT(offsetof(struct Object, priorityFlags) == 0x23);
STATIC_ASSERT(offsetof(struct Object, arrivalAxis) == 0x56);
STATIC_ASSERT(offsetof(struct Object, conditionResult) == 0x57);
STATIC_ASSERT(offsetof(struct Object, snapToTarget) == 0x58);
STATIC_ASSERT(offsetof(struct Object, collisionFlags) == 0x59);
STATIC_ASSERT(offsetof(struct Object, wait) == 0x5e);
STATIC_ASSERT(offsetof(struct Object, footprintSide) == 0x66);

enum {
  OBJECT_SNAPSHOT_CAPACITY = 32, ///< object events a snapshot keeps
  OBJECT_SNAPSHOT_EMPTY = 255    ///< a free slot, and the end of the list
};

/// Object events kept across a battle or a saved map, with parallel sprite
/// state. The slot list ends in OBJECT_SNAPSHOT_EMPTY unless every entry is
/// filled.
struct ObjectTableSnapshot {
  u8 slots[OBJECT_SNAPSHOT_CAPACITY]; ///< local ids, or
                                      ///< OBJECT_SNAPSHOT_EMPTY
  struct Object objectEvents[OBJECT_SNAPSHOT_CAPACITY]; ///< copied by DMA
  u8 animations[OBJECT_SNAPSHOT_CAPACITY];  ///< each sprite's lastNo
  u8 spriteFlags[OBJECT_SNAPSHOT_CAPACITY]; ///< restored through the dispatcher
  u8 priorities[OBJECT_SNAPSHOT_CAPACITY];  ///< both image and shadow priority
};

STATIC_ASSERT(offsetof(struct ObjectTableSnapshot, objectEvents) == 0x20);
STATIC_ASSERT(offsetof(struct ObjectTableSnapshot, animations) == 0xe20);
STATIC_ASSERT(offsetof(struct ObjectTableSnapshot, spriteFlags) == 0xe40);
STATIC_ASSERT(offsetof(struct ObjectTableSnapshot, priorities) == 0xe60);
STATIC_ASSERT(sizeof(struct ObjectTableSnapshot) == 0xe80);

/// The one snapshot, in EWRAM outside the heap so that it outlives the map
/// change into and out of a battle. ObjectTable_Snapshot fills it before a
/// random battle and when saving; ObjectTable_Restore puts the object events
/// back when the party returns keeping its position (FLAG_KEEP_PARTY_POSITION).
extern struct ObjectTableSnapshot gObjectSnapshots;

#endif
