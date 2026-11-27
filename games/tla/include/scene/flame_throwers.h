#ifndef ALCHEMY_FLAME_THROWERS_H
#define ALCHEMY_FLAME_THROWERS_H

// The state of the Mars Lighthouse rooms' flame throwers and of the fireballs that knock the leader
// back, or carry him along on a ride, when they hit him; it is kept in the inventory snapshot's
// spare space.

#include "fixed_math.h"
#include "types.h"
#include "game/game.h"

struct ACTOR;

// A flame thrower's flags (the flame thrower tables): the flame's reach in the low four bits, and
// which flame it sends, or none.
typedef enum FLAME_THROWER_FLAGS
{
	FLAME_FORWARD     = 0x00, // scFlameThrowerSendForward
	FLAME_EAST        = 0x10, // scFlameThrowerSendEast
	FLAME_WEST        = 0x20, // scFlameThrowerSendWest
	FLAME_BACKWARD    = 0x30, // scFlameThrowerSendBackward
	FLAME_THROWER_DRY = 0x40, // nothing comes out of it
	FLAME_SPARK       = 0x80  // scFlameThrowerSendSpark
} FLAME_THROWER_FLAGS;

// A flame thrower's packed state: the flame's reach in the low four bits, FLAME_THROWER_FLAGS in
// the next four.
#define FLAME_THROWER_REACH_BITS 0xf
#define FLAME_THROWER_FLAG_BITS  0xf0

// One of the state's four flame throwers.
typedef struct FLAME_THROWER
{
	Sint16        active;
	Sint16        timer; // ticks to the next puff, 8 after each
	// FLAME_THROWER_FLAGS in the high nibble (the FlameThrowerSend function each picks), the reach
	// in the low.
	Sint16        flags;
	Sint16        offScreen; // puffs off screen and keeps the flames' sprites
	struct ACTOR *actor;
	FIXED         reach;     // 16.16, the low nibble of flags << 16
} FLAME_THROWER;

// The way the leader was last knocked back along x and z, 1 or -1; the other axis is 0.
typedef struct KNOCK_BACK_DIRECTION
{
	Uint16 x;
	Uint16 z;
} KNOCK_BACK_DIRECTION;

// An actor the flames check against as they move: a flame stops beside it unless it is marked
// blocked, which sets the state's own blocked flag.
typedef struct FLAME_STOP
{
	Uint16 id; // the actor's id; 0 ends the list
	Uint16 blocked;
} FLAME_STOP;

// The flame throwers' state: the hit counter, what a flame met, the flame throwers and the actors
// they stop at, and the fireball carrying the leader.
typedef struct FLAME_HIT_STATE
{
	Sint32               count;  // scFlameThrowersAdvanceCounter's count
	Sint16               active; // set once a flame or fireball has hit the leader
	// Set when a flame met a blocked slot's actor, and called back once for.
	Sint16               blocked;
	Sint16               wasBlocked; // blocked as it was the tick before
	Uint16               hitCue;     // the step cue raised when the leader is hit
	// the kept flame's VRAM cache entry (scFlameThrowersCreate)
	Uint16               flameVramBlock;
	// scFlameThrowersSetMode: the flames are not paused by Psynergy
	Sint16               runsDuringPsynergy;
	void                 (*onBlocked)(void);  // called once a flame meets its stop
	FLAME_THROWER        entries[4];
	FLAME_STOP           slots[4];
	Uint8                unusedAfterSlots[4]; // no code names it
	KNOCK_BACK_DIRECTION direction;
	struct ACTOR        *car;                 // the fireball carrying the hurt leader
	// the flame whose sprite the crumbs share (scFlameThrowersCreate)
	void                *sharedFlame;
} FLAME_HIT_STATE;

// The step cues the flame throwers raise: a flame that hits the leader raises the cue
// scFlameThrowersStart is given, a rolling fireball that reaches him two more.
enum
{
	STEP_CUE_FLAME_HIT    = 88,
	STEP_CUE_FIREBALL_HIT = STEP_CUE_FLAME_HIT + 2
};

// The flame-hit state, in the inventory snapshot's spare space.
#define FLAME_THROWERS_STATE          ((FLAME_HIT_STATE *)(InventorySnapshot + INVENTORY_SNAPSHOT_SPARE))

// The knock-back (shared/leader_knock_back.c) as the four Mars Lighthouse rooms that keep a copy
// name it, and what it calls: the fall damage the leader takes, the update hook that blinks him
// while he is thrown, his animation, the terrain of his metatile, and the collision check a step
// ahead.
#define KNOCK_BACK                    fdLeaderKnockBack
#define KNOCK_BACK_TAKE_DAMAGE        gmPartyTakeFallDamage
#define KNOCK_BACK_BLINK              fdActorBlink
#define KNOCK_BACK_SET_ANIMATION      fdActorSelectAnimation
#define KNOCK_BACK_METATILE_ATTRIBUTE fdMapGridGetTerrainKindOrBumpAt
#define KNOCK_BACK_CHECK_COLLISION    fdMapGridCheckCollision

#endif
