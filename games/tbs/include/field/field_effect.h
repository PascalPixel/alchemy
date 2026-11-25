#ifndef ALCHEMY_FIELD_EFFECT_H
#define ALCHEMY_FIELD_EFFECT_H

/// @file
/// Field effects, their scripts and the flags that spawn them.

#include "edition.h"
#include "fixed_math.h"
#include "field/actor_script.h"
#include "field/event.h"
#include "system/vector3.h"

/// A field effect: a small actor on the field that moves at its velocity and
/// grows and spins at its rates, run by its script and update routine.
struct FieldEffect {
  /// Actor's script pointer, cursor and facing; not named by this view.
  ///@{
  const s32 *script;
  u16 scriptCursor;
  u16 facing;
  ///@}
  FIXED x;          ///< 16.16 fixed-point pixels
  FIXED y;          ///< 16.16 fixed-point pixels
  FIXED z;          ///< 16.16 fixed-point pixels
  s32 groundHeight; ///< Actor's; not named by this view
  FIXED scaleX;     ///< 16.16 fixed point, FIXED_ONE full size
  FIXED scaleY;     ///< 16.16 fixed point, FIXED_ONE full size
  u16 radius;       ///< Actor's; not named by this view
  u8 layer;         ///< Actor's; not named by this view
  /// ActorPriorityFlag bits: the spawner clears
  /// ACTOR_PRIORITY_AUTOMATIC when EFFECT_USE_PRIORITY sets the sprite's
  /// priority.
  u8 priorityFlags;
  /// Actor's velocityX to velocityZ; effects move by velocity instead.
  struct Vector3 objectVelocity;
  /// Added to scaleX each frame by the update routines, in 16.16 fixed point a
  /// frame; zeroed by the spawner.
  FIXED scaleRateX;
  s32 scaleRateY;
  /// Actor's targetX to targetZ; not named by this view.
  ///@{
  s32 targetX;
  s32 targetY;
  s32 targetZ;
  ///@}
  /// Added to x, y and z each frame by the update routines, in 16.16
  /// fixed-point pixels per frame; Actor's bounce, gravity and sceneWord.
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
  /// Actor's steeringFlags to sceneByte; not named by this view.
  ///@{
  u8 steeringFlags;
  u8 paused;
  u8 retainSpriteCache;
  u8 repeatCount;
  u16 wait;
  u8 refused;
  u8 frozen;
  u8 sceneCounter;
  u8 sceneByte;
  ///@}
  /// Added to the sprite's rotation each frame by the update routines, in
  /// 1/65536 turns per frame; Imil's rising effects keep a drift here instead.
  u16 spin;
  s16 userValue; ///< Actor's; not named by this view
  /// Frames until a scene's update routine releases the effect: 20 for Altin
  /// Peak's spray, 60 for Imil's rising effects.
  s32 countdown;
  /// Called each frame while the effect is live: the spawner sets
  /// FieldEffect_UpdateMotionScaleAndSpin, or options.update.
  void (*update)(struct Actor *actor);
};

STATIC_ASSERT(offsetof(struct FieldEffect, scaleRateX) == 48);
STATIC_ASSERT(offsetof(struct FieldEffect, velocity) == 68);
STATIC_ASSERT(offsetof(struct FieldEffect, sprite) == 80);
STATIC_ASSERT(offsetof(struct FieldEffect, spin) == 100);
STATIC_ASSERT(offsetof(struct FieldEffect, countdown) == 104);
STATIC_ASSERT(offsetof(struct FieldEffect, update) == 108);

/// An actor read as an actor and as a field effect at once, as
/// Babi Lighthouse's Scene_Initialize writes one.
union ActorOrEffect {
  struct Actor actor;
  struct FieldEffect effect;
};

