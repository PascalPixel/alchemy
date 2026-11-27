#ifndef ALCHEMY_FIELD_PSYNERGY_STATE_H
#define ALCHEMY_FIELD_PSYNERGY_STATE_H

/* The field Psynergy's state (FIELD_EFFECT_STATE) and its particle. */

#include "fixed_math.h"
#include "types.h"
#include "static_assert.h"
#include "field_particle.h"
#include "field/effect_sprite.h"

struct SPR;
union FIELD_OBJECT;

/* The tick a scene's Psynergy script is given once the cast has ended
   (fdRunPsynergyScript, as Burst passes it): its last call. */
#define PSYNERGY_SCRIPT_LAST_TICK 999

/* A particle of a field Psynergy's effect: its place and velocity, in 16.16
   pixels on the field or on the screen, and the ticks since it began (negative
   before it starts, or while it is free). */
typedef struct FIELD_PSYNERGY_PARTICLE {
	FIXED  x;
	FIXED  y;
	FIXED  z;
	FIXED  velX;
	FIXED  velY;
	FIXED  velZ;
	Sint32 tmr;
} FIELD_PSYNERGY_PARTICLE;

/* The effect sprites a field Psynergy has (FIELD_EFFECT_STATE.particles). */
#define PSYNERGY_EFFECT_SLOT_COUNT 24
/* The tick of Scoop's dig at which the ground is dug, its stage 1
   (fdPsynergyScoop). */
#define SCOOP_DUG_TICK			   30

/* The particles of dust and sparkles Scoop throws up. */
#define SCOOP_PARTICLE_COUNT 64
/* The earth that rises as the ground is dug, 32 pixels square in 256
   colours. */
#define SCOOP_PICTURE_SIZE	 32

/* Scoop's state (heap slot HEAP_SLOT_BATTLE_EFFECT): the dust and sparkle
   tiles, the picture that ripples as the ground is dug, 64 particles, the
   sprite that digs and the one left where it dug, and the stage of the cast. */
typedef struct SCOOP_STATE {
	Uint8					tiles[1024];
	Uint8					picture[SCOOP_PICTURE_SIZE * SCOOP_PICTURE_SIZE];
	EFFECT_SPR				sprs[SCOOP_PARTICLE_COUNT];
	FIELD_PSYNERGY_PARTICLE particles[SCOOP_PARTICLE_COUNT];
	EFFECT_SPR				pictureSpr;
	/* the stage of the cast; FIELD_PSYNERGY_FINISHED once it ends */
	Sint16 phase;
	Sint16 tmr; /* ticks into the stage */
	Sint16 particleTile;
	Sint16 pictureTile;
	/* SPRITE_SCOOP_HAND, which digs and sinks as it does */
	struct ACTOR *actor;
	/* SPRITE_BROWN_BOULDER (grey where lit), where it digs */
	struct ACTOR *other;
	/* The kind of dig: 2 cast on a target; cast at nothing, 1 one time in
	   sixteen (an item turns up), else 0, or 3 where lit is set. */
	Sint16 mode;
} SCOOP_STATE;

/* The state of a field Psynergy (heap slot HEAP_SLOT_FIELD_EFFECT): its caster
   and target, where it takes place, the map state it restores, its field effect
   sprites and the sprite and stage a lasting Psynergy keeps. */
