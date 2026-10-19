#ifndef ALCHEMY_EFFECT_SLOT_H
#define ALCHEMY_EFFECT_SLOT_H

/// @file
/// Effect slots: the particles Psynergy effects move and draw.

#include "edition.h"
#include "types.h"
#include "static_assert.h"

#define EFFECT_NO_TARGET ((s32)0x80000000)

struct EffectSlot;
typedef void (*EffectCallback)(struct EffectSlot *);

/// A particle a Psynergy effect moves and draws: its sprite, its position and
/// target, its speed, scale and heading, and its per-frame callback.
struct EffectSlot {
  void *object;
  s32 x;            ///< 16.16 fixed-point pixels
  s32 z;            ///< 16.16 fixed-point pixels
  s32 targetX;      ///< 16.16 fixed-point pixels
  s32 targetZ;      ///< 16.16 fixed-point pixels
  s32 originX;      ///< 16.16 fixed-point pixels
  s32 originZ;      ///< 16.16 fixed-point pixels
  s32 speed;        ///< 16.16 fixed-point pixels per frame
  s32 maxSpeed;     ///< 16.16 fixed-point pixels per frame
  s32 acceleration; ///< 16.16 fixed-point pixels per frame, each frame
  s32 scaleX;       ///< 16.16 fixed point, 0x10000 full size
  s32 scaleY;       ///< 16.16 fixed point, 0x10000 full size
  u16 heading;      ///< 1/65536 turns
  s16 maxTurnStep;  ///< 1/65536 turns a frame
  EffectCallback callback;
  u16 age;           ///< frames
  s16 callbackDelay; ///< frames
  s16 orbitRadius;   ///< pixels
  s16 orbitAngle;    ///< 1/32 turns
  s8 state;
  s8 stopAtTarget;
  s8 limitTurnStep; ///< Clamp steering to maxTurnStep; zero faces the target
                    ///< directly.
  s8 updateMotion;
  s8 render;
  s8 active;
  u8 randomValue;
  u8 flags;
};

STATIC_ASSERT(sizeof(struct EffectSlot) == 0x48);
STATIC_ASSERT(offsetof(struct EffectSlot, orbitRadius) == 0x3c);
STATIC_ASSERT(offsetof(struct EffectSlot, state) == 0x40);

#endif
