#ifndef ALCHEMY_FIELD_EFFECT_H
#define ALCHEMY_FIELD_EFFECT_H

/// @file
/// Field effects, their scripts and the flags that spawn them.

#include "edition.h"
#include "field_event.h"
#include "vector3.h"

/// A field effect: a small object on the field that moves at its velocity and
/// grows and spins at its rates, run by its script and update hook.
struct FieldEffect {
  u8 unk00[8];
  s32 x; ///< 16.16 fixed-point pixels
  s32 y; ///< 16.16 fixed-point pixels
  s32 z; ///< 16.16 fixed-point pixels
  u8 unk14[4];
  s32 scaleX; ///< 16.16 fixed point, 0x10000 full size
  s32 scaleY; ///< 16.16 fixed point, 0x10000 full size
  u8 unk20[3];
  u8 priorityFlags;
  u8 unk24[0x0c];
  s32 scaleRateX; ///< 16.16 fixed point a frame
  s32 scaleRateY; ///< 16.16 fixed point a frame
  u8 unk38[0x0c];
  struct Vector3 velocity; ///< 16.16 fixed-point pixels per frame
  struct AnimationObject *sprite;
  u8 unk54;
  u8 motionFlags;
  u8 unk56[3];
  u8 collisionFlags;
  u8 unk5a[0x0a];
  u16 spin; ///< 1/65536 turns per frame
  u8 unk66[2];
  s32 countdown;
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
  EFFECT_SCRIPT_MASK = 0x0f,
  EFFECT_USE_PALETTE = 0x10000,
  EFFECT_USE_PRIORITY = 0x20000,
  EFFECT_SCALE_TO_TARGET = 0x40000,
  EFFECT_USE_START_SCALE = 0x80000,
  EFFECT_USE_TYPE = 0x100000,
  EFFECT_USE_SCRIPT = 0x200000,
  EFFECT_USE_ROTATION = 0x400000,
  EFFECT_USE_SPIN = 0x800000,
  EFFECT_USE_UPDATE = 0x1000000
};

/// The options of a spawned field effect, each used when its EFFECT_USE_ flag
/// (or EFFECT_SCALE_TO_TARGET) is set.
struct EffectOptions {
  s32 priority;
  s32 palette;
  s32 startScaleX;  ///< 16.16 fixed point, 0x10000 full size
  s32 startScaleY;  ///< 16.16 fixed point, 0x10000 full size
  s32 targetScaleX; ///< 16.16 fixed point, 0x10000 full size
  s32 targetScaleY; ///< 16.16 fixed point, 0x10000 full size
  s16 type;
  const s32 *script;
  u16 rotation; ///< 1/65536 turns
  u16 spin;     ///< 1/65536 turns per frame
  void (*update)(union FieldObject *object);
};

STATIC_ASSERT(offsetof(struct EffectOptions, type) == 0x18);
STATIC_ASSERT(offsetof(struct EffectOptions, update) == 0x24);

void Effect_SpawnResident(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                          s32 velocityZ, u32 flags,
                          const struct EffectOptions *options);

#endif
