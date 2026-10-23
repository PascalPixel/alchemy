#ifndef ALCHEMY_OBJECT_EVENT_H
#define ALCHEMY_OBJECT_EVENT_H

/// @file
/// Field objects and object events, and the map transition styles.

#include "types.h"
#include "static_assert.h"

#include "event_work.h"

/// The running field map's work, heap slot HEAP_SLOT_FIELD, as the maps
/// read it; Map_Run takes it and frees it.
extern struct EventWork *gFieldWork;

/// How a map fades in and out: MAP_TRANSITION's high byte, which
/// ScreenTransition_Start and ScreenTransition_Finish read.
enum MapTransitionStyle {
  TRANSITION_FADE = 0,          ///< from or to black
  TRANSITION_BACKDROP_FADE = 1, ///< from or to the backdrop colour
  TRANSITION_WINDOW = 2,        ///< a window shape the variant chooses
  TRANSITION_DISSOLVE = 3,      ///< a dissolve
  /// Opening from, or closing to, the centre line; the variant picks how.
  TRANSITION_CENTRE_LINE = 4
};

/// A map transition as EventWork.transition holds it. For
/// TRANSITION_WINDOW the variant is the window mode, listed at
/// ScreenTransition_UpdateScanlineTable: 3 a circle at the centre, 9 an
/// ellipse around the leader, and others.
#define MAP_TRANSITION(style, variant) ((style) << 8 | (variant))

#include "field_coordinate.h"

#include "animspr.h"

union FieldObject;

/// An object event: a person or thing on the field with a position, motion
/// toward a target, sprites and an update callback.
struct ObjectEvent {
  const s32 *script;       ///< the object script it runs; null when it is free
  u16 scriptCursor;        ///< in words (Object's step)
  u16 facing;              ///< 1/65536 turns
  union FieldCoordinate x; ///< 16.16 fixed-point pixels
  union FieldCoordinate y; ///< 16.16 fixed-point pixels
  union FieldCoordinate z; ///< 16.16 fixed-point pixels
  s32 groundHeight;        ///< 16.16 fixed-point pixels
  s32 scaleX;              ///< 16.16 fixed point, 0x10000 full size
  s32 scaleY;              ///< 16.16 fixed point, 0x10000 full size
  /// Collision radius in pixels: 16, or half the sprite metadata's boxY, when
  /// created; the overlap checks use it less 2.
  u16 radius;
  /// The map layer (0 to 3) its ground height is read from, which the
  /// scrolling camera updates from the cell under it; the fixed camera passes
  /// it as the sprite's draw slot.
  u8 layer;
  /// ObjectEventPriorityFlag bits, read by the scrolling camera.
  u8 priorityFlags;
  s32 velocityX; ///< 16.16 fixed-point pixels per frame
  s32 velocityY; ///< 16.16 fixed-point pixels per frame
  s32 velocityZ; ///< 16.16 fixed-point pixels per frame
  s32 speed;     ///< speed limit, 16.16 fixed-point pixels per frame
  /// Added toward the target each frame, or taken off the speed when there is
  /// none, in 16.16 fixed-point pixels per frame; 0x10000 when created.
  s32 acceleration;
  s32 targetX; ///< 16.16 fixed-point pixels; OBJ_EVENT_NO_TARGET when unset
  s32 targetY; ///< 16.16 fixed-point pixels; OBJ_EVENT_NO_TARGET when unset
  s32 targetZ; ///< 16.16 fixed-point pixels; OBJ_EVENT_NO_TARGET when unset
  /// The share of its falling speed kept as it bounces on landing, 16.16
  /// fixed point (0x4000 when created); a floating object's phase instead.
  s32 bounce;
  /// Taken off the vertical velocity each frame while it is above the
  /// ground, in 16.16 fixed-point pixels per frame (0x10000 when created);
  /// also scales a floating object's bob.
  s32 gravity;
  /// Zeroed by Object_CreateField and not read by the engine; map code
  /// keeps a resting height, a sway or a countdown here.
  s32 riseBaseY;
  union {
    struct AnimationObject *sprite; ///< OBJECT_SPRITE_ONE
    void **sprites; ///< OBJECT_SPRITE_GROUP: four, empty ones null
  };
  /// Zero hides sprites; one draws a single sprite, two draws a group.
  /// The engine reads the low four bits; SC_SHOW_SPRITE and SC_HIDE_SPRITES set
  /// 1 and 0.
  u8 spriteStorageKind;
  /// ObjectEventMotionFlag bits; none is free flight in 3D, and
  /// OBJ_EVENT_FOLLOWS_TERRAIN and OBJ_EVENT_FALLS are set on creation.
  u8 motionFlags;
  u8 arrivalAxis; ///< the ARRIVAL_AXIS_* watched for arrival
  /// The object script's last flag test (ObjectScriptContext.conditionResult).
  u8 conditionResult;
  u8 snapToTarget; ///< nonzero lands it exactly on its target on arrival
  /// ObjectEventCollisionFlag bits.
  u8 collisionFlags;
  u8 steeringFlags; ///< ObjectEventSteeringFlag bits
  u8 paused;        ///< nonzero skips its script and motion, not its update
  /// Nonzero keeps its sprite's VRAM cache entry while sprites are suspended
  /// or it is off screen (ObjectSystem_PlaceSprites).
  u8 retainSpriteCache;
  u8 repeatCount; ///< the object script's SC_LOOP count (ObjectScriptContext)
  u16 wait;       ///< the object script's wait, in frames (ScriptMotionObject)
  /// Frames a solid object has refused its move (ScriptMotionObject's
  /// refused); SC_SYNC stops waiting after 60.
  u8 refused;
  /// Nonzero skips steering, terrain, gravity and floating; its velocity
  /// still moves it (ScriptMotionObject.frozen).
  u8 frozen;
  u8 riseCounter; ///< not read by the engine; map code's own counter
  u8 riseEnabled; ///< not read by the engine; map code's own state
  /// The parameter its action script reads (a follow distance or an angle to
  /// face), the player's frames to its next footprint, or map scratch.
  s16 timer;
  /// The side of the player's next footprint (2 after a stop), or map
  /// scratch.
  u16 work;
  /// The object its action script follows or faces.
  struct ObjectEvent *linkedObject;
  /// Called each frame before the object's script, while the object is live.
  void (*update)(union FieldObject *fieldObject);
};

