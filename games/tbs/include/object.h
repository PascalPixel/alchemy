#ifndef ALCHEMY_OBJECT_H
#define ALCHEMY_OBJECT_H

/// @file
/// Object, an object's runtime state, and the object snapshots.

#include "unit/field/field.h"
#include "types.h"
#include "static_assert.h"

struct Sprite;
union FieldObject;

/// One of the 64 objects (HEAP_SLOT_OBJECTS): its script, position, motion
/// toward a target, sprites and update hook.
///
/// ObjectSystem_Initialize takes the objects with the object system's work
/// (HEAP_SLOT_OBJECT_SYSTEM), so it lives until the next map change. The
/// engine reads every object through this struct; maps read the same 0x70
/// bytes as struct ObjectEvent, or as struct FieldEffect for an effect, and
/// the few views kept for matching are listed in the field README.
struct Object {
  const s32
      *script;      ///< the object script it runs; null when the object is free
  s16 scriptCursor; ///< the script cursor, in words
  u16 facing;       ///< 1/65536 turns
  s32 x;            ///< 16.16 fixed-point pixels
  s32 y;            ///< 16.16 fixed-point pixels
  s32 z;            ///< 16.16 fixed-point pixels
  s32 groundHeight; ///< 16.16 fixed-point pixels
  s32 scaleX;       ///< 16.16 fixed point, 0x10000 full size
  s32 scaleY;       ///< 16.16 fixed point, 0x10000 full size
  u16 radius;       ///< collision radius in pixels
  u8 layer;         ///< the map layer its ground height is read from
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
  /// The share of its falling speed kept as it bounces on landing, 16.16
  /// fixed point; a floating object's phase instead.
  s32 bounce;
  /// Taken off velocityY each frame while it is above the ground, in 16.16
  /// fixed-point pixels per frame; also scales a floating object's bob.
  s32 gravity;
  /// Zeroed by Object_CreateField and not read by the engine; map code keeps a
  /// resting height here.
  s32 riseBaseY;
  /// Its sprite, by spriteStorageKind: with OBJECT_SPRITE_GROUP, a list of four
  /// sprite pointers, empty ones null, read through a (void **) or (struct
  /// Sprite **) cast. Neither a union nor a void pointer: as a union its
  /// stores move after the next store (Object_CreateField,
  /// ObjectEvent_Restore), and as void * the leader's sprite priority load in
  /// FieldEffect_UpdatePhasedRadialParticle moves.
  struct Sprite *sprite;
  u8 spriteStorageKind; ///< an enum ObjectSpriteKind in its low four bits
  /// ObjectEventMotionFlag bits: 3 when created, 4 for a placed object event
  /// above the ground; zero is free flight in 3D.
  u8 motionFlags;
  u8 arrivalAxis;     ///< the ARRIVAL_AXIS_* that decides arrival
  u8 conditionResult; ///< the object script's last flag test
  u8 snapToTarget;    ///< nonzero lands it exactly on its target
  /// ObjectEvent.collisionFlags: bit 0 solid, bit 1 the player pushes it,
  /// bits 2 to 4 for talk and Psynergy targets; placed object events take 1.
  u8 collisionFlags;
  u8 steeringFlags; ///< bit 0 faces its motion
  u8 paused;        ///< nonzero skips its script and motion
  /// Nonzero keeps its sprite's VRAM cache entry while sprites are suspended
  /// or it is off screen.
  u8 retainSpriteCache;
  u8 repeatCount; ///< the object script's SC_LOOP count
  s16 wait;       ///< the object script's wait, in frames; signed, as
                  ///< Object_UpdateAllThumb reads it
  /// Frames in a row a solid object has refused the object's move; SC_SYNC
  /// stops waiting after 60.
  u8 refused;
  /// Nonzero skips steering, terrain, gravity and floating; its velocity
  /// still moves it.
  u8 frozen;
  u8 riseCounter; ///< not read by the engine; map code's own counter
  u8 riseEnabled; ///< not read by the engine; map code's own state
  /// Two halfwords whose meaning depends on what runs the object.
  union {
    struct {
      /// The parameter its action script reads (a follow distance or an
      /// angle), or the player's frames to its next footprint.
      s16 action;
      /// The side of the player's next footprint (2 after a stop).
      s16 footprintSide;
    };
    /// Where SC_WANDER_HOME wanders around, in pixels: where it was created
    /// or placed.
    struct {
      s16 homeX;
      s16 homeZ;
    };
    /// Avoid's caster (FieldEffect_UpdatePairedArcSpawner).
    struct {
      s16 arcPhase;  ///< 1 spins the caster 0xc00 a frame
      s16 arcFrames; ///< frames since the arcs started
    };
    /// A Venus Djinni's falling motes
    /// (FieldEffect_UpdateDescendingOrbitObject).
    struct {
      u16 orbitFrames; ///< frames left of the spiral, 100 at the start
      s16 orbitAngle;  ///< its start on the circle, 1/65536 turns
    };
    /// A Whirlwind particle (FieldEffect_CircleAnchor,
    /// FieldEffect_WanderAroundAnchor).
    struct {
      u16 swirlFrames; ///< frames it has circled or wandered so far
      s16 turnFrames;  ///< frames left of a sharp turn
    };
    /// A spin that speeds up (FieldEffect_AdvanceAngleWithRamp).
    struct {
      s16 spinRampFrames; ///< frames the spin has been speeding up for
    };
  };
  /// The object its action script follows or faces.
  struct Object *linkedObject;
  /// The update hook (ObjectEvent.update), which the unused
  /// Object_SetUpdateValue sets as a word.
  void (*update)(union FieldObject *fieldObject);
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
  OBJECT_EVENT_SNAPSHOT_CAPACITY = 32, ///< object events a snapshot keeps
  OBJECT_EVENT_SNAPSHOT_EMPTY = 255    ///< a free slot, and the end of the list
};

