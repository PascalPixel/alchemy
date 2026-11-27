/* The field Psynergy that spread effects around their target: Whirlwind, Halt,
   Catch, Lash, Retreat and Avoid, with the particle rings and arcs they share.
   Part of field_psynergy.c. */

#include "field/stubs.h"
#include "macros.h"

#include "field/object.h"
#include "field/field_psynergy.h"
#include "field/script.h"
#include "window/stubs.h"
#include "text/msg_ids.h"
#include "window/window.h"
#include "item_ids.h"

extern const Sint32 FieldEffectReleaseScript[];

/* Whirlwind: its leaves, their circling and wandering, the target's shrinking
   and the orb's flight, in ticks. */
enum {
	WHIRLWIND_LEAF_COUNT = 16,
	WHIRLWIND_ORBIT_RADIUS = toFIXED(8),
	WHIRLWIND_ORBIT_ANGLE_STEP = TRIG_TURN / 32,
	WHIRLWIND_ORBIT_TICKS = 121,
	WHIRLWIND_WANDER_TICKS = 101,
	WHIRLWIND_TARGET_SCALE_STEP = toFIXED(1) / 512,
	WHIRLWIND_TARGET_MIN_SCALE = toFIXED(1) / 2,
	WHIRLWIND_TRAVEL_TICKS = 10
};

/* The radial ring's particles, a 16th of a turn apart; the last goes where the
   first went. */
#define RADIAL_RING_STEPS 16

/* Spawns 17 particles that spread from origin in a flat ring, their x velocity
   1.5 times their z velocity, so the ring is wider than deep as the field is
   drawn. */
void fdEffectSpawnRadialParticleRing(MOTION_OBJECT *origin)
{
	EFFECT_OPTIONS options;
	FIXED		   vel[3];
	FIXED		   sine;
	Sint32		   ang;
	Uint32		   i;

	options.pal = 0;
	options.update = fdEffectAdvanceMotion;
	options.startScaleX = toFIXED(4) / 5;
	options.startScaleY = toFIXED(4) / 5;
	for(i = 0; i <= RADIAL_RING_STEPS; i++) {
		ang = i * (TRIG_TURN / RADIAL_RING_STEPS);
		vel[0] = syCos(ang) * 3 / 2;
		vel[1] = 0;
		sine = sySin(ang);
		vel[2] = sine;
		fdEffectSpawnResident(origin->x, origin->y, origin->z, vel[0], vel[1],
			sine,
			EFFECT_USE_UPDATE | EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE | 1,
			&options);
	}
}

/* Per-tick update: wanders away from the anchor, 2 to 3 pixels a tick, now
   and then turning sharply; after 101 ticks it hands over to the shared
   particle script. */
void fdEffectWanderAroundAnchor(ACTOR *effect)
{
	FIXED  radius;
	Sint32 ang;
	FIXED  dx;
	FIXED  dz;

	radius = syRandom() + toFIXED(2);
	ang = effect->facing;
	dx = syMulFX(radius, syCos(ang));
	dz = syMulFX(radius, sySin(ang));
	effect->x.fixed += dx;
	effect->z.fixed += dz;
	/* A small backwards drift, a 4096th of a turn a tick. */
	effect->facing += -(TRIG_TURN / 4096);
	if(effect->turnFrm != 0) {
		effect->turnFrm--;
		effect->facing += TRIG_TURN / 32;
	} else if(syRandomBelow(32) == 0) {			 /* one chance in 32 */
		effect->turnFrm = syRandomBelow(16) + 8; /* 8 to 23 ticks */
	}
	if(++effect->swirlFrm == WHIRLWIND_WANDER_TICKS)
		fdActorStartScriptStub(effect, FieldEffectReleaseScript);
}

/* Per-tick update of a Whirlwind particle: circles the anchor at 8 pixels, a
   32nd of a turn a tick; after 121 ticks it starts to wander
   (fdEffectWanderAroundAnchor) with upward speed and gravity. As The Broken
   Seal's fdEffectCircleAnchor. */
void fdEffectCircleAnchor(ACTOR *effect)
{
	ACTOR *anchor;
	Sint32 angle;
	FIXED  radius;
	FIXED  dx;
	FIXED  dz;

	anchor = &effect->watchedActor->actor;
	radius = WHIRLWIND_ORBIT_RADIUS;
	angle = effect->facing;
	dx = syMulFX(radius, syCos(angle));
	dz = syMulFX(radius, sySin(angle));
	effect->x.fixed = anchor->x.fixed + dx;
	effect->z.fixed = anchor->z.fixed + dz;
	effect->facing += WHIRLWIND_ORBIT_ANGLE_STEP;
	if(++effect->swirlFrm == WHIRLWIND_ORBIT_TICKS) {
		effect->update = (OBJECT_UPDATE)fdEffectWanderAroundAnchor;
		effect->swirlFrm = 0;
		effect->turnFrm = 0;
		effect->gravity = toFIXED(1) / 10;
		effect->velY = toFIXED(3);
		effect->facing = syRandom();
	}
}

/* Per-tick update of Whirlwind's target: it shrinks a 512th a tick and, below
   half its size, is put away at the map's origin. */
void fdWhirlwindUpdateTarget(ACTOR *actor)
{
	Sint32 scale;

	scale = actor->scaleX - WHIRLWIND_TARGET_SCALE_STEP;
	actor->scaleY = scale;
	actor->scaleX = scale;
	if(scale < WHIRLWIND_TARGET_MIN_SCALE) {
		fdActorSetPosAndResetMotionStub(actor, 0, 0, 0);
		actor->update = 0;
	}
}

/* Runs Whirlwind cast on a target, using the target already stored in the field
   effect. */
void fdPsynergyWhirlwindOnTarget(Sint32 unused)
{
	fdPsynergyWhirlwind();
}

void fdWhirlwindUpdateSpentOrb(ACTOR *actor);

/* Whirlwind: a whirlwind orb flies from the caster to the spot over ten ticks,
   growing from a quarter size. With nothing to blow it whooshes away; on a
   target, leaves circle it while the target shrinks away; on sprouting leaves
   the leaves scatter, and there or on a seedling the vine swings
   (fdHangingVineSwing). */
