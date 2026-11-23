#ifndef ALCHEMY_FIELD_EFFECT_H
#define ALCHEMY_FIELD_EFFECT_H

/// @file
/// Field effects, the small objects on the field that move, grow and spin by
/// their scripts, and the flags of a scene's effect spawner.

#include "edition.h"
#include "field/field_event.h"
#include "fixed_math.h"
#include "vector3.h"
#include "types.h"

/// A field effect: a small object on the field that moves at its velocity and
/// grows and spins at its rates, run by its script and update hook.
struct FieldEffect {
  /// Actor's script pointer, cursor and facing; not named by this view.
  ///@{
  const s32 *script;
  s16 scriptCursor;
  u16 facing;
  ///@}
  FIXED x;          ///< 16.16 fixed-point pixels
  FIXED y;          ///< 16.16 fixed-point pixels
  FIXED z;          ///< 16.16 fixed-point pixels
  s32 groundHeight; ///< Actor's; not named by this view
  FIXED scaleX;     ///< 16.16 fixed point, 0x10000 full size
  FIXED scaleY;     ///< 16.16 fixed point, 0x10000 full size
  u16 radius;       ///< Actor's; not named by this view
  u8 layer;         ///< Actor's; not named by this view
  /// The spawner clears bit 0 when EFFECT_USE_PRIORITY sets the sprite's
  /// priority.
  u8 priorityFlags;
  /// Actor's velocityX to velocityZ; effects move by velocity instead.
  struct Vector3 objectVelocity;
  /// Added to scaleX each frame by the update hooks, in 16.16 fixed point a
  /// frame; zeroed by the spawner.
  FIXED scaleRateX;
  s32 scaleRateY; ///< as scaleRateX, for scaleY
  /// Actor's targetX to targetZ; not named by this view.
  ///@{
  s32 targetX;
  s32 targetY;
  s32 targetZ;
  ///@}
  /// Added to x, y and z each frame by the update hooks, in 16.16 fixed-point
  /// pixels per frame.
  struct Vector3 velocity;
  struct FieldSprite *sprite; ///< the sprite spin turns
  u8 spriteStorageKind;       ///< Actor's; not named by this view
  u8 motionFlags;             ///< zeroed by the spawner
  /// Actor's arrival axis, last flag test and snap flag; not named by this
  /// view.
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
  s16 wait;
  u8 refused;
  u8 frozen;
  u8 riseCounter;
  u8 riseEnabled;
  ///@}
  /// Added to the sprite's rotation each frame by the update hooks, in
  /// 1/65536 turns per frame; zeroed by the spawner.
  u16 spin;
  s16 userValue; ///< Actor's; not named by this view
  s32 countdown; ///< no code names it
  /// Called each frame while the effect is live: the spawner sets
  /// FieldEffect_UpdateMotionScaleAndSpin, or options.update.
  void (*update)(union FieldObject *object);
};

STATIC_ASSERT(offsetof(struct FieldEffect, scaleRateX) == 48);
STATIC_ASSERT(offsetof(struct FieldEffect, velocity) == 68);
STATIC_ASSERT(offsetof(struct FieldEffect, sprite) == 80);
STATIC_ASSERT(offsetof(struct FieldEffect, spin) == 100);
STATIC_ASSERT(offsetof(struct FieldEffect, countdown) == 104);
STATIC_ASSERT(offsetof(struct FieldEffect, update) == 108);

/// An object on the field, read as an actor or as an effect.
union FieldObject {
  struct Actor actor;
  struct FieldEffect effect;
};

/// The flags of a scene's effect spawner (FieldEffect_Spawn): a script number
/// in the low bits and which of the spawn options' fields to use.
enum EffectSpawnFlags {
  /// Picks one of the spawner's effect scripts; the object's mode is this
  /// plus 1.
  EFFECT_SCRIPT_MASK = 0x0f,
  EFFECT_USE_PALETTE = 0x10000,  ///< options.palette
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
/// The spawn flags that pick an option, above the script number.
#define EFFECT_OPTION_MASK 0xffff0000
/// The sprite an effect takes unless EFFECT_USE_TYPE gives another
/// (sprite_ids.h).
#define EFFECT_DEFAULT_TYPE SPRITE_DUST_CLOUD

/// The scripts EFFECT_SCRIPT_MASK picks from gFieldEffect_DurationScripts: each
/// holds the effect for a while, then releases it.
enum EffectDuration {
  EFFECT_SCRIPT_SHORT = 0,  ///< held 24 frames
  EFFECT_SCRIPT_MEDIUM = 1, ///< held 46 frames
  EFFECT_SCRIPT_LONG = 2    ///< held 128 frames
};
/// The word of a duration script that holds its repeat count, the frames
/// EFFECT_SCALE_TO_TARGET grows over.
#define EFFECT_SCRIPT_REPEAT_COUNT 3

/// The names field/field_effect_spawn.c compiles a scene's copy of the effect
/// spawner under, the same in every scene that includes it: the spawner, the
/// import that creates its actor, and the duration scripts EFFECT_SCRIPT_MASK
/// picks from. The scene names EFFECT_SET_MODE and EFFECT_SET_SCRIPT itself.
#define EFFECT_SPAWN FieldEffect_Spawn
#define EFFECT_CREATE Import_ActorCreate
#define EFFECT_SCRIPTS gFieldEffect_DurationScripts

/// A drifting effect loses a FIELD_EFFECT_DRAG_X-th of its x speed and a
/// FIELD_EFFECT_DRAG_Z-th of its z speed each frame.
#define FIELD_EFFECT_DRAG_X 18
#define FIELD_EFFECT_DRAG_Z 16

/// Sixteen puffs of dust in a ring round the leader as he lands from above,
/// the first thrown twice.
#define DUST_RING_PUFFS 16

/// The options of a spawned field effect, each used when its EFFECT_USE_ flag
/// (or EFFECT_SCALE_TO_TARGET) is set.
struct EffectOptions {
  s32 priority;       ///< the sprite's priority
  s32 palette;        ///< passed to Actor_SetSpriteColor
  FIXED startScaleX;  ///< 16.16 fixed point, 0x10000 full size
  FIXED startScaleY;  ///< 16.16 fixed point, 0x10000 full size
  FIXED targetScaleX; ///< 16.16 fixed point, 0x10000 full size
  FIXED targetScaleY; ///< 16.16 fixed point, 0x10000 full size
  s16 type;           ///< the object type to create
  const s32 *script;  ///< the object script, run in mode 1
  u16 rotation;       ///< 1/65536 turns
  u16 spin;           ///< 1/65536 turns per frame
  void (*update)(union FieldObject *object); ///< replaces the update hook
};

STATIC_ASSERT(offsetof(struct EffectOptions, type) == 24);
STATIC_ASSERT(offsetof(struct EffectOptions, update) == 36);

void FieldEffect_UpdateMotionScaleAndSpin(union FieldObject *object);

struct EffectOptions;

void FieldEffect_SpawnResident(s32 x, s32 y, s32 z, s32 velocityX,
                               s32 velocityY, s32 velocityZ, u32 flags,
                               const struct EffectOptions *options);

#endif
