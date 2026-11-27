/* @file */
/* Frost: snowflakes spiral in on the target, and a puddle freezes into an */
/* ice pillar. */
/* Part of event_commands.c. */

#include "event_commands_internal.h"
#include "scene/frost_glint.h"

#define FROST_SNOWFLAKES	16 /* spiralling in on the Psynergy's position */
#define FROST_SPIRAL_FRAMES 60 /* each snowflake's spiral */
#define FROST_FLASHES		16 /* the target's white flashes */

/* Frost's glint on its target. */
DEFINE_FROST_GLINT(, fdActorSetSprColModeStub)

/* Per-frame update: shrinks by 1/16 a frame and is released at 1/16. */
void fdEffectUpdateOrbitingParticleFade(ACTOR *particle) {
	FIXED scale;

	if(particle != NULL) {
		scale = particle->scaleX - FIXED_ONE / 16;
		particle->scaleY -= FIXED_ONE / 16;
		particle->scaleX = scale;
		if(scale <= FIXED_ONE / 16)
			fdActorStartScriptStub(particle, FieldEffect_SharedParticleScript);
	}
}

/* Both winding directions share the countdown and position update. timer */
/* starts at FROST_SPIRAL_FRAMES (fdPsynergyFrost). */
static inline void fdEffectUpdateSpiralParticle(ACTOR *actor, s32 clockwise) {
	ACTOR *particle = actor;
	VECTOR3 position;
	s16 remainingFrames;
	s32 decrementedTimer;

	if(particle != NULL) {
		/* timer counts the spiral's frames down; the radius shrinks with it. */
		decrementedTimer = (u16)particle->tmr - 1;
		particle->tmr = decrementedTimer;
		remainingFrames = decrementedTimer;
		if(remainingFrames != 0) {
			position.x = particle->tgtX;
			position.y = particle->tgtY;
			position.z = particle->tgtZ;
			syVectorAddPolarOffset(
				remainingFrames * toFIXED(2),
				clockwise
					? particle->userValue - remainingFrames * (TRIG_TURN / 32)
					: particle->userValue + remainingFrames * (TRIG_TURN / 32),
				(FIXED *)&position);
			particle->x.fixed = position.x;
			particle->y.fixed = position.y;
			particle->z.fixed = position.z;
		}
		else {
			particle->update = fdEffectUpdateOrbitingParticleFade;
		}
	}
}

/* Per-frame update of a snowflake: spirals anticlockwise in to the target */
/* point (targetX to targetZ), at twice the frames left in pixels, a 32nd of a
 */
/* turn further round for each, then fades. */
void fdEffectUpdateOrbitingParticleLeft(ACTOR *actor) {
	fdEffectUpdateSpiralParticle(actor, 0);
}

/* Per-frame update: as fdEffectUpdateOrbitingParticleLeft, clockwise. */
void fdEffectUpdateOrbitingParticleRight(ACTOR *actor) {
	fdEffectUpdateSpiralParticle(actor, 1);
}

/* Runs Frost cast on a target. */
void fdPsynergyFrostOnTarget(s32 unused) {
	FIELD_EFFECT_STATE *effectState = FieldEffectStateWork;
	ACTOR *target = effectState->tgt;

	if(target != 0) {
		/* With leaveTgt the target neither flashes nor freezes. */
		if(effectState->leaveTgt != 0) {
			effectState->aimAhead = 1;
		}
		target->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
		fdPsynergyFrost();
	}
}

/* FROST_SNOWFLAKES snowflakes spiral in on the Psynergy's position and make */
/* the target flash FROST_FLASHES times; without sceneVariant the target */
/* freezes. */
void fdPsynergyFrost(void) {
	s32 eventTarget;
	VECTOR3 pos;
	FIELD_EFFECT_STATE *effectState;
	ACTOR *target;
	ACTOR *particle;
	void *event;
	s32 i;

	effectState = FieldEffectStateWork;
	target = effectState->tgt;
	fdPsynergyFadeToCastCol();
	sySndPlayStub(SE_ICE);

	/* A particle every 3 frames, 0.5 to 1 size, turning either way at random,
	 */
	/* centred 2 to 6 pixels from the Psynergy's position. */
	for(i = 0; i < FROST_SNOWFLAKES; i++) {
		particle = fdActorSpawn(SPRITE_SNOWFLAKE, 0, 0, 0);
		if(particle != NULL) {
			u32 scale;
			FIXED radius;

			scale = (syRandom() >> 1) + FIXED_ONE / 2;
			particle->scaleY = scale;
			particle->scaleX = scale;
			if((syRandom() & 1) != 0)
				particle->update = fdEffectUpdateOrbitingParticleLeft;
			else
				particle->update = fdEffectUpdateOrbitingParticleRight;

			particle->facing = syRandom();
			particle->tmr = FROST_SPIRAL_FRAMES;
			particle->userValue = syRandom();
			fdActorSetSprColModeStub(particle, SPRITE_COLOR_BLUE_TINT);

			SET_POSITION(&pos, effectState->x, effectState->y, effectState->z);
			radius = (syRandom() << 2) + toFIXED(2);
			syVectorAddPolarOffset(radius, syRandom(), (FIXED *)&pos);
			particle->tgtX = pos.x;
			particle->tgtY = pos.y;
			particle->tgtZ = pos.z;
		}

		sySynch(3);
	}

	sySynch(10);
	sySndPlayStub(SE_ICE);
	sySynch(50);

	if(target != NULL && effectState->aimAhead == 0) {
		sySndPlayStub(SE_POWER_UP);

		/* Flash for 1 frame in every 5. */
		for(i = 0; i < FROST_FLASHES; i++) {
			fdActorSetSprColModeStub(target, SPRITE_COLOR_WHITE);
			sySynch(1);
			fdActorSetSprColModeStub(target, SPRITE_COLOR_NORMAL);
			sySynch(4);
		}

		if(effectState->sceneVariant == 0) {
			sySndPlayStub(SE_SHIMMER);
			fdActorSelectAnimationStub(target, PUDDLE_ANIMATION_FREEZE);
		}

		target->update = fdEffectUpdateFrostGlint;
		event = scFindPsynergyTrigger(PSYNERGY_TRIGGER(PSYNERGY_PHASE_ACTS),
									  FIELD_PSYNERGY_FROST, &eventTarget);
		if(event != NULL) {
			fdRunPsynergyEvent(event, MainRoutineStateWork.leaderActorId,
							   eventTarget);
		}
		sySynch(20);
	}

	fdPsynergyFadeFromCastCol();
}

#undef FROST_SNOWFLAKES
#undef FROST_SPIRAL_FRAMES
#undef FROST_FLASHES