void fdPsynergyWhirlwind(void)
{
	FIELD_EFFECT_STATE *scene = RamHeapSlots->fieldEffState;
	ACTOR			   *caster = scene->caster;
	ACTOR			   *tgt = scene->tgt;
	SPR				   *spr = tgt->spr;
	VECTOR3				pos;
	VECTOR3				from;
	VECTOR3				to;
	ACTOR			   *orb;
	ACTOR			   *leaf;
	Sint32				kind = ORB_PLAIN;
	Sint32				step;
	Sint32				i;
	Sint32				ticks = WHIRLWIND_TRAVEL_TICKS + 1;

	if(scene->sceneVariant != 0)
		return;
	if(scene->aimAhead == 0) {
		Sint32 anm;

		kind = ORB_ORBITING;
		anm = spr->entries[0]->anmId;
		if(anm == SPRITE_SPROUTING_LEAVES)
			kind = ORB_WANDER;
		if(anm == SPRITE_SEEDLING)
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

		SET_ACTOR_POSITION(orb,
			from.x + (to.x - from.x) * step / WHIRLWIND_TRAVEL_TICKS,
			from.y + (to.y - from.y) * step / WHIRLWIND_TRAVEL_TICKS,
			from.z + (to.z - from.z) * step / WHIRLWIND_TRAVEL_TICKS);
		/* from 0.25 to 1.3 */
		scale =
			step * (toFIXED(21) / 20) / WHIRLWIND_TRAVEL_TICKS + toFIXED(1) / 4;
		orb->scaleX = scale;
		orb->scaleY = scale;
		sySynch(1);
		step++;
	} while(step < ticks);
	orb->scaleX = toFIXED(17) / 10;
	orb->scaleY = toFIXED(13) / 10;
	sySndPlayStub(SE_RUSH);

	if(kind == ORB_PLAIN) {
		fdActorSelectAnimationStub(orb, 1);
		sySynch(30);
		sySndPlayStub(SE_WHOOSH);
		fdActorWaitForTrackEndStub(orb);
		fdActorReleaseStub(orb);
	} else if(kind == ORB_ORBITING) {
		/* Sixteen leaves, one every six ticks, 4 pixels above the orb and 0.8
		   higher each, 3 to 8 pixels out, circle it while the target
		   shrinks. */
		sySynch(20);
		if(tgt != 0)
			tgt->update = (OBJECT_UPDATE)fdWhirlwindUpdateTarget;
		for(i = 0; i < WHIRLWIND_LEAF_COUNT; i++) {
			Sint32 distance;

			SET_POSITION(&pos, orb->x.fixed,
				orb->y.fixed + i * (toFIXED(4) / 5) + toFIXED(4), orb->z.fixed);
			distance = syRandom() * 5 + toFIXED(3);
			syVectorAddPolarOffset(distance, syRandom(), &pos);
			leaf = fdActorSpawn(SPRITE_LEAF, pos.x, pos.y, pos.z);
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
	} else if(kind == ORB_WANDER || kind == ORB_HANDOFF) {
		sySynch(5);
		fdActorSelectAnimationStub(orb, 1);
		if(kind == ORB_WANDER) {
			/* Sixteen leaves that wander off the orb, the first handed to the
			   shared particle script soonest. */
			for(i = 0; i < WHIRLWIND_LEAF_COUNT; i++) {
				Sint32 distance;

				SET_POSITION(&pos, orb->x.fixed,
					orb->y.fixed + i * (toFIXED(4) / 5) + toFIXED(4),
					orb->z.fixed);
				distance = syRandom() * 5;
				distance += toFIXED(3);
				syVectorAddPolarOffset(distance, syRandom(), &pos);
				leaf = fdActorSpawn(SPRITE_LEAF, pos.x, pos.y, pos.z);
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

/* Per-tick update of Whirlwind's orb once it has done its work: it whooshes on
   its 80th tick. */
void fdWhirlwindUpdateSpentOrb(ACTOR *actor)
{
	if(actor->tmr == 80) {
		actor->update = 0;
		sySndPlayStub(SE_WHOOSH);
	}
	actor->tmr++;
}

/* Field effect sprite callback (OUT_AND_BACK_STATE): the particle heads for a
   point 40 to 70 pixels from where it appeared, then for a point 4 pixels round
   8 pixels above the Psynergy's position on screen, and is freed on arrival. */
void fdEffectUpdateHaltParticle(FIELD_PARTICLE *effect)
{
	FIELD_EFFECT_STATE *effectState;
	VECTOR3				pos;
	Sint16				angle;
	Sint32				state;

	effectState = RamHeapSlots->fieldEffState;
top:
	state = effect->state;
	if(state == OUT_AND_BACK_AIM_OUT) {
		pos.x = effect->originX;
		pos.z = effect->originZ;
		angle = syRandom();
		syVectorAddPolarOffset(
			syRandom() * 30 + toFIXED(40), (Uint16)angle, &pos);
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
		fdCameraWorldToScreen(&pos);
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
void fdPsynergyHaltOnTarget(Sint32 unused)
{
	ACTOR *tgt = ((FIELD_EFFECT_STATE *)RamHeapSlots->fieldEffState)->tgt;

	tgt->paused = 1;
	fdPsynergyHalt();
}

/* A thrown stopwatch's or hand's steps, a tenth of the way a tick: it arrives
   on the 11th tick. */
#define CAST_THROW_STEPS 10
#define HALT_PARTICLES	 16

/* Halt: throws a stopwatch (SPRITE_STOPWATCH) from the caster to the Psynergy's
   position over CAST_THROW_STEPS ticks, ticks three low tones and a high one,
   and scatters it into HALT_PARTICLES particles. */
void fdPsynergyHalt(void)
{
	FIELD_EFFECT_STATE *effectState = RamHeapSlots->fieldEffState;
	ACTOR			   *caster = effectState->caster;
	ACTOR			   *orb;
	FIELD_PARTICLE	   *effect;
	VECTOR3				pos;
	VECTOR3				from;
	VECTOR3				to;
	Sint32				step;
	Sint32				i;

	effectState->y = caster->y.fixed;
	orb = fdActorSpawn(SPRITE_STOPWATCH, 0, 0, 0);
	step = 0;
	fdActorSelectAnimationStub(orb, 0);
	if(orb == 0)
		return;

	/* From 16 pixels above the caster to 8 pixels above the Psynergy's
	   position. */
	fdPsynergyFadeToCastCol();
	from.x = caster->x.fixed;
	from.y = caster->y.fixed + toFIXED(16);
	from.z = caster->z.fixed;
	to.x = effectState->x;
	to.y = effectState->y + toFIXED(8);
	to.z = effectState->z;
	/* Grown to full size as it arrives. */
	for(;;) {
		Sint32 steps = CAST_THROW_STEPS + 1;
		Sint32 scale;

		SET_ACTOR_POSITION(orb,
			from.x + (to.x - from.x) * step / CAST_THROW_STEPS,
			from.y + (to.y - from.y) * step / CAST_THROW_STEPS,
			from.z + (to.z - from.z) * step / CAST_THROW_STEPS);
		/* from 0.25 to full */
		scale = fdEffectInterpolate(toFIXED(1), toFIXED(1) / 4, step);
		orb->scaleX = scale;
		orb->scaleY = scale;
		sySynch(1);
		step++;
		if(step >= steps)
			break;
	}

	/* Three low tones and a high one, 10 ticks apart, as the orb changes to
	   its second animation. */
	sySynch(5);
	fdActorSelectAnimationStub(orb, 1);
	sySndPlayStub(SE_TRIPLE_TONE_LOW);
	sySynch(10);
	sySndPlayStub(SE_TRIPLE_TONE_LOW);
	sySynch(10);
	sySndPlayStub(SE_TRIPLE_TONE_LOW);
	sySynch(10);
	sySndPlayStub(SE_TRIPLE_TONE_HIGH);

	/* Particles 4 pixels round it fly out, then come back to the Psynergy's
	   position. */
	effect = &effectState->particles[0];
	for(i = 0; i < HALT_PARTICLES; i++) {
		pos.x = orb->x.fixed;
		pos.y = orb->y.fixed + toFIXED(8);
		pos.z = orb->z.fixed;
		fdCameraWorldToScreen((VECTOR3 *)&pos.x);
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

/* Per-tick update of an actor held by Halt: its sprite flickers through the
   colour modes of HaltFlickerColorModes at random. */
void fdPsynergyUpdateHaltedActor(ACTOR *actor)
{
	const Sint8 *tbl = HaltFlickerColorModes;
	Sint32		 random = syRandom();
	fdActorSetSprColModeStub(actor, tbl[(Uint32)(random * 8) >> 16]);
}

/* Holds an actor still under Halt, keeping its per-tick update and its first
   animation's colour mode in MainRoutineStateWork for
   fdPsynergyResumeHaltedActor. */
void fdPsynergyHaltActor(Sint32 actorId)
{
	ACTOR		  *actor;
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

/* Lets a halted actor move again, with the update and colour mode kept by
   fdPsynergyHaltActor. */
void fdPsynergyResumeHaltedActor(Sint32 actorId)
{
	ACTOR *actor = fdActorLookup(actorId);

	if(actor != NULL) {
		if(actor->update == (OBJECT_UPDATE)fdPsynergyUpdateHaltedActor) {
			actor->update = MainRoutineStateWork.pausedUpdate;
			MainRoutineStateWork.pausedUpdate = NULL;
			fdActorSetSprColModeStub(
				actor, (Sint8)MainRoutineStateWork.pausedColMode);
		}
		actor->paused = 0;
		fdActorSetAnimationSpeedStub(actor, ANIM_STEP_NORMAL);
	}
}

Sint32 fdSpeakerGetHaltedLines(Sint32 id);

/* Returns no target (-1) when Halt passes actor id over (its sprite has no
   halted lines, SPEAKER_HALTED_LINES_NONE), else id. */
Sint32 fdPsynergyFilterHaltTarget(Sint32 id)
{
	if(fdSpeakerGetHaltedLines(id) == SPEAKER_HALTED_LINES_NONE)
		return -1;
	return id;
}

/* Catch cast on a target: runs fdPsynergyCatch. */
void fdPsynergyCatchOnTarget(Sint32 unused)
{
	fdPsynergyCatch();
}

/* The value step tenths of the way from from to to. */
static __inline__ Sint32 Interpolate(Sint32 from, Sint32 to, Sint32 step)
{
	return from + (to - from) * step / CAST_THROW_STEPS;
}

/* The ticks Catch's hand presses down, 2 pixels a tick, and rises again, 1.5
   pixels a tick. */
#define CATCH_PRESS_TICKS 10
#define CATCH_RISE_TICKS  13

/* Catch: carries the hand (SPRITE_MOVE_HAND) from 16 pixels above the caster to
   the goal in 10 steps, growing from a quarter to full size, presses down 20
   pixels moving the target to (-9, -9), rises 19.5 pixels and shrinks back. */
void fdPsynergyCatch(void)
{
	FIELD_EFFECT_STATE *scene = RamHeapSlots->fieldEffState;
	ACTOR			   *mainActor = scene->caster;
	ACTOR			   *secondary = scene->tgt;
	ACTOR			   *actor;
	VECTOR3				spawn;
	VECTOR3				origin;
	VECTOR3				tgt;
	Sint32				step;
	Sint32				idx;
	Sint32				steps = CAST_THROW_STEPS + 1;

	SET_POSITION(&origin, mainActor->x.fixed, mainActor->y.fixed + toFIXED(16),
		mainActor->z.fixed);
	if(scene->aimAhead) {
		SET_POSITION(&tgt, mainActor->x.fixed, mainActor->y.fixed + toFIXED(32),
			mainActor->z.fixed);
		syVectorAddPolarOffset(toFIXED(32), scene->ang, &tgt.x);
	} else {
		SET_POSITION(&tgt, scene->x, scene->y + toFIXED(32), scene->z);
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
		Sint32 scale;

		SET_ACTOR_POSITION(actor,
			origin.x + (tgt.x - origin.x) * step / CAST_THROW_STEPS,
			origin.y + (tgt.y - origin.y) * step / CAST_THROW_STEPS,
			origin.z + (tgt.z - origin.z) * step / CAST_THROW_STEPS);
		scale = Interpolate(toFIXED(1) / 4, toFIXED(1), step);
		actor->scaleX = scale;
		actor->scaleY = scale;
		sySynch(1);
		step++;
	} while(step < steps);
	sySynch(10);
	fdActorSelectAnimationStub(actor, MOVE_HAND_ANIMATION_CLENCH);
	sySynch(15);
	for(idx = 0; idx < CATCH_PRESS_TICKS; idx++) {
		actor->y.fixed -= toFIXED(2);
		sySynch(1);
	}
	fdActorSelectAnimationStub(actor, MOVE_HAND_ANIMATION_FIST);
	sySndPlayStub(SE_WATER);
	if(secondary != 0)
		fdActorSetPosAndResetMotionStub(
			secondary, -toFIXED(9), secondary->y.fixed, -toFIXED(9));
	sySynch(20);
	for(idx = 0; idx < CATCH_RISE_TICKS; idx++) {
		actor->y.fixed += toFIXED(3) / 2;
		sySynch(1);
	}
	sySynch(10);
	sySndPlayStub(SE_MENU_ERROR);
	step = 0;
	do {
		Sint32 scale;

		SET_ACTOR_POSITION(actor,
			tgt.x + (origin.x - tgt.x) * step / CAST_THROW_STEPS,
			tgt.y + (origin.y - tgt.y) * step / CAST_THROW_STEPS,
			tgt.z + (origin.z - tgt.z) * step / CAST_THROW_STEPS);
		scale = Interpolate(toFIXED(1), toFIXED(1) / 4, step);
		actor->scaleX = scale;
		actor->scaleY = scale;
		sySynch(1);
		step++;
	} while(step < steps);
	fdActorReleaseStub(actor);
	fdPsynergyFadeFromCastCol();
}

void fdRopeGetEndNodePos(Sint32 tgt, VECTOR3 *pos);

/* The stages of Lash's hand on a rope (SCATTER_STATE.phase). */
typedef enum LASH_PHASE {
	LASH_HAND_SHAKES = 0, /* trembling over the rope's end */
	LASH_HAND_STILL = 1,
	LASH_HAND_PULLS = 2, /* on the rope's end until the rope is tied */
	LASH_HAND_RISES = 3,
	LASH_HAND_GONE = 4 /* its sparkles die away */
} LASH_PHASE;

/* Event of the Lash on a rope: the hand trembles, takes the rope's end and
   rises away, scattering sparkles all the while. */
void fdPsynergyUpdateLash(void)
{
	SCATTER_STATE			*state = (SCATTER_STATE *)RamHeapSlots->battleEff;
	FIELD_EFFECT_STATE		*scene = RamHeapSlots->fieldEffState;
	FIELD_STATE				*field = RamHeapSlots->fieldState;
	ACTOR					*actor = state->actor;
	VECTOR3					 pos;
	EFFECT_SPR				*spr;
	FIELD_PSYNERGY_PARTICLE *particle;
	Sint32					 i;

	state->flash = 0;
	for(i = 0; i < state->spawnCnt; i++) {
		FIXED  distance;
		Sint32 speed;

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
		Sint32 tmr = particle->tmr;

		if(tmr >= 0) {
			spr = &state->sprs[i];
			if(i & 1)
				spr->oam.idx = state->tileA + (tmr & 3) * 2;
			else
				spr->oam.idx = state->tileB + (tmr & 3) * 2;
			fdEffectSprDrawAtWorldPos(spr, particle);
			fdAdvanceWithGravity3D(particle, 63, -(toFIXED(5) / 64));
			if(++particle->tmr == 32)
				particle->tmr = -1;
		}
	}

	switch(state->phase) {
	case LASH_HAND_SHAKES: {
		Sint32 a = syRandom();
		Sint32 b = syRandom();
		Sint32 c, d;

		pos.x = scene->x + a * 3 - b * 3;
		c = syRandom();
		d = syRandom();
		pos.y = scene->y + c * 3 - d * 3;
		pos.z = scene->z;
		fdActorSetPosAndResetMotionStub(actor, pos.x, pos.y, pos.z);
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
		fdRopeGetEndNodePos(field->tgtX, &pos);
		SET_ACTOR_POSITION(actor, pos.x, pos.y - toFIXED(4), pos.z);
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

/* Lash cast on a target: finds the rope's two ends (fdRopeFindById); with none,
   Lash runs as if cast on nothing (enabled), else the field state keeps the
   rope and its ends. Then runs fdPsynergyLash. */
void fdPsynergyLashOnTarget(Sint32 value)
{
	FIELD_STATE		   *field = RamHeapSlots->fieldState;
	FIELD_EFFECT_STATE *state = RamHeapSlots->fieldEffState;
	Sint32				second;
	Sint32				first;

	if(fdRopeFindById(value, &first, &second)) {
		state->aimAhead = 1;
	} else {
		field->tgtY = value;
		field->tgtZ = first;
		field->tgtX = second;
	}
	fdPsynergyLash();
}

/* The ticks Lash's hand comes down, 2 pixels a tick, and the sparkles it
   bursts into with no target. */
#define LASH_FALL_TICKS 16
#define LASH_SPARKLES	16

/* Lash: a hand forms over the spot and comes down on it. With no target it
   bursts into sparkles; cast on a rope's end it whips out and ties the rope
   (fdRopeLash) at the script's target. */
void fdPsynergyLash(void)
{
	SCATTER_STATE *state =
		syHeapSlotAlloc(HEAP_SLOT_BATTLE_EFFECT, sizeof(SCATTER_STATE));
	FIELD_EFFECT_STATE		*scene = RamHeapSlots->fieldEffState;
	FIELD_STATE				*field = RamHeapSlots->fieldState;
	ACTOR					*caster = scene->caster;
	ACTOR					*actor;
	EFFECT_SPR				*spr;
	FIELD_PSYNERGY_PARTICLE *particle;
	Sint32					 slotA;
	Sint32					 slotB;
	Sint32					 tileA;
	Sint32					 i;

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
		syDecompressTaggedLZ(
			syResourceGetTableEntry(RESOURCE_SPARKLE_FADE), state);
		slotA = syVRAMCacheFindFreeEntry();
		tileA = syVRAMCacheLoad(slotA, 256, state);
		state->tileA = tileA;
		syDecompressTaggedLZ(
			syResourceGetTableEntry(RESOURCE_STAR_SPARKLE_FADE), state);
		slotB = syVRAMCacheFindFreeEntry();
		state->tileB = syVRAMCacheLoad(slotB, 256, state);

		particle = state->particles;
		spr = state->sprs;
		for(i = 0; i < SCATTER_PARTICLE_COUNT; i++) {
			fdEffectSprInit(spr, 4, 4, 0, tileA);
			spr->depth = EFFECT_SPRITE_FRONT_DEPTH;
			spr->oam.pri = 0;
			spr->oam.fullCol = 1;
			spr->oam.pal = 0;
			SET_POSITION(
				particle, actor->x.fixed, actor->y.fixed, actor->z.fixed);
			syVectorAddPolarOffset(toFIXED(12), syRandom(), particle);
			particle->tmr = -1;
			spr++;
			particle++;
		}

		fdPsynergyGrowFromCaster(
			actor, scene->x, scene->y + toFIXED(32), scene->z);
		fdActorSelectAnimationStub(actor, 1);
		sySynch(10);
		for(i = 0; i < LASH_FALL_TICKS; i++) {
			actor->y.fixed += -toFIXED(2);
			sySynch(1);
		}
		sySndPlayStub(SE_WATER);
		if(scene->aimAhead != 0) {
			for(i = 0; i < LASH_SPARKLES; i++)
				fdBattleEffectSpawnScatterParticle(
					actor->x.fixed, actor->y.fixed, actor->z.fixed);
			sySynch(1);
		} else {
			sySynch(6);
			state->actor = actor;
			state->phase = LASH_HAND_SHAKES;
			state->tmr = 0;
			state->next = 0;
			state->spawnCnt = 1;
			state->hit = 0;
			sySetEvent(fdPsynergyUpdateLash, EVENT_PASS_TICK);
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
		syVRAMCacheFreeEntry(slotA);
		syVRAMCacheFreeEntry(slotB);
		fdActorReleaseStub(actor);
	}
	fdPsynergyFadeFromCastCol();
	syHeapSlotRelease(HEAP_SLOT_BATTLE_EFFECT);
}

/* Per-tick update: spins by FACING_STEP + timer * 80 a tick, speeding up
   (timer counts up) until timer * 80 reaches FACING_STEP: from 1/16 turn a
   tick to just over 1/8 in 52 ticks, as fdEffectAdvanceAngWithRamp. */
void fdEffectUpdateSpinAng(ACTOR *actor)
{
	Uint32 step;

	step = actor->tmr * 80;
	actor->facing = actor->facing + step + FACING_STEP;
	if(step < FACING_STEP)
		actor->tmr = actor->tmr + 1;
}

/* Per-tick update: spins 1/8 turn a tick as it shrinks, rising a pixel a
   tick; its sprite is hidden once its scale falls below 3/16. */
void fdEffectUpdateRetreatCaster(ACTOR *actor)
{
	Uint32 scale;

	actor->scaleY -= toFIXED(5) / 512;
	scale = actor->scaleX - toFIXED(5) / 512;
	actor->facing += DEGtoANG(45);
	actor->y.fixed += toFIXED(1);
	actor->scaleX = scale;
	if((Sint32)scale < toFIXED(3) / 16)
		actor->sprStorageKind = ACTOR_SPRITE_NONE;
}

/* Field effect sprite callback: a particle flies 120 pixels from the main
   actor's screen position (8 pixels lower) in a direction within 1/8 turn of up
   the screen, at up to 5 pixels a tick, and starts again from the main actor
   on arrival until it takes its last flight. */
void fdRetreatUpdateSpark(FIELD_PARTICLE *effect)
{
	VECTOR3 pos;
	ACTOR  *main;
	Sint32	state;
	Uint32	spread;

	main = ((FIELD_EFFECT_STATE *)RamHeapSlots->fieldEffState)->caster;
	state = effect->state;

	if(state == RETREAT_PARTICLE_AIM) {
		pos.x = main->x.fixed;
		pos.y = main->y.fixed;
		pos.z = main->z.fixed;
		fdCameraWorldToScreen(&pos);

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

		if((TickCount & 2) != 0)
			sySndPlayStub(SE_GLITTER);
	} else if(state == RETREAT_PARTICLE_FLYING) {
		if(fdParticleHasPendingTarget(effect) == 0)
			effect->state--;
	} else if(state == RETREAT_PARTICLE_LAST_FLIGHT) {
		if(fdParticleHasPendingTarget(effect) == 0)
			fdEffectClearOwnedSlot(effect);
	}
}

/* Retreat ("Return to the dungeon's entrance."): the caster turns to face the
   player, raises their arms, spins faster and faster and shrinks away while
   sparks fly up the screen, one a tick. */
void fdPsynergyRetreat(void)
{
	FIELD_EFFECT_STATE *effectState = RamHeapSlots->fieldEffState;
	ACTOR			   *actor = effectState->caster;
	VECTOR3				pos;
	FIELD_PARTICLE	   *effect;
	FIELD_PARTICLE	   *effects;
	Uint32				i;

	fdScriptStart();
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
	fdCameraWorldToScreen(&pos);
	i = 0;
	effect = effectState->particles;
	effects = effect;
	do {
		FIXED scale;

		fdParticleInit(effect, SPRITE_PARTICLE, pos.x, pos.z);
		fdParticleSetCallback(effect, fdRetreatUpdateSpark);
		fdParticleSetAnimation(effect, ANIM_PARTICLE_TWINKLE);
		fdSprSetAnimationColModesStub(effect->spr, syRandomBelow(7));
		scale = (syRandom() >> 1) + toFIXED(6) / 5; /* 1.2 to 1.7 */
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

/* The back half of one of Avoid's falling rings, a golden arc
   (FieldEffectArcSparkTiles) behind the caster: it grows to full size by 3/64
   a tick as it falls 2 pixels a tick around the caster, 9 pixels south of
   them and up to 5 more while small, and is gone 10 pixels above the ground (4
   on the World Map). */
void fdAvoidUpdateRingBackHalf(ACTOR *particle)
{
	ACTOR *src;
	FIXED  y;
	FIXED  limit;
	FIXED  scale;

	limit = particle->groundHeight + toFIXED(10);
	src = &particle->watchedActor->actor;
	if(MainRoutineStateWork.mapId == SCENE_CLEAR)
		limit = particle->groundHeight + toFIXED(4);

	y = particle->y.fixed;
	if(y <= limit) {
		fdActorReleaseStub(particle);
		return;
	}

	scale = particle->scaleX + toFIXED(3) / 64;
	if(scale > toFIXED(1))
		scale = toFIXED(1);
	particle->scaleX = scale;
	particle->scaleY = scale;
	particle->x.fixed = src->x.fixed;
	particle->y.fixed = y - toFIXED(2);
	particle->z.fixed = src->z.fixed + ((toFIXED(1) - scale) * 5) + toFIXED(9);
}

/* The ring's front half: the same arc drawn upside down in front of the caster,
   16 pixels south and up to 5 nearer while small. */
void fdAvoidUpdateRingFrontHalf(ACTOR *particle)
{
	ACTOR *src;
	FIXED  y;
	FIXED  limit;
	FIXED  scale;

	limit = particle->groundHeight + toFIXED(10);
	src = &particle->watchedActor->actor;
	if(MainRoutineStateWork.mapId == SCENE_CLEAR)
		limit = particle->groundHeight + toFIXED(4);

	y = particle->y.fixed;
	if(y <= limit) {
		fdActorReleaseStub(particle);
		return;
	}

	scale = particle->scaleX + toFIXED(3) / 64;
	if(scale > toFIXED(1))
		scale = toFIXED(1);
	particle->scaleX = scale;
	particle->scaleY = -scale;
	particle->x.fixed = src->x.fixed;
	particle->y.fixed = y - toFIXED(2);
	particle->z.fixed = src->z.fixed - ((toFIXED(1) - scale) * 5) + toFIXED(16);
}

/* Drops one of Avoid's rings onto source: its two halves start 64 pixels above
   it at 0.4 of full size, drawn with the arc's tiles
   (FIELD_EFFECT_STATE.arcTileSlot), the front half over every sprite. */
void fdAvoidDropRing(ACTOR *src)
{
	FIELD_EFFECT_STATE *effectState = RamHeapSlots->fieldEffState;
	ACTOR			   *leader = effectState->caster;
	ACTOR			   *actors[2];
	ACTOR			   *actor;
	DESCENDING_ARC_SPR *spr;
	Sint32				arcIdx;

	for(arcIdx = 0; arcIdx <= 1; arcIdx++) {
		actor = fdActorCreateStub(SPRITE_GOLDEN_RINGS, src->x.fixed,
			src->y.fixed + toFIXED(64), src->z.fixed);
		actors[arcIdx] = actor;
		if(actor == NULL)
			continue;
		actor->groundHeight = src->groundHeight;
		spr = (DESCENDING_ARC_SPR *)actor->spr;
		actor->motionFlg = 0;
		actor->tmr = 0;
		actor->watchedActor = (FIELD_OBJECT *)src;
		actor->scaleY = toFIXED(2) / 5;
		actor->scaleX = toFIXED(2) / 5;
		if(spr == NULL)
			continue;
		fdSprStartAnimationStub((SPR *)spr, 0);
		spr->flg = 0;
		syVRAMCacheFreeEntry(spr->slot);
		spr->slot = effectState->arcTileSlot;
		spr->retainCacheEntry = 1;
		spr->attr.half.tile = VramCacheEntries[spr->slot].ofs >> 5;
		spr->fullCol = 0;
		spr->shape = 1;
		spr->size = 2;
		spr->firstEntry->picture = 0;
	}
	/* BUG: failed allocations above leave a missing arc or sprite here. */
	actors[0]->update = (OBJECT_UPDATE)fdAvoidUpdateRingFrontHalf;
	((DESCENDING_ARC_SPR *)actors[0]->spr)->attr.byte.pri = 0;
	actors[1]->update = (OBJECT_UPDATE)fdAvoidUpdateRingBackHalf;
	((DESCENDING_ARC_SPR *)actors[1]->spr)->attr.byte.pri =
		((DESCENDING_ARC_SPR *)leader->spr)->attr.byte.pri;
}

/* The caster during Avoid (fdPsynergyAvoid): drops a ring onto itself every 5
   ticks (7 on the World Map), and spins in phase 1, 3/64 turn a tick. */
void fdEffectUpdateAvoidCaster(ACTOR *caster)
{
	Sint16 phase;
	Sint32 elapsedTicks;

	phase = caster->arcPhase;
	elapsedTicks = caster->arcFrm++;

	if(MainRoutineStateWork.mapId == SCENE_CLEAR) {
		if(elapsedTicks % 7 == 0)
			fdAvoidDropRing(caster);
	} else if(elapsedTicks % 5 == 0) {
		fdAvoidDropRing(caster);
	}

	if(phase == 1)
		caster->facing += TRIG_TURN * 3 / 64;
}

/* Avoid (or the Sacred Feather) has worn off, and the game says so. "Avoid's
   effects wore off." (or "Sacred Feather's ...").

   Clears MainRoutineStateWork.avoidDistance. Reached through
   fdPsynergyRunExtraHandler when the Psynergy request the leader's walking
   raises as Avoid runs out (field_mode_walking.c) is handled. */
void fdPsynergyShowAvoidWoreOff(void)
{
	MainRoutineStateWork.avoidDistance = 0;
	fdWorldMapFreeze();
	if(MainRoutineStateWork.avoidByItem == 0) {
		mnMessageArgumentPushStub(ABILITY_AVOID, MESSAGE_ARGUMENT_ABILITY);
		mnFieldMessageShowStub((Sint32)&MsgAbilityWoreOff, FIELD_MESSAGE_ZOOM);
	} else {
		mnMessageArgumentPushStub(ITEM_SACRED_FEATHER, MESSAGE_ARGUMENT_ITEM);
		mnFieldMessageShowStub((Sint32)&MsgItemWoreOff, FIELD_MESSAGE_ZOOM);
	}
	fdWorldMapReloadPendingAreaGraphics();
}

/* Avoid's flashes, 5 ticks each; then the ticks its outline blinks, the chime
   coming on the 15th. */
#define AVOID_FLASHES	  20
#define AVOID_BLINK_TICKS 70
#define AVOID_CHIME_TICK  15

/* Avoid: the caster spins and flashes, then blinks, and encounters are kept
   away until Avoid wears off. */
void fdPsynergyAvoid(void)
{
	FIELD_EFFECT_STATE *effectState;
	ACTOR			   *actor;
	SPR				   *group;
	SPR_ANIMATION	   *entry;
	const Uint8		   *tiles;
	Uint32				ang;
	Sint32				flg;
	Sint32				slot;
	Sint32				cnt;
	Sint32				phase;

	effectState = RamHeapSlots->fieldEffState;
	actor = effectState->caster;
	group = actor->spr;
	entry = group->entries[0];
	ang = actor->facing;
	flg = group->flg;
	slot = syVRAMCacheFindFreeEntry();
	/* The arcs' tiles (256 bytes) stay loaded until the Psynergy's end, and
	   monsters stay away for the next 2,400 pixels the leader walks. */
	effectState->arcTileSlot = slot;
	tiles = FieldEffectArcSparkTiles;
	syVRAMCacheLoad((Sint16)slot, 256, tiles);
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
	/* Flash white for 2 ticks in every 5. */
	for(cnt = 0; cnt < AVOID_FLASHES; cnt++) {
		entry->colMode = SPRITE_COLOR_WHITE;
		group->dirty = 1;
		sySynch(2);
		entry->colMode = SPRITE_COLOR_NORMAL;
		group->dirty = 1;
		group->flg = SPRITE_FLAG_SHADOW;
		sySynch(3);
	}
	actor->update = NULL;
	actor->facing = ang;

	/* Blink its outline beside its own flags on a five-tick cycle. */
	for(cnt = 0; cnt < AVOID_BLINK_TICKS; cnt++) {
		phase = cnt % 5;
		if(phase == 0) {
			group->dirty = 1;
			group->flg = flg | SPRITE_FLAG_OUTLINED;
		} else if(phase == 2) {
			group->dirty = 1;
			group->flg = flg;
			entry->colMode = SPRITE_COLOR_NORMAL;
		}
		if(cnt == AVOID_CHIME_TICK)
			sySndPlayStub(SE_NOTE_MID);
		sySynch(1);
	}
	group->flg = flg;
	group->dirty = 1;
	{
		Uint32 cleared = SPRITE_COLOR_NORMAL;

		entry->colMode = cleared;
	}
	sySynch(1);

	syVRAMCacheFreeEntry(effectState->arcTileSlot);
	mnFieldMessageShowStub((Sint32)&MsgMonstersAttackLess, FIELD_MESSAGE_ZOOM);
}

#define ARC_GROUPS_WAIT_TICKS 90

/* Waits, at most ARC_GROUPS_WAIT_TICKS, until both pairs of swelling arcs have
   run their 32 ticks. */
void fdBattleEffectWaitForArcGroups(void)
{
	ARC_EFFECT_STATE *state = RamHeapSlots->arcEff;
	Sint32			  i = 0;

	while(state->age[0] < ARC_GROUP_TICKS || state->age[1] < ARC_GROUP_TICKS) {
		sySynch(1);
		i++;
		if(i >= ARC_GROUPS_WAIT_TICKS)
			return;
	}
}

/* Per-tick update: flashes an actor between colour mode 7 and 0 every 2 ticks
   and spawns the swelling arcs on it every 16. */
void fdEffectFlickerActorAndSpawnArcs(ACTOR *actor)
{
	if((TickCount & 2) != 0) {
		fdActorSetSprColModeStub(actor, SPRITE_COLOR_WHITE);
	} else {
		fdActorSetSprColModeStub(actor, SPRITE_COLOR_NORMAL);
	}
	if((TickCount & 15) == 0) {
		fdBattleEffectSpawnScaledArcObjects(actor);
	}
}

/* Per-tick update: cycles an actor through colour modes 0 to 5, one step every
   2 ticks, and spawns the swelling arcs on it every 16. */
void fdEffectCycleActorAndSpawnArcs(ACTOR *actor)
{
	if(TickCount & 1)
		fdActorSetSprColModeStub(actor, (TickCount >> 1) % 6);
	if((TickCount & 15) == 0)
		fdBattleEffectSpawnScaledArcObjects(actor);
}

/* Gives the caster its look while it casts (CASTER_LOOK). */
void fdBattleEffectSetObjectUpdateMode(ACTOR *actor, Sint32 mode)
{
	if(mode == CASTER_LOOK_NORMAL) {
		actor->update = 0;
		fdActorSetSprColModeStub(actor, SPRITE_COLOR_NORMAL);
	} else if(mode == CASTER_LOOK_FLASHING_WHITE) {
		actor->update = (OBJECT_UPDATE)fdEffectFlickerActorAndSpawnArcs;
	} else {
		actor->update = (OBJECT_UPDATE)fdEffectCycleActorAndSpawnArcs;
	}
}

/* Sets up the current group's two arc sprites from the effect's tiles, at the
   target's priority, then restarts that group's age, records the target and
   switches to the other group. */
void fdBattleEffectSpawnScaledArcObjects(ACTOR *tgt)
{
	ARC_EFFECT_STATE *state = RamHeapSlots->arcEff;
	Sint32			  tiles = VramCacheEntries[state->resource].ofs >> 5;
	Sint32			  group = state->group;
	EFFECT_SPR		 *spr;
	Sint32			  i;

	for(i = 0; i < 2; i++) {
		spr = &state->sprs[group][i];
		fdEffectSprInit(spr, 16, 16, OBJ_SIZE_32X16, tiles);
		spr->oam.fullCol = 0;
		spr->oam.pal = 0;
		spr->oam.pri = fdActorGetSprPriority(tgt);
	}
	state->age[group] = 0;
	state->tgt = tgt;
	state->group ^= 1;
}

/* Event: draws the two pairs of swelling arcs on their actor: each pair swells
   and shrinks over 32 ticks (scale sin(age / 64 turn)) as it rises a pixel a
   tick, the second arc of a pair upside down, one in front of the actor and
   one behind. */
void fdBattleEffectUpdateArcEffect(void)
{
	ARC_EFFECT_STATE *state = RamHeapSlots->arcEff;
	ACTOR			 *tgt = state->tgt;
	Sint32			  group;
	FIXED			  pos[3];
	Sint32			  i;

	for(group = 0; group < 2; group++) {
		FIXED scale = sySin(state->age[group] * (TRIG_TURN / 64));

		if(state->age[group] < 0)
			continue;
		if(state->age[group] < ARC_GROUP_TICKS) {
			EFFECT_SPR *spr;
			FIXED		x, y;

			pos[0] = tgt->x.fixed;
			pos[1] = tgt->y.fixed + toFIXED(state->age[group]);
			pos[2] = tgt->z.fixed;
			fdCameraWorldToScreen(pos);
			x = pos[0];
			y = pos[2];
			for(i = 0; i < 2; i++) {
				spr = &state->sprs[group][i];
				spr->x = x;
				spr->y = y;
				spr->scaleX = scale;
				spr->scaleY = scale;
				if(i == 0) {
					spr->depth = fdActorGetSprDepth(state->tgt) - 1;
				} else {
					spr->depth = fdActorGetSprDepth(state->tgt) + 1;
					spr->y -= toFIXED(1);
					spr->scaleY = -spr->scaleY;
				}
				fdEffectSprDraw(spr);
			}
			state->age[group]++;
		}
	}
}

/* Starts the swelling arcs over a caster: loads their tiles and starts
   fdBattleEffectUpdateArcEffect with no pair showing. */
void fdBattleEffectStartArcEffect(void)
{
	ARC_EFFECT_STATE *state =
		syHeapSlotAlloc(HEAP_SLOT_ARC_EFFECT, sizeof(*state));

	state->resource = syVRAMCacheFindFreeEntry();
	syVRAMCacheLoad(state->resource, 256, FieldEffectSparkTiles);
	state->group = 0;
	state->age[0] = ARC_GROUP_UNSPAWNED;
	state->age[1] = ARC_GROUP_UNSPAWNED;
	state->tgt = 0;
	sySetEvent((Sint32)fdBattleEffectUpdateArcEffect, EVENT_PASS_TICK);
}

/* Ends the swelling arcs: frees their tiles and their state, and stops their
   event. */
void fdBattleEffectEndArcEffect(void)
{
	ARC_EFFECT_STATE *state = RamHeapSlots->arcEff;

	syVRAMCacheFreeEntry(state->resource);
	syCloseEvent((Uint32)fdBattleEffectUpdateArcEffect);
	syHeapSlotRelease(HEAP_SLOT_ARC_EFFECT);
}

/* Moves the state on to its next phase, its timer restarted. */
#define BOW_NEXT_PHASE(state) ((state)->phase++, (state)->tmr = -1)
/* Of the BOW_SPARK_COUNT sparks, those the bow throws off. */
#define BOW_SPARKS_THROWN	  10
/* While the thrown sparks repeat, each starts again after this many ticks. */
#define BOW_SPARK_TICKS		  10

/* The stages of the Arrow (BOW_STATE.phase). */
typedef enum ARROW_PHASE {
	ARROW_BOW_GROWS = 0, /* the bow grows and turns to face ahead */
	ARROW_BOW_DRAWN = 1, /* the arrow is drawn back and loosed */
	ARROW_FLIES = 2,	 /* until it strikes or has flown 80 pixels */
	ARROW_STOPS = 3,
	ARROW_FALLS = 4,	  /* a struck arrow quivers; a spent one shrinks away */
	ARROW_BOW_SHRINKS = 5 /* ends once the bow is gone */
} ARROW_PHASE;

/* Event of the Arrow, the field Psynergy of the unused ability "A??" (named
   here for the bow it draws and the arrow it looses): the bow and the flaming
   arrow. */
void fdPsynergyUpdateArrow(void)
{
	FIELD_EFFECT_STATE *scene = RamHeapSlots->fieldEffState;
	BOW_STATE		   *state = (BOW_STATE *)RamHeapSlots->battleEff;
	CANVAS_SPR_STATE   *canvas = RamHeapSlots->canvasSpr;
	FIELD_STATE		   *field = RamHeapSlots->fieldState;
	ACTOR			   *actor;
	BOW_MOTION		   *motion;
	EFFECT_SPR		   *spr;
	Sint32				pos[3];
	Sint32				objectIdx;
	Sint32				i;
	Sint32				dx;
	Sint32				dz;

again:
	switch(state->phase) {
	case ARROW_BOW_GROWS:
		if(state->tmr == 0) {
			state->sparking = 0;
			state->sparksRepeat = 1;
			state->arrowMoving = 0;
			state->bowGrowing = 1;
			state->frm = BOW_TICK_IDLE;
		} else if(state->bowGrowing == 0) {
			state->frm = 0;
			BOW_NEXT_PHASE(state);
		}
		break;
	case ARROW_BOW_DRAWN:
		if(state->tmr == 0) {
			spr = &state->arrow;
			spr->scaleX = spr->scaleY = toFIXED(1);
			motion = &state->arrowMotion;
			pos[0] = pos[1] = pos[2] = 0;
			syVectorAddPolarOffset(
				toFIXED(3) / 20, scene->ang8 + DEGtoANG(180), pos);
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
		for(objectIdx = 0; objectIdx < FIELD_ACTOR_COUNT; objectIdx++) {
			actor = field->actors[objectIdx];
			if(actor != NULL && actor->script != NULL &&
				(actor->collisionFlg & ACTOR_SOLID) && actor != scene->caster &&
				fdPsynergyCheckOverlap(
					&motion->x, 2, &actor->x.fixed, actor->radius - 2) >= 0) {
				state->done = 1;
				state->result = objectIdx;
				BOW_NEXT_PHASE(state);
				goto again;
			}
		}
		break;
	case ARROW_STOPS:
		if(state->done != 0) {
			state->arrowMoving = 0;
		} else {
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
		spr = &state->arrow;
		if(state->done != 0) {
			if(state->tmr < 32) {
				Sint32 wobble[8];
				Sint32 fade[4];
				Sint32 w;

				memcpy(wobble, ArrowWobbleAngles, sizeof(wobble));
				memcpy(fade, ArrowWobbleFades, sizeof(fade));
				w = wobble[state->tmr & 7];
				spr->ang = scene->ang8 + w * fade[state->tmr / 8] / (TRIG_TURN);
			}
			if(state->tmr == 32)
				state->bowShrinking = 1;
			if(state->tmr == 50) {
				spr->scaleX = spr->scaleY = 0;
				state->arrowMoving = 1;
				state->sparksRepeat = 0;
				BOW_NEXT_PHASE(state);
			}
		} else {
			spr->scaleX += -(toFIXED(3) / 16);
			spr->scaleY = spr->scaleX;
			if(spr->scaleX < 0) {
				spr->scaleX = spr->scaleY = 0;
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
		spr = &state->bow;
		spr->scaleX += toFIXED(1) / 64;
		spr->scaleY = spr->scaleX;
		spr->ang += DEGtoANG(45);
		if(spr->scaleX >= toFIXED(1)) {
			spr->scaleX = spr->scaleY = toFIXED(1);
			spr->ang = scene->ang8;
			state->bowGrowing = 0;
		}
	}
	if(state->bowShrinking != 0) {
		spr = &state->bow;
		spr->scaleX += -(toFIXED(1) / 64);
		spr->scaleY = spr->scaleX;
		spr->ang += DEGtoANG(45);
		if(spr->scaleX <= toFIXED(0.4) - 1) {
			spr->scaleX = spr->scaleY = 0;
			state->bowShrinking = 0;
		}
	}

	/* The bow's drawing pose, by tick: drawn back, then loosed and
	   recovered. */
	if(state->frm != BOW_TICK_IDLE) {
		state->frm++;
		spr = &state->bow;
		switch(state->frm) {
		case 10:
			spr->oam.idx = state->bowTile + BOW_POSE_TILES;
			break;
		case 20:
			spr->oam.idx = state->bowTile + BOW_POSE_TILES * 2;
			break;
		case 54:
			spr->oam.idx = state->bowTile + BOW_POSE_TILES;
			break;
		case 62:
			spr->oam.idx = state->bowTile + BOW_POSE_TILES * 3;
			break;
		case 65:
			spr->oam.idx = state->bowTile + BOW_POSE_TILES * 4;
			break;
		case 68:
			spr->oam.idx = state->bowTile + BOW_POSE_TILES * 5;
			break;
		case 58:
		case 71:
			spr->oam.idx = state->bowTile;
			break;
		case 72:
			state->frm = BOW_TICK_IDLE;
			break;
		}
	}

	motion = &state->bowMotion;
	pos[0] = motion->x;
	pos[1] = motion->y;
	pos[2] = motion->z;
	spr = &state->bow;
	fdCameraWorldToScreen(pos);
	spr->x = pos[0];
	spr->y = pos[2];
	fdEffectSprDraw(spr);
	motion = &state->arrowMotion;
	pos[0] = motion->x;
	pos[1] = motion->y;
	pos[2] = motion->z;
	fdCameraWorldToScreen(pos);
	spr = &state->arrow;
	spr->x = pos[0];
	spr->y = pos[2];
	fdEffectSprDraw(spr);
	if(state->arrowMoving != 0)
		fdAdvanceWithGravity3D(&motion->x, 64, 0);

	if(state->sparking != 0) {
		motion = state->sparks;
		for(i = 0; i < BOW_SPARKS_THROWN; i++) {
			if(motion->delay == 0) {
				Sint32 speed;
				Sint32 ang;

				speed = (syRandom() & 255) + 256;
				ang = (syRandom() & 0x1fff) + TRIG_TURN * 7 / 16;
				motion->x = toFIXED(60);
				motion->y = toFIXED(28);
				motion->velX = syCos(ang) * speed >> 6;
				motion->velY = sySin(ang) * speed >> 6;
			}
			if(motion->delay > 0) {
				Sint32 x = (Sint16)FIXEDtoINT(motion->x);
				Sint32 y = (Sint16)FIXEDtoINT(motion->y);

				fdBattleEffectDrawCanvasBlock(
					ArrowGlintPixels + (motion->delay & 1) * 64, x, y, 8, 8);
				fdAdvanceWithGravity2D(&motion->x, 60, 0);
			}
			motion->delay++;
			if(state->sparksRepeat == 1 && motion->delay == BOW_SPARK_TICKS)
				motion->delay = 0;
			motion++;
		}
	}

	motion = &state->arrowMotion;
	pos[0] = motion->x;
	pos[1] = motion->y;
	pos[2] = motion->z;
	syVectorAddPolarOffset(-toFIXED(36), scene->ang8, pos);
	fdCameraWorldToScreen(pos);
	canvas->x = (Sint16)FIXEDtoINT(pos[0]);
	canvas->y = (Sint16)FIXEDtoINT(pos[2]);
	canvas->command = CANVAS_UPLOAD_AND_DARKEN_FOUR_LEVELS;
	state->tmr++;
}

/* The Arrow cast on a target: runs fdPsynergyArrow. Nothing calls it, as the
   dispatch of Psynergy cast on a target has no case for the Arrow. */
void fdPsynergyArrowOnTarget(void)
{
	fdPsynergyArrow();
}

/* The Arrow: a bow grows in front of the caster and draws, throwing off glints,
   and looses a flaming arrow that flies up to 80 pixels; an actor it strikes
   becomes the field state's psynergyHitObj. */
void fdPsynergyArrow(void)
{
	BOW_STATE *state =
		syHeapSlotAlloc(HEAP_SLOT_BATTLE_EFFECT, sizeof(BOW_STATE));
	FIELD_EFFECT_STATE *scene = RamHeapSlots->fieldEffState;
	FIELD_STATE		   *field = RamHeapSlots->fieldState;
	CANVAS_SPR_STATE   *canvas;
	EFFECT_SPR		   *spr;
	BOW_MOTION		   *motion;
	FIXED				pos[3];
	Sint32				arrowSlot;
	Sint32				bowSlot;
	Sint32				tile;
	Uint16			   *palette;
	Sint32				blue;
	Sint32				shade;
	Sint32				i;

	fdPsynergyFadeToCastCol();
	/* The spark pointer and the sprite pointer are one variable. */
	spr = (EFFECT_SPR *)state->sparks;
	for(i = 0; i < BOW_SPARK_COUNT; i++) {
		((BOW_MOTION *)spr)->delay = -i;
		spr = (EFFECT_SPR *)((BOW_MOTION *)spr + 1);
	}

	state->tiles = syHeapSlotAlloc(HEAP_SLOT_BATTLE_CANVAS, BOW_TILE_BYTES);
	syDecompressTaggedLZ(
		syResourceGetTableEntry(RESOURCE_BOW_DRAW), state->tiles);
	bowSlot = syVRAMCacheFindFreeEntry();
	tile = syVRAMCacheLoad(bowSlot, BOW_TILE_BYTES, state->tiles);
	state->bowTile = tile;
	spr = &state->bow;
	fdEffectSprInit(spr, 16, 16, OBJ_SIZE_32X32, tile);
	spr->oam.fullCol = 1;
	spr->oam.pal = 0;
	spr->oam.pri = fdActorGetSprPriority(scene->caster);
	spr->depth = fdActorGetSprDepth(scene->caster) + 1;
	spr->scaleX = toFIXED(2) / 5;
	spr->scaleY = toFIXED(2) / 5;
	spr->ang = scene->ang8;
	pos[0] = scene->caster->x.fixed;
	pos[2] = scene->caster->z.fixed;
	motion = &state->bowMotion;
	syVectorAddPolarOffset(toFIXED(8), scene->ang8, pos);
	SET_POSITION(motion, pos[0], toFIXED(16), pos[2]);

	syDecompressTaggedLZ(
		syResourceGetTableEntry(RESOURCE_FLAMING_ARROW), state->tiles);
	arrowSlot = syVRAMCacheFindFreeEntry();
	tile = syVRAMCacheLoad(arrowSlot, 256, state->tiles);
	spr = &state->arrow;
	fdEffectSprInit(spr, 12, 8, OBJ_SIZE_16X16, tile);
	spr->oam.fullCol = 1;
	spr->oam.pal = 0;
	spr->oam.pri = fdActorGetSprPriority(scene->caster);
	spr->depth = fdActorGetSprDepth(scene->caster) + 2;
	spr->scaleX = 0;
	spr->scaleY = 0;
	spr->ang = scene->ang8;
	pos[0] = scene->caster->x.fixed;
	pos[2] = scene->caster->z.fixed;
	motion = &state->arrowMotion;
	syVectorAddPolarOffset(toFIXED(18), scene->ang8, pos);
	SET_POSITION(motion, pos[0], toFIXED(16), pos[2]);
	syHeapSlotRelease(HEAP_SLOT_BATTLE_CANVAS);

	fdBattleEffectStartCanvasSpr();
	canvas = RamHeapSlots->canvasSpr;
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
	palette = (Uint16 *)&OBJ_PLTT_COLOR(CANVAS_PALETTE, 0);
	for(i = 0; i < PALETTE_COLORS; i++) {
		shade = i + (RGB_LEVEL_MAX + 1) / 2;
		syIOWrite16(RGB(0, shade, 0) | blue | shade, palette);
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
	sySetEvent(fdPsynergyUpdateArrow, EVENT_PASS_TICK);
	while(state->phase != FIELD_PSYNERGY_FINISHED)
		sySynch(1);
	syCloseEvent(fdPsynergyUpdateArrow);
	if(state->done != 0 && state->result != -1)
		field->psynergyHitObj = state->result;
	syVRAMCacheFreeEntry(arrowSlot);
	syVRAMCacheFreeEntry(bowSlot);
	fdPsynergyFadeFromCastCol();
	fdBattleEffectEndCanvasSpr();
	syHeapSlotRelease(HEAP_SLOT_BATTLE_EFFECT);
}

/* Finds what Cyclone reaches: of the 21 metatiles round the caster
   (FieldPsynergyNearbyMetatileOffsets) on its layer, those of a
   CYCLONE_METATILE_KIND within reach are listed, as metatiles, in the field
   state's points, which end with -1, -1. A metatile to blow away
   (CYCLONE_METATILE_BURST) also sets CYCLONE_STATE.kind; the last carry and
   lift metatiles are kept as the pixels of their centres, in cycloneCarryX, Z
   and cycloneLiftX, Z. */
void fdPsynergyGatherNearbyMetatiles(void)
{
	FIELD_EFFECT_STATE *effect = RamHeapSlots->fieldEffState;
	POINT_STATE		   *field = RamHeapSlots->fieldState;
	CYCLONE_STATE	   *state = RamHeapSlots->battleEff;
	Sint16			   *points = field->points;
	ACTOR			   *owner = effect->caster;
	FIXED				pos[3];
	FIXED				x, z;
	Sint32				kind;
	Uint32				i;
	Sint32				cnt = 0;

	state->kind = cnt;
	field->cycloneCarryX = -1;
	field->cycloneCarryZ = -1;
	field->cycloneLiftX = -1;
	field->cycloneLiftZ = -1;
	for(i = 0; i < FIELD_PSYNERGY_NEARBY_METATILES; i++) {
		x = owner->x.fixed +
			FIELD_METATILE_ORIGIN(FieldPsynergyNearbyMetatileOffsets[i * 2]);
		z = owner->z.fixed +
			FIELD_METATILE_ORIGIN(
				FieldPsynergyNearbyMetatileOffsets[i * 2 + 1]);
		kind = fdMapGridGetCycloneMetatileKind(x, z, owner->layer);
		/* position[1] is never set: only x and z reach
		   fdMeasurePsynergyHeight. */
		pos[0] = x;
		pos[2] = z;
		if(kind == CYCLONE_METATILE_BURST &&
			fdMeasurePsynergyHeight(effect->fieldPsynergy, owner, pos) != -1) {
			points[cnt * 2] = x / toFIXED(16);
			points[cnt * 2 + 1] = z / toFIXED(16);
			cnt++;
			state->kind = kind;
		}
		if(kind == CYCLONE_METATILE_CARRY &&
			fdMeasurePsynergyHeight(effect->fieldPsynergy, owner, pos) != -1) {
			field->cycloneCarryX = FIXEDtoINT(x);
			field->cycloneCarryZ = FIXEDtoINT(z);
			field->cycloneCarryX =
				(field->cycloneCarryX & ~(MAPGRID_METATILE_PIXELS - 1)) +
				MAPGRID_METATILE_PIXELS / 2;
			field->cycloneCarryZ =
				(field->cycloneCarryZ & ~(MAPGRID_METATILE_PIXELS - 1)) +
				MAPGRID_METATILE_PIXELS / 2;
			points[cnt * 2] = x / toFIXED(16);
			points[cnt * 2 + 1] = z / toFIXED(16);
			cnt++;
		}
		if(kind == CYCLONE_METATILE_LIFT &&
			fdMeasurePsynergyHeight(effect->fieldPsynergy, owner, pos) != -1) {
			field->cycloneLiftX = FIXEDtoINT(x);
			field->cycloneLiftZ = FIXEDtoINT(z);
			field->cycloneLiftX =
				(field->cycloneLiftX & ~(MAPGRID_METATILE_PIXELS - 1)) +
				MAPGRID_METATILE_PIXELS / 2;
			field->cycloneLiftZ =
				(field->cycloneLiftZ & ~(MAPGRID_METATILE_PIXELS - 1)) +
				MAPGRID_METATILE_PIXELS / 2;
			points[cnt * 2] = x / toFIXED(16);
			points[cnt * 2 + 1] = z / toFIXED(16);
			cnt++;
		}
	}
	points[cnt * 2] = -1;
	points[cnt * 2 + 1] = -1;
}

/* The ROM's callers here are compiled against this form, which returns an
   Sint32 where the definition returns a Uint8. */
Sint32 fdMapGridGetMetatileBehavior(Sint32 x, Sint32 z, Sint32 layer);

/* Runs the place's Psynergy script once for each kind of metatile Cyclone
   touched (FIELD_STATE.psynergyMetatiles), lowest metatile behaviour first. */
void fdPsynergyRunTouchedScripts(void)
{
	FIELD_EFFECT_STATE *effectState = RamHeapSlots->fieldEffState;
	TOUCH_METATILE	   *metatiles =
		(TOUCH_METATILE *)FieldStateWork->psynergyMetatiles;
	ACTOR *caster = effectState->caster;
	Sint32 cnt = 0;
	Sint16 ids[24];
	Sint32 i, j;

	for(i = 0; metatiles[i].x != -1; i++) {
		Sint16 id =
			fdMapGridGetMetatileBehavior(FIELD_METATILE_ORIGIN(metatiles[i].x),
				FIELD_METATILE_ORIGIN(metatiles[i].z), caster->layer);

		for(j = 0; j < cnt; j++) {
			if(ids[j] == id)
				break;
		}
		if(j == cnt)
			ids[cnt++] = id;
	}
	ids[cnt] = -1;

	if(cnt > 1) {
		for(i = 0; i < cnt - 1; i++) {
			for(j = i + 1; j < cnt; j++) {
				if(ids[i] > ids[j]) {
					Sint16 swap = ids[i];

					ids[i] = ids[j];
					ids[j] = swap;
				}
			}
		}
	}

	for(i = 0; ids[i] != -1; i++) {
		Sint32 id = ids[i];
		Sint32 event = fdFindPsynergyCue(
			PSYNERGY_CUE(PSYNERGY_PHASE_ACTS), effectState->fieldPsynergy, id);

		if(event != 0)
			fdRunPsynergyScript(
				event, effectState->casterId, effectState->tgtId);
	}
}

MAP_METATILE *fdMapGridGetMetatileAt(Sint32 x, Sint32 z, Sint32 layer);

/* Clears what Cyclone blew away: each metatile of FIELD_STATE.psynergyMetatiles
   that falls in the width by height window at (winX, winZ) of layerIdx
   (counted from the layer's scroll and the leader's height) takes the metatile
   picture from the same place of the window at (srcX, srcZ), is opened on
   layer 2, and is recorded (up to 64) in TouchCopiesWork to be made again. */
void fdTileCopiesCopyTouchedMetatiles(Sint32 srcX, Sint32 srcZ, Sint32 width,
	Sint32 height, Sint32 winX, Sint16 winZ, Sint32 layerIdx)
{
	FIELD_STATE *field = RamHeapSlots->fieldState;
	ACTOR		*owner = fdActorGetById(fdPartyGetLeaderActor());
	Sint16		*metatile = field->psynergyMetatiles;
	Sint32		 ownerHeight = FIELD_METATILE(owner->groundHeight);
	MAP_STATE	*map = RamHeapSlots->mapGrid;
	MAP_LAYER	*layer = &map->layers[layerIdx];
	Sint32		 i;

	for(i = 0; metatile[i * 2] != -1; i++) {
		Sint32 x = metatile[i * 2] - winX;
		Sint32 z = metatile[i * 2 + 1] - winZ - ownerHeight;

		x += FIELD_METATILE(layer->ofsX);
		z += FIELD_METATILE(layer->ofsY);

		if(x >= 0 && x < width && z >= 0 && z < height) {
			Sint32 n;

			fdMapGridCopyMetatileTilesMetatilesRectStub(
				srcX + x, srcZ + z, 1, 1, winX + x, winZ + z);
			fdMapGridGetMetatileAt(FIELD_METATILE_ORIGIN(metatile[i * 2]),
				FIELD_METATILE_ORIGIN(metatile[i * 2 + 1]), 2)
				->collisionCode = 0;
			n = TouchCopiesWork.cnt;
			if(n < (Sint32)ARRAY_COUNT(TouchCopiesWork.copies)) {
				TouchCopiesWork.copies[n].toX = srcX + x;
				TouchCopiesWork.copies[n].toZ = srcZ + z;
				TouchCopiesWork.copies[n].fromX = winX + x;
				TouchCopiesWork.copies[n].fromZ = winZ + z;
				TouchCopiesWork.copies[n].metatileX = metatile[i * 2];
				TouchCopiesWork.copies[n].metatileZ = metatile[i * 2 + 1];
				TouchCopiesWork.cnt++;
			}
		}
	}
}

#undef BOW_NEXT_PHASE

/* Makes again each copy fdTileCopiesCopyTouchedMetatiles recorded: copies the
   metatile picture back and opens the metatile on layer 2. */
void fdTileCopiesRestore(void)
{
	TOUCH_COPIES *base = &TouchCopiesWork;
	Sint32		  i;

	for(i = 0; i < TouchCopiesWork.cnt; i++) {
		TOUCH_COPY *entry = &base->copies[i];

		fdMapGridCopyMetatileTilesMetatilesRectStub(
			entry->toX, entry->toZ, 1, 1, entry->fromX, entry->fromZ);
		fdMapGridGetMetatileAt(FIELD_METATILE_ORIGIN(entry->metatileX),
			FIELD_METATILE_ORIGIN(entry->metatileZ), 2)
			->collisionCode = 0;
	}
}
