/* @file */
/* The field Psynergy that spread effects around their target: Whirlwind, */
/* Halt, Catch, Lash, Retreat and Avoid, with the particle rings and arcs */
/* they share. */
/* Part of field_psynergy.c. */

#include "field/stubs.h"
#include "macros.h"

#include "field/object.h"
#include "field/field_psynergy.h"
#include "field/event.h"
#include "window/stubs.h"
#include "text/msg_ids.h"
#include "window/window.h"
#include "item_ids.h"

extern const s32 FieldEffect_ReleaseScript[];

/* Whirlwind: its leaves, their circling and wandering, the target's shrinking
 */
/* and the orb's flight, in frames. */
enum {
	WHIRLWIND_LEAF_COUNT = 16,
	WHIRLWIND_ORBIT_RADIUS = toFIXED(8),
	WHIRLWIND_ORBIT_ANGLE_STEP = TRIG_TURN / 32,
	WHIRLWIND_ORBIT_FRAMES = 121,
	WHIRLWIND_WANDER_FRAMES = 101,
	WHIRLWIND_TARGET_SCALE_STEP = FIXED_ONE / 512,
	WHIRLWIND_TARGET_MIN_SCALE = FIXED_ONE / 2,
	WHIRLWIND_TRAVEL_FRAMES = 10
};

/* The radial ring's particles, a 16th of a turn apart; the last goes where the
 */
/* first went. */
#define RADIAL_RING_STEPS 16

/* Spawns 17 particles that spread from origin in a flat ring, their x velocity
 */
/* 1.5 times their z velocity, so the ring is wider than deep as the field is */
/* drawn. */
void fdEffectSpawnRadialParticleRing(MOTION_OBJECT *origin) {
	EFFECT_OPTIONS options;
	FIXED velocity[3];
	FIXED sine;
	s32 angle;
	u32 i;

	options.pal = 0;
	options.update = fdEffectAdvanceMotion;
	options.startScaleX = FIXED_ONE * 4 / 5;
	options.startScaleY = FIXED_ONE * 4 / 5;
	for(i = 0; i <= RADIAL_RING_STEPS; i++) {
		angle = i * (TRIG_TURN / RADIAL_RING_STEPS);
		velocity[0] = syCos(angle) * 3 / 2;
		velocity[1] = 0;
		sine = sySin(angle);
		velocity[2] = sine;
		fdEffectSpawnResident(
			origin->x, origin->y, origin->z, velocity[0], velocity[1], sine,
			EFFECT_USE_UPDATE | EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE | 1,
			&options);
	}
}

/* Per-frame update: wanders away from the anchor, 2 to 3 pixels a frame, now */
/* and then turning sharply; after 101 frames it hands over to the shared */
/* particle script. */
void fdEffectWanderAroundAnchor(ACTOR *effect) {
	FIXED radius;
	s32 angle;
	FIXED dx;
	FIXED dz;

	radius = syRandom() + toFIXED(2);
	angle = effect->facing;
	dx = syMulFX(radius, syCos(angle));
	dz = syMulFX(radius, sySin(angle));
	effect->x.fixed += dx;
	effect->z.fixed += dz;
	/* A small backwards drift, a 4096th of a turn a frame. */
	effect->facing += -(TRIG_TURN / 4096);
	if(effect->turnFrm != 0) {
		effect->turnFrm--;
		effect->facing += TRIG_TURN / 32;
	}
	else if(syRandomBelow(32) == 0) {			 /* one chance in 32 */
		effect->turnFrm = syRandomBelow(16) + 8; /* 8 to 23 frames */
	}
	if(++effect->swirlFrm == WHIRLWIND_WANDER_FRAMES)
		fdActorStartScriptStub(effect, FieldEffect_ReleaseScript);
}

/* Per-frame update of a Whirlwind particle: circles the anchor at 8 pixels, */
/* a 32nd of a turn a frame; after 121 frames it starts to wander */
/* (fdEffectWanderAroundAnchor) with upward speed and gravity. As The */
/* Broken Seal's fdEffectCircleAnchor. */
void fdEffectCircleAnchor(ACTOR *effect) {
	ACTOR *anchor;
	s32 angle;
	FIXED radius;
	FIXED dx;
	FIXED dz;

	anchor = &effect->watchedActor->actor;
	radius = WHIRLWIND_ORBIT_RADIUS;
	angle = effect->facing;
	dx = syMulFX(radius, syCos(angle));
	dz = syMulFX(radius, sySin(angle));
	effect->x.fixed = anchor->x.fixed + dx;
	effect->z.fixed = anchor->z.fixed + dz;
	effect->facing += WHIRLWIND_ORBIT_ANGLE_STEP;
	if(++effect->swirlFrm == WHIRLWIND_ORBIT_FRAMES) {
		effect->update = (OBJECT_UPDATE)fdEffectWanderAroundAnchor;
		effect->swirlFrm = 0;
		effect->turnFrm = 0;
		effect->gravity = toFIXED(1) / 10;
		effect->velY = toFIXED(3);
		effect->facing = syRandom();
	}
}

/* Per-frame update of Whirlwind's target: it shrinks a 512th a frame and, */
/* below half its size, is put away at the map's origin. */
void fdWhirlwindUpdateTarget(ACTOR *actor) {
	s32 scale;

	scale = actor->scaleX - WHIRLWIND_TARGET_SCALE_STEP;
	actor->scaleY = scale;
	actor->scaleX = scale;
	if(scale < WHIRLWIND_TARGET_MIN_SCALE) {
		fdActorSetPosAndResetMotionStub(actor, 0, 0, 0);
		actor->update = 0;
	}
}

/* Runs Whirlwind cast on a target, using the target already stored in the */
/* field effect. */
void fdPsynergyWhirlwindOnTarget(s32 unused) { fdPsynergyWhirlwind(); }

void fdWhirlwindUpdateSpentOrb(ACTOR *actor);

/* Whirlwind: a whirlwind orb flies from the caster to the spot over ten */
/* frames, growing from a quarter size. With nothing to blow it whooshes away;
 */
/* on a target, leaves circle it while the target shrinks away; on sprouting */
/* leaves the leaves scatter, and there or on a seedling the vine swings */
/* (fdHangingVineSwing). */
void fdPsynergyWhirlwind(void) {
	FIELD_EFFECT_STATE *scene = Ram_HeapSlots->fieldEffState;
	ACTOR *caster = scene->caster;
	ACTOR *target = scene->tgt;
	SPR *sprite = target->spr;
	VECTOR3 position;
	VECTOR3 from;
	VECTOR3 to;
	ACTOR *orb;
	ACTOR *leaf;
	s32 kind = ORB_PLAIN;
	s32 step;
	s32 i;
	s32 frames = WHIRLWIND_TRAVEL_FRAMES + 1;

	if(scene->sceneVariant != 0)
		return;
	if(scene->aimAhead == 0) {
		s32 animation;

		kind = ORB_ORBITING;
		animation = sprite->entries[0]->anmId;
		if(animation == SPRITE_SPROUTING_LEAVES)
			kind = ORB_WANDER;
		if(animation == SPRITE_SEEDLING)
			kind = ORB_HANDOFF;
	}

	SET_POSITION(&from, caster->x.fixed, caster->y.fixed, caster->z.fixed);
	SET_POSITION(&to, scene->x, scene->y - toFIXED(4), scene->z);
	orb = fdActorSpawn(SPRITE_WHIRLWIND, 0, 0, 0);
	if(orb == 0)
		return;

	fdPsynergyFadeToCastCol();
	fdActorSelectAnimationStub(orb, 2);
	step = 0;
	do {
		FIXED scale;

		SET_ACTOR_POSITION(
			orb, from.x + (to.x - from.x) * step / WHIRLWIND_TRAVEL_FRAMES,
			from.y + (to.y - from.y) * step / WHIRLWIND_TRAVEL_FRAMES,
			from.z + (to.z - from.z) * step / WHIRLWIND_TRAVEL_FRAMES);
		scale = step * (FIXED_ONE * 21 / 20) / WHIRLWIND_TRAVEL_FRAMES +
				FIXED_ONE / 4; /* from 0.25 to 1.3 */
		orb->scaleX = scale;
		orb->scaleY = scale;
		sySynch(1);
		step++;
	} while(step < frames);
	orb->scaleX = FIXED_ONE * 17 / 10;
	orb->scaleY = FIXED_ONE * 13 / 10;
	sySndPlayStub(SE_RUSH);

	if(kind == ORB_PLAIN) {
		fdActorSelectAnimationStub(orb, 1);
		sySynch(30);
		sySndPlayStub(SE_WHOOSH);
		fdActorWaitForTrackEndStub(orb);
		fdActorReleaseStub(orb);
	}
	else if(kind == ORB_ORBITING) {
		/* Sixteen leaves, one every six frames, 4 pixels above the orb and 0.8
		 */
		/* higher each, 3 to 8 pixels out, circle it while the target shrinks.
		 */
		sySynch(20);
		if(target != 0)
			target->update = (OBJECT_UPDATE)fdWhirlwindUpdateTarget;
		for(i = 0; i < WHIRLWIND_LEAF_COUNT; i++) {
			s32 distance;

			SET_POSITION(&position, orb->x.fixed,
						 orb->y.fixed + i * (toFIXED(4) / 5) + toFIXED(4),
						 orb->z.fixed);
			distance = syRandom() * 5 + toFIXED(3);
			syVectorAddPolarOffset(distance, syRandom(), &position);
			leaf =
				fdActorSpawn(SPRITE_LEAF, position.x, position.y, position.z);
			if(leaf != 0) {
				leaf->update = (OBJECT_UPDATE)fdEffectCircleAnchor;
				leaf->watchedActor = (FIELD_OBJECT *)orb;
				leaf->swirlFrm = 0;
				leaf->turnFrm = 0;
				leaf->facing = syRandom();
			}
			sySynch(6);
		}
		sySynch(20);
		sySynch(120);
		fdActorSelectAnimationStub(orb, 1);
		sySynch(30);
		sySndPlayStub(SE_WHOOSH);
		fdActorWaitForTrackEndStub(orb);
		fdActorReleaseStub(orb);
	}
	else if(kind == ORB_WANDER || kind == ORB_HANDOFF) {
		sySynch(5);
		fdActorSelectAnimationStub(orb, 1);
		if(kind == ORB_WANDER) {
			/* Sixteen leaves that wander off the orb, the first handed to the
			 * shared */
			/* particle script soonest. */
			for(i = 0; i < WHIRLWIND_LEAF_COUNT; i++) {
				s32 distance;

				SET_POSITION(&position, orb->x.fixed,
							 orb->y.fixed + i * (toFIXED(4) / 5) + toFIXED(4),
							 orb->z.fixed);
				distance = syRandom() * 5;
				distance += toFIXED(3);
				syVectorAddPolarOffset(distance, syRandom(), &position);
				leaf = fdActorSpawn(SPRITE_LEAF, position.x, position.y,
									position.z);
				if(leaf != 0) {
					leaf->update = (OBJECT_UPDATE)fdEffectWanderAroundAnchor;
					leaf->swirlFrm = 20 + i * -2;
					leaf->turnFrm = 0;
					leaf->gravity = toFIXED(1) / 10;
					leaf->velY = toFIXED(3);
					leaf->facing = syRandom();
				}
			}
		}
		fdBattleEffectSetObjectUpdateMode(scene->caster, CASTER_LOOK_NORMAL);
		fdPsynergyFadeFromCastCol();
		orb->tmr = 0;
		orb->update = (OBJECT_UPDATE)fdWhirlwindUpdateSpentOrb;
		fdHangingVineSwing(scene->tgtId);
		fdActorWaitForTrackEndStub(orb);
		fdActorReleaseStub(orb);
	}
	fdPsynergyFadeFromCastCol();
}

