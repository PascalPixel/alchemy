#ifndef ALCHEMY_FIELD_EFFECT_H
#define ALCHEMY_FIELD_EFFECT_H

#include "edition.h"
#include "field_event.h"
#include "vector3.h"

/// A field effect: a small object on the field that moves at its velocity and
/// grows and spins at its rates, run by its script and update hook.
struct FieldEffect {
  /// FieldActor's unk00 and facing; not named by this view.
  u8 unk00[8];
  s32 x;       ///< 16.16 fixed-point pixels
  s32 y;       ///< 16.16 fixed-point pixels
  s32 z;       ///< 16.16 fixed-point pixels
  u8 unk14[4]; ///< FieldActor's terrainHeight; not named by this view
  s32 scaleX;  ///< 16.16 fixed point, 0x10000 full size
  s32 scaleY;  ///< 16.16 fixed point, 0x10000 full size
  u8 unk20[3]; ///< not named by this view
  /// The spawner clears bit 0 when EFFECT_USE_PRIORITY sets the sprite's
  /// priority.
  u8 priorityFlags;
  /// FieldActor's velocityX to velocityZ; effects move by velocity instead.
  u8 unk24[0x0c];
  /// Added to scaleX each frame by the update hooks, in 16.16 fixed point a
  /// frame; zeroed by the spawner.
  s32 scaleRateX;
  s32 scaleRateY; ///< as scaleRateX, for scaleY
  u8 unk38[0x0c]; ///< FieldActor's targetX to targetZ; not named by this view
  /// Added to x, y and z each frame by the update hooks, in 16.16 fixed-point
  /// pixels per frame.
  struct Vector3 velocity;
  struct FieldSprite *sprite; ///< the sprite spin turns
  u8 unk54;          ///< FieldActor's animationKind; not named by this view
  u8 motionFlags;    ///< zeroed by the spawner
  u8 unk56[3];       ///< FieldActor's unk56; not named by this view
  u8 collisionFlags; ///< FieldActor's collisionFlags; not named by this view
  u8 unk5a[0x0a];    ///< FieldActor's unk5a; not named by this view
  /// Added to the sprite's rotation each frame by the update hooks, in
  /// 1/65536 turns per frame; zeroed by the spawner.
  u16 spin;
  u8 unk66[2];   ///< not named by this view
  s32 countdown; ///< no TLA code names it
  /// Called each frame while the effect is live: the spawner sets
  /// BattleEffect_UpdateObjectMotionScaleAndLinkedAngle, or options.update.
  void (*update)(union FieldObject *object);
};

STATIC_ASSERT(offsetof(struct FieldEffect, scaleRateX) == 0x30);
STATIC_ASSERT(offsetof(struct FieldEffect, velocity) == 0x44);
STATIC_ASSERT(offsetof(struct FieldEffect, sprite) == 0x50);
STATIC_ASSERT(offsetof(struct FieldEffect, spin) == 0x64);
STATIC_ASSERT(offsetof(struct FieldEffect, countdown) == 0x68);
STATIC_ASSERT(offsetof(struct FieldEffect, update) == 0x6c);

/// An object on the field, read as an actor or as an effect.
union FieldObject {
  struct FieldActor actor;
  struct FieldEffect effect;
};

#define SCRIPT_WAIT(frames) 0, (frames)
#define SCRIPT_SET_POSITION(x, y, z) 2, (x), (y), (z)
#define SCRIPT_REPEAT(count, label) 12, (count), (label)
#define SCRIPT_DELETE 27

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
  EFFECT_USE_TYPE = 0x100000,       ///< options.type, else the default type
  EFFECT_USE_SCRIPT = 0x200000,     ///< options.script, run in mode 1
  EFFECT_USE_ROTATION = 0x400000,   ///< options.rotation, as the sprite's
  EFFECT_USE_SPIN = 0x800000,       ///< options.spin
  EFFECT_USE_UPDATE = 0x1000000     ///< options.update
};

/// The options of a spawned field effect, each used when its EFFECT_USE_ flag
/// (or EFFECT_SCALE_TO_TARGET) is set.
struct EffectOptions {
  s32 priority;      ///< the sprite's priority
  s32 palette;       ///< passed to Object_SetPartAttribute
  s32 startScaleX;   ///< 16.16 fixed point, 0x10000 full size
  s32 startScaleY;   ///< 16.16 fixed point, 0x10000 full size
  s32 targetScaleX;  ///< 16.16 fixed point, 0x10000 full size
  s32 targetScaleY;  ///< 16.16 fixed point, 0x10000 full size
  s16 type;          ///< the object type to create
  const s32 *script; ///< the object script, run in mode 1
  u16 rotation;      ///< 1/65536 turns
  u16 spin;          ///< 1/65536 turns per frame
  void (*update)(union FieldObject *object); ///< replaces the update hook
};

STATIC_ASSERT(offsetof(struct EffectOptions, type) == 0x18);
STATIC_ASSERT(offsetof(struct EffectOptions, update) == 0x24);

void Effect_SetPriority(struct FieldEffect *effect, s32 priority);
struct FieldEffect *Effect_CreateTranslucent(s32 x, s32 y, s32 z, s32 type);
struct FieldEffect *Effect_Create(s32 x, s32 y, s32 z, s32 type);
void BattleEffect_UpdateObjectMotionScaleAndLinkedAngle(
    union FieldObject *object);
void Effect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                  s32 velocityZ, u32 flags,
                  const struct EffectOptions *options);
void Effect_SpawnResident(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                          s32 velocityZ, u32 flags,
                          const struct EffectOptions *options);

#endif
