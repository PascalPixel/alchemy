#ifndef ALCHEMY_EFFECT_SLOT_H
#define ALCHEMY_EFFECT_SLOT_H

/// @file
/// Effect slots: the particles Psynergy effects move and draw.

#include "edition.h"
#include "types.h"
#include "static_assert.h"

/// EffectSlot.targetX and targetZ while the slot has no target, at rest.
#define EFFECT_NO_TARGET ((s32)0x80000000)

struct EffectSlot;
/// An effect slot's per-frame routine (EffectSlot.callback).
typedef void (*EffectCallback)(struct EffectSlot *);

/// A particle a Psynergy effect moves and draws: its sprite, its position and
/// target, its speed, scale and heading, and its per-frame callback. The 24 are
/// BattleFxScene.slots, in the HEAP_SLOT_FX work slot.
struct EffectSlot {
  void *object;     ///< the slot's sprite, released with the slot
  s32 x;            ///< 16.16 fixed-point pixels
  s32 z;            ///< 16.16 fixed-point pixels
  s32 targetX;      ///< 16.16 fixed-point pixels; EFFECT_NO_TARGET at rest
  s32 targetZ;      ///< 16.16 fixed-point pixels; EFFECT_NO_TARGET at rest
  s32 originX;      ///< 16.16 fixed-point pixels; where the slot started
  s32 originZ;      ///< 16.16 fixed-point pixels; where the slot started
  s32 speed;        ///< 16.16 fixed-point pixels per frame
  s32 maxSpeed;     ///< 16.16 fixed-point pixels per frame
  s32 acceleration; ///< 16.16 fixed-point pixels per frame, each frame
  s32 scaleX;       ///< 16.16 fixed point, 0x10000 full size
  s32 scaleY;       ///< 16.16 fixed point, 0x10000 full size
  u16 heading;      ///< 1/65536 turns
  s16 maxTurnStep;  ///< 1/65536 turns a frame
  EffectCallback callback; ///< run each frame once callbackDelay is 0
  u16 age;                 ///< frames
  s16 callbackDelay;       ///< frames
  s16 orbitRadius;         ///< pixels
  s16 orbitAngle;          ///< 1/32 turns
  s8 state;                ///< the callback's own step; 0 when it is given
  s8 stopAtTarget;         ///< nonzero: lands on the target within 8 pixels
  /// Nonzero turns the heading toward the target by at most maxTurnStep a
  /// frame; zero faces the target at once.
  s8 limitTurnStep;
  s8 updateMotion; ///< nonzero: EffectSlot_UpdateMotion moves it each frame
  s8 render;       ///< nonzero: drawn each frame
  s8 active;       ///< nonzero while the slot is in use
  u8 randomValue;  ///< random when filled; not read by name in TBS
  /// bits: 2 z also lifts the sprite (set when the slot is filled)
  u8 flags;
};

STATIC_ASSERT(sizeof(struct EffectSlot) == 0x48);
STATIC_ASSERT(offsetof(struct EffectSlot, orbitRadius) == 0x3c);
STATIC_ASSERT(offsetof(struct EffectSlot, state) == 0x40);

#endif
