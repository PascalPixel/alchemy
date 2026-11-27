/* @file */
/* Whirlwind: an orb flies from the caster and spins up a ring of leaves that */
/* blows its target away. */
/* Part of event_commands.c. */

#include "event_commands_internal.h"

/* Whirlwind's particles: the ring that spreads from the caster, the leaves */
/* that circle the orb 8 pixels out, a 32nd of a turn a frame, for 121 frames */
/* before they wander off for 101, the target shrinking to half its size, and */
/* the orb's 10 frames from caster to target. */
enum {
	WHIRLWIND_RADIAL_DIRECTIONS = 16,
	WHIRLWIND_ORBIT_PARTICLES = 16,
	WHIRLWIND_ORBIT_RADIUS = toFIXED(8),
	WHIRLWIND_ORBIT_ANGLE_STEP = TRIG_TURN / 32,
	WHIRLWIND_ORBIT_FRAMES = 121,
	WHIRLWIND_WANDER_FRAMES = 101,
	WHIRLWIND_TARGET_SCALE_STEP = FIXED_ONE / 512,
	WHIRLWIND_TARGET_MIN_SCALE = FIXED_ONE / 2,
	WHIRLWIND_TRAVEL_FRAMES = 10
};

/* The shared effect-spawning source, at its place in the ROM. */
#define EFFECT_SPAWN_RESIDENT
#include "../scene/shared/field_effect_spawn.c"
#include "../scene/shared/field_effect_advance.c"
#undef EFFECT_SPAWN_RESIDENT

/* Spawns 17 particles that spread from origin in a flat ring, their x velocity
 */
/* 1.5 times their z velocity, so the ring is wider than deep as the field is */
/* drawn. */
void fdEffectSpawnRadialParticleRing(ACTOR *origin) {
	EFFECT_OPTIONS opt;
	FIXED vel[3];
	FIXED vz;
	s32 angle;
	u32 i;

	opt.pal = 0;
	opt.update = scFieldEffectAdvanceMotion;
	opt.startScaleX = FIXED_ONE * 4 / 5;
	opt.startScaleY = FIXED_ONE * 4 / 5;
	for(i = 0; i <= WHIRLWIND_RADIAL_DIRECTIONS; i++) {
		angle = i * (TRIG_TURN / WHIRLWIND_RADIAL_DIRECTIONS);
		vel[0] = syCos(angle) * 3 / 2;
		vel[1] = 0;
		vz = sySin(angle);
		vel[2] = vz;
		fdEffectSpawnResident(origin->x.fixed, origin->y.fixed, origin->z.fixed,
							  vel[0], vel[1], vz,
							  EFFECT_USE_UPDATE | EFFECT_USE_START_SCALE |
								  EFFECT_USE_PALETTE | EFFECT_SCRIPT_MEDIUM,
							  &opt);
	}
}

/* Per-frame update: wanders away from the anchor, 2 to 3 pixels a frame, now */
/* and then turning sharply; after 101 frames it hands over to the shared */
/* particle script. */
void fdEffectWanderAroundAnchor(ACTOR *effect) {
	FIXED len;
	s32 angle;
	FIXED dx;
	FIXED dz;

	/* syRandom is 0 to 1 in 16.16, so 2 to 3 pixels. */
	len = syRandom() + toFIXED(2);
	angle = effect->facing;
	dx = syMulFX(len, syCos(angle));
	dz = syMulFX(len, sySin(angle));
	effect->x.fixed += dx;
	effect->z.fixed += dz;
	/* A small backwards drift, a 4096th of a turn a frame. */
	effect->facing += -(TRIG_TURN / 4096);
	if(effect->turnFrm != 0) {
		effect->turnFrm--;
		effect->facing += TRIG_TURN / 32;
	}
	else if(syRandom16Below(32) == 0) {
		effect->turnFrm = syRandom16Below(16) + 8; /* 8 to 23 frames */
	}
	if(++effect->swirlFrm == WHIRLWIND_WANDER_FRAMES)
		fdActorStartScriptStub(effect, FieldEffect_SharedParticleScript);
}

/* Per-frame update: circles the anchor at 8 pixels; after 121 frames it starts
 */