/// The flags of a scene's effect spawner (EFFECT_SPAWN in
/// scene/shared/field_effect_spawn.c): a script number in the low bits and
/// which of struct EffectOptions' fields to use.
enum EffectSpawnFlags {
  /// An EffectDuration, picking one of gFieldEffect_DurationScripts; the
  /// actor's mode is this plus 1.
  EFFECT_SCRIPT_MASK = 0x0f,
  EFFECT_USE_PALETTE = 0x10000,  ///< options.palette, as its group child value
  EFFECT_USE_PRIORITY = 0x20000, ///< options.priority, as the sprite's priority
  /// Grows from the start scale, or full size, to the target scale over the
  /// script's repeat count.
  EFFECT_SCALE_TO_TARGET = 0x40000,
  EFFECT_USE_START_SCALE = 0x80000, ///< options.startScaleX and startScaleY
  EFFECT_USE_TYPE = 0x100000,       ///< options.type, else EFFECT_DEFAULT_TYPE
  EFFECT_USE_SCRIPT = 0x200000,     ///< options.script, run in mode 1
  EFFECT_USE_ROTATION = 0x400000,   ///< options.rotation, as the sprite's
  EFFECT_USE_SPIN = 0x800000,       ///< options.spin
  EFFECT_USE_UPDATE = 0x1000000     ///< options.update
};
/// The spawn flags that pick one of struct EffectOptions' fields
/// (EFFECT_USE_PALETTE and up), above the script number.
#define EFFECT_OPTION_MASK 0xffff0000
/// The sprite an effect takes unless EFFECT_USE_TYPE gives another
/// (sprite_ids.h).
#define EFFECT_DEFAULT_TYPE SPRITE_DUST_PUFF

/// The scripts EFFECT_SCRIPT_MASK picks from gFieldEffect_DurationScripts, in a
/// scene's spawner and the resident one alike: each holds the effect for a
/// while, then releases it.
enum EffectDuration {
  EFFECT_SCRIPT_SHORT =
      0, ///< 22 one-frame repeats in a scene, 24 frames resident
  EFFECT_SCRIPT_MEDIUM = 1, ///< 44 repeats in a scene, 46 frames resident
  EFFECT_SCRIPT_LONG = 2    ///< 126 repeats in a scene, 128 frames resident
};
/// The word of a duration script that holds its repeat count, the frames
/// EFFECT_SCALE_TO_TARGET grows over.
#define EFFECT_SCRIPT_REPEAT_COUNT 3

/// The resident scripts, by EffectDuration (field_sequence_tables.c).
extern const s32 gFieldEffect_ShortEffectScript[];
extern const s32 gFieldEffect_MediumEffectScript[];
extern const s32 gFieldEffect_LongEffectScript[];

/// The three scripts a scene's own effect spawner picks from by
/// EFFECT_SCRIPT_MASK, wrapped so that the spawner can copy them into its frame
/// whole.
struct EffectDurationScripts {
  const s32 *script[3];
};

/// The options of a spawned field effect, each used when its EFFECT_USE_ flag
/// (or EFFECT_SCALE_TO_TARGET) is set.
struct EffectOptions {
  s32 priority;       ///< the sprite's priority
  s32 palette;        ///< passed to Actor_SetSpriteColor
  FIXED startScaleX;  ///< 16.16 fixed point, FIXED_ONE full size
  FIXED startScaleY;  ///< 16.16 fixed point, FIXED_ONE full size
  FIXED targetScaleX; ///< 16.16 fixed point, FIXED_ONE full size
  FIXED targetScaleY; ///< 16.16 fixed point, FIXED_ONE full size
  s16 type;           ///< the sprite, SPRITE_DUST_PUFF without the flag
  const s32 *script;  ///< the actor script, run in mode 1
  u16 rotation;       ///< 1/65536 turns
  u16 spin;           ///< 1/65536 turns per frame
  void (*update)(struct Actor *actor); ///< replaces the update routine
};

STATIC_ASSERT(offsetof(struct EffectOptions, type) == 24);
STATIC_ASSERT(offsetof(struct EffectOptions, update) == 36);

void FieldEffect_SpawnResident(s32 x, s32 y, s32 z, s32 velocityX,
                               s32 velocityY, s32 velocityZ, u32 flags,
                               const struct EffectOptions *options);

#endif
