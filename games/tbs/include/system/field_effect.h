#ifndef ALCHEMY_FIELD_EFFECT_H
#define ALCHEMY_FIELD_EFFECT_H

/// @file
/// Field effects, their scripts and the flags that spawn them.

#include "edition.h"
#include "script_layer.h"
#include "vector3.h"
#include "objcmd.h"

/// A field effect: a small object on the field that moves at its velocity and
/// grows and spins at its rates, run by its script and update hook.
struct FieldEffect {
  /// Actor's script pointer, cursor and facing; not named by this view.
  ///@{
  const s32 *script;
  u16 scriptCursor;
  u16 facing;
  ///@}
  s32 x;            ///< 16.16 fixed-point pixels
  s32 y;            ///< 16.16 fixed-point pixels
  s32 z;            ///< 16.16 fixed-point pixels
  s32 groundHeight; ///< Actor's; not named by this view
  s32 scaleX;       ///< 16.16 fixed point, 0x10000 full size
  s32 scaleY;       ///< 16.16 fixed point, 0x10000 full size
  u16 radius;       ///< Actor's; not named by this view
  u8 layer;         ///< Actor's; not named by this view
  /// ActorPriorityFlag bits: the spawner clears
  /// ACTOR_PRIORITY_AUTOMATIC when EFFECT_USE_PRIORITY sets the sprite's
  /// priority.
  u8 priorityFlags;
  /// Actor's velocityX to velocityZ; effects move by velocity instead.
  struct Vector3 objectVelocity;
  /// Added to scaleX each frame by the update hooks, in 16.16 fixed point a
  /// frame; zeroed by the spawner.
  s32 scaleRateX;
  s32 scaleRateY; ///< as scaleRateX, for scaleY
  /// Actor's targetX to targetZ; not named by this view.
  ///@{
  s32 targetX;
  s32 targetY;
  s32 targetZ;
  ///@}
  /// Added to x, y and z each frame by the update hooks, in 16.16 fixed-point
  /// pixels per frame; Actor's bounce, gravity and riseBaseY.
  struct Vector3 velocity;
  struct Sprite *sprite; ///< the sprite spin turns
  u8 spriteStorageKind;  ///< Actor's; not named by this view
  u8 motionFlags;        ///< ActorMotionFlag bits; zeroed by the spawner
  /// Actor's arrival axis, last flag test and snap flag; not named by
  /// this view.
  ///@{
  u8 arrivalAxis;
  u8 conditionResult;
  u8 snapToTarget;
  ///@}
  u8 collisionFlags; ///< Actor's collisionFlags; not named by this view
  /// Actor's steeringFlags to riseEnabled; not named by this view.
  ///@{
  u8 steeringFlags;
  u8 paused;
  u8 retainSpriteCache;
  u8 repeatCount;
  u16 wait;
  u8 refused;
  u8 frozen;
  u8 riseCounter;
  u8 riseEnabled;
  ///@}
  /// Added to the sprite's rotation each frame by the update hooks, in
  /// 1/65536 turns per frame; Imil's rising effects keep a drift here instead.
  u16 spin;
  u16 work; ///< Actor's; not named by this view
  /// Frames until a map's update hook releases the effect: 20 for Altin
  /// Peak's spray, 60 for Imil's rising effects.
  s32 countdown;
  /// Called each frame while the effect is live: the spawner sets
  /// FieldEffect_UpdateMotionScaleAndSpin, or options.update.
  void (*update)(union ActorOrEffect *fieldObject);
};

STATIC_ASSERT(offsetof(struct FieldEffect, scaleRateX) == 0x30);
STATIC_ASSERT(offsetof(struct FieldEffect, velocity) == 0x44);
STATIC_ASSERT(offsetof(struct FieldEffect, sprite) == 0x50);
STATIC_ASSERT(offsetof(struct FieldEffect, spin) == 0x64);
STATIC_ASSERT(offsetof(struct FieldEffect, countdown) == 0x68);
STATIC_ASSERT(offsetof(struct FieldEffect, update) == 0x6c);

/// An object on the field, read as an actor or as an effect.
union ActorOrEffect {
  struct Actor actor;
  struct FieldEffect effect;
};

/// The flags of a map's effect spawner (EFFECT_SPAWN in
/// scene/common/effect_spawn.h): a script number in the low bits and which of
/// struct EffectOptions' fields to use.
enum EffectSpawnFlags {
  /// An EffectDuration, picking one of gFieldEffect_DurationScripts; the
  /// object's mode is
  /// this
  /// plus 1.
  EFFECT_SCRIPT_MASK = 0x0f,
  EFFECT_USE_PALETTE = 0x10000,  ///< options.palette, as its group child value
  EFFECT_USE_PRIORITY = 0x20000, ///< options.priority, as the sprite's priority
  /// Grows from the start scale, or full size, to the target scale over the
  /// script's repeat count.
  EFFECT_SCALE_TO_TARGET = 0x40000,
  EFFECT_USE_START_SCALE = 0x80000, ///< options.startScaleX and startScaleY
  EFFECT_USE_TYPE = 0x100000,       ///< options.type, else object type 222
  EFFECT_USE_SCRIPT = 0x200000,     ///< options.script, run in mode 1
  EFFECT_USE_ROTATION = 0x400000,   ///< options.rotation, as the sprite's
  EFFECT_USE_SPIN = 0x800000,       ///< options.spin
  EFFECT_USE_UPDATE = 0x1000000     ///< options.update
};

/// The scripts EFFECT_SCRIPT_MASK picks from gFieldEffect_DurationScripts, in a
/// map's spawner and the resident one alike: each holds the effect for a while,
/// then frees it.
enum EffectDuration {
  EFFECT_SCRIPT_SHORT =
      0, ///< 22 one-frame repeats in a map, 24 frames resident
  EFFECT_SCRIPT_MEDIUM = 1, ///< 44 repeats in a map, 46 frames resident
  EFFECT_SCRIPT_LONG = 2    ///< 126 repeats in a map, 128 frames resident
};

/// The three scripts a map's own effect spawner picks from by
/// EFFECT_SCRIPT_MASK, wrapped so that the spawner can copy them into its frame
/// whole.
struct EffectDurationScripts {
  const s32 *script[3];
};

/// The options of a spawned field effect, each used when its EFFECT_USE_ flag
/// (or EFFECT_SCALE_TO_TARGET) is set.
struct EffectOptions {
  s32 priority;      ///< the sprite's priority
  s32 palette;       ///< passed to Actor_SetSpriteColor
  s32 startScaleX;   ///< 16.16 fixed point, 0x10000 full size
  s32 startScaleY;   ///< 16.16 fixed point, 0x10000 full size
  s32 targetScaleX;  ///< 16.16 fixed point, 0x10000 full size
  s32 targetScaleY;  ///< 16.16 fixed point, 0x10000 full size
  s16 type;          ///< the object type to create; 222 without the flag
  const s32 *script; ///< the object script, run in mode 1
  u16 rotation;      ///< 1/65536 turns
  u16 spin;          ///< 1/65536 turns per frame
  void (*update)(
      union ActorOrEffect *fieldObject); ///< replaces the update hook
};

STATIC_ASSERT(offsetof(struct EffectOptions, type) == 0x18);
STATIC_ASSERT(offsetof(struct EffectOptions, update) == 0x24);

void FieldEffect_SpawnResident(s32 x, s32 y, s32 z, s32 velocityX,
                               s32 velocityY, s32 velocityZ, u32 flags,
                               const struct EffectOptions *options);

#endif
