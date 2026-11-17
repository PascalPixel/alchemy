#ifndef ALCHEMY_ACTOR_H
#define ALCHEMY_ACTOR_H

/// @file
/// Actors, their flags and the screen transition styles.

#include "field/field_coordinate.h"
#include "fixed_math.h"
#include "types.h"

/// How a scene fades in and out: SCENE_TRANSITION's high byte, which
/// ScreenTransition_Start and ScreenTransition_Finish read.
enum ScreenTransitionStyle {
  TRANSITION_FADE = 0,          ///< from or to black
  TRANSITION_BACKDROP_FADE = 1, ///< from or to the backdrop colour
  TRANSITION_WINDOW = 2,        ///< a window shape the variant chooses
  TRANSITION_DISSOLVE = 3,
  /// Opening from, or closing to, the centre line; the variant picks how.
  TRANSITION_CENTRE_LINE = 4
};

/// A scene transition as FieldWork.transition holds it.
#define SCENE_TRANSITION(style, variant) ((style) << 8 | (variant))

/// Actor.targetX, targetY and targetZ when it has no movement target.
#define TARGET_UNSET ((s32)0x80000000)

/// The actor id that ends a list of actors, or of rows that start with one.
#define ACTOR_LIST_END 0xffff

/// A field object's sprite animation, which Actor.sprite points to
/// when animationKind is 1.
/// Sets an actor's position in one statement, as SET_POSITION does for a
/// particle.
#define SET_ACTOR_POSITION(p, fixedX, fixedY, fixedZ)                          \
  ((p)->x.fixed = (fixedX), (p)->y.fixed = (fixedY), (p)->z.fixed = (fixedZ))

/// Actor.priorityFlags bits.
enum ActorPriorityFlag {
  /// The sprite takes its priority from the map cell under it.
  ACTOR_PRIORITY_AUTOMATIC = 0x01,
  /// The depth sort treats it as 320 pixels higher on the screen.
  ACTOR_PRIORITY_UNDERFOOT = 0x02,
  /// The depth sort treats it as 320 pixels lower on the screen.
  ACTOR_PRIORITY_OVERHEAD = 0x04,
  /// Drawn at the depth its riseCounter byte holds, not its own place
  /// (Render_ApplyProjectedPlacementAtDepth).
  ACTOR_PRIORITY_OWN_DEPTH = 0x08,
  ACTOR_PRIORITY_NOT_DRAWN = 0x10, ///< placed nowhere, as if off screen
  /// With ACTOR_PRIORITY_AUTOMATIC, takes the priority of the cells under it
  /// even where the ground in front rises, which would put it behind.
  ACTOR_PRIORITY_IGNORES_RISES = 0x20,
  /// The actor steps up onto platform actors a cell below it, as the leader
  /// does onto pushed blocks and statues.
  ACTOR_PRIORITY_WALKS_ON_PLATFORMS = 0x40,
  /// Others with ACTOR_PRIORITY_WALKS_ON_PLATFORMS may stand on it.
  ACTOR_PRIORITY_PLATFORM = 0x80
};

/// Actor.motionFlags bits.
enum ActorMotionFlag {
  ACTOR_FOLLOWS_TERRAIN = 0x01, ///< moves with the ground height under it
  ACTOR_FALLS = 0x02,           ///< falls by its gravity and bounces
  ACTOR_FLOATS = 0x04,          ///< bobs up and down by its gravity
  ACTOR_FLOATS_HIGH = 0x08,     ///< with ACTOR_FLOATS, four times as far
  /// Keeps its speed on slopes, which slow others down.
  ACTOR_KEEPS_SPEED_ON_SLOPES = 0x10,
  /// Unused: nothing sets or reads it; a jump clears it with
  /// ACTOR_FOLLOWS_TERRAIN.
  ACTOR_MOTION_SPARE = 0x80
};

/// The motion flags a jump keeps as the actor leaves the ground: all but
/// ACTOR_FOLLOWS_TERRAIN and the spare bit.
#define ACTOR_JUMP_KEEPS_MOTION                                                \
  ((u8)~(ACTOR_FOLLOWS_TERRAIN | ACTOR_MOTION_SPARE))

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