/// Object events kept across a battle or a saved map, with parallel sprite
/// state. The slot list ends in OBJECT_EVENT_SNAPSHOT_EMPTY unless every entry
/// is filled.
struct ObjectEventSnapshot {
  u8 slots[OBJECT_EVENT_SNAPSHOT_CAPACITY]; ///< local ids, or
                                            ///< OBJECT_EVENT_SNAPSHOT_EMPTY
  struct Object objectEvents[OBJECT_EVENT_SNAPSHOT_CAPACITY]; ///< copied by DMA
  u8 animations[OBJECT_EVENT_SNAPSHOT_CAPACITY];  ///< each sprite's lastNo
  u8 spriteFlags[OBJECT_EVENT_SNAPSHOT_CAPACITY]; ///< restored through the
                                                  ///< dispatcher
  u8 priorities[OBJECT_EVENT_SNAPSHOT_CAPACITY];  ///< both image and shadow
                                                  ///< priority
};

STATIC_ASSERT(offsetof(struct ObjectEventSnapshot, objectEvents) == 0x20);
STATIC_ASSERT(offsetof(struct ObjectEventSnapshot, animations) == 0xe20);
STATIC_ASSERT(offsetof(struct ObjectEventSnapshot, spriteFlags) == 0xe40);
STATIC_ASSERT(offsetof(struct ObjectEventSnapshot, priorities) == 0xe60);
STATIC_ASSERT(sizeof(struct ObjectEventSnapshot) == 0xe80);

/// The one snapshot, in EWRAM outside the heap so that it outlives the map
/// change into and out of a battle. ObjectEvent_Snapshot fills it before a
/// random battle and when saving; ObjectEvent_Restore puts the object events
/// back when the party returns keeping its position (FLAG_KEEP_PARTY_POSITION).
extern struct ObjectEventSnapshot gObjectEventSnapshot;

#endif