/* to wander (fdEffectWanderAroundAnchor). */
void fdEffectCircleAnchor(ACTOR *effect) {
	ACTOR *anchor;
	s32 angle;
	FIXED dx;
	FIXED dz;

	anchor = (ACTOR *)effect->watchedActor;
	{
		FIXED radius = WHIRLWIND_ORBIT_RADIUS;

		angle = effect->facing;
		dx = syMulFX(radius, syCos(angle));
		dz = syMulFX(radius, sySin(angle));
		effect->x.fixed = anchor->x.fixed + dx;
		effect->z.fixed = anchor->z.fixed + dz;
		effect->facing += WHIRLWIND_ORBIT_ANGLE_STEP;
		if(++effect->swirlFrm == WHIRLWIND_ORBIT_FRAMES) {
			/* The field engine integrates height after this update function. */
			effect->update = fdEffectWanderAroundAnchor;
			effect->swirlFrm = 0;
			effect->turnFrm = 0;
			effect->gravity = toFIXED(1) / 10;
			effect->velY = toFIXED(3);
			effect->facing = syRandom();
		}
	}
}

/* Shrinks an event-selected Whirlwind target each frame; below half size, */
/* moves it to the origin, stops it and removes the update. */
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

/* Whirlwind: an orb grows as it flies from the caster to 4 pixels below the */
/* Psynergy's position in 10 frames, swells and gusts; cast at a target a */
/* trigger names, it shrinks the target away and sends 16 leaves circling, then
 */
/* it whooshes off. */
void fdPsynergyWhirlwind(void) {
	FIELD_EFFECT_STATE *effectState = FieldEffectStateWork;
	ACTOR *caster = effectState->caster;
	ACTOR *target = effectState->tgt;
	VECTOR3 pos;
	VECTOR3 from;
	VECTOR3 to;
	ACTOR *orb;
	s32 step;
	ACTOR *leaf;
	s32 steps = WHIRLWIND_TRAVEL_FRAMES + 1;

	step = 0;
	from.x = caster->x.fixed;
	from.y = caster->y.fixed;
	from.z = caster->z.fixed;
	to.x = effectState->x;
	to.y = effectState->y - toFIXED(4);
	to.z = effectState->z;
	orb = fdActorSpawn(SPRITE_WHIRLWIND, 0, 0, 0);
	if(orb == 0) {
		return;
	}
	fdPsynergyFadeToCastCol();
	fdActorSelectAnimationStub(orb, 2);
	/* Both ends of the ten-frame flight. */
	for(; step < steps; step++) {
		FIXED scale;
		orb->x.fixed =
			from.x + step * (to.x - from.x) / WHIRLWIND_TRAVEL_FRAMES;
		orb->y.fixed =
			from.y + step * (to.y - from.y) / WHIRLWIND_TRAVEL_FRAMES;
		orb->z.fixed =
			from.z + step * (to.z - from.z) / WHIRLWIND_TRAVEL_FRAMES;
		scale =
			FIXED_ONE / 4 + step * (FIXED_ONE * 21 / 20) /
								WHIRLWIND_TRAVEL_FRAMES; /* from 0.25 to 1.3 */
		orb->scaleX = scale;
		orb->scaleY = scale;
		sySynch(1);
	}
	orb->scaleX = FIXED_ONE * 17 / 10; /* wide */
	orb->scaleY = FIXED_ONE * 13 / 10; /* high */
	sySndPlayStub(SE_RUSH);
	sySynch(20);
	/* Cleared by fdPsynergyDispatch: cast on a target a trigger */
	/* names, the target is blown away. */
	if(effectState->aimAhead == 0) {
		if(target != 0)
			target->update = fdWhirlwindUpdateTarget;

		/* Leaves from 4 pixels above the orb, each 0.8 pixel higher, 3 to 8 */
		/* pixels away. */
		for(step = 0; step < WHIRLWIND_ORBIT_PARTICLES; step++) {
			FIXED dist;

			pos.x = orb->x.fixed;
			pos.y = orb->y.fixed + step * (toFIXED(4) / 5) + toFIXED(4);
			pos.z = orb->z.fixed;
			dist = syRandom() * 5 + toFIXED(3);
			syVectorAddPolarOffset(dist, syRandom(), (FIXED *)&pos);
			leaf = fdActorSpawn(SPRITE_LEAF, pos.x, pos.y, pos.z);
			if(leaf != 0) {
				leaf->update = fdEffectCircleAnchor;
				leaf->watchedActor = (ACTOR_OR_EFFECT *)orb;
				leaf->tmr = 0;
				leaf->userValue = 0;
				leaf->facing = syRandom();
			}
			sySynch(6);
		}
		sySynch(20);
		sySynch(120);
	}
	fdActorSelectAnimationStub(orb, 1);
	sySynch(30);
	sySndPlayStub(SE_WHOOSH);
	sySynch(20);
	/* Every leaf has left its orbit by now. */
	fdActorReleaseStub(orb);
	fdPsynergyFadeFromCastCol();
}