/* Per-frame update of Whirlwind's orb once it has done its work: it whooshes */
/* on its 80th frame. */
void fdWhirlwindUpdateSpentOrb(ACTOR *actor) {
	if(actor->tmr == 80) {
		actor->update = 0;
		sySndPlayStub(SE_WHOOSH);
	}
	actor->tmr++;
}

/* Field effect sprite callback (OUT_AND_BACK_STATE): the particle heads for
 */
/* a point 40 to 70 pixels from where it appeared, then for a point 4 pixels */
/* round 8 pixels above the Psynergy's position on screen, and is freed on */
/* arrival. */
void fdEffectUpdateHaltParticle(FIELD_PARTICLE *effect) {
	FIELD_EFFECT_STATE *effectState;
	VECTOR3 pos;
	s16 angle;
	s32 state;

	effectState = Ram_HeapSlots->fieldEffState;
top:
	state = effect->state;
	if(state == OUT_AND_BACK_AIM_OUT) {
		pos.x = effect->originX;
		pos.z = effect->originZ;
		angle = syRandom();
		syVectorAddPolarOffset(syRandom() * 30 + toFIXED(40), (u16)angle, &pos);
		effect->tgtX = pos.x;
		effect->tgtZ = pos.z;
		effect->acceleration = toFIXED(4);
		effect->speedMax = toFIXED(4);
		effect->turnStepLimited = state;
		effect->state++;
		return;
	}
	if(state == OUT_AND_BACK_FLYING_OUT) {
		if(fdParticleHasPendingTarget(effect) != 0)
			return;
		effect->state++;
		goto top;
	}
	if(state == OUT_AND_BACK_AIM_BACK) {
		pos.x = effectState->x;
		pos.y = effectState->y + toFIXED(8);
		pos.z = effectState->z;
		fdCameraWorldToScr(&pos);
		syVectorAddPolarOffset(toFIXED(4), syRandom(), &pos);
		effect->tgtX = pos.x;
		effect->tgtZ = pos.z;
		effect->turnStepMax = TRIG_TURN / 16;
		effect->turnStepLimited = 1;
		effect->state++;
		return;
	}
	if(state == OUT_AND_BACK_FLYING_BACK &&
	   fdParticleHasPendingTarget(effect) == 0)
		fdEffectClearOwnedSlot(effect);
}

/* Pauses the target (effectState->child) and runs fdPsynergyHalt. */
void fdPsynergyHaltOnTarget(s32 unused) {
	ACTOR *target = ((FIELD_EFFECT_STATE *)Ram_HeapSlots->fieldEffState)->tgt;

	target->paused = 1;
	fdPsynergyHalt();
}

/* A thrown stopwatch's or hand's steps, a tenth of the way a frame: it */
/* arrives on the 11th frame. */
#define CAST_THROW_STEPS 10
#define HALT_PARTICLES	 16

/* Halt: throws a stopwatch (SPRITE_STOPWATCH) from the caster to the */
/* Psynergy's position over CAST_THROW_STEPS frames, ticks three low tones */
/* and a high one, and scatters it into HALT_PARTICLES particles. */
void fdPsynergyHalt(void) {
	FIELD_EFFECT_STATE *effectState = Ram_HeapSlots->fieldEffState;
	ACTOR *caster = effectState->caster;
	ACTOR *orb;
	FIELD_PARTICLE *effect;
	VECTOR3 pos;
	VECTOR3 from;
	VECTOR3 to;
	s32 step;
	s32 i;

	effectState->y = caster->y.fixed;
	orb = fdActorSpawn(SPRITE_STOPWATCH, 0, 0, 0);
	step = 0;
	fdActorSelectAnimationStub(orb, 0);
	if(orb == 0)
		return;

	/* From 16 pixels above the caster to 8 pixels above the Psynergy's
	 * position. */
	fdPsynergyFadeToCastCol();
	from.x = caster->x.fixed;
	from.y = caster->y.fixed + toFIXED(16);
	from.z = caster->z.fixed;
	to.x = effectState->x;
	to.y = effectState->y + toFIXED(8);
	to.z = effectState->z;
	/* Grown to full size as it arrives. */
	for(;;) {
		s32 steps = CAST_THROW_STEPS + 1;
		s32 scale;

		SET_ACTOR_POSITION(orb,
						   from.x + (to.x - from.x) * step / CAST_THROW_STEPS,
						   from.y + (to.y - from.y) * step / CAST_THROW_STEPS,
						   from.z + (to.z - from.z) * step / CAST_THROW_STEPS);
		scale = fdEffectInterpolate(FIXED_ONE, FIXED_ONE / 4,
									step); /* from 0.25 to full */
		orb->scaleX = scale;
		orb->scaleY = scale;
		sySynch(1);
		step++;
		if(step >= steps)
			break;
	}

	/* Three low tones and a high one, 10 frames apart, as the orb changes to */
	/* its second animation. */
	sySynch(5);
	fdActorSelectAnimationStub(orb, 1);
	sySndPlayStub(SE_TRIPLE_TONE_LOW);
	sySynch(10);
	sySndPlayStub(SE_TRIPLE_TONE_LOW);
	sySynch(10);
	sySndPlayStub(SE_TRIPLE_TONE_LOW);
	sySynch(10);
	sySndPlayStub(SE_TRIPLE_TONE_HIGH);

	/* Particles 4 pixels round it fly out, then come back to the Psynergy's */
	/* position. */
	effect = &effectState->particles[0];
	for(i = 0; i < HALT_PARTICLES; i++) {
		pos.x = orb->x.fixed;
		pos.y = orb->y.fixed + toFIXED(8);
		pos.z = orb->z.fixed;
		fdCameraWorldToScr((VECTOR3 *)&pos.x);
		syVectorAddPolarOffset(toFIXED(4), syRandom(), &pos.x);
		fdParticleInit(effect, SPRITE_SPARKLE, pos.x, pos.z);
		fdParticleSetCallback(effect, fdEffectUpdateHaltParticle);
		fdSprSetAnimationColModesStub(effect->spr, SPRITE_COLOR_WHITE);
		effect++;
	}

	pos.x = orb->x.fixed;
	pos.y = orb->y.fixed + toFIXED(8);
	pos.z = orb->z.fixed;
	sySynch(8);
	fdActorReleaseStub(orb);
	sySynch(4);
	sySynch(30);
	fdPsynergyFadeFromCastCol();
}

/* Per-frame update of an actor held by Halt: its sprite flickers through the */
/* colour modes of Halt_FlickerColorModes at random. */
void fdPsynergyUpdateHaltedActor(ACTOR *actor) {
	const s8 *table = Halt_FlickerColorModes;
	s32 random = syRandom();
	fdActorSetSprColModeStub(actor, table[(u32)(random * 8) >> 16]);
}

/* Holds an actor still under Halt, keeping its per-frame update and its first
 */
/* animation's colour mode in MainRoutineStateWork for */
/* fdPsynergyResumeHaltedActor. */
void fdPsynergyHaltActor(s32 actorId) {
	ACTOR *actor;
	SPR_ANIMATION *entry;

	actor = fdActorLookup(actorId);
	if(actor != NULL) {
		MainRoutineStateWork.pausedUpdate = actor->update;
		MainRoutineStateWork.pausedColMode = 0;
		if(actor->sprStorageKind == ACTOR_SPRITE_ONE) {
			entry = ((SPR *)actor->spr)->entries[0];
			if(entry != NULL) {
				MainRoutineStateWork.pausedColMode = entry->colMode;
			}
		}
		actor->update = (OBJECT_UPDATE)fdPsynergyUpdateHaltedActor;
		actor->paused = 1;
		fdActorSetAnimationSpeedStub(actor, 0);
	}
}

/* Lets a halted actor move again, with the update and colour mode kept by */
/* fdPsynergyHaltActor. */
void fdPsynergyResumeHaltedActor(s32 actorId) {
	ACTOR *actor = fdActorLookup(actorId);

	if(actor != NULL) {
		if(actor->update == (OBJECT_UPDATE)fdPsynergyUpdateHaltedActor) {
			actor->update = MainRoutineStateWork.pausedUpdate;
			MainRoutineStateWork.pausedUpdate = NULL;
			fdActorSetSprColModeStub(actor,
									 (s8)MainRoutineStateWork.pausedColMode);
		}
		actor->paused = 0;
		fdActorSetAnimationSpeedStub(actor, ANIM_STEP_NORMAL);
	}
}

s32 fdSpeakerGetHaltedLines(s32 id);

/* Returns no target (-1) when Halt passes actor id over (its sprite has no */
/* halted lines, SPEAKER_HALTED_LINES_NONE), else id. */
s32 fdPsynergyFilterHaltTarget(s32 id) {
	if(fdSpeakerGetHaltedLines(id) == SPEAKER_HALTED_LINES_NONE)
		return -1;
	return id;
}

