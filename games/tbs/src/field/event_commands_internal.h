#ifndef ALCHEMY_EVENT_COMMANDS_INTERNAL_H
#define ALCHEMY_EVENT_COMMANDS_INTERNAL_H

/* @file */
/* What every part of event_commands.c shares: its includes and the macros */
/* and types more than one part uses. event_commands.c includes it before the */
/* parts, and each part includes it too, so that a part read on its own */
/* resolves. It places no bytes. */

#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "macros.h"
#include "resource_ids.h"
#include "battle/battle.h"
#include "field/event.h"
#include "field/field_blend.h"
#include "field/field_effect_scripts.h"
#include "field/field_psynergy_state.h"
#include "field/ground_mask.h"
#include "field/line_scroll.h"
#include "field/world_map_lines.h"
#include "graphics/display_transition.h"
#include "system/sprite_metadata.h"
#include "text/msg_ids.h"

/* Sends an actor toward the point magnitude (16.16 pixels) away from it in */
/* direction angle (actor_polar_target.h). */
void fdActorSetTargetPosFromMagnitudeAng(ACTOR *actor, s32 magnitude,
										 s32 angle);

/* The field effect sprites of a field Psynergy (FIELD_EFFECT_STATE.particles).
 */
#define PSYNERGY_EFFECT_SLOT_COUNT 24

/* A pending Djinni battle's event (MainRoutineStateWork.pendingDjinni, */
/* event_commands_requests.c, event_commands_djinni.c): this plus the */
/* Djinni's id (DJINN_PER_ELEMENT to an element), with DJINN_NO_HOP beside it.
 */
#define DJINNI_BATTLE_EVENT_FIRST 300

/* MainRoutineStateWork.pendingDjinni's parts, as fdDjinnResolvePending reads */
/* them. bits: 15..12 kind (PENDING_BATTLE_KIND) | 11..0 event or flag. */
#define DJINN_PENDING_KIND_MASK 0xf000
/* What waits in MainRoutineStateWork.pendingDjinni through a battle. */
typedef enum PENDING_BATTLE_KIND {
	/* A Djinni battle, by its DJINNI_BATTLE_EVENT_FIRST event. */
	PENDING_DJINNI_BATTLE = 0,
	/* A mimic chest's battle, by the chest's flag, which the field sequence */
	/* stores with CONDITION_FLAG_SET (field_sequence_triggers.c). */
	PENDING_MIMIC_CHEST = CONDITION_FLAG_SET
} PENDING_BATTLE_KIND;
#define DJINN_PENDING_EVENT_MASK 0xfff
/* A Djinni battle's event without DJINN_NO_HOP. */
#define DJINN_PENDING_BATTLE_EVENT_MASK 0x7ff

/* The steps, one a frame, in which an object grows out of the caster on its */
/* way to the target or shrinks back into it: fdEffectInterpolate's tenths. */
#define GROW_STEPS 10

/* The trailing copies of a moving Djinni or Force's hand are drawn a little */
/* smaller than what they follow. */
#define AFTERIMAGE_SCALE (FIXED_ONE * 15 / 16)

/* Per-frame update of an afterimage: closes half the distance to the actor */
/* it watches, on all three axes. */
static inline void fdEffectMoveAfterimageHalfway(ACTOR *afterimage) {
	ACTOR *target = (ACTOR *)afterimage->watchedActor;

	afterimage->x.fixed += (target->x.fixed - afterimage->x.fixed) / 2;
	afterimage->y.fixed += (target->y.fixed - afterimage->y.fixed) / 2;
	afterimage->z.fixed += (target->z.fixed - afterimage->z.fixed) / 2;
}

/* Runs the update of each of the 24 field effect sprites. */
static __inline__ void fdEffectUpdateEffectSlots(void) {
	FIELD_PARTICLE *particles =
		((FIELD_EFFECT_STATE *)FieldEffectStateWork)->particles;
	s32 i;

	for(i = 0; i < PSYNERGY_EFFECT_SLOT_COUNT; i++)
		fdParticleUpdate(&particles[i]);
}

