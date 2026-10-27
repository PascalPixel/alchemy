#ifndef ALCHEMY_FIELD_PARTICLE_H
#define ALCHEMY_FIELD_PARTICLE_H

/// @file
/// Field effect sprites: the particles field Psynergy moves and draws.

#include "edition.h"
#include "types.h"
#include "static_assert.h"

/// FieldParticle.targetX and targetZ while the sprite has no target, at
/// rest.
#define FIELD_EFFECT_SPRITE_NO_TARGET ((s32)0x80000000)

struct FieldParticle;
/// A field effect sprite's per-frame routine (FieldParticle.callback).
typedef void (*EffectCallback)(struct FieldParticle *);

/// A particle a field Psynergy moves and draws: its sprite, its position and
/// target, its speed, scale and heading, and its per-frame routine. The 24 are
/// FieldEffectWork.sprites, in the HEAP_SLOT_FIELD_EFFECT heap slot.
struct FieldParticle {
  void *actor;  ///< the actor it draws, released with it
  s32 x;        ///< 16.16 fixed-point pixels
  s32 z;        ///< 16.16 fixed-point pixels
  s32 targetX;  ///< 16.16 fixed-point pixels; FIELD_EFFECT_SPRITE_NO_TARGET at
                ///< rest
  s32 targetZ;  ///< 16.16 fixed-point pixels; FIELD_EFFECT_SPRITE_NO_TARGET at
                ///< rest
  s32 originX;  ///< 16.16 fixed-point pixels; where the sprite started
  s32 originZ;  ///< 16.16 fixed-point pixels; where the sprite started
  s32 speed;    ///< 16.16 fixed-point pixels per frame
  s32 speedMax; ///< 16.16 fixed-point pixels per frame
  s32 acceleration;        ///< 16.16 fixed-point pixels per frame, each frame
  s32 scaleX;              ///< 16.16 fixed point, 0x10000 full size
  s32 scaleY;              ///< 16.16 fixed point, 0x10000 full size
  u16 heading;             ///< 1/65536 turns
  s16 turnStepMax;         ///< 1/65536 turns a frame
  EffectCallback callback; ///< run each frame once callbackDelay is 0
  u16 age;                 ///< frames
  s16 callbackDelay;       ///< frames
  s16 orbitRadius;         ///< pixels
  s16 orbitAngle;          ///< 1/32 turns
  s8 state;                ///< the routine's own step; 0 when it is given
  s8 stopAtTarget;         ///< nonzero: lands on the target within 8 pixels
  /// Nonzero turns the heading toward the target by at most turnStepMax a
  /// frame; zero faces the target at once.
  s8 turnStepLimited;
  s8 updateMotion; ///< nonzero: FieldParticle_UpdateMotion moves it each
                   ///< frame
  s8 render;       ///< nonzero: drawn each frame
  s8 active;       ///< nonzero while the sprite is in use
  u8 randomValue;  ///< random when filled; not read by name in TBS
  /// bits: 2 z also lifts the sprite (set when it is filled)
  u8 flags;
};

STATIC_ASSERT(sizeof(struct FieldParticle) == 0x48);
STATIC_ASSERT(offsetof(struct FieldParticle, orbitRadius) == 0x3c);
STATIC_ASSERT(offsetof(struct FieldParticle, state) == 0x40);

#endif
