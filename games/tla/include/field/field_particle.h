#ifndef ALCHEMY_FIELD_PARTICLE_H
#define ALCHEMY_FIELD_PARTICLE_H

// The field effect sprites the field Psynergy keeps (FIELD_PARTICLE).

#include "edition.h"
#include "fixed_math.h"
#include "types.h"
#include "static_assert.h"

// A targetX that marks an effect with no target to steer toward.
#define EFFECT_NO_TARGET ((Sint32)0x80000000)

struct FIELD_PARTICLE;
typedef void (*EFFECT_CALLBACK)(struct FIELD_PARTICLE *);

// A particle a Psynergy effect moves and draws: its sprite, its position and target, its speed,
// scale and heading, and its per-tick callback.
typedef struct FIELD_PARTICLE
{
    void           *sprite;        // the sprite it draws (SPR), released with it
    FIXED           x;             // 16.16 fixed-point pixels
    FIXED           z;             // 16.16 fixed-point pixels
    FIXED           targetX;       // 16.16 fixed-point pixels; EFFECT_NO_TARGET at rest
    FIXED           targetZ;       // 16.16 fixed-point pixels; EFFECT_NO_TARGET at rest
    FIXED           originX;       // 16.16 fixed-point pixels
    FIXED           originZ;       // 16.16 fixed-point pixels
    FIXED           speed;         // 16.16 fixed-point pixels per tick
    FIXED           speedMax;      // 16.16 fixed-point pixels per tick
    FIXED           acceleration;  // 16.16 fixed-point pixels per tick, each tick
    FIXED           scaleX;        // 16.16 fixed point, toFIXED(1) full size
    FIXED           scaleY;        // 16.16 fixed point, toFIXED(1) full size
    Uint16          heading;       // 1/65536 turns
    Sint16          turnStepMax;   // 1/65536 turns a tick
    EFFECT_CALLBACK callback;      // run each tick once callbackDelay is 0
    Uint16          age;           // ticks
    Sint16          callbackDelay; // ticks
    Sint16          orbitRadius;   // pixels
    Sint16          orbitAngle;    // 1/32 turns
    Sint8           state;         // the callback's own step
    Sint8           stopAtTarget;  // nonzero: lands on the target within 8 pixels
    // Nonzero turns it toward the target at most turnStepMax a tick; zero faces it straight at the
    // target.
    Sint8           turnStepLimited;
    // nonzero: fdParticleUpdateMotion moves it each tick
    Sint8           updateMotion;
    Sint8           render;      // nonzero: drawn each tick
    Sint8           active;      // nonzero while the slot is in use
    Uint8           randomValue; // not read by name
    Uint8           flags;       // FIELD_PARTICLE_LIFTED_BY_Z
} FIELD_PARTICLE;

// FIELD_PARTICLE.flags: z also lifts the sprite off the ground.
#define FIELD_PARTICLE_LIFTED_BY_Z 0x04

STATIC_ASSERT(sizeof(FIELD_PARTICLE) == 72);
STATIC_ASSERT(offsetof(FIELD_PARTICLE, orbitRadius) == 60);
STATIC_ASSERT(offsetof(FIELD_PARTICLE, state) == 64);

// The states of a particle that flies out and back: Halt's and Force's (fdEffectUpdateHaltParticle,
// fdEffectUpdateRadialBurst).
typedef enum OUT_AND_BACK_STATE
{
    OUT_AND_BACK_AIM_OUT     = 0, // picks the point it flies out to
    OUT_AND_BACK_FLYING_OUT  = 1, // waits to arrive there
    OUT_AND_BACK_AIM_BACK    = 2, // picks the point it comes back to
    OUT_AND_BACK_FLYING_BACK = 3  // frees its slot once it arrives
} OUT_AND_BACK_STATE;

#endif