/* Defines a per-frame update that spins an actor 1/8 turn a frame while its */
/* scale changes by step, releasing the actor once the scale falls below */
/* FIXED_ONE * 3 / 16. step is negative: from full size the actor */
/* lasts (FIXED_ONE * 13 / 16) / -step frames, rounded down, plus one. */
#define DEFINE_SHRINK_AND_DESTROY(name, step)                                  \
	void name(ACTOR *actor) {                                                  \
		s32 scale;                                                             \
                                                                               \
		scale = actor->scaleX + step;                                          \
		actor->scaleY += step;                                                 \
		actor->facing += DEGtoANG(45);                                         \
		actor->scaleX = scale;                                                 \
		if(scale < FIXED_ONE * 3 / 16) {                                       \
			fdActorReleaseStub(actor);                                         \
		}                                                                      \
	}

/* A particle's sprite (SPR), seen as the two fields Douse and Mind */
/* Read set on it; each member says which Sprite fields it covers. */
typedef struct WAVE_EFFECT_SPR {
	u32 linkAndPos[2];	 /* image.link and its first two attributes */
	u8 tileLow;			 /* image.tile's low byte */
	u32 tileHigh : 2;	 /* image.tile's high bits */
	u32 pri : 2;		 /* image.pri */
	u32 palBits : 4;	 /* image.pal */
	u8 attr3ToDirty[28]; /* image.attr3 to the sprite's dirty flag */
	u8 flg;				 /* 0 drops shadow and outline */
} WAVE_EFFECT_SPR;

/* Move's hand and Carry shed the same short-lived falling particle: it falls */
/* under a gravity of 0.1 pixel a frame, each frame, and is freed after 12 */
/* frames. */
static inline void fdEffectInitFallingFragment(ACTOR *fragment) {
	/* The caller spawns the fragment and places it. */
	if(fragment != NULL) {
		fragment->motionFlg = ACTOR_FALLS;
		fragment->gravity = toFIXED(1) / 10;
		fdActorSelectAnimationStub(fragment, 0);
		fragment->wait = 12;
		fdActorStartScriptStub(fragment, FieldEffect_SharedParticleScript);
	}
}

/* The sparkles DEFINE_CLAPPING_HANDS_CAST's casts scatter. */
#define CLAP_SPARKLE_COUNT 24

/* Defines Lift's or Carry's cast with no target: spawn makes a hand 16 pixels
 */
/* above the cast's position and 32 pixels to either side, each facing the */
/* middle; the two move 24 pixels inward and clap, and 24 sparkles scatter 8 to
 */
/* 32 pixels. */
#define DEFINE_CLAPPING_HANDS_CAST(name, spawn)                                \
	void name(void) {                                                          \
		FIELD_EFFECT_STATE *effectState;                                       \
		ACTOR *obj;                                                            \
		ACTOR *pair[2];                                                        \
		VECTOR3 pos;                                                           \
		ACTOR **walk;                                                          \
		s32 n;                                                                 \
		s32 dist;                                                              \
                                                                               \
		effectState = FieldEffectStateWork;                                    \
		fdPsynergyFadeToCastCol();                                             \
		pos.x = effectState->x;                                                \
		pos.y = effectState->y + toFIXED(16);                                  \
		pos.z = effectState->z;                                                \
		pair[0] = spawn(pos.x + toFIXED(32), pos.y, pos.z, FACING_WEST);       \
		pair[1] = spawn(pos.x - toFIXED(32), pos.y, pos.z, FACING_EAST);       \
                                                                               \
		sySynch(15);                                                           \
		walk = pair;                                                           \
		for(n = 0; n < 2; n++) {                                               \
			obj = *walk++;                                                     \
			if(obj != 0)                                                       \
				fdActorSetTargetPosFromMagnitudeAng(obj, toFIXED(24),          \
													obj->facing);              \
		}                                                                      \
                                                                               \
		fdActorWaitForMoveStub(pair[0]);                                       \
		sySndPlayStub(SE_HEAVY_IMPACT);                                        \
		for(n = 0; n < CLAP_SPARKLE_COUNT; n++) {                              \
			pos.x = effectState->x;                                            \
			pos.y = effectState->y + toFIXED(16);                              \
			pos.z = effectState->z;                                            \
			obj = fdActorSpawn(SPRITE_SPARKLE, pos.x, pos.y, pos.z);           \
			if(obj != 0) {                                                     \
				fdActorStartScriptStub(obj, FieldEffect_FragmentScript);       \
				obj->speedLimit = syRandom() + toFIXED(2);                     \
				obj->acceleration = toFIXED(2);                                \
				obj->motionFlg = 0;                                            \
				dist = syRandom() * 24 + toFIXED(8);                           \
				fdActorSetTargetPosFromMagnitudeAng(obj, dist, syRandom());    \
			}                                                                  \
		}                                                                      \
                                                                               \
		fdActorReleaseStub(pair[0]);                                           \
		fdActorReleaseStub(pair[1]);                                           \
		fdPsynergyFadeFromCastCol();                                           \
	}

