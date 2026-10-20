#ifndef ALCHEMY_FIELD_OBJ_H
#define ALCHEMY_FIELD_OBJ_H

/// @file
/// Field objects and actors, and the scene transition styles.

#include "types.h"
#include "static_assert.h"

#include "event_work.h"

/// The running field scene's work, heap slot HEAP_SLOT_EVENT, as the scenes
/// read it; Field_RunScene takes it and frees it.
extern struct EventWork *gEventWork;

/// How a scene fades in and out: SCENE_TRANSITION's high byte, which
/// DisplayTransition_Start and DisplayTransition_Finish read. Kinds 3, a
/// dissolve, and 4, opening from the centre line, have no name here.
enum SceneTransitionStyle {
  TRANSITION_FADE = 0,          ///< from or to black
  TRANSITION_BACKDROP_FADE = 1, ///< from or to the backdrop colour
  TRANSITION_WINDOW = 2         ///< a window shape the variant chooses
};

/// A scene transition as EventWork.startTransition holds it. For
/// TRANSITION_WINDOW the variant is the window mode, listed at
/// DisplayTransition_UpdateScanlineTable: 3 a circle at the centre, 9 an
/// ellipse around the leader, and others.
#define SCENE_TRANSITION(style, variant) ((style) << 8 | (variant))

#include "field_coordinate.h"

#include "animspr.h"

union FieldObject;