/* Catch cast on a target: runs fdPsynergyCatch. */
void fdPsynergyCatchOnTarget(s32 unused) { fdPsynergyCatch(); }

/* The value step tenths of the way from from to to. */
static __inline__ s32 Interpolate(s32 from, s32 to, s32 step) {
	return from + (to - from) * step / CAST_THROW_STEPS;
}

/* The frames Catch's hand presses down, 2 pixels a frame, and rises again, */
/* 1.5 pixels a frame. */
#define CATCH_PRESS_FRAMES 10
#define CATCH_RISE_FRAMES  13

/* Catch: carries the hand (SPRITE_MOVE_HAND) from 16 pixels above the caster */
/* to the goal in 10 steps, growing from a quarter to full size, presses down */
/* 20 pixels moving the target to (-9, -9), rises 19.5 pixels and shrinks back.
 */
void fdPsynergyCatch(void) {
	FIELD_EFFECT_STATE *scene = Ram_HeapSlots->fieldEffState;
	ACTOR *mainActor = scene->caster;
	ACTOR *secondary = scene->tgt;
	ACTOR *actor;
	VECTOR3 spawn;
	VECTOR3 origin;
	VECTOR3 target;
	s32 step;
	s32 index;
	s32 steps = CAST_THROW_STEPS + 1;

	SET_POSITION(&origin, mainActor->x.fixed, mainActor->y.fixed + toFIXED(16),
				 mainActor->z.fixed);
	if(scene->aimAhead) {
		SET_POSITION(&target, mainActor->x.fixed,
					 mainActor->y.fixed + toFIXED(32), mainActor->z.fixed);
		syVectorAddPolarOffset(toFIXED(32), scene->ang, &target.x);
	}
	else {
		SET_POSITION(&target, scene->x, scene->y + toFIXED(32), scene->z);
	}
	SET_POSITION(&spawn, scene->x, scene->y + toFIXED(32), scene->z);
	actor = fdActorSpawn(SPRITE_MOVE_HAND, spawn.x, spawn.y, spawn.z);
	if(actor == 0)
		return;
	fdPsynergyFadeToCastCol();
	sySndPlayStub(SE_ITEM_BREAK);
	actor->facing = mainActor->facing;
	actor->speedLimit = toFIXED(13) / 10;
	actor->motionFlg = 0;
	fdActorSelectAnimationStub(actor, MOVE_HAND_ANIMATION_FIST);
	fdActorSetSprColModeStub(actor, SPRITE_COLOR_GRB);
	step = 0;
	do {
		s32 scale;

		SET_ACTOR_POSITION(
			actor, origin.x + (target.x - origin.x) * step / CAST_THROW_STEPS,
			origin.y + (target.y - origin.y) * step / CAST_THROW_STEPS,
			origin.z + (target.z - origin.z) * step / CAST_THROW_STEPS);
		scale = Interpolate(FIXED_ONE / 4, FIXED_ONE, step);
		actor->scaleX = scale;
		actor->scaleY = scale;
		sySynch(1);
		step++;
	} while(step < steps);
	sySynch(10);
	fdActorSelectAnimationStub(actor, MOVE_HAND_ANIMATION_CLENCH);
	sySynch(15);
	for(index = 0; index < CATCH_PRESS_FRAMES; index++) {
		actor->y.fixed -= toFIXED(2);
		sySynch(1);
	}
	fdActorSelectAnimationStub(actor, MOVE_HAND_ANIMATION_FIST);
	sySndPlayStub(SE_WATER);
	if(secondary != 0)
		fdActorSetPosAndResetMotionStub(secondary, -toFIXED(9),
										secondary->y.fixed, -toFIXED(9));
	sySynch(20);
	for(index = 0; index < CATCH_RISE_FRAMES; index++) {
		actor->y.fixed += toFIXED(3) / 2;
		sySynch(1);
	}
	sySynch(10);
	sySndPlayStub(SE_MENU_ERROR);
	step = 0;
	do {
		s32 scale;

		SET_ACTOR_POSITION(
			actor, target.x + (origin.x - target.x) * step / CAST_THROW_STEPS,
			target.y + (origin.y - target.y) * step / CAST_THROW_STEPS,
			target.z + (origin.z - target.z) * step / CAST_THROW_STEPS);
		scale = Interpolate(FIXED_ONE, FIXED_ONE / 4, step);
		actor->scaleX = scale;
		actor->scaleY = scale;
		sySynch(1);
		step++;
	} while(step < steps);
	fdActorReleaseStub(actor);
	fdPsynergyFadeFromCastCol();
}

void fdRopeGetEndNodePos(s32 target, VECTOR3 *position);

/* The stages of Lash's hand on a rope (SCATTER_STATE.phase). */
typedef enum LASH_PHASE {
	LASH_HAND_SHAKES = 0, /* trembling over the rope's end */
	LASH_HAND_STILL = 1,
	LASH_HAND_PULLS = 2, /* on the rope's end until the rope is tied */
	LASH_HAND_RISES = 3,
	LASH_HAND_GONE = 4 /* its sparkles die away */
} LASH_PHASE;

/* Task of the Lash on a rope: the hand trembles, takes the rope's end and */
/* rises away, scattering sparkles all the while. */
void fdPsynergyUpdateLash(void) {
	SCATTER_STATE *state = (SCATTER_STATE *)Ram_HeapSlots->battleEff;
	FIELD_EFFECT_STATE *scene = Ram_HeapSlots->fieldEffState;
	FIELD_STATE *field = Ram_HeapSlots->fieldState;
	ACTOR *actor = state->actor;
	VECTOR3 position;
	EFFECT_SPR *sprite;
	FIELD_PSYNERGY_PARTICLE *particle;
	s32 i;

	state->flash = 0;
	for(i = 0; i < state->spawnCnt; i++) {
		FIXED distance;
		s32 speed;

		particle = &state->particles[state->next];
		SET_POSITION(particle, actor->x.fixed, actor->y.fixed, actor->z.fixed);
		distance = syRandom() * 12 + toFIXED(4);
		syVectorAddPolarOffset(distance, syRandom(), particle);
		particle->velX = 0;
		particle->velY = toFIXED(2) - syRandom() * 2;
		particle->velZ = 0;
		speed = syRandom() + toFIXED(3) / 2;
		syVectorAddPolarOffset(speed, syRandom(), &particle->velX);
		particle->tmr = 0;
		state->next++;
		state->next &= SCATTER_PARTICLE_COUNT - 1;
	}

	for(i = 0, particle = state->particles; i < SCATTER_PARTICLE_COUNT;
		i++, particle++) {
		s32 timer = particle->tmr;

		if(timer >= 0) {
			sprite = &state->sprs[i];
			if(i & 1)
				sprite->oam.idx = state->tileA + (timer & 3) * 2;
			else
				sprite->oam.idx = state->tileB + (timer & 3) * 2;
			fdEffectSprDrawAtWorldPos(sprite, particle);
			fdAdvanceWithGravity3D(particle, 63, -(toFIXED(5) / 64));
			if(++particle->tmr == 32)
				particle->tmr = -1;
		}
	}

	switch(state->phase) {
	case LASH_HAND_SHAKES: {
		s32 a = syRandom();
		s32 b = syRandom();
		s32 c, d;

		position.x = scene->x + a * 3 - b * 3;
		c = syRandom();
		d = syRandom();
		position.y = scene->y + c * 3 - d * 3;
		position.z = scene->z;
		fdActorSetPosAndResetMotionStub(actor, position.x, position.y,
										position.z);
		if(state->tmr == 20) {
			state->phase++;
			state->tmr = -1;
		}
		break;
	}
	case LASH_HAND_STILL:
		fdActorSetPosAndResetMotionStub(actor, scene->x, scene->y, scene->z);
		if(state->tmr == 6) {
			state->phase++;
			state->tmr = -1;
		}
		break;
	case LASH_HAND_PULLS:
		if(state->tmr == 0)
			state->flash = 1;
		fdRopeGetEndNodePos(field->tgtX, &position);
		SET_ACTOR_POSITION(actor, position.x, position.y - toFIXED(4),
						   position.z);
		if(state->hit != 0) {
			state->phase++;
			state->tmr = -1;
		}
		if(state->tmr == 90) {
			state->phase++;
			state->tmr = -1;
		}
		break;
	case LASH_HAND_RISES:
		fdActorSelectAnimationStub(actor, 3);
		state->spawnCnt = 3;
		actor->y.fixed += state->tmr * toFIXED(2);
		if(state->tmr == 10) {
			state->phase++;
			state->tmr = -1;
		}
		break;
	case LASH_HAND_GONE:
		actor->scaleX = 0;
		state->spawnCnt = 0;
		if(state->tmr == 30)
			state->phase = FIELD_PSYNERGY_FINISHED;
		break;
	}
	state->tmr++;
}

/* Lash cast on a target: finds the rope's two ends (fdRopeFindById); with */
/* none, Lash runs as if cast on nothing (enabled), else the field state keeps
 */
/* the rope and its ends. Then runs fdPsynergyLash. */
void fdPsynergyLashOnTarget(s32 value) {
	FIELD_STATE *field = Ram_HeapSlots->fieldState;
	FIELD_EFFECT_STATE *state = Ram_HeapSlots->fieldEffState;
	s32 second;
	s32 first;

	if(fdRopeFindById(value, &first, &second)) {
		state->aimAhead = 1;
	}
	else {
		field->tgtY = value;
		field->tgtZ = first;
		field->tgtX = second;
	}
	fdPsynergyLash();
}

/* The frames Lash's hand comes down, 2 pixels a frame, and the sparkles it */
/* bursts into with no target. */
#define LASH_FALL_FRAMES 16
#define LASH_SPARKLES	 16