#define PSYNERGY_EFFECT_HEIGHT toFIXED(16) /* 16 pixels up */

/* The states of a particle that flies out and back: Halt's, Force's and Move's
 */
/* (fdEffectUpdateHaltParticle, fdEffectUpdateRadialBurst, */
/* fdEffectUpdateOutAndBack). */
typedef enum OUT_AND_BACK_STATE {
	OUT_AND_BACK_AIM_OUT = 0,	 /* picks the point it flies out to */
	OUT_AND_BACK_FLYING_OUT = 1, /* waits to arrive there */
	OUT_AND_BACK_AIM_BACK = 2,	 /* picks the point it comes back to */
	OUT_AND_BACK_FLYING_BACK = 3 /* frees its slot once it arrives */
} OUT_AND_BACK_STATE;

/* The states of a Venus Djinni's rising particle */
/* (fdEffectUpdateRadialSpread), which fdDjinnRunVenusJoinEffect */
/* moves on to VENUS_PARTICLE_LAST_RISE. */
typedef enum VENUS_PARTICLE_STATE {
	VENUS_PARTICLE_AIM = 0, /* starts round the leader, aimed up the screen */
	VENUS_PARTICLE_RISING = 1, /* back to VENUS_PARTICLE_AIM once it arrives */
	VENUS_PARTICLE_LAST_RISE = 2 /* freed once it arrives */
} VENUS_PARTICLE_STATE;

/* The states of a Mercury Djinni's droplet */
/* (fdEffectUpdateRandomTargetParticle), which */
/* fdDjinnRunMercuryJoinEffect moves on to MERCURY_DROPLET_LAST_FALL. */
typedef enum MERCURY_DROPLET_STATE {
	MERCURY_DROPLET_AIM =
		0, /* placed above the leader, aimed at a spot by it */
	/* Splashes where it lands, then falls again (MERCURY_DROPLET_AIM). */
	MERCURY_DROPLET_FALLING = 1,
	/* Splashes where it lands, for the last time. */
	MERCURY_DROPLET_LAST_FALL = 2,
	MERCURY_DROPLET_DONE = 3 /* frees its slot */
} MERCURY_DROPLET_STATE;

/* The states of a Mars Djinni's spark */
/* (fdEffectUpdateMarsSpark), which fdDjinnRunMarsJoinEffect */
/* moves on to MARS_SPARK_STOPPING and then MARS_SPARK_LEAVING. */
typedef enum MARS_SPARK_STATE {
	MARS_SPARK_AIM = 0,	   /* aimed 120 pixels up the screen from its origin */
	MARS_SPARK_RISING = 1, /* back to MARS_SPARK_AIM once it arrives */
	/* Stops where it arrives, hidden, and waits 40 frames. */
	MARS_SPARK_STOPPING = 2,
	MARS_SPARK_AIM_LEADER = 3, /* aimed at a point by the leader */
	MARS_SPARK_SWARMING = 4,   /* back to MARS_SPARK_AIM_LEADER on arrival */
	MARS_SPARK_LEAVING = 5	   /* freed once it arrives */
} MARS_SPARK_STATE;

#endif