typedef struct FIELD_EFFECT_STATE {
	/* The caster's facing, rounded to four directions, 1/65536 turns. */
	Uint16 ang;
	/* The caster's facing, rounded to eight directions, 1/65536 turns. */
	Uint16		  ang8;
	FIXED		  x;			 /* 16.16 fixed-point pixels */
	FIXED		  y;			 /* 16.16 fixed-point pixels */
	FIXED		  z;			 /* 16.16 fixed-point pixels */
	struct ACTOR *caster;		 /* the caster's actor */
	struct ACTOR *tgt;			 /* the target's actor, or NULL */
	Sint16		  casterId;		 /* the caster's actor id */
	Sint16		  tgtId;		 /* the target's actor id, -1 for none */
	Sint16		  ability;		 /* the ability id */
	Sint16		  fieldPsynergy; /* the ability's field Psynergy kind */
	/* Set at the start; cleared once the target has been dispatched. */
	Sint8 aimAhead;
	Uint8 mode; /* where it runs; ACTOR_SYSTEM_WORLD_MAP on the World Map */
	Sint8 flashCaster;	/* set at the start: the caster flashes with a sound */
	Sint8 announceName; /* set at the start: the ability's name is announced */
	void (*finishFunc)(void); /* run, when set, as the Psynergy ends */
	Uint8 unusedAfterFinish[12];
	/* Set by some scenes before a field Psynergy; it treats the target
	   differently with it. */
	Sint8 sceneVariant;
	Sint8 childMode; /* set by some scenes: the Psynergy leaves the target be */
	Uint8 paddingAfterChildMode[2];
	/* The target's update function and script, saved at the start and restored
	   when the Psynergy lets it go. */
	void (*savedFunc)(union FIELD_OBJECT *object);
	const Sint32 *savedScript;
	Uint8 savedPal; /* the target's first sprite animation's colour mode */
	/* Set in a few scenes (Gabomba Catacombs, Tundaria Tower, Jupiter
	   Lighthouse's aerie, Loho) before a field Psynergy. */
	Uint8  lit;
	Uint16 freeBlocks; /* 512 less the free VRAM cache blocks at the start */
	/* The map's shake and its decay, restored when the field Psynergy ends. */
	Sint32 savedShakeX;
	Sint32 savedShakeY;
	Sint32 savedShakeDecay;
	/* Its field effect sprites. */
	FIELD_PARTICLE particles[PSYNERGY_EFFECT_SLOT_COUNT];
	Sint16 arcTileSlot; /* the VRAM cache entry of the falling arcs' tiles */
	Sint8  msgMode;		/* the announcement's box mode: 1, a script adds 8 */
	Uint8  msgShown;	/* set while the announcement's message is open */
	Uint8  unusedAfterMsgShown[4]; /* no code names it */
	/* The caster's priority flags and sprite priority, saved at the start and
	   restored as it ends. */
	Uint8 savedPriFlg;
	Uint8 savedSprPri;
	Uint8 unusedAfterSavedSprPri[170]; /* no code names it */
	/* A lasting Psynergy's stage and tick, passed to its script as it runs. */
	Sint16	   stage;
	Sint16	   frm;
	EFFECT_SPR spr;			/* a field Psynergy's own sprite on screen */
	Sint16	   sprVramSlot; /* its VRAM cache entry */
	Sint16	   sprActive;	/* nonzero while sprite is drawn */
} FIELD_EFFECT_STATE;

STATIC_ASSERT(sizeof(FIELD_EFFECT_STATE) == 2036);
STATIC_ASSERT(offsetof(FIELD_EFFECT_STATE, caster) == 16);
STATIC_ASSERT(offsetof(FIELD_EFFECT_STATE, ability) == 28);
STATIC_ASSERT(offsetof(FIELD_EFFECT_STATE, sceneVariant) == 52);
STATIC_ASSERT(offsetof(FIELD_EFFECT_STATE, savedFunc) == 56);
STATIC_ASSERT(offsetof(FIELD_EFFECT_STATE, lit) == 65);
STATIC_ASSERT(offsetof(FIELD_EFFECT_STATE, savedShakeX) == 68);
STATIC_ASSERT(offsetof(FIELD_EFFECT_STATE, particles) == 80);
STATIC_ASSERT(offsetof(FIELD_EFFECT_STATE, msgMode) == 1810);
STATIC_ASSERT(offsetof(FIELD_EFFECT_STATE, savedPriFlg) == 1816);
STATIC_ASSERT(offsetof(FIELD_EFFECT_STATE, stage) == 1988);
STATIC_ASSERT(offsetof(FIELD_EFFECT_STATE, spr) == 1992);
STATIC_ASSERT(offsetof(FIELD_EFFECT_STATE, sprActive) == 2034);

/* Returns the value step tenths of the way from `from` to `to`; the field
   Psynergy that uses it steps from 0 to 10, one a tick. */
static __inline__ Sint32 fdEffectInterpolate(
	Sint32 to, Sint32 from, Sint32 step)
{
	return from + (to - from) * step / 10;
}

#endif