/// A field actor: a person or thing on the field with a position, motion
/// toward a target, sprites and an update callback.
struct FieldActor {
  /// The object's script pointer (nonzero while the object is live) and its
  /// cursor, which ObjectRuntime names script and step.
  u8 unk00[6];
  u16 facing;              ///< 1/65536 turns
  union FieldCoordinate x; ///< 16.16 fixed-point pixels
  union FieldCoordinate y; ///< 16.16 fixed-point pixels
  union FieldCoordinate z; ///< 16.16 fixed-point pixels
  s32 terrainHeight;       ///< 16.16 fixed-point pixels
  s32 scaleX;              ///< 16.16 fixed point, 0x10000 full size
  s32 scaleY;              ///< 16.16 fixed point, 0x10000 full size
  /// Collision radius in pixels: 16, or half the sprite metadata's boxY, when
  /// created; the overlap checks use it less 2.
  u16 radius;
  /// The map layer (0 to 3) its ground height is read from, which the
  /// scrolling camera updates from the cell under it; the fixed camera passes
  /// it as the sprite's draw slot.
  u8 terrainId;
  /// ActorPriorityFlag bits, read by the scrolling camera.
  /// bits: 2 sorted as if 320 pixels lower | 1 sorted as if 320 pixels
  /// higher | 0 sprite priority from the cell under it
  u8 priorityFlags;
  s32 velocityX; ///< 16.16 fixed-point pixels per frame
  s32 velocityY; ///< 16.16 fixed-point pixels per frame
  s32 velocityZ; ///< 16.16 fixed-point pixels per frame
  s32 speed;     ///< speed limit, 16.16 fixed-point pixels per frame
  /// Added toward the target each frame, or taken off the speed when there is
  /// none, in 16.16 fixed-point pixels per frame; 0x10000 when created.
  s32 acceleration;
  s32 targetX; ///< 16.16 fixed-point pixels; ACTOR_NO_TARGET when unset
  s32 targetY; ///< 16.16 fixed-point pixels; ACTOR_NO_TARGET when unset
  s32 targetZ; ///< 16.16 fixed-point pixels; ACTOR_NO_TARGET when unset
  /// The share of its falling speed kept as it bounces on landing, 16.16
  /// fixed point (0x4000 when created); a floating object's phase instead.
  s32 bounce;
  /// Taken off the vertical velocity each frame while it is above the
  /// ground, in 16.16 fixed-point pixels per frame (0x10000 when created);
  /// also scales a floating object's bob.
  s32 gravity;
  /// Zeroed by FieldObject_Create and not read by the engine; scene code
  /// keeps a resting height, a sway or a countdown here.
  s32 riseBaseY;
  union {
    struct AnimationObject *sprite; ///< spriteStorageKind 1
    void **sprites; ///< spriteStorageKind 2: four sprites, empty ones null
  };
  /// Zero hides sprites; one draws a single sprite, two draws a group.
  /// The engine reads the low four bits; SC_B54_ON and SC_B54_OFF set 1 and 0.
  u8 spriteStorageKind;
  /// ActorMotionFlag bits; zero is free flight in 3D, and 3 is set on
  /// creation.
  /// bits: 4 no slowdown on slopes | 3 bigger float | 2 floats | 1 falls |
  /// 0 follows the terrain
  u8 motionFlags;
  /// The ARRIVAL_AXIS_* watched for arrival, the object script's last flag
  /// test (ScriptInterpreter.conditionResult), and whether it snaps onto its
  /// target on arrival: ScriptMotionObject names 0x56 and 0x58.
  u8 unk56[3];
  /// bits: 7 refuses to move into a solid object | 4 nearer for Psynergy
  /// targets | 3 never a talk or Psynergy target | 2 nearer to talk to |
  /// 1 the player pushes it | 0 solid
  u8 collisionFlags;
  u8 steeringFlags; ///< ActorSteeringFlag bits
  u8 paused;        ///< nonzero skips its script and motion, not its update
  /// Nonzero keeps its sprite's VRAM cache entry while sprites are suspended
  /// or it is off screen (ObjectSystem_UpdateCamera).
  u8 retainSpriteCache;
  u8 repeatCount; ///< the object script's SC_LOOP count (ScriptInterpreter)
  u16 wait;       ///< the object script's wait, in frames (ScriptMotionObject)
  /// Frames a solid object has refused its move (ScriptMotionObject's
  /// refused); SC_SYNC stops waiting after 60.
  u8 refused;
  /// Nonzero skips steering, terrain, gravity and floating; its velocity
  /// still moves it (ScriptMotionObject.frozen).
  u8 frozen;
  u8 riseCounter; ///< not read by the engine; scene code's own counter
  u8 riseEnabled; ///< not read by the engine; scene code's own state
  /// The parameter its action script reads (a follow distance or an angle to
  /// face), the player's frames to its next footprint, or scene scratch.
  s16 timer;
  /// The side of the player's next footprint (2 after a stop), or scene
  /// scratch.
  u16 work;
  /// The object its action script follows or faces.
  struct FieldActor *linkedObject;
  /// Called each frame before the object's script, while the object is live.
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
STATIC_ASSERT(offsetof(struct FieldActor, retainSpriteCache) == 0x5c);
STATIC_ASSERT(offsetof(struct FieldActor, repeatCount) == 0x5d);
STATIC_ASSERT(offsetof(struct FieldActor, wait) == 0x5e);
STATIC_ASSERT(offsetof(struct FieldActor, refused) == 0x60);
STATIC_ASSERT(offsetof(struct FieldActor, frozen) == 0x61);
STATIC_ASSERT(offsetof(struct FieldActor, riseEnabled) == 0x63);
STATIC_ASSERT(offsetof(struct FieldActor, bounce) == 0x44);
STATIC_ASSERT(offsetof(struct FieldActor, gravity) == 0x48);
STATIC_ASSERT(offsetof(struct FieldActor, riseBaseY) == 0x4c);
STATIC_ASSERT(offsetof(struct FieldActor, velocityY) == 0x28);
STATIC_ASSERT(offsetof(struct FieldActor, speed) == 0x30);
STATIC_ASSERT(offsetof(struct FieldActor, targetX) == 0x38);
STATIC_ASSERT(offsetof(struct FieldActor, sprite) == 0x50);
STATIC_ASSERT(offsetof(struct FieldActor, update) == 0x6c);

/// A target coordinate that is not set: the object has no target.
#define ACTOR_NO_TARGET ((s32)0x80000000)

/// Preserves the current action and motion when assigning a linked target.
enum ActorLinkFlag { ACTOR_LINK_KEEP_MOTION = 0x10000 };

/// FieldActor.priorityFlags bits.
enum ActorPriorityFlag {
  /// The sprite takes its priority from the map cell under it.
  ACTOR_PRIORITY_AUTOMATIC = 0x01,
  /// The depth sort treats it as 320 pixels higher on the screen.
  ACTOR_PRIORITY_UNDERFOOT = 0x02
};

/// FieldActor.motionFlags bits.
enum ActorMotionFlag {
  ACTOR_FOLLOWS_TERRAIN = 0x01, ///< moves with the ground height under it
  ACTOR_FALLS = 0x02            ///< falls by its gravity and bounces
};

/// FieldActor.steeringFlags bits.
enum ActorSteeringFlag {
  ACTOR_FACES_MOTION = 0x01 ///< turns toward its velocity, 1/16 turn a frame
};

#endif
