#ifndef ALCHEMY_FIELD_PSYNERGY_STATE_H
#define ALCHEMY_FIELD_PSYNERGY_STATE_H

/* FIELD_EFFECT_STATE, the state of a field Psynergy. */

#include "fixed_math.h"
#include "static_assert.h"
#include "types.h"
#include "field/field_particle.h"

struct SPR;
struct ACTOR;

/* The state of a field Psynergy: its caster and target, where it takes place,
   the map state it restores and its 24 field effect sprites. */
typedef struct FIELD_EFFECT_STATE {
	Sint32		  angle;	/* 1/65536 turns */
	FIXED		  x;		/* 16.16 fixed-point pixels */
	FIXED		  y;		/* 16.16 fixed-point pixels */
	FIXED		  z;		/* 16.16 fixed-point pixels */
	struct ACTOR *caster;	/* the caster's actor */
	struct ACTOR *target;	/* the target's actor, or NULL */
	Sint16		  casterId; /* the caster's actor id */
	Sint16		  targetId; /* the target's actor id, -1 for none */
	Uint16		  ability;	/* the ability id */
	Sint16 fieldPsynergy;	/* the ability's field kind (FIELD_PSYNERGY_) */
	/* Set at the start: Force then aims in front of the caster; cleared once
	   the target has been dispatched. */
	Sint8 aimAhead;
	Uint8 mode; /* where it runs; ACTOR_SYSTEM_WORLD_MAP on the world map */
	Sint8 flashCaster;	/* set at the start: the caster flashes with a sound */
	Sint8 announceName; /* set at the start: the ability's name is announced */
	void (*finishCallback)(void); /* run, when set, as Ply ends */
	Uint8 unusedAfterFinish[12];
	/* Set by some scenes before a field Psynergy; Carry, Frost, Force and
	   Reveal treat the target differently with it. */
	Sint8 sceneVariant;
	/* set by some scenes: Frost and Force leave the target be */
	Sint8 leaveTarget;
	Uint8
		paddingAfterLeaveTarget[2]; /* up to the word savedCallback starts on */
	/* The target's update function and script, saved at the start and restored
	   when Move or Carry lets it go. */
	void (*savedCallback)(struct ACTOR *actor);
	const Sint32 *savedScript;
	Uint8		  unusedAfterSavedScript[4];
	Uint8 savedPalette; /* the target's first sprite's param, saved likewise */
	Uint8
		/* set in two Mercury Lighthouse scenes: Douse lets particles sway
		   down */
		   lit;
	Sint16 tileSlot; /* resource entry holding the spark tiles */
	Uint16 unusedAfterTileSlot;
	Uint16 freeBlocks; /* 512 less the free resource blocks at the start */
	/* The map's shake and its decay, restored when the field Psynergy ends. */
	Sint32		   savedShakeX;
	Sint32		   savedShakeY;
	Sint32		   savedShakeDecay;
	FIELD_PARTICLE particles[24];
	Uint8		   unusedAfterSlots[2];
	Sint16 arcTileSlot; /* resource entry of Avoid's falling arcs' tiles */
	Sint8  msgMode;		/* the announcement's box mode: 1, a script adds 8 */
	Uint8  paddingAtEnd[3]; /* up to the struct's word-aligned size */
} FIELD_EFFECT_STATE;

STATIC_ASSERT(sizeof(FIELD_EFFECT_STATE) == 1824);
STATIC_ASSERT(offsetof(FIELD_EFFECT_STATE, caster) == 16);
STATIC_ASSERT(offsetof(FIELD_EFFECT_STATE, ability) == 28);
STATIC_ASSERT(offsetof(FIELD_EFFECT_STATE, savedCallback) == 56);
STATIC_ASSERT(offsetof(FIELD_EFFECT_STATE, tileSlot) == 70);
STATIC_ASSERT(offsetof(FIELD_EFFECT_STATE, savedShakeX) == 76);
STATIC_ASSERT(offsetof(FIELD_EFFECT_STATE, particles) == 88);
STATIC_ASSERT(offsetof(FIELD_EFFECT_STATE, arcTileSlot) == 1818);
STATIC_ASSERT(offsetof(FIELD_EFFECT_STATE, msgMode) == 1820);

/* Returns the value step tenths of the way from `from` to `to`; the field
   Psynergy that uses it steps from 0 to 10, one a tick. */
static __inline__ Sint32 fdEffectInterpolate(
	Sint32 to, Sint32 from, Sint32 step)
{
	return from + (to - from) * step / 10;
}

#endif
