#ifndef ALCHEMY_FIELD_PARTICLE_H
#define ALCHEMY_FIELD_PARTICLE_H

/// @file
/// The field effect sprites the field Psynergy keeps (FieldParticle).

#include "edition.h"
#include "fixed_math.h"
#include "types.h"
#include "static_assert.h"

/// A targetX that marks an effect with no target to steer toward.
#define EFFECT_NO_TARGET ((s32)0x80000000)

struct FieldParticle;
typedef void (*EffectCallback)(struct FieldParticle *);

/// A particle a Psynergy effect moves and draws: its sprite, its position and
/// target, its speed, scale and heading, and its per-frame callback.
struct FieldParticle {
  void *sprite;       ///< the sprite it draws (struct Sprite), released with it
  FIXED x;            ///< 16.16 fixed-point pixels
  FIXED z;            ///< 16.16 fixed-point pixels
  FIXED targetX;      ///< 16.16 fixed-point pixels; EFFECT_NO_TARGET at rest
  FIXED targetZ;      ///< 16.16 fixed-point pixels; EFFECT_NO_TARGET at rest
  FIXED originX;      ///< 16.16 fixed-point pixels
  FIXED originZ;      ///< 16.16 fixed-point pixels
  FIXED speed;        ///< 16.16 fixed-point pixels per frame
  FIXED speedMax;     ///< 16.16 fixed-point pixels per frame
  FIXED acceleration; ///< 16.16 fixed-point pixels per frame, each frame
  FIXED scaleX;       ///< 16.16 fixed point, FIXED_ONE full size
  FIXED scaleY;       ///< 16.16 fixed point, FIXED_ONE full size
  u16 heading;        ///< 1/65536 turns
  s16 turnStepMax;    ///< 1/65536 turns a frame
  EffectCallback callback; ///< run each frame once callbackDelay is 0
  u16 age;                 ///< frames
  s16 callbackDelay;       ///< frames
  s16 orbitRadius;         ///< pixels
  s16 orbitAngle;          ///< 1/32 turns
  s8 state;                ///< the callback's own step
  s8 stopAtTarget;         ///< nonzero: lands on the target within 8 pixels
  /// Nonzero turns it toward the target at most turnStepMax a frame; zero
  /// faces it straight at the target.
  s8 turnStepLimited;
  s8 updateMotion; ///< nonzero: FieldParticle_UpdateMotion moves it each frame
  s8 render;       ///< nonzero: drawn each frame
  s8 active;       ///< nonzero while the slot is in use
  u8 randomValue;  ///< not read by name
  u8 flags;        ///< FIELD_PARTICLE_LIFTED_BY_Z
};

/// FieldParticle.flags: z also lifts the sprite off the ground.
#define FIELD_PARTICLE_LIFTED_BY_Z 0x04

STATIC_ASSERT(sizeof(struct FieldParticle) == 72);
STATIC_ASSERT(offsetof(struct FieldParticle, orbitRadius) == 60);
STATIC_ASSERT(offsetof(struct FieldParticle, state) == 64);

u32 FieldParticle_HasPendingTarget(struct FieldParticle *);
void FieldParticle_SetPosition(struct FieldParticle *, s32, s32);
void FieldParticle_SetAnimation(struct FieldParticle *, s32);
void FieldParticle_SetCallback(struct FieldParticle *, EffectCallback);
void FieldParticle_Update(struct FieldParticle *);

/// The states of a particle that flies out and back: Halt's and Force's
/// (FieldEffect_UpdateHaltParticle, FieldEffect_UpdateRadialBurst).
enum OutAndBackState {
  OUT_AND_BACK_AIM_OUT = 0,    ///< picks the point it flies out to
  OUT_AND_BACK_FLYING_OUT = 1, ///< waits to arrive there
  OUT_AND_BACK_AIM_BACK = 2,   ///< picks the point it comes back to
  OUT_AND_BACK_FLYING_BACK = 3 ///< frees its slot once it arrives
};

#endif
