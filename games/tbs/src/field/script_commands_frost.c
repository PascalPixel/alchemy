/* Frost: snowflakes spiral in on the target, and a puddle freezes into an ice
   pillar. Part of script_commands.c. */

#include "script_commands_internal.h"
#include "scene/frost_glint.h"

#define FROST_SNOWFLAKES   16 /* spiralling in on the Psynergy's position */
#define FROST_SPIRAL_TICKS 60 /* each snowflake's spiral */
#define FROST_FLASHES	   16 /* the target's white flashes */

/* Frost's glint on its target. */
DEFINE_FROST_GLINT(, fdActorSetSpriteColorModeStub)

/* Per-tick update: shrinks by 1/16 a tick and is released at 1/16. */
void fdEffectUpdateOrbitingParticleFade(ACTOR *particle)
{
	FIXED scale;

	if(particle != NULL) {
		scale = particle->scaleX - toFIXED(1) / 16;
		particle->scaleY -= toFIXED(1) / 16;
		particle->scaleX = scale;
		if(scale <= toFIXED(1) / 16)
			fdActorStartScriptStub(particle, FieldEffectSharedParticleScript);
	}
}

/* Both winding directions share the countdown and position update. timer starts
   at FROST_SPIRAL_TICKS (fdPsynergyFrost). */
static inline void fdEffectUpdateSpiralParticle(ACTOR *actor, Sint32 clockwise)
{
	ACTOR  *particle = actor;
	VECTOR3 pos;
	Sint16	remainingTicks;
	Sint32	decrementedTimer;

	if(particle != NULL) {
		/* timer counts the spiral's ticks down; the radius shrinks with it. */
		decrementedTimer = (Uint16)particle->timer - 1;
		particle->timer = decrementedTimer;
		remainingTicks = decrementedTimer;
		if(remainingTicks != 0) {
			pos.x = particle->targetX;
			pos.y = particle->targetY;
			pos.z = particle->targetZ;
			syVectorAddPolarOffset(remainingTicks * toFIXED(2),
				clockwise
					? particle->userValue - remainingTicks * (TRIG_TURN / 32)
					: particle->userValue + remainingTicks * (TRIG_TURN / 32),
				(FIXED *)&pos);
			particle->x.fixed = pos.x;
			particle->y.fixed = pos.y;
			particle->z.fixed = pos.z;
		} else {
			particle->update = fdEffectUpdateOrbitingParticleFade;
		}
	}
}

/* Per-tick update of a snowflake: spirals anticlockwise in to the target point
   (targetX to targetZ), at twice the ticks left in pixels, a 32nd of a turn
   further round for each, then fades. */
void fdEffectUpdateOrbitingParticleLeft(ACTOR *actor)
{
	fdEffectUpdateSpiralParticle(actor, 0);
}

/* Per-tick update: as fdEffectUpdateOrbitingParticleLeft, clockwise. */
void fdEffectUpdateOrbitingParticleRight(ACTOR *actor)
{
	fdEffectUpdateSpiralParticle(actor, 1);
}

/* Runs Frost cast on a target. */
void fdPsynergyFrostOnTarget(Sint32 unused)
{
	FIELD_EFFECT_STATE *effectState = FieldEffectStateWork;
	ACTOR			   *target = effectState->target;

	if(target != 0) {
		/* With leaveTarget the target neither flashes nor freezes. */
		if(effectState->leaveTarget != 0) {
			effectState->aimAhead = 1;
		}
		target->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
		fdPsynergyFrost();
	}
}

/* FROST_SNOWFLAKES snowflakes spiral in on the Psynergy's position and make the
   target flash FROST_FLASHES times; without sceneVariant the target freezes. */
void fdPsynergyFrost(void)
{
	Sint32				eventTarget;
	VECTOR3				pos;
	FIELD_EFFECT_STATE *effectState;
	ACTOR			   *target;
	ACTOR			   *particle;
	void			   *event;
	Sint32				i;

	effectState = FieldEffectStateWork;
	target = effectState->target;
	fdPsynergyFadeToCastColor();
	sySoundPlayStub(SE_ICE);

	/* A particle every 3 ticks, 0.5 to 1 size, turning either way at random,
	   centred 2 to 6 pixels from the Psynergy's position. */
	for(i = 0; i < FROST_SNOWFLAKES; i++) {
		particle = fdActorSpawn(SPRITE_SNOWFLAKE, 0, 0, 0);
		if(particle != NULL) {
			Uint32 scale;
			FIXED  radius;

			scale = (syRandom() >> 1) + toFIXED(1) / 2;
			particle->scaleY = scale;
			particle->scaleX = scale;
			if((syRandom() & 1) != 0)
				particle->update = fdEffectUpdateOrbitingParticleLeft;
			else
				particle->update = fdEffectUpdateOrbitingParticleRight;

			particle->facing = syRandom();
			particle->timer = FROST_SPIRAL_TICKS;
			particle->userValue = syRandom();
			fdActorSetSpriteColorModeStub(particle, SPRITE_COLOR_BLUE_TINT);

			SET_POSITION(&pos, effectState->x, effectState->y, effectState->z);
			radius = (syRandom() << 2) + toFIXED(2);
			syVectorAddPolarOffset(radius, syRandom(), (FIXED *)&pos);
			particle->targetX = pos.x;
			particle->targetY = pos.y;
			particle->targetZ = pos.z;
		}

		sySynch(3);
	}

	sySynch(10);
	sySoundPlayStub(SE_ICE);
	sySynch(50);

	if(target != NULL && effectState->aimAhead == 0) {
		sySoundPlayStub(SE_POWER_UP);

		/* Flash for 1 tick in every 5. */
		for(i = 0; i < FROST_FLASHES; i++) {
			fdActorSetSpriteColorModeStub(target, SPRITE_COLOR_WHITE);
			sySynch(1);
			fdActorSetSpriteColorModeStub(target, SPRITE_COLOR_NORMAL);
			sySynch(4);
		}

		if(effectState->sceneVariant == 0) {
			sySoundPlayStub(SE_SHIMMER);
			fdActorSelectAnimationStub(target, PUDDLE_ANIMATION_FREEZE);
		}

		target->update = fdEffectUpdateFrostGlint;
		event = fdFindPsynergyCue(PSYNERGY_CUE(PSYNERGY_PHASE_ACTS),
			FIELD_PSYNERGY_FROST, &eventTarget);
		if(event != NULL) {
			fdRunPsynergyScript(
				event, MainRoutineStateWork.leaderActorId, eventTarget);
		}
		sySynch(20);
	}

	fdPsynergyFadeFromCastColor();
}

#undef FROST_SNOWFLAKES
#undef FROST_SPIRAL_TICKS
#undef FROST_FLASHES
