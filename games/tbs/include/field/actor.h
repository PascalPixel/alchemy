#ifndef ALCHEMY_ACTOR_H
#define ALCHEMY_ACTOR_H

/// @file
/// Actors, the actor snapshot, and the screen transition styles.

#include "types.h"
#include "static_assert.h"

#include "field_work.h"

/// How a scene fades in and out: SCENE_TRANSITION's high byte, which
/// ScreenTransition_Start and ScreenTransition_Finish read.
enum ScreenTransitionStyle {
  TRANSITION_FADE = 0,          ///< from or to black
  TRANSITION_BACKDROP_FADE = 1, ///< from or to the backdrop colour
  TRANSITION_WINDOW = 2,        ///< a window shape the variant chooses
  TRANSITION_DISSOLVE = 3,      ///< a dissolve
  /// Opening from, or closing to, the centre line; the variant picks how.
  TRANSITION_CENTRE_LINE = 4
};

/// A scene transition as FieldWork.transition holds it. For
/// TRANSITION_WINDOW the variant is the window mode, listed at
/// ScreenTransition_UpdateScanlineTable: 3 a circle at the centre, 9 an
/// ellipse around the leader, and others.
#define SCENE_TRANSITION(style, variant) ((style) << 8 | (variant))

#include "field_coordinate.h"

#include "sprite.h"

struct Actor;

