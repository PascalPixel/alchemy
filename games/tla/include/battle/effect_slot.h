#ifndef ALCHEMY_EFFECT_SLOT_H
#define ALCHEMY_EFFECT_SLOT_H

#include "edition.h"
#include "types.h"
#include "static_assert.h"

#define EFFECT_NO_TARGET ((s32)0x80000000)

struct FieldParticle;
typedef void (*EffectCallback)(struct FieldParticle *);

/// A particle a Psynergy effect moves and draws: its sprite, its position and
/// target, its speed, scale and heading, and its per-frame callback.
struct FieldParticle {
  void *object;     ///< the slot's sprite
  s32 x;            ///< 16.16 fixed-point pixels
  s32 z;            ///< 16.16 fixed-point pixels
  s32 targetX;      ///< 16.16 fixed-point pixels; EFFECT_NO_TARGET at rest
  s32 targetZ;      ///< 16.16 fixed-point pixels; EFFECT_NO_TARGET at rest
  s32 originX;      ///< 16.16 fixed-point pixels
  s32 originZ;      ///< 16.16 fixed-point pixels
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
  s8 state;                ///< the callback's own step
  s8 stopAtTarget;         ///< nonzero: lands on the target within 8 pixels
  s8 limitTurnStep; ///< Clamp steering to maxTurnStep; zero faces the target
                    ///< directly.
  s8 updateMotion;  ///< nonzero: FieldParticle_UpdateMotion moves it each frame
  s8 render;        ///< nonzero: drawn each frame
  s8 active;        ///< nonzero while the slot is in use
  u8 randomValue;   ///< not read by name in TLA
  /// bits: 2 z also lifts the sprite
  u8 flags;
};

STATIC_ASSERT(sizeof(struct FieldParticle) == 0x48);
STATIC_ASSERT(offsetof(struct FieldParticle, orbitRadius) == 0x3c);
STATIC_ASSERT(offsetof(struct FieldParticle, state) == 0x40);

u32 FieldParticle_HasPendingTarget(struct FieldParticle *);
void FieldParticle_SetPosition(struct FieldParticle *, s32, s32);
void FieldParticle_SetActorMode(struct FieldParticle *, s32);
void FieldParticle_SetCallback(struct FieldParticle *, EffectCallback);
void FieldParticle_Update(struct FieldParticle *);

#endif