/* Lash: a hand forms over the spot and comes down on it. With no target it */
/* bursts into sparkles; cast on a rope's end it whips out and ties the rope */
/* (fdRopeLash) at the event's target. */
void fdPsynergyLash(void) {
	SCATTER_STATE *state =
		syHeapSlotAlloc(HEAP_SLOT_BATTLE_EFFECT, sizeof(SCATTER_STATE));
	FIELD_EFFECT_STATE *scene = Ram_HeapSlots->fieldEffState;
	FIELD_STATE *field = Ram_HeapSlots->fieldState;
	ACTOR *caster = scene->caster;
	ACTOR *actor;
	EFFECT_SPR *sprite;
	FIELD_PSYNERGY_PARTICLE *particle;
	s32 slotA;
	s32 slotB;
	s32 tileA;
	s32 i;

	if(scene->tgt != 0) {
		scene->y += toFIXED(4);
		scene->z += toFIXED(4);
	}
	fdPsynergyFadeToCastCol();
	actor = fdActorSpawn(SPRITE_LASH_HAND, 0, 0, 0);
	if(actor != 0) {
		actor->facing = caster->facing;
		actor->motionFlg = 0;
		if(caster->priFlg & ACTOR_PRIORITY_IGNORES_RISES)
			actor->priFlg |= ACTOR_PRIORITY_IGNORES_RISES;
		fdActorSelectAnimationStub(actor, 0);
		sySndPlayStub(SE_ITEM_BREAK);
		syDecompressTaggedLz(syResourceGetTableEntry(RESOURCE_SPARKLE_FADE),
							 state);
		slotA = syVramCacheFindFreeEntry();
		tileA = syVramCacheLoad(slotA, 256, state);
		state->tileA = tileA;
		syDecompressTaggedLz(
			syResourceGetTableEntry(RESOURCE_STAR_SPARKLE_FADE), state);
		slotB = syVramCacheFindFreeEntry();
		state->tileB = syVramCacheLoad(slotB, 256, state);

		particle = state->particles;
		sprite = state->sprs;
		for(i = 0; i < SCATTER_PARTICLE_COUNT; i++) {
			fdEffectSprInit(sprite, 4, 4, 0, tileA);
			sprite->depth = EFFECT_SPRITE_FRONT_DEPTH;
			sprite->oam.pri = 0;
			sprite->oam.fullCol = 1;
			sprite->oam.pal = 0;
			SET_POSITION(particle, actor->x.fixed, actor->y.fixed,
						 actor->z.fixed);
			syVectorAddPolarOffset(toFIXED(12), syRandom(), particle);
			particle->tmr = -1;
			sprite++;
			particle++;
		}

		fdPsynergyGrowFromCaster(actor, scene->x, scene->y + toFIXED(32),
								 scene->z);
		fdActorSelectAnimationStub(actor, 1);
		sySynch(10);
		for(i = 0; i < LASH_FALL_FRAMES; i++) {
			actor->y.fixed += -toFIXED(2);
			sySynch(1);
		}
		sySndPlayStub(SE_WATER);
		if(scene->aimAhead != 0) {
			for(i = 0; i < LASH_SPARKLES; i++)
				fdBattleEffectSpawnScatterParticle(
					actor->x.fixed, actor->y.fixed, actor->z.fixed);
			sySynch(1);
		}
		else {
			sySynch(6);
			state->actor = actor;
			state->phase = LASH_HAND_SHAKES;
			state->tmr = 0;
			state->next = 0;
			state->spawnCnt = 1;
			state->hit = 0;
			sySetEvent(fdPsynergyUpdateLash, TASK_PASS_FRAME);
			while(state->phase != FIELD_PSYNERGY_FINISHED) {
				if(state->flash == 1) {
					sySndPlayStub(SE_LASH);
					fdRopeLash(field->tgtX, field->tgtY, field->tgtZ);
					state->hit = 1;
					sySndPlayStub(SOUND_FADE_OUT_EXIT_SE);
				}
				sySynch(1);
			}
			syCloseEvent(fdPsynergyUpdateLash);
		}
		syVramCacheFreeEntry(slotA);
		syVramCacheFreeEntry(slotB);
		fdActorReleaseStub(actor);
	}
	fdPsynergyFadeFromCastCol();
	syHeapSlotRelease(HEAP_SLOT_BATTLE_EFFECT);
}

/* Per-frame update: spins by FACING_STEP + timer * 80 a frame, speeding up */
/* (timer counts up) until timer * 80 reaches FACING_STEP: from 1/16 turn a */
/* frame to just over 1/8 in 52 frames, as fdEffectAdvanceAngWithRamp. */
void fdEffectUpdateSpinAng(ACTOR *actor) {
	u32 step;

	step = actor->tmr * 80;
	actor->facing = actor->facing + step + FACING_STEP;
	if(step < FACING_STEP)
		actor->tmr = actor->tmr + 1;
}

/* Per-frame update: spins 1/8 turn a frame as it shrinks, rising a pixel a */
/* frame; its sprite is hidden once its scale falls below 3/16. */
void fdEffectUpdateRetreatCaster(ACTOR *actor) {
	u32 scale;

	actor->scaleY -= FIXED_ONE * 5 / 512;
	scale = actor->scaleX - FIXED_ONE * 5 / 512;
	actor->facing += DEGtoANG(45);
	actor->y.fixed += toFIXED(1);
	actor->scaleX = scale;
	if((s32)scale < FIXED_ONE * 3 / 16)
		actor->sprStorageKind = ACTOR_SPRITE_NONE;
}

/* Field effect sprite callback: a particle flies 120 pixels from the main */
/* actor's screen position (8 pixels lower) in a direction within 1/8 turn of */
/* up the screen, at up to 5 pixels a frame, and starts again from the main */
/* actor on arrival until it takes its last flight. */
void fdRetreatUpdateSpark(FIELD_PARTICLE *effect) {
	VECTOR3 pos;
	ACTOR *main;
	s32 state;
	u32 spread;

	main = ((FIELD_EFFECT_STATE *)Ram_HeapSlots->fieldEffState)->caster;
	state = effect->state;

	if(state == RETREAT_PARTICLE_AIM) {
		pos.x = main->x.fixed;
		pos.y = main->y.fixed;
		pos.z = main->z.fixed;
		fdCameraWorldToScr(&pos);

		effect->x = effect->originX = pos.x;
		effect->z = effect->originZ = pos.z + toFIXED(8);

		/* Up the screen, within 1/8 turn either way. */
		spread = syRandom();
		syVectorAddPolarOffset(toFIXED(120),
							   FIXEDtoINT(spread * (TRIG_TURN / 8)) -
								   syRandomBelow(TRIG_TURN / 8) + FACING_NORTH,
							   &pos);

		effect->tgtX = pos.x;
		effect->tgtZ = pos.z;
		effect->acceleration = toFIXED(5);
		effect->speedMax = toFIXED(5);
		effect->turnStepLimited = state;
		effect->state++;

		if((FrameCount & 2) != 0)
			sySndPlayStub(SE_GLITTER);
	}
	else if(state == RETREAT_PARTICLE_FLYING) {
		if(fdParticleHasPendingTarget(effect) == 0)
			effect->state--;
	}
	else if(state == RETREAT_PARTICLE_LAST_FLIGHT) {
		if(fdParticleHasPendingTarget(effect) == 0)
			fdEffectClearOwnedSlot(effect);
	}
}

/* Retreat ("Return to the dungeon's entrance."): the caster turns to face the
 */
