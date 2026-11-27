#ifndef ALCHEMY_FIELD_PARTICLE_H
#define ALCHEMY_FIELD_PARTICLE_H

// Field effect sprites: the particles field Psynergy moves and draws.

#include "edition.h"
#include "fixed_math.h"
#include "static_assert.h"
#include "types.h"

// FIELD_PARTICLE.targetX and targetZ while the sprite has no target, at rest.
#define FIELD_EFFECT_SPRITE_NO_TARGET ((Sint32)0x80000000)

struct FIELD_PARTICLE;
// A field effect sprite's per-tick function (FIELD_PARTICLE.callback).
typedef void (*EFFECT_CALLBACK)(struct FIELD_PARTICLE *);

// A particle a field Psynergy moves and draws: its sprite, its position and target, its speed,
// scale and heading, and its per-tick function.
typedef struct FIELD_PARTICLE
{
	void           *sprite; // the sprite it draws, released with it
	FIXED           x;      // 16.16 fixed-point pixels
	FIXED           z;      // 16.16 fixed-point pixels
	// 16.16 fixed-point pixels; FIELD_EFFECT_SPRITE_NO_TARGET at rest
	FIXED           targetX;
	// 16.16 fixed-point pixels; FIELD_EFFECT_SPRITE_NO_TARGET at rest
	FIXED           targetZ;
	FIXED           originX;       // 16.16 fixed-point pixels; where the sprite started
	FIXED           originZ;       // 16.16 fixed-point pixels; where the sprite started
	FIXED           speed;         // 16.16 fixed-point pixels per tick
	FIXED           speedMax;      // 16.16 fixed-point pixels per tick
	FIXED           acceleration;  // 16.16 fixed-point pixels per tick, each tick
	FIXED           scaleX;        // 16.16 fixed point, toFIXED(1) full size
	FIXED           scaleY;        // 16.16 fixed point, toFIXED(1) full size
	Uint16          heading;       // 1/65536 turns
	Sint16          turnStepMax;   // 1/65536 turns a tick
	EFFECT_CALLBACK callback;      // run each tick once callbackDelay is 0
	Sint16          age;           // ticks
	Sint16          callbackDelay; // ticks
	Sint16          orbitRadius;   // pixels
	Sint16          orbitAngle;    // 1/32 turns
	Sint8           state;         // the function's own step; 0 when it is given
	Sint8           stopAtTarget;  // nonzero: lands on the target within 8 pixels
	// Nonzero turns the heading toward the target by at most turnStepMax a tick; zero faces the
	// target at once.
	Sint8           turnStepLimited;
	// nonzero: fdParticleUpdateMotion moves it each tick
	Sint8           updateMotion;
	Sint8           render;      // nonzero: drawn each tick
	Sint8           active;      // nonzero while the sprite is in use
	Uint8           randomValue; // random when filled
	Uint8           flags;       // FIELD_PARTICLE_FLAG bits
} FIELD_PARTICLE;

// FIELD_PARTICLE.flags bits.
typedef enum FIELD_PARTICLE_FLAG
{
	// Its z also lifts the sprite up the screen, from 508 pixels down; set when it is filled
	// (fdParticleInit).
	FIELD_PARTICLE_LIFTED_BY_Z = 0x04
} FIELD_PARTICLE_FLAG;

STATIC_ASSERT(sizeof(FIELD_PARTICLE) == 72);
STATIC_ASSERT(offsetof(FIELD_PARTICLE, orbitRadius) == 60);
STATIC_ASSERT(offsetof(FIELD_PARTICLE, state) == 64);

#endif