/// The axis a moving actor watches to know it has arrived at its target: the
/// one along which it has the furthest to go.
enum ArrivalAxis {
  ARRIVAL_AXIS_NONE = 0, ///< no arrival test; set again once it arrives
  ARRIVAL_AXIS_X = 16,
  ARRIVAL_AXIS_Y = 17,
  ARRIVAL_AXIS_Z = 18
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
  FIXED groundHeight;       ///< 16.16 fixed-point pixels
  FIXED scaleX;             ///< 16.16 fixed point, 0x10000 full size
  FIXED scaleY;             ///< 16.16 fixed point, 0x10000 full size
  u16 radius;               ///< collision radius in pixels
  u8 layer;                 ///< the map layer its ground height is read from
  /// Bit 0 is cleared when Actor_SetSpritePriorityDirect fixes its
  /// sprite's priority.
  u8 priorityFlags;
  FIXED velocityX;  ///< 16.16 fixed-point pixels per frame
  FIXED velocityY;  ///< 16.16 fixed-point pixels per frame
  FIXED velocityZ;  ///< 16.16 fixed-point pixels per frame
  FIXED speedLimit; ///< 16.16 fixed-point pixels per frame
  /// Added toward the target each frame, or taken off the speed when there is
  /// none, in 16.16 fixed-point pixels per frame.
  FIXED acceleration;
  FIXED targetX; ///< 16.16 fixed-point pixels; 0x80000000 when unset
  FIXED targetY; ///< 16.16 fixed-point pixels; 0x80000000 when unset
  FIXED targetZ; ///< 16.16 fixed-point pixels; 0x80000000 when unset
  /// The share of its falling speed kept as it bounces on landing, 16.16
  /// fixed point.
  FIXED bounce;
  /// Taken off the vertical velocity each frame while it is above the
  /// ground, in 16.16 fixed-point pixels per frame.
  FIXED gravity;
  /// Not read by the motion update; field code keeps its own values here (a
  /// drifting speed, a 0x31415927 marker), and scene code a resting height.
  s32 riseBaseY;
  /// Its sprite (a struct FieldSprite) when spriteStorageKind is 1, or its list
  /// of sprites when it is 2. Code that reads one kind converts it.
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
      u8 riseEnabled;
    };
    /// The ship on the World Map (FieldMoveByKeys): frames to its next wake,
    /// and a wake's own frames.
    struct {
      u8 unusedBeforeWake[2]; ///< refused and frozen
      u8 wakeTimer;           ///< frames to the ship's next wake
      u8 wakeFrames;          ///< a wake's frames left
    };
    /// An actor showing an item (Actor_SetItemIcon).
    struct {
      u8 unusedBeforeIconItem[2]; ///< refused and frozen
      u8 iconItem;                ///< the item whose icon its sprite shows
    };
    /// The leader while it crosses a rope or tightrope.
    struct {
      u8 unusedBeforeSavedPriority[2]; ///< refused and frozen
      u8 savedSpritePriority; ///< its sprite's priority, given back after
    };
    /// A member of the party looking about in the Gabomba Statue
    /// (GabombaStatue_LookAround).
    struct {
      u8 unusedBeforeLookTimer[2]; ///< refused and frozen
      u8 lookTimer;                ///< frames to its next turn
      u8 lookTurned;               ///< nonzero while it faces lookOtherFacing
    };
    /// A magical charge running along the Gabomba Statue's channel
    /// (GabombaStatue_MoveCharge).
    struct {
      u8 unusedBeforeChargeHeading[2]; ///< refused and frozen
      u8 chargeHeading;                ///< the quarter turn it runs toward
      /// 1 while it runs, 2 as it sinks or rises off the channel (its script
      /// then sets 0), 7 once it reaches the cell (24, 14).
      u8 chargeState;
    };
    /// A magma rock rolling between a pair of statues at Magma Rock
    /// (MagmaRock_RollMagmaRock).
    struct {
      u8 unusedBeforeRollSide[2]; ///< refused and frozen
      u8 rollSide; ///< 0 or 1: which statue of its pair it rolls toward
    };
    /// A part of a statue's rising flame at Magma Rock
    /// (MagmaRock_RaiseFlamePart).
    struct {
      u8 unusedBeforeFlameLevel[2]; ///< refused and frozen
      u8 flameLevel; ///< its height in the column, 16 pixels a level
    };
    /// A spark circling in to its centre, at Magma Rock's Blaze tablet
    /// (MagmaRockInside_OrbitParent).
    struct {
      u8 unusedBeforeStepsLeft[2]; ///< refused and frozen
      u8 stepsLeft;                ///< its steps left before it is released
    };
    /// An actor that blinks, as Magma Rock's statues and blasted wall do
    /// (MagmaRock_FlashStatue).
    struct {
      u8 unusedBeforeFlashMask[3]; ///< refused, frozen and riseCounter
      u8 flashMask; ///< its sprite flashes while gFrameCount & this is set
    };
    /// A statue that launches a fireball (Actor_LaunchAhead), or the fireball
    /// an attached actor sends rolling (Actor_Attach).
    struct {
      u8 unusedBeforeLaunchedActor[2]; ///< refused and frozen
      u8 launchedActorId;              ///< the actor sent rolling ahead
    };
    /// An actor whose update flickers its colour (ShamanVillage_FlickerRock).
    struct {
      u8 unusedBeforeFlicker[2]; ///< refused and frozen
      u8 flickerColor; ///< the colour attribute shown on its lit frames
      u8 flickerShift; ///< lit every other frame at 0, every other two at 1
    };
  };
  /// Two halfwords whose meaning depends on what runs the actor.
  union {
    struct {
      /// The parameter its action script reads (a follow distance or an
      /// angle to face).
      s16 timer;
      /// The side of the player's next footprint, or scene scratch.
      s16 userValue;
    };
    /// Where SC_WANDER_HOME wanders around, in pixels: where it was created
    /// or placed.
    struct {
      s16 homeX;
      s16 homeZ;
    };
    /// Avoid's caster (FieldEffect_UpdateAvoidCaster).
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
    /// The two headings a member of the party looking about turns between
    /// (GabombaStatue_LookAround), 1/65536 turns.
    struct {
      u16 lookFacing;
      u16 lookOtherFacing;
    };
    /// A charge's cell of the channel (GabombaStatue_MoveCharge).
    struct {
      s16 chargeColumn;
      s16 chargeRow;
    };
  };
  /// The object its action script follows or faces.
  struct Actor *watchedActor;
  void (*update)(void); ///< the object's per-frame hook
  /// What a carrier moved this frame, which Actor_MoveWithCarriers adds to
  /// an actor riding it, and that actor's carrier.
  s32 stepX;
  s32 stepY;
  s32 stepZ;
  struct Actor *carrier;
};