/// An actor: one of the 64 people and things on the field (gActors,
/// HEAP_SLOT_ACTORS), with its actor script, position, motion toward a target,
/// sprites and update routine.
///
/// ActorSystem_Initialize takes the actors with the actor system's work
/// (HEAP_SLOT_ACTOR_SYSTEM), so they live until the next scene change. A scene
/// knows the actors it placed by actor id (FieldWork.actors). A field effect is
/// an actor read as struct FieldEffect; the few other views kept for matching
/// are listed in the field README.
struct Actor {
  /// The actor script it runs; null when its slot is free.
  const s32 *script;
  s16 scriptCursor;        ///< the script cursor, in words
  u16 facing;              ///< 1/65536 turns
  union FieldCoordinate x; ///< 16.16 fixed-point pixels
  union FieldCoordinate y; ///< 16.16 fixed-point pixels
  union FieldCoordinate z; ///< 16.16 fixed-point pixels
  s32 groundHeight;        ///< 16.16 fixed-point pixels
  s32 scaleX;              ///< 16.16 fixed point, Q16_ONE full size
  s32 scaleY;              ///< 16.16 fixed point, Q16_ONE full size
  /// Collision radius in pixels: 16, or half the sprite metadata's boxY, when
  /// created; the overlap checks use it less 2.
  u16 radius;
  /// The map layer (0 to 3) its ground height is read from, which the
  /// scrolling camera updates from the cell under it; the fixed camera passes
  /// it as the sprite's draw slot.
  u8 layer;
  /// ActorPriorityFlag bits, read by the scrolling camera: 1 for placed
  /// actors, 2 for footprints.
  u8 priorityFlags;
  s32 velocityX;  ///< 16.16 fixed-point pixels per frame
  s32 velocityY;  ///< 16.16 fixed-point pixels per frame
  s32 velocityZ;  ///< 16.16 fixed-point pixels per frame
  s32 speedLimit; ///< 16.16 fixed-point pixels per frame
  /// Added toward the target each frame, or taken off the speed when there is
  /// none, in 16.16 fixed-point pixels per frame; Q16_ONE when created.
  s32 acceleration;
  s32 targetX; ///< 16.16 fixed-point pixels; ACTOR_NO_TARGET when unset
  s32 targetY; ///< 16.16 fixed-point pixels; ACTOR_NO_TARGET when unset
  s32 targetZ; ///< 16.16 fixed-point pixels; ACTOR_NO_TARGET when unset
  /// The share of its falling speed kept as it bounces on landing, 16.16
  /// fixed point (a quarter when created); a floating actor's phase instead.
  s32 bounce;
  /// Taken off velocityY each frame while it is above the ground, in 16.16
  /// fixed-point pixels per frame (Q16_ONE when created); also scales a
  /// floating actor's bob.
  s32 gravity;
  /// Zeroed by FieldActor_Create and not read by the field; scene code keeps a
  /// resting height, a sway or a countdown here.
  s32 riseBaseY;
  /// Its sprite, by spriteStorageKind.
  union {
    struct Sprite *sprite; ///< ACTOR_SPRITE_ONE
    void **sprites;        ///< ACTOR_SPRITE_GROUP: four, empty ones null
  };
  /// Zero hides sprites; one draws a single sprite, two draws a group. The
  /// field reads the low four bits (enum ActorSpriteKind); SC_SHOW_SPRITE and
  /// SC_HIDE_SPRITES set 1 and 0.
  u8 spriteStorageKind;
  /// ActorMotionFlag bits; none is free flight in 3D, and
  /// ACTOR_FOLLOWS_TERRAIN and ACTOR_FALLS are set on creation.
  u8 motionFlags;
  u8 arrivalAxis;     ///< the ARRIVAL_AXIS_* watched for arrival
  u8 conditionResult; ///< the actor script's last flag test
  u8 snapToTarget;    ///< nonzero lands it exactly on its target on arrival
  /// ActorCollisionFlag bits: bit 0 solid, bit 1 the player pushes it, bits 2
  /// to 4 for talk and Psynergy targets; placed actors take 1.
  u8 collisionFlags;
  u8 steeringFlags; ///< ActorSteeringFlag bits
  u8 paused;        ///< nonzero skips its script and motion, not its update
  /// Nonzero keeps its sprite's VRAM cache entry while sprites are suspended
  /// or it is off screen (ActorSystem_PlaceSprites).
  u8 retainSpriteCache;
  u8 repeatCount; ///< the actor script's SC_LOOP count
  s16 wait;       ///< the actor script's wait, in frames
  /// Frames in a row a solid actor has refused its move; SC_SYNC stops
  /// waiting after 60.
  u8 refused;
  /// Nonzero skips steering, terrain, gravity and floating; its velocity
  /// still moves it.
  u8 frozen;
  u8 riseCounter; ///< not read by the field; scene code's own counter
  u8 riseEnabled; ///< not read by the field; scene code's own state
  /// Two halfwords whose meaning depends on what runs the actor.
  union {
    struct {
      /// The parameter its actor script reads (a follow distance or an angle
      /// to face), the player's frames to its next footprint, or scene
      /// scratch.
      s16 timer;
      /// The side of the player's next footprint (2 after a stop), or scene
      /// scratch.
      u16 userValue;
    };
    /// Where SC_WANDER_HOME wanders around, in pixels: where it was created
    /// or placed.
    struct {
      s16 homeX;
      s16 homeZ;
    };
    /// Avoid's caster (FieldEffect_UpdatePairedArcSpawner).
    struct {
      s16 arcPhase;  ///< 1 spins the caster 3/64 turn a frame
      s16 arcFrames; ///< frames since the arcs started
    };
    /// A Venus Djinni's falling motes
    /// (FieldEffect_UpdateDescendingOrbitActor).
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
  /// The actor its actor script follows or faces.
  struct Actor *watchedActor;
  /// Called each frame before its script, while the actor is live.
  void (*update)(struct Actor *actor);
};

/// Sets an actor's position in one statement, as SET_POSITION does for a
/// particle.
#define SET_ACTOR_POSITION(p, px, py, pz)                                      \
  ((p)->x.fixed = (px), (p)->y.fixed = (py), (p)->z.fixed = (pz))

STATIC_ASSERT(sizeof(struct Actor) == 0x70);
STATIC_ASSERT(offsetof(struct Actor, arrivalAxis) == 0x56);
STATIC_ASSERT(offsetof(struct Actor, conditionResult) == 0x57);
STATIC_ASSERT(offsetof(struct Actor, snapToTarget) == 0x58);
STATIC_ASSERT(offsetof(struct Actor, userValue) == 0x66);
STATIC_ASSERT(offsetof(struct Actor, facing) == 0x06);
STATIC_ASSERT(offsetof(struct Actor, x) == 0x08);
STATIC_ASSERT(offsetof(struct Actor, z) == 0x10);
STATIC_ASSERT(offsetof(struct Actor, groundHeight) == 0x14);
STATIC_ASSERT(offsetof(struct Actor, scaleY) == 0x1c);
STATIC_ASSERT(offsetof(struct Actor, priorityFlags) == 0x23);
STATIC_ASSERT(offsetof(struct Actor, motionFlags) == 0x55);
STATIC_ASSERT(offsetof(struct Actor, collisionFlags) == 0x59);
STATIC_ASSERT(offsetof(struct Actor, steeringFlags) == 0x5a);
STATIC_ASSERT(offsetof(struct Actor, paused) == 0x5b);
STATIC_ASSERT(offsetof(struct Actor, retainSpriteCache) == 0x5c);
STATIC_ASSERT(offsetof(struct Actor, repeatCount) == 0x5d);
STATIC_ASSERT(offsetof(struct Actor, wait) == 0x5e);
STATIC_ASSERT(offsetof(struct Actor, refused) == 0x60);
STATIC_ASSERT(offsetof(struct Actor, frozen) == 0x61);
STATIC_ASSERT(offsetof(struct Actor, riseEnabled) == 0x63);
STATIC_ASSERT(offsetof(struct Actor, bounce) == 0x44);
STATIC_ASSERT(offsetof(struct Actor, gravity) == 0x48);
STATIC_ASSERT(offsetof(struct Actor, riseBaseY) == 0x4c);
STATIC_ASSERT(offsetof(struct Actor, velocityY) == 0x28);
STATIC_ASSERT(offsetof(struct Actor, speedLimit) == 0x30);
STATIC_ASSERT(offsetof(struct Actor, targetX) == 0x38);
STATIC_ASSERT(offsetof(struct Actor, sprite) == 0x50);
STATIC_ASSERT(offsetof(struct Actor, update) == 0x6c);

/// The values of Actor.targetX, targetY and targetZ that are not
/// coordinates.
enum ActorTarget {
  /// A target coordinate that is not set: the actor has no target.
  ACTOR_NO_TARGET = (s32)0x80000000
};

/// Preserves the current action and motion when assigning a watched actor.
enum ActorWatchFlag { ACTOR_WATCH_KEEP_MOTION = 0x10000 };

/// Actor.priorityFlags bits.
enum ActorPriorityFlag {
  /// The sprite takes its priority from the map cell under it.
  ACTOR_PRIORITY_AUTOMATIC = 0x01,
  /// The depth sort treats it as 320 pixels higher on the screen.
  ACTOR_PRIORITY_UNDERFOOT = 0x02,
  /// The depth sort treats it as 320 pixels lower on the screen.
  ACTOR_PRIORITY_OVERHEAD = 0x04
};

/// Actor.motionFlags bits.
enum ActorMotionFlag {
  ACTOR_FOLLOWS_TERRAIN = 0x01, ///< moves with the ground height under it
  ACTOR_FALLS = 0x02,           ///< falls by its gravity and bounces
  ACTOR_FLOATS = 0x04,          ///< bobs up and down by its gravity
  ACTOR_FLOATS_HIGH = 0x08,     ///< with ACTOR_FLOATS, four times as far
  /// Keeps its speed on slopes, which slow others down.
  ACTOR_KEEPS_SPEED_ON_SLOPES = 0x10
};

/// Actor.collisionFlags bits.
enum ActorCollisionFlag {
  ACTOR_SOLID = 0x01,         ///< blocks other actors and the player
  ACTOR_PUSHABLE = 0x02,      ///< the player pushes it
  ACTOR_NEAR_TALK = 0x04,     ///< nearer to talk to
  ACTOR_NOT_TARGETED = 0x08,  ///< never a talk or Psynergy target
  ACTOR_NEAR_PSYNERGY = 0x10, ///< nearer for Psynergy targets
  /// Refuses to move into a solid actor (Actor.refused).
  ACTOR_BLOCKED_BY_SOLID = 0x80
};

/// Actor.steeringFlags bits.
enum ActorSteeringFlag {
  ACTOR_FACES_MOTION = 0x01 ///< turns toward its velocity, 1/16 turn a frame
};

enum {
  ACTOR_SNAPSHOT_CAPACITY = 32, ///< actors a snapshot keeps
  ACTOR_SNAPSHOT_EMPTY = 255    ///< an empty slot, and the end of the list
};

/// Actors kept across a battle or a saved scene, with parallel sprite
/// state. The slot list ends in ACTOR_SNAPSHOT_EMPTY unless every entry
/// is filled.
struct ActorSnapshot {
  u8 slots[ACTOR_SNAPSHOT_CAPACITY];            ///< actor ids, or
                                                ///< ACTOR_SNAPSHOT_EMPTY
  struct Actor actors[ACTOR_SNAPSHOT_CAPACITY]; ///< copied by DMA
  u8 animations[ACTOR_SNAPSHOT_CAPACITY];       ///< each sprite's lastNo
  u8 spriteFlags[ACTOR_SNAPSHOT_CAPACITY];      ///< restored through the
                                                ///< dispatcher
  u8 priorities[ACTOR_SNAPSHOT_CAPACITY];       ///< both image and shadow
                                                ///< priority
};

STATIC_ASSERT(offsetof(struct ActorSnapshot, actors) == 0x20);
STATIC_ASSERT(offsetof(struct ActorSnapshot, animations) == 0xe20);
STATIC_ASSERT(offsetof(struct ActorSnapshot, spriteFlags) == 0xe40);
STATIC_ASSERT(offsetof(struct ActorSnapshot, priorities) == 0xe60);
STATIC_ASSERT(sizeof(struct ActorSnapshot) == 0xe80);

/// The one snapshot, in EWRAM outside the heap so that it outlives the scene
/// change into and out of a battle. Actor_Snapshot fills it before a
/// random battle and when saving; Actor_Restore puts the actors
/// back when the party returns keeping its position (FLAG_KEEP_PARTY_POSITION).
extern struct ActorSnapshot gActorSnapshot;

#endif