STATIC_ASSERT(offsetof(struct ObjectEvent, facing) == 0x06);
STATIC_ASSERT(offsetof(struct ObjectEvent, x) == 0x08);
STATIC_ASSERT(offsetof(struct ObjectEvent, z) == 0x10);
STATIC_ASSERT(offsetof(struct ObjectEvent, groundHeight) == 0x14);
STATIC_ASSERT(offsetof(struct ObjectEvent, scaleY) == 0x1c);
STATIC_ASSERT(offsetof(struct ObjectEvent, priorityFlags) == 0x23);
STATIC_ASSERT(offsetof(struct ObjectEvent, motionFlags) == 0x55);
STATIC_ASSERT(offsetof(struct ObjectEvent, collisionFlags) == 0x59);
STATIC_ASSERT(offsetof(struct ObjectEvent, steeringFlags) == 0x5a);
STATIC_ASSERT(offsetof(struct ObjectEvent, paused) == 0x5b);
STATIC_ASSERT(offsetof(struct ObjectEvent, retainSpriteCache) == 0x5c);
STATIC_ASSERT(offsetof(struct ObjectEvent, repeatCount) == 0x5d);
STATIC_ASSERT(offsetof(struct ObjectEvent, wait) == 0x5e);
STATIC_ASSERT(offsetof(struct ObjectEvent, refused) == 0x60);
STATIC_ASSERT(offsetof(struct ObjectEvent, frozen) == 0x61);
STATIC_ASSERT(offsetof(struct ObjectEvent, riseEnabled) == 0x63);
STATIC_ASSERT(offsetof(struct ObjectEvent, bounce) == 0x44);
STATIC_ASSERT(offsetof(struct ObjectEvent, gravity) == 0x48);
STATIC_ASSERT(offsetof(struct ObjectEvent, riseBaseY) == 0x4c);
STATIC_ASSERT(offsetof(struct ObjectEvent, velocityY) == 0x28);
STATIC_ASSERT(offsetof(struct ObjectEvent, speed) == 0x30);
STATIC_ASSERT(offsetof(struct ObjectEvent, targetX) == 0x38);
STATIC_ASSERT(offsetof(struct ObjectEvent, sprite) == 0x50);
STATIC_ASSERT(offsetof(struct ObjectEvent, update) == 0x6c);

/// The values of ObjectEvent.targetX, targetY and targetZ that are not
/// coordinates.
enum ObjectEventTarget {
  /// A target coordinate that is not set: the object has no target.
  OBJ_EVENT_NO_TARGET = (s32)0x80000000
};

/// Preserves the current action and motion when assigning a linked target.
enum ObjectEventLinkFlag { OBJ_EVENT_LINK_KEEP_MOTION = 0x10000 };

/// ObjectEvent.priorityFlags bits.
enum ObjectEventPriorityFlag {
  /// The sprite takes its priority from the map cell under it.
  OBJ_EVENT_PRIORITY_AUTOMATIC = 0x01,
  /// The depth sort treats it as 320 pixels higher on the screen.
  OBJ_EVENT_PRIORITY_UNDERFOOT = 0x02,
  /// The depth sort treats it as 320 pixels lower on the screen.
  OBJ_EVENT_PRIORITY_OVERHEAD = 0x04
};

/// ObjectEvent.motionFlags bits.
enum ObjectEventMotionFlag {
  OBJ_EVENT_FOLLOWS_TERRAIN = 0x01, ///< moves with the ground height under it
  OBJ_EVENT_FALLS = 0x02,           ///< falls by its gravity and bounces
  OBJ_EVENT_FLOATS = 0x04,          ///< bobs up and down by its gravity
  OBJ_EVENT_FLOATS_HIGH = 0x08, ///< with OBJ_EVENT_FLOATS, four times as far
  /// Keeps its speed on slopes, which slow others down.
  OBJ_EVENT_KEEPS_SPEED_ON_SLOPES = 0x10
};

/// ObjectEvent.collisionFlags bits.
enum ObjectEventCollisionFlag {
  OBJ_EVENT_SOLID = 0x01,         ///< blocks other objects and the player
  OBJ_EVENT_PUSHABLE = 0x02,      ///< the player pushes it
  OBJ_EVENT_NEAR_TALK = 0x04,     ///< nearer to talk to
  OBJ_EVENT_NOT_TARGETED = 0x08,  ///< never a talk or Psynergy target
  OBJ_EVENT_NEAR_PSYNERGY = 0x10, ///< nearer for Psynergy targets
  /// Refuses to move into a solid object (ObjectEvent.refused).
  OBJ_EVENT_BLOCKED_BY_SOLID = 0x80
};

/// ObjectEvent.steeringFlags bits.
enum ObjectEventSteeringFlag {
  OBJ_EVENT_FACES_MOTION =
      0x01 ///< turns toward its velocity, 1/16 turn a frame
};

#endif