/* player, raises their arms, spins faster and faster and shrinks away while */
/* sparks fly up the screen, one a frame. */
void fdPsynergyRetreat(void) {
	FIELD_EFFECT_STATE *effectState = Ram_HeapSlots->fieldEffState;
	ACTOR *actor = effectState->caster;
	VECTOR3 pos;
	FIELD_PARTICLE *effect;
	FIELD_PARTICLE *effects;
	u32 i;

	fdEventStart();
	fdCameraMoveWithinMapBounds(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	fdPsynergyFadeToCastCol();
	sySynch(10);
	fdActorFaceDirectionById(effectState->casterId, FACING_SOUTH, 0);
	sySynch(30);
	actor->update = (OBJECT_UPDATE)fdActorCycleSprCols;
	sySndPlayStub(SE_PSYNERGY_SPARKLE);
	fdActorSelectAnimationStub(actor, ANIM_LEADER_ARMS_RAISED);
	sySynch(40);

	/* Start spinning. */
	sySndPlayStub(SE_SHIMMER);
	fdActorSetSprColModeStub(actor, SPRITE_COLOR_NORMAL);
	fdActorSelectAnimationStub(actor, 3);
	actor->update = (OBJECT_UPDATE)fdEffectUpdateSpinAng;
	actor->tmr = 0;
	sySynch(70);

	/* Start shrinking, and send particles from where it is on screen. */
	fdActorSetSprFlagsStub(actor, 0);
	actor->motionFlg = 0;
	actor->update = (OBJECT_UPDATE)fdEffectUpdateRetreatCaster;
	actor->tgtX = ACTOR_NO_TARGET;
	SET_POSITION(&pos, actor->x.fixed, actor->y.fixed, actor->z.fixed);
	fdCameraWorldToScr(&pos);
	i = 0;
	effect = effectState->particles;
	effects = effect;
	do {
		FIXED scale;

		fdParticleInit(effect, SPRITE_PARTICLE, pos.x, pos.z);
		fdParticleSetCallback(effect, fdRetreatUpdateSpark);
		fdParticleSetAnimation(effect, ANIM_PARTICLE_TWINKLE);
		fdSprSetAnimationColModesStub(effect->spr, syRandomBelow(7));
		scale = (syRandom() >> 1) + FIXED_ONE * 6 / 5; /* 1.2 to 1.7 */
		effect->scaleY = scale;
		effect->scaleX = scale;
		sySynch(1);
		i++;
		effect++;
	} while(i < PSYNERGY_EFFECT_SLOT_COUNT);
	sySynch(70);

	/* The flying particles vanish once they arrive. */
	i = 0;
	effect = effects;
	do {
		if(effect->active != 0)
			effect->state = RETREAT_PARTICLE_LAST_FLIGHT;
		i++;
		effect++;
	} while(i < PSYNERGY_EFFECT_SLOT_COUNT);
	sySynch(40);
	fdPsynergyFadeFromCastCol();
	actor->scaleX = 0;
	actor->y.fixed = actor->groundHeight;
	sySynch(10);
}

/* The back half of one of Avoid's falling rings, a golden arc */
/* (FieldEffect_ArcSparkTiles) behind the caster: it grows to full size by */
/* 3/64 a frame as it falls 2 pixels a frame around the caster, 9 pixels south
 */
/* of them and up to 5 more while small, and is gone 10 pixels above the ground
 */
/* (4 on the World Map). */
void fdAvoidUpdateRingBackHalf(ACTOR *particle) {
	ACTOR *source;
	FIXED y;
	FIXED limit;
	FIXED scale;

	limit = particle->groundHeight + toFIXED(10);
	source = &particle->watchedActor->actor;
	if(MainRoutineStateWork.mapId == SCENE_CLEAR)
		limit = particle->groundHeight + toFIXED(4);

	y = particle->y.fixed;
	if(y <= limit) {
		fdActorReleaseStub(particle);
		return;
	}

	scale = particle->scaleX + FIXED_ONE * 3 / 64;
	if(scale > FIXED_ONE)
		scale = FIXED_ONE;
	particle->scaleX = scale;
	particle->scaleY = scale;
	particle->x.fixed = source->x.fixed;
	particle->y.fixed = y - toFIXED(2);
	particle->z.fixed =
		source->z.fixed + ((FIXED_ONE - scale) * 5) + toFIXED(9);
}

/* The ring's front half: the same arc drawn upside down in front of the */
/* caster, 16 pixels south and up to 5 nearer while small. */
void fdAvoidUpdateRingFrontHalf(ACTOR *particle) {
	ACTOR *source;
	FIXED y;
	FIXED limit;
	FIXED scale;

	limit = particle->groundHeight + toFIXED(10);
	source = &particle->watchedActor->actor;
	if(MainRoutineStateWork.mapId == SCENE_CLEAR)
		limit = particle->groundHeight + toFIXED(4);

	y = particle->y.fixed;
	if(y <= limit) {
		fdActorReleaseStub(particle);
		return;
	}

	scale = particle->scaleX + FIXED_ONE * 3 / 64;
	if(scale > FIXED_ONE)
		scale = FIXED_ONE;
	particle->scaleX = scale;
	particle->scaleY = -scale;
	particle->x.fixed = source->x.fixed;
	particle->y.fixed = y - toFIXED(2);
	particle->z.fixed =
		source->z.fixed - ((FIXED_ONE - scale) * 5) + toFIXED(16);
}

/* Drops one of Avoid's rings onto source: its two halves start 64 pixels */
/* above it at 0.4 of full size, drawn with the arc's tiles */
/* (FIELD_EFFECT_STATE.arcTileSlot), the front half over every sprite. */
void fdAvoidDropRing(ACTOR *source) {
	FIELD_EFFECT_STATE *effectState = Ram_HeapSlots->fieldEffState;
	ACTOR *leader = effectState->caster;
	ACTOR *actors[2];
	ACTOR *actor;
	DESCENDING_ARC_SPR *sprite;
	s32 arcIndex;

	for(arcIndex = 0; arcIndex <= 1; arcIndex++) {
		actor =
			fdActorCreateStub(SPRITE_GOLDEN_RINGS, source->x.fixed,
							  source->y.fixed + toFIXED(64), source->z.fixed);
		actors[arcIndex] = actor;
		if(actor == NULL)
			continue;
		actor->groundHeight = source->groundHeight;
		sprite = (DESCENDING_ARC_SPR *)actor->spr;
		actor->motionFlg = 0;
		actor->tmr = 0;
		actor->watchedActor = (FIELD_OBJECT *)source;
		actor->scaleY = FIXED_ONE * 2 / 5;
		actor->scaleX = FIXED_ONE * 2 / 5;
		if(sprite == NULL)
			continue;
		fdSprStartAnimationStub((SPR *)sprite, 0);
		sprite->flg = 0;
		syVramCacheFreeEntry(sprite->slot);
		sprite->slot = effectState->arcTileSlot;
		sprite->retainCacheEntry = 1;
		sprite->attr.half.tile = VramCacheEntries[sprite->slot].ofs >> 5;
		sprite->fullCol = 0;
		sprite->shape = 1;
		sprite->size = 2;
		sprite->firstEntry->picture = 0;
	}
	/* BUG: failed allocations above leave a missing arc or sprite here. */
	actors[0]->update = (OBJECT_UPDATE)fdAvoidUpdateRingFrontHalf;
	((DESCENDING_ARC_SPR *)actors[0]->spr)->attr.byte.pri = 0;
	actors[1]->update = (OBJECT_UPDATE)fdAvoidUpdateRingBackHalf;
	((DESCENDING_ARC_SPR *)actors[1]->spr)->attr.byte.pri =
		((DESCENDING_ARC_SPR *)leader->spr)->attr.byte.pri;
}

/* The caster during Avoid (fdPsynergyAvoid): drops a ring onto itself */
/* every 5 frames (7 on the World Map), and spins in phase 1, 3/64 turn a */
/* frame. */
void fdEffectUpdateAvoidCaster(ACTOR *caster) {
	s16 phase;
	s32 elapsedFrames;

	phase = caster->arcPhase;
	elapsedFrames = caster->arcFrm++;

	if(MainRoutineStateWork.mapId == SCENE_CLEAR) {
		if(elapsedFrames % 7 == 0)
			fdAvoidDropRing(caster);
	}
	else if(elapsedFrames % 5 == 0) {
		fdAvoidDropRing(caster);
	}

	if(phase == 1)
		caster->facing += TRIG_TURN * 3 / 64;
}

/* Avoid (or the Sacred Feather) has worn off, and the game says so. */
/* "Avoid's effects wore off." (or "Sacred Feather's ..."). */
/* */
/* Clears MainRoutineStateWork.avoidDistance. Reached through */
/* fdPsynergyRunExtraHandler when the Psynergy request the leader's walking */
/* raises as Avoid runs out (field_sequence_walking.c) is handled. */
void fdPsynergyShowAvoidWoreOff(void) {
	MainRoutineStateWork.avoidDistance = 0;
	fdWorldMapFreeze();
	if(MainRoutineStateWork.avoidByItem == 0) {
		mnMessageArgumentPushStub(ABILITY_AVOID, MESSAGE_ARGUMENT_ABILITY);
		fdMessageShowStub((s32)&MsgAbilityWoreOff, FIELD_MESSAGE_ZOOM);
	}
	else {
		mnMessageArgumentPushStub(ITEM_SACRED_FEATHER, MESSAGE_ARGUMENT_ITEM);
		fdMessageShowStub((s32)&MsgItemWoreOff, FIELD_MESSAGE_ZOOM);
	}
	fdWorldMapReloadPendingAreaGraphics();
}

/* Avoid's flashes, 5 frames each; then the frames its outline blinks, the */
/* chime coming on the 15th. */
#define AVOID_FLASHES	   20
#define AVOID_BLINK_FRAMES 70
#define AVOID_CHIME_FRAME  15

/* Avoid: the caster spins and flashes, then blinks, and encounters are kept */
/* away until Avoid wears off. */
void fdPsynergyAvoid(void) {
	FIELD_EFFECT_STATE *effectState;
	ACTOR *actor;
	SPR *group;
	SPR_ANIMATION *entry;
	const u8 *tiles;
	u32 angle;
	s32 flags;
	s32 slot;
	s32 count;
	s32 phase;

	effectState = Ram_HeapSlots->fieldEffState;
	actor = effectState->caster;
	group = actor->spr;
	entry = group->entries[0];
	angle = actor->facing;
	flags = group->flg;
	slot = syVramCacheFindFreeEntry();
	/* The arcs' tiles (256 bytes) stay loaded until the Psynergy's end, and */
	/* monsters stay away for the next 2,400 pixels the leader walks. */
	effectState->arcTileSlot = slot;
	tiles = FieldEffect_ArcSparkTiles;
	syVramCacheLoad((s16)slot, 256, tiles);
	MainRoutineStateWork.avoidDistance = toFIXED(2400);
	MainRoutineStateWork.avoidByItem = syGameFlagGet(FLAG_PSYNERGY_FROM_ITEM);
	fdActorSetSprColModeStub(actor, SPRITE_COLOR_NORMAL);
	actor->update = (OBJECT_UPDATE)fdEffectUpdateAvoidCaster;
	actor->arcPhase = 0;
	actor->arcFrm = 0;
	sySndPlayStub(SE_SPIN);
	sySynch(15);
	actor->arcPhase = 1;
	sySynch(10);
	/* Flash white for 2 frames in every 5. */
	for(count = 0; count < AVOID_FLASHES; count++) {
		entry->colMode = SPRITE_COLOR_WHITE;
		group->dirty = 1;
		sySynch(2);
		entry->colMode = SPRITE_COLOR_NORMAL;
		group->dirty = 1;
		group->flg = SPRITE_FLAG_SHADOW;
		sySynch(3);
	}
	actor->update = NULL;
	actor->facing = angle;

	/* Blink its outline beside its own flags on a five-frame cycle. */
	for(count = 0; count < AVOID_BLINK_FRAMES; count++) {
		phase = count % 5;
		if(phase == 0) {
			group->dirty = 1;
			group->flg = flags | SPRITE_FLAG_OUTLINED;
		}
		else if(phase == 2) {
			group->dirty = 1;
			group->flg = flags;
			entry->colMode = SPRITE_COLOR_NORMAL;
		}
		if(count == AVOID_CHIME_FRAME)
			sySndPlayStub(SE_NOTE_MID);
		sySynch(1);
	}
	group->flg = flags;
	group->dirty = 1;
	{
		u32 cleared = SPRITE_COLOR_NORMAL;

		entry->colMode = cleared;
	}
	sySynch(1);

	syVramCacheFreeEntry(effectState->arcTileSlot);
	fdMessageShowStub((s32)&MsgMonstersAttackLess, FIELD_MESSAGE_ZOOM);
}

#define ARC_GROUPS_WAIT_FRAMES 90

/* Waits, at most ARC_GROUPS_WAIT_FRAMES, until both pairs of swelling arcs */
/* have run their 32 frames. */
void fdBattleEffectWaitForArcGroups(void) {
	ARC_EFFECT_STATE *state = Ram_HeapSlots->arcEff;
	s32 i = 0;

	while(state->age[0] < ARC_GROUP_FRAMES ||
		  state->age[1] < ARC_GROUP_FRAMES) {
		sySynch(1);
		i++;
		if(i >= ARC_GROUPS_WAIT_FRAMES)
			return;
	}
}

/* Per-frame update: flashes an actor between colour mode 7 and 0 every 2 */
/* frames and spawns the swelling arcs on it every 16. */
void fdEffectFlickerActorAndSpawnArcs(ACTOR *actor) {
	if((FrameCount & 2) != 0) {
		fdActorSetSprColModeStub(actor, SPRITE_COLOR_WHITE);
	}
	else {
		fdActorSetSprColModeStub(actor, SPRITE_COLOR_NORMAL);
	}
	if((FrameCount & 15) == 0) {
		fdBattleEffectSpawnScaledArcObjects(actor);
	}
}

/* Per-frame update: cycles an actor through colour modes 0 to 5, one step */
/* every 2 frames, and spawns the swelling arcs on it every 16. */
void fdEffectCycleActorAndSpawnArcs(ACTOR *actor) {
	if(FrameCount & 1)
		fdActorSetSprColModeStub(actor, (FrameCount >> 1) % 6);
	if((FrameCount & 15) == 0)
		fdBattleEffectSpawnScaledArcObjects(actor);
}

/* Gives the caster its look while it casts (CASTER_LOOK). */
void fdBattleEffectSetObjectUpdateMode(ACTOR *actor, s32 mode) {
	if(mode == CASTER_LOOK_NORMAL) {
		actor->update = 0;
		fdActorSetSprColModeStub(actor, SPRITE_COLOR_NORMAL);
	}
	else if(mode == CASTER_LOOK_FLASHING_WHITE) {
		actor->update = (OBJECT_UPDATE)fdEffectFlickerActorAndSpawnArcs;
	}
	else {
		actor->update = (OBJECT_UPDATE)fdEffectCycleActorAndSpawnArcs;
	}
}

/* Sets up the current group's two arc sprites from the effect's tiles, at the
 */
/* target's priority, then restarts that group's age, records the target and */
/* switches to the other group. */
void fdBattleEffectSpawnScaledArcObjects(ACTOR *target) {
	ARC_EFFECT_STATE *state = Ram_HeapSlots->arcEff;
	s32 tiles = VramCacheEntries[state->resource].ofs >> 5;
	s32 group = state->group;
	EFFECT_SPR *sprite;
	s32 i;

	for(i = 0; i < 2; i++) {
		sprite = &state->sprs[group][i];
		fdEffectSprInit(sprite, 16, 16, OBJ_SIZE_32X16, tiles);
		sprite->oam.fullCol = 0;
		sprite->oam.pal = 0;
		sprite->oam.pri = fdActorGetSprPriority(target);
	}
	state->age[group] = 0;
	state->tgt = target;
	state->group ^= 1;
}

/* Task: draws the two pairs of swelling arcs on their actor: each pair swells
 */
/* and shrinks over 32 frames (scale sin(age / 64 turn)) as it rises a pixel a
 */
/* frame, the second arc of a pair upside down, one in front of the actor and */
/* one behind. */
void fdBattleEffectUpdateArcEffect(void) {
	ARC_EFFECT_STATE *state = Ram_HeapSlots->arcEff;
	ACTOR *target = state->tgt;
	s32 group;
	FIXED position[3];
	s32 i;

	for(group = 0; group < 2; group++) {
		FIXED scale = sySin(state->age[group] * (TRIG_TURN / 64));

		if(state->age[group] < 0)
			continue;
		if(state->age[group] < ARC_GROUP_FRAMES) {
			EFFECT_SPR *sprite;
			FIXED x, y;

			position[0] = target->x.fixed;
			position[1] = target->y.fixed + toFIXED(state->age[group]);
			position[2] = target->z.fixed;
			fdCameraWorldToScr(position);
			x = position[0];
			y = position[2];
			for(i = 0; i < 2; i++) {
				sprite = &state->sprs[group][i];
				sprite->x = x;
				sprite->y = y;
				sprite->scaleX = scale;
				sprite->scaleY = scale;
				if(i == 0) {
					sprite->depth = fdActorGetSprDepth(state->tgt) - 1;
				}
				else {
					sprite->depth = fdActorGetSprDepth(state->tgt) + 1;
					sprite->y -= toFIXED(1);
					sprite->scaleY = -sprite->scaleY;
				}
				fdEffectSprDraw(sprite);
			}
			state->age[group]++;
		}
	}
}

/* Starts the swelling arcs over a caster: loads their tiles and starts */
/* fdBattleEffectUpdateArcEffect with no pair showing. */
void fdBattleEffectStartArcEffect(void) {
	ARC_EFFECT_STATE *state =
		syHeapSlotAlloc(HEAP_SLOT_ARC_EFFECT, sizeof(*state));

	state->resource = syVramCacheFindFreeEntry();
	syVramCacheLoad(state->resource, 256, FieldEffect_SparkTiles);
	state->group = 0;
	state->age[0] = ARC_GROUP_UNSPAWNED;
	state->age[1] = ARC_GROUP_UNSPAWNED;
	state->tgt = 0;
	sySetEvent((s32)fdBattleEffectUpdateArcEffect, TASK_PASS_FRAME);
}

/* Ends the swelling arcs: frees their tiles and their state, and stops their */
/* task. */
void fdBattleEffectEndArcEffect(void) {
	ARC_EFFECT_STATE *state = Ram_HeapSlots->arcEff;

	syVramCacheFreeEntry(state->resource);
	syCloseEvent((u32)fdBattleEffectUpdateArcEffect);
	syHeapSlotRelease(HEAP_SLOT_ARC_EFFECT);
}

/* Moves the state on to its next phase, its timer restarted. */
#define BOW_NEXT_PHASE(state) ((state)->phase++, (state)->tmr = -1)
/* Of the BOW_SPARK_COUNT sparks, those the bow throws off. */
#define BOW_SPARKS_THROWN 10
/* While the thrown sparks repeat, each starts again after this many frames. */
#define BOW_SPARK_FRAMES 10

/* The stages of the Arrow (BOW_STATE.phase). */
typedef enum ARROW_PHASE {
	ARROW_BOW_GROWS = 0, /* the bow grows and turns to face ahead */
	ARROW_BOW_DRAWN = 1, /* the arrow is drawn back and loosed */
	ARROW_FLIES = 2,	 /* until it strikes or has flown 80 pixels */
	ARROW_STOPS = 3,
	ARROW_FALLS = 4,	  /* a struck arrow quivers; a spent one shrinks away */
	ARROW_BOW_SHRINKS = 5 /* ends once the bow is gone */
} ARROW_PHASE;

/* Task of the Arrow, the field Psynergy of the unused ability "A??" (named */
/* here for the bow it draws and the arrow it looses): the bow and the flaming
 */
/* arrow. */
void fdPsynergyUpdateArrow(void) {
	FIELD_EFFECT_STATE *scene = Ram_HeapSlots->fieldEffState;
	BOW_STATE *state = (BOW_STATE *)Ram_HeapSlots->battleEff;
	CANVAS_SPR_STATE *canvas = Ram_HeapSlots->canvasSpr;
	FIELD_STATE *field = Ram_HeapSlots->fieldState;
	ACTOR *actor;
	BOW_MOTION *motion;
	EFFECT_SPR *sprite;
	s32 pos[3];
	s32 objectIndex;
	s32 i;
	s32 dx;
	s32 dz;

again:
	switch(state->phase) {
	case ARROW_BOW_GROWS:
		if(state->tmr == 0) {
			state->sparking = 0;
			state->sparksRepeat = 1;
			state->arrowMoving = 0;
			state->bowGrowing = 1;
			state->frm = BOW_FRAME_IDLE;
		}
		else if(state->bowGrowing == 0) {
			state->frm = 0;
			BOW_NEXT_PHASE(state);
		}
		break;
	case ARROW_BOW_DRAWN:
		if(state->tmr == 0) {
			sprite = &state->arrow;
			sprite->scaleX = sprite->scaleY = FIXED_ONE;
			motion = &state->arrowMotion;
			pos[0] = pos[1] = pos[2] = 0;
			syVectorAddPolarOffset(toFIXED(3) / 20, scene->ang8 + DEGtoANG(180),
								   pos);
			motion->velX = pos[0];
			motion->velY = pos[1];
			motion->velZ = pos[2];
			state->arrowMoving = 1;
		}
		if(state->tmr == 20)
			state->sparking = 1;
		if(state->tmr == 56) {
			motion = &state->arrowMotion;
			pos[0] = pos[1] = pos[2] = 0;
			syVectorAddPolarOffset(toFIXED(5), scene->ang8, pos);
			motion->velX = pos[0];
			motion->velY = pos[1];
			motion->velZ = pos[2];
			BOW_NEXT_PHASE(state);
		}
		break;
	case ARROW_FLIES:
		motion = &state->arrowMotion;
		if(state->tmr == 90) {
			BOW_NEXT_PHASE(state);
			break;
		}
		dx = state->tgt[0] - state->start[0];
		if(dx < 0)
			dx = state->start[0] - state->tgt[0];
		dz = motion->x - state->start[0];
		if(dz < 0)
			dz = state->start[0] - motion->x;
		if(dx < dz) {
			BOW_NEXT_PHASE(state);
			goto again;
		}
		dx = state->tgt[2] - state->start[2];
		if(dx < 0)
			dx = state->start[2] - state->tgt[2];
		dz = motion->z - state->start[2];
		if(dz < 0)
			dz = state->start[2] - motion->z;
		if(dx < dz) {
			BOW_NEXT_PHASE(state);
			goto again;
		}
		pos[0] = motion->x;
		pos[1] = motion->y;
		pos[2] = motion->z;
		if(fdMapGridCheckCollisionStub(scene->caster, pos) > 0) {
			state->done = 1;
			state->result = -1;
			state->phase++;
			state->tmr = -1;
			goto again;
		}
		for(objectIndex = 0; objectIndex < FIELD_ACTOR_COUNT; objectIndex++) {
			actor = field->actors[objectIndex];
			if(actor != NULL && actor->script != NULL &&
			   (actor->collisionFlg & ACTOR_SOLID) && actor != scene->caster &&
			   fdPsynergyCheckOverlap(&motion->x, 2, &actor->x.fixed,
									  actor->radius - 2) >= 0) {
				state->done = 1;
				state->result = objectIndex;
				BOW_NEXT_PHASE(state);
				goto again;
			}
		}
		break;
	case ARROW_STOPS:
		if(state->done != 0) {
			state->arrowMoving = 0;
		}
		else {
			state->sparksRepeat = 0;
			state->bowShrinking = 1;
		}
		motion = &state->arrowMotion;
		pos[0] = pos[1] = pos[2] = 0;
		syVectorAddPolarOffset(toFIXED(2), scene->ang8, pos);
		motion->velX = pos[0];
		motion->velY = pos[1];
		motion->velZ = pos[2];
		BOW_NEXT_PHASE(state);
		break;
	case ARROW_FALLS:
		sprite = &state->arrow;
		if(state->done != 0) {
			if(state->tmr < 32) {
				s32 wobble[8];
				s32 fade[4];
				s32 w;

				memcpy(wobble, Arrow_WobbleAngles, sizeof(wobble));
				memcpy(fade, Arrow_WobbleFades, sizeof(fade));
				w = wobble[state->tmr & 7];
				sprite->ang =
					scene->ang8 + w * fade[state->tmr / 8] / (TRIG_TURN);
			}
			if(state->tmr == 32)
				state->bowShrinking = 1;
			if(state->tmr == 50) {
				sprite->scaleX = sprite->scaleY = 0;
				state->arrowMoving = 1;
				state->sparksRepeat = 0;
				BOW_NEXT_PHASE(state);
			}
		}
		else {
			sprite->scaleX += -(FIXED_ONE * 3 / 16);
			sprite->scaleY = sprite->scaleX;
			if(sprite->scaleX < 0) {
				sprite->scaleX = sprite->scaleY = 0;
				BOW_NEXT_PHASE(state);
			}
		}
		break;
	case ARROW_BOW_SHRINKS:
		if(state->bowShrinking == 0)
			state->phase = FIELD_PSYNERGY_FINISHED;
		break;
	}

	if(state->bowGrowing != 0) {
		sprite = &state->bow;
		sprite->scaleX += FIXED_ONE / 64;
		sprite->scaleY = sprite->scaleX;
		sprite->ang += DEGtoANG(45);
		if(sprite->scaleX >= FIXED_ONE) {
			sprite->scaleX = sprite->scaleY = FIXED_ONE;
			sprite->ang = scene->ang8;
			state->bowGrowing = 0;
		}
	}
	if(state->bowShrinking != 0) {
		sprite = &state->bow;
		sprite->scaleX += -(FIXED_ONE / 64);
		sprite->scaleY = sprite->scaleX;
		sprite->ang += DEGtoANG(45);
		if(sprite->scaleX <= toFIXED(0.4) - 1) {
			sprite->scaleX = sprite->scaleY = 0;
			state->bowShrinking = 0;
		}
	}

	/* The bow's drawing pose, by frame: drawn back, then loosed and recovered.
	 */
	if(state->frm != BOW_FRAME_IDLE) {
		state->frm++;
		sprite = &state->bow;
		switch(state->frm) {
		case 10:
			sprite->oam.idx = state->bowTile + BOW_POSE_TILES;
			break;
		case 20:
			sprite->oam.idx = state->bowTile + BOW_POSE_TILES * 2;
			break;
		case 54:
			sprite->oam.idx = state->bowTile + BOW_POSE_TILES;
			break;
		case 62:
			sprite->oam.idx = state->bowTile + BOW_POSE_TILES * 3;
			break;
		case 65:
			sprite->oam.idx = state->bowTile + BOW_POSE_TILES * 4;
			break;
		case 68:
			sprite->oam.idx = state->bowTile + BOW_POSE_TILES * 5;
			break;
		case 58:
		case 71:
			sprite->oam.idx = state->bowTile;
			break;
		case 72:
			state->frm = BOW_FRAME_IDLE;
			break;
		}
	}

	motion = &state->bowMotion;
	pos[0] = motion->x;
	pos[1] = motion->y;
	pos[2] = motion->z;
	sprite = &state->bow;
	fdCameraWorldToScr(pos);
	sprite->x = pos[0];
	sprite->y = pos[2];
	fdEffectSprDraw(sprite);
	motion = &state->arrowMotion;
	pos[0] = motion->x;
	pos[1] = motion->y;
	pos[2] = motion->z;
	fdCameraWorldToScr(pos);
	sprite = &state->arrow;
	sprite->x = pos[0];
	sprite->y = pos[2];
	fdEffectSprDraw(sprite);
	if(state->arrowMoving != 0)
		fdAdvanceWithGravity3D(&motion->x, 64, 0);

	if(state->sparking != 0) {
		motion = state->sparks;
		for(i = 0; i < BOW_SPARKS_THROWN; i++) {
			if(motion->delay == 0) {
				s32 speed;
				s32 angle;

				speed = (syRandom() & 255) + 256;
				angle = (syRandom() & 0x1fff) + TRIG_TURN * 7 / 16;
				motion->x = toFIXED(60);
				motion->y = toFIXED(28);
				motion->velX = syCos(angle) * speed >> 6;
				motion->velY = sySin(angle) * speed >> 6;
			}
			if(motion->delay > 0) {
				s32 x = (s16)FIXEDtoINT(motion->x);
				s32 y = (s16)FIXEDtoINT(motion->y);

				fdBattleEffectDrawCanvasBlock(
					Arrow_GlintPixels + (motion->delay & 1) * 64, x, y, 8, 8);
				fdAdvanceWithGravity2D(&motion->x, 60, 0);
			}
			motion->delay++;
			if(state->sparksRepeat == 1 && motion->delay == BOW_SPARK_FRAMES)
				motion->delay = 0;
			motion++;
		}
	}

	motion = &state->arrowMotion;
	pos[0] = motion->x;
	pos[1] = motion->y;
	pos[2] = motion->z;
	syVectorAddPolarOffset(-toFIXED(36), scene->ang8, pos);
	fdCameraWorldToScr(pos);
	canvas->x = (s16)FIXEDtoINT(pos[0]);
	canvas->y = (s16)FIXEDtoINT(pos[2]);
	canvas->command = CANVAS_UPLOAD_AND_DARKEN_FOUR_LEVELS;
	state->tmr++;
}

/* The Arrow cast on a target: runs fdPsynergyArrow. Nothing calls it, as */
/* the dispatch of Psynergy cast on a target has no case for the Arrow. */
void fdPsynergyArrowOnTarget(void) { fdPsynergyArrow(); }

/* The Arrow: a bow grows in front of the caster and draws, throwing off */
/* glints, and looses a flaming arrow that flies up to 80 pixels; an actor it */
/* strikes becomes the field state's psynergyHitObj. */
void fdPsynergyArrow(void) {
	BOW_STATE *state =
		syHeapSlotAlloc(HEAP_SLOT_BATTLE_EFFECT, sizeof(BOW_STATE));
	FIELD_EFFECT_STATE *scene = Ram_HeapSlots->fieldEffState;
	FIELD_STATE *field = Ram_HeapSlots->fieldState;
	CANVAS_SPR_STATE *canvas;
	EFFECT_SPR *sprite;
	BOW_MOTION *motion;
	FIXED pos[3];
	s32 arrowSlot;
	s32 bowSlot;
	s32 tile;
	u16 *palette;
	s32 blue;
	s32 shade;
	s32 i;

	fdPsynergyFadeToCastCol();
	/* The spark pointer and the sprite pointer are one variable. */
	sprite = (EFFECT_SPR *)state->sparks;
	for(i = 0; i < BOW_SPARK_COUNT; i++) {
		((BOW_MOTION *)sprite)->delay = -i;
		sprite = (EFFECT_SPR *)((BOW_MOTION *)sprite + 1);
	}

	state->tiles = syHeapSlotAlloc(HEAP_SLOT_BATTLE_CANVAS, BOW_TILE_BYTES);
	syDecompressTaggedLz(syResourceGetTableEntry(RESOURCE_BOW_DRAW),
						 state->tiles);
	bowSlot = syVramCacheFindFreeEntry();
	tile = syVramCacheLoad(bowSlot, BOW_TILE_BYTES, state->tiles);
	state->bowTile = tile;
	sprite = &state->bow;
	fdEffectSprInit(sprite, 16, 16, OBJ_SIZE_32X32, tile);
	sprite->oam.fullCol = 1;
	sprite->oam.pal = 0;
	sprite->oam.pri = fdActorGetSprPriority(scene->caster);
	sprite->depth = fdActorGetSprDepth(scene->caster) + 1;
	sprite->scaleX = FIXED_ONE * 2 / 5;
	sprite->scaleY = FIXED_ONE * 2 / 5;
	sprite->ang = scene->ang8;
	pos[0] = scene->caster->x.fixed;
	pos[2] = scene->caster->z.fixed;
	motion = &state->bowMotion;
	syVectorAddPolarOffset(toFIXED(8), scene->ang8, pos);
	SET_POSITION(motion, pos[0], toFIXED(16), pos[2]);

	syDecompressTaggedLz(syResourceGetTableEntry(RESOURCE_FLAMING_ARROW),
						 state->tiles);
	arrowSlot = syVramCacheFindFreeEntry();
	tile = syVramCacheLoad(arrowSlot, 256, state->tiles);
	sprite = &state->arrow;
	fdEffectSprInit(sprite, 12, 8, OBJ_SIZE_16X16, tile);
	sprite->oam.fullCol = 1;
	sprite->oam.pal = 0;
	sprite->oam.pri = fdActorGetSprPriority(scene->caster);
	sprite->depth = fdActorGetSprDepth(scene->caster) + 2;
	sprite->scaleX = 0;
	sprite->scaleY = 0;
	sprite->ang = scene->ang8;
	pos[0] = scene->caster->x.fixed;
	pos[2] = scene->caster->z.fixed;
	motion = &state->arrowMotion;
	syVectorAddPolarOffset(toFIXED(18), scene->ang8, pos);
	SET_POSITION(motion, pos[0], toFIXED(16), pos[2]);
	syHeapSlotRelease(HEAP_SLOT_BATTLE_CANVAS);

	fdBattleEffectStartCanvasSpr();
	canvas = Ram_HeapSlots->canvasSpr;
	canvas->pri = fdActorGetSprPriority(scene->caster);
	canvas->depth = fdActorGetSprDepth(scene->caster) + 3;
	canvas->mode = CANVAS_SPRITE_256_COLORS;
	canvas->command = 0;
	canvas->ang = scene->ang8;
	state->start[0] = scene->caster->x.fixed;
	state->start[1] = scene->caster->y.fixed;
	state->start[2] = scene->caster->z.fixed;
	blue = RGB(0, 0, RGB_LEVEL_MAX);
	state->tgt[0] = scene->caster->x.fixed;
	state->tgt[1] = scene->caster->y.fixed;
	state->tgt[2] = scene->caster->z.fixed;
	syVectorAddPolarOffset(toFIXED(80), scene->ang8, state->tgt);

	/* The canvas palette: blue full, red and green rising from half to full. */
	palette = (u16 *)&OBJ_PLTT_COLOR(CANVAS_PALETTE, 0);
	for(i = 0; i < PALETTE_COLORS; i++) {
		shade = i + (RGB_LEVEL_MAX + 1) / 2;
		syIoWrite16(RGB(0, shade, 0) | blue | shade, palette);
		palette++;
	}

	sySndPlayStub(SE_PSYNERGY_START);
	sySynch(1);
	state->bowGrowing = 0;
	state->bowShrinking = 0;
	state->done = 0;
	state->result = -1;
	state->phase = ARROW_BOW_GROWS;
	state->tmr = 0;
	sySetEvent(fdPsynergyUpdateArrow, TASK_PASS_FRAME);
	while(state->phase != FIELD_PSYNERGY_FINISHED)
		sySynch(1);
	syCloseEvent(fdPsynergyUpdateArrow);
	if(state->done != 0 && state->result != -1)
		field->psynergyHitObj = state->result;
	syVramCacheFreeEntry(arrowSlot);
	syVramCacheFreeEntry(bowSlot);
	fdPsynergyFadeFromCastCol();
	fdBattleEffectEndCanvasSpr();
	syHeapSlotRelease(HEAP_SLOT_BATTLE_EFFECT);
}

/* Finds what Cyclone reaches: of the 21 cells round the caster */
/* (FieldPsynergy_NearbyCellOffsets) on its layer, those of a CYCLONE_CELL_KIND
 */
/* within reach are listed, as cells, in the field state's points, which end */
/* with -1, -1. A cell to blow away (CYCLONE_CELL_BURST) also sets */
/* CYCLONE_STATE.kind; the last carry and lift cells are kept as the pixels of
 */
/* their centres, in cycloneCarryX, Z and cycloneLiftX, Z. */
void fdPsynergyGatherNearbyCells(void) {
	FIELD_EFFECT_STATE *effect = Ram_HeapSlots->fieldEffState;
	POINT_STATE *field = Ram_HeapSlots->fieldState;
	CYCLONE_STATE *state = Ram_HeapSlots->battleEff;
	s16 *points = field->points;
	ACTOR *owner = effect->caster;
	FIXED position[3];
	FIXED x, z;
	s32 kind;
	u32 i;
	s32 count = 0;

	state->kind = count;
	field->cycloneCarryX = -1;
	field->cycloneCarryZ = -1;
	field->cycloneLiftX = -1;
	field->cycloneLiftZ = -1;
	for(i = 0; i < FIELD_PSYNERGY_NEARBY_CELLS; i++) {
		x = owner->x.fixed +
			FIELD_CELL_ORIGIN(FieldPsynergy_NearbyCellOffsets[i * 2]);
		z = owner->z.fixed +
			FIELD_CELL_ORIGIN(FieldPsynergy_NearbyCellOffsets[i * 2 + 1]);
		kind = fdMapGridGetCycloneCellKind(x, z, owner->layer);
		/* position[1] is never set: only x and z reach fdMeasurePsynergyHeight.
		 */
		position[0] = x;
		position[2] = z;
		if(kind == CYCLONE_CELL_BURST &&
		   fdMeasurePsynergyHeight(effect->fieldPsynergy, owner, position) !=
			   -1) {
			points[count * 2] = x / toFIXED(16);
			points[count * 2 + 1] = z / toFIXED(16);
			count++;
			state->kind = kind;
		}
		if(kind == CYCLONE_CELL_CARRY &&
		   fdMeasurePsynergyHeight(effect->fieldPsynergy, owner, position) !=
			   -1) {
			field->cycloneCarryX = FIXEDtoINT(x);
			field->cycloneCarryZ = FIXEDtoINT(z);
			field->cycloneCarryX =
				(field->cycloneCarryX & ~(MAPGRID_CELL_PIXELS - 1)) +
				MAPGRID_CELL_PIXELS / 2;
			field->cycloneCarryZ =
				(field->cycloneCarryZ & ~(MAPGRID_CELL_PIXELS - 1)) +
				MAPGRID_CELL_PIXELS / 2;
			points[count * 2] = x / toFIXED(16);
			points[count * 2 + 1] = z / toFIXED(16);
			count++;
		}
		if(kind == CYCLONE_CELL_LIFT &&
		   fdMeasurePsynergyHeight(effect->fieldPsynergy, owner, position) !=
			   -1) {
			field->cycloneLiftX = FIXEDtoINT(x);
			field->cycloneLiftZ = FIXEDtoINT(z);
			field->cycloneLiftX =
				(field->cycloneLiftX & ~(MAPGRID_CELL_PIXELS - 1)) +
				MAPGRID_CELL_PIXELS / 2;
			field->cycloneLiftZ =
				(field->cycloneLiftZ & ~(MAPGRID_CELL_PIXELS - 1)) +
				MAPGRID_CELL_PIXELS / 2;
			points[count * 2] = x / toFIXED(16);
			points[count * 2 + 1] = z / toFIXED(16);
			count++;
		}
	}
	points[count * 2] = -1;
	points[count * 2 + 1] = -1;
}

/* The ROM's callers here are compiled against this form, which returns an */
/* s32 where the definition returns a u8. */
s32 fdMapGridGetCellBehavior(s32 x, s32 z, s32 layer);

/* Runs the place's Psynergy event once for each kind of cell Cyclone touched */
/* (FIELD_STATE.psynergyCells), lowest cell behaviour first. */
void fdPsynergyRunTouchedEvents(void) {
	FIELD_EFFECT_STATE *effectState = Ram_HeapSlots->fieldEffState;
	TOUCH_CELL *cells = (TOUCH_CELL *)FieldStateWork->psynergyCells;
	ACTOR *caster = effectState->caster;
	s32 count = 0;
	s16 ids[24];
	s32 i, j;

	for(i = 0; cells[i].x != -1; i++) {
		s16 id = fdMapGridGetCellBehavior(FIELD_CELL_ORIGIN(cells[i].x),
										  FIELD_CELL_ORIGIN(cells[i].z),
										  caster->layer);

		for(j = 0; j < count; j++) {
			if(ids[j] == id)
				break;
		}
		if(j == count)
			ids[count++] = id;
	}
	ids[count] = -1;

	if(count > 1) {
		for(i = 0; i < count - 1; i++) {
			for(j = i + 1; j < count; j++) {
				if(ids[i] > ids[j]) {
					s16 swap = ids[i];

					ids[i] = ids[j];
					ids[j] = swap;
				}
			}
		}
	}

	for(i = 0; ids[i] != -1; i++) {
		s32 id = ids[i];
		s32 event = scFindPsynergyTrigger(PSYNERGY_TRIGGER(PSYNERGY_PHASE_ACTS),
										  effectState->fieldPsynergy, id);

		if(event != 0)
			fdRunPsynergyEvent(event, effectState->casterId,
							   effectState->tgtId);
	}
}

MAP_CELL *fdMapGridGetCellAt(s32 x, s32 z, s32 layer);

/* Clears what Cyclone blew away: each cell of FIELD_STATE.psynergyCells that */
/* falls in the width by height window at (windowX, windowZ) of layerIndex */
/* (counted from the layer's scroll and the leader's height) takes the cell */
/* picture from the same place of the window at (sourceX, sourceZ), is opened */
/* on layer 2, and is recorded (up to 64) in TouchCopiesWork to be made again.
 */
void fdTileCopiesCopyTouchedCells(s32 sourceX, s32 sourceZ, s32 width,
								  s32 height, s32 windowX, s16 windowZ,
								  s32 layerIndex) {
	FIELD_STATE *field = Ram_HeapSlots->fieldState;
	ACTOR *owner = fdActorGetById(fdPartyGetLeaderActor());
	s16 *cell = field->psynergyCells;
	s32 ownerHeight = FIELD_CELL(owner->groundHeight);
	MAP_STATE *map = Ram_HeapSlots->mapGrid;
	MAP_LAYER *layer = &map->layers[layerIndex];
	s32 i;

	for(i = 0; cell[i * 2] != -1; i++) {
		s32 x = cell[i * 2] - windowX;
		s32 z = cell[i * 2 + 1] - windowZ - ownerHeight;

		x += FIELD_CELL(layer->ofsX);
		z += FIELD_CELL(layer->ofsY);

		if(x >= 0 && x < width && z >= 0 && z < height) {
			s32 n;

			fdMapGridCopyCellTilesCellsRectStub(sourceX + x, sourceZ + z, 1, 1,
												windowX + x, windowZ + z);
			fdMapGridGetCellAt(FIELD_CELL_ORIGIN(cell[i * 2]),
							   FIELD_CELL_ORIGIN(cell[i * 2 + 1]), 2)
				->collisionCode = 0;
			n = TouchCopiesWork.cnt;
			if(n < (s32)ARRAY_COUNT(TouchCopiesWork.copies)) {
				TouchCopiesWork.copies[n].toX = sourceX + x;
				TouchCopiesWork.copies[n].toZ = sourceZ + z;
				TouchCopiesWork.copies[n].fromX = windowX + x;
				TouchCopiesWork.copies[n].fromZ = windowZ + z;
				TouchCopiesWork.copies[n].cellX = cell[i * 2];
				TouchCopiesWork.copies[n].cellZ = cell[i * 2 + 1];
				TouchCopiesWork.cnt++;
			}
		}
	}
}
#undef BOW_NEXT_PHASE

/* Makes again each copy fdTileCopiesCopyTouchedCells recorded: copies the cell
 */
/* picture back and opens the cell on layer 2. */
void fdTileCopiesRestore(void) {
	TOUCH_COPIES *base = &TouchCopiesWork;
	s32 i;

	for(i = 0; i < TouchCopiesWork.cnt; i++) {
		TOUCH_COPY *entry = &base->copies[i];

		fdMapGridCopyCellTilesCellsRectStub(entry->toX, entry->toZ, 1, 1,
											entry->fromX, entry->fromZ);
		fdMapGridGetCellAt(FIELD_CELL_ORIGIN(entry->cellX),
						   FIELD_CELL_ORIGIN(entry->cellZ), 2)
			->collisionCode = 0;
	}
}