typedef char Actor_size[sizeof(struct Actor) == 128 ? 1 : -1];

typedef char Actor_collision_flags_offset
    [(u32)&((struct Actor *)0)->collisionFlags == 89 ? 1 : -1];

void Actor_SetPositionAndResetMotion(struct Actor *actor, FIXED x, FIXED y,
                                     FIXED z);
void Actor_MoveHalfwayTowardTarget(struct Actor *actor);
void Actor_MoveHalfwayTowardTargetCopy(struct Actor *actor);

struct Actor *Actor_Lookup(u32 objectId);
void Actor_StartIdleFar(struct Actor *);
void Actor_SetMoveTargetFar(struct Actor *, s32, s32, s32);
/// Waits up to 600 frames for a field object's movement target to be unset.
void Actor_WaitForMoveFar(struct Actor *);
/// Switches every sprite of an actor, one or a group of four, to an animation.
void Actor_SelectAnimationFar(struct Actor *, s32);
void Actor_WaitForScriptEndFar(struct Actor *);
void Actor_SetMovementType(struct Actor *, s32);
void Actor_SetSpritePriority(u32, s32);
void OverlayObject_WaitUntilIdle(struct Actor *actor);
void Actor_WaitForAnimationChange(u32);
void Actor_SetAnimationById(u32, s32);
void Actor_WaitForScriptEndById(u32);
void Sound_PlayFar(s32);

extern const u8 gActor_FaceWatchedScript[];
extern const u8 gActor_OffsetScript[];
extern const u8 gActor_VariantScripts[];

/// Sets bits in one of an actor's flag bytes.
static inline void ActorFlags_Set(u8 *flags, s32 bits) {
  /* FAKEMATCH(temporary): the bits held in a local; as *flags |= bits. */
  s32 value = bits;

  value |= *flags;
  *flags = value;
}

#endif
