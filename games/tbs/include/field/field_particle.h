#ifndef ALCHEMY_FIELD_PARTICLE_H
#define ALCHEMY_FIELD_PARTICLE_H

/// @file
/// Field effect sprites: the particles field Psynergy moves and draws.

#include "edition.h"
#include "fixed_math.h"
#include "static_assert.h"
#include "types.h"

/// FieldParticle.targetX and targetZ while the sprite has no target, at
/// rest.
#define FIELD_EFFECT_SPRITE_NO_TARGET ((s32)0x80000000)

struct FieldParticle;
/// A field effect sprite's per-frame routine (FieldParticle.callback).
typedef void (*EffectCallback)(struct FieldParticle *);

/// A particle a field Psynergy moves and draws: its sprite, its position and
/// target, its speed, scale and heading, and its per-frame routine.
struct FieldParticle {
  void *actor;   ///< the actor it draws, released with it
  FIXED x;       ///< 16.16 fixed-point pixels
  FIXED z;       ///< 16.16 fixed-point pixels
  FIXED targetX; ///< 16.16 fixed-point pixels; FIELD_EFFECT_SPRITE_NO_TARGET at
                 ///< rest
  FIXED targetZ; ///< 16.16 fixed-point pixels; FIELD_EFFECT_SPRITE_NO_TARGET at
                 ///< rest
  FIXED originX; ///< 16.16 fixed-point pixels; where the sprite started
  FIXED originZ; ///< 16.16 fixed-point pixels; where the sprite started
  FIXED speed;   ///< 16.16 fixed-point pixels per frame
  FIXED speedMax;          ///< 16.16 fixed-point pixels per frame
  FIXED acceleration;      ///< 16.16 fixed-point pixels per frame, each frame
  FIXED scaleX;            ///< 16.16 fixed point, FIXED_ONE full size
  FIXED scaleY;            ///< 16.16 fixed point, FIXED_ONE full size
  u16 heading;             ///< 1/65536 turns
  s16 turnStepMax;         ///< 1/65536 turns a frame
  EffectCallback callback; ///< run each frame once callbackDelay is 0
  s16 age;                 ///< frames
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
  u8 randomValue;  ///< random when filled
  u8 flags;        ///< FieldParticleFlag bits
};

/// FieldParticle.flags bits.
enum FieldParticleFlag {
  /// Its z also lifts the sprite up the screen, from 508 pixels down; set when
  /// it is filled (FieldParticle_Initialize).
  FIELD_PARTICLE_LIFTED_BY_Z = 0x04
};

STATIC_ASSERT(sizeof(struct FieldParticle) == 72);
STATIC_ASSERT(offsetof(struct FieldParticle, orbitRadius) == 60);
STATIC_ASSERT(offsetof(struct FieldParticle, state) == 64);

/// Gives every particle in use in `work->member` the state `newState`:
/// `cursor`, a byte pointer, walks their state bytes and `count` counts them
/// down, both the caller's.
#define FIELD_PARTICLES_SET_ACTIVE_STATE(cursor, count, work, member,          \
                                         newState)                             \
  {                                                                            \
    s32 state_ = (newState);                                                   \
    (cursor) = (void *)(work);                                                 \
    (cursor) += offsetof(__typeof__(*(work)), member) +                        \
                offsetof(struct FieldParticle, state);                         \
    (count) = sizeof((work)->member) / sizeof(struct FieldParticle) - 1;       \
    do {                                                                       \
      if (((s8 *)(cursor))[offsetof(struct FieldParticle, active) -            \
                           offsetof(struct FieldParticle, state)] != 0)        \
        *(cursor) = state_;                                                    \
      (count)--;                                                               \
      (cursor) += sizeof(struct FieldParticle);                                \
    } while ((count) >= 0);                                                    \
  }

#endif
