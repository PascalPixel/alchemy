#ifndef GUARD_CONFIGURED_EFFECT_SPAWN_H
#define GUARD_CONFIGURED_EFFECT_SPAWN_H

/// @file
/// The sprite and effect views and the options a scene's configured effect
/// spawner uses.

#include "types.h"

/// A scene actor's sprite, as far as the angle a scene turns it by.
struct Sprite {
  u8 pad00[9];
  /// AnimationObject's attribute 2 high byte; not named by TBS code.
  /// bits: 7..4 palette | 3..2 priority | 1..0 tile bits 9..8
  u8 flags9;
  u8 pad0a[20];
  u16 angle; ///< AnimationObject's rotation, in 1/65536 turns
  u8 pad20[6];
  u8 state26; ///< AnimationObject's flags; not named by TBS code
};

/// A field effect's layout, as FieldEffect gives it; no TBS code names this
/// struct.
struct Effect {
  u8 pad00[8];
  s32 position[3]; ///< FieldEffect's x, y and z
  u8 pad14[4];
  s32 accum18; ///< FieldEffect's scaleX
  s32 accum1c; ///< FieldEffect's scaleY
  u8 pad20[3];
  u8 flags23; ///< FieldEffect's priorityFlags
  u8 pad24[12];
  s32 rate30; ///< FieldEffect's scaleRateX
  s32 rate34; ///< FieldEffect's scaleRateY
  u8 pad38[12];
  s32 velocityX; ///< FieldEffect's velocity.x
  s32 velocityY; ///< FieldEffect's velocity.y
  s32 velocityZ; ///< FieldEffect's velocity.z
  struct Sprite *sprite;
  u8 pad54;
  u8 mode55; ///< FieldEffect's motionFlags
  u8 pad56[14];
  u16 step64; ///< FieldEffect's spin
  u8 pad66[6];
  u32 callback; ///< FieldEffect's update
};

/// The options a scene passes to Effect_Spawn for a configured effect: its
/// kind, mode, rates and callback.
struct ConfiguredEffectOptions {
  s32 modeBits; ///< EffectOptions' priority; not named by TBS code
  s32 mode;     ///< EffectOptions' palette; not named by TBS code
  /// EffectOptions' startScaleX, in 16.16 fixed point (0x10000 full size);
  /// the waterway stump's ring starts at 0x8000.
  s32 accum18;
  s32 accum1c;  ///< EffectOptions' startScaleY, 16.16 fixed point
  s32 target30; ///< EffectOptions' targetScaleX, 16.16 fixed point
  s32 target34; ///< EffectOptions' targetScaleY, 16.16 fixed point
  s16 kind;     ///< EffectOptions' type, the object type to create
  u16 pad1a;
  s32 callbackArg; ///< EffectOptions' script; not named by TBS code
  u16 angle;       ///< EffectOptions' rotation; not named by TBS code
  u16 step;        ///< EffectOptions' spin; not named by TBS code
  u32 callback;    ///< EffectOptions' update; not named by TBS code
};

#endif
