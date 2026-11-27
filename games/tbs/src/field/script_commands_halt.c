/* Halt: a stopwatch flies out to the target, ticks and bursts into sparkles,
   and the target stands frozen, its colours flickering. Part of
   script_commands.c. */

#include "script_commands_internal.h"

/* The sparkles the stopwatch bursts into. */
#define HALT_SPARKLES 16

/* Field effect sprite callback of a Halt sparkle (OUT_AND_BACK_STATE): it heads
   for a point 40 to 70 pixels from where it appeared, then for a point 4 pixels
   round 8 pixels above the Psynergy's position on screen, and is freed on
   arrival. */
void fdEffectUpdateHaltParticle(FIELD_PARTICLE *effect)
{
	FIELD_EFFECT_STATE *effectState;
	VECTOR3				pos;
	Sint16				angle;
	Sint32				state;

	effectState = FieldEffectStateWork;
top:
	state = effect->state;
	if(state == OUT_AND_BACK_AIM_OUT) {
		pos.x = effect->originX;
		pos.z = effect->originZ;
		angle = syRandom();
		syVectorAddPolarOffset(
			syRandom() * 30 + toFIXED(40), (Uint16)angle, (FIXED *)&pos);
		effect->targetX = pos.x;
		effect->targetZ = pos.z;
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
		syVectorAddPolarOffset(toFIXED(4), syRandom(), (FIXED *)&pos);
		effect->targetX = pos.x;
		effect->targetZ = pos.z;
		effect->turnStepMax = TRIG_TURN / 16;
		effect->turnStepLimited = 1;
		effect->state++;
		return;
	}
	if(state == OUT_AND_BACK_FLYING_BACK &&
		fdParticleHasPendingTarget(effect) == 0)
		fdEffectClearOwnedSlot(effect);
}

/* Pauses the target (effectState->target) and runs fdPsynergyHalt. */
void fdPsynergyHaltOnTarget(Sint32 unused)
{
	ACTOR *target = ((FIELD_EFFECT_STATE *)FieldEffectStateWork)->target;

	target->paused = 1;
	fdPsynergyHalt();
}

/* A stopwatch (SPRITE_STOPWATCH) flies from over the caster to the Psynergy's
   position over GROW_STEPS ticks, growing, sounds three low tones and a high
   one, and bursts into HALT_SPARKLES sparkles that fly out and back. */
void fdPsynergyHalt(void)
{
	FIELD_EFFECT_STATE *effectState = FieldEffectStateWork;
	ACTOR			   *caster = effectState->caster;
	ACTOR			   *stopwatch;
	FIELD_PARTICLE	   *effect;
	VECTOR3				pos;
	VECTOR3				from;
	VECTOR3				to;
	Sint32				step;
	Sint32				i;

	effectState->y = caster->y.fixed;
	stopwatch = fdActorSpawn(SPRITE_STOPWATCH, 0, 0, 0);
	step = 0;
	fdActorSelectAnimationStub(stopwatch, 0);
	if(stopwatch == NULL)
		return;

	/* From 16 pixels above the caster to 8 pixels above the Psynergy's
	   position. */
	fdPsynergyFadeToCastColor();
	from.x = caster->x.fixed;
	from.y = caster->y.fixed + toFIXED(16);
	from.z = caster->z.fixed;
	to.x = effectState->x;
	to.y = effectState->y + toFIXED(8);
	to.z = effectState->z;
	/* It arrives on the last step, grown from a quarter to full size. */
	for(;;) {
		Sint32 steps = GROW_STEPS + 1;
		Sint32 scale;

		SET_ACTOR_POSITION(stopwatch,
			from.x + (to.x - from.x) * step / GROW_STEPS,
			from.y + (to.y - from.y) * step / GROW_STEPS,
			from.z + (to.z - from.z) * step / GROW_STEPS);
		scale = fdEffectInterpolate(toFIXED(1), toFIXED(1) / 4, step);
		stopwatch->scaleX = scale;
		stopwatch->scaleY = scale;
		sySynch(1);
		step++;
		if(step >= steps)
			break;
	}

	/* Three low tones and a high one, 10 ticks apart, as the stopwatch changes
	   to its second animation. */
	sySynch(5);
	fdActorSelectAnimationStub(stopwatch, 1);
	sySoundPlayStub(SE_TRIPLE_TONE_LOW);
	sySynch(10);
	sySoundPlayStub(SE_TRIPLE_TONE_LOW);
	sySynch(10);
	sySoundPlayStub(SE_TRIPLE_TONE_LOW);
	sySynch(10);
	sySoundPlayStub(SE_TRIPLE_TONE_HIGH);

	/* Sparkles 4 pixels round it fly out, then come back to the Psynergy's
	   position. */
	effect = &effectState->particles[0];
	for(i = 0; i < HALT_SPARKLES; i++) {
		pos.x = stopwatch->x.fixed;
		pos.y = stopwatch->y.fixed + toFIXED(8);
		pos.z = stopwatch->z.fixed;
		fdCameraWorldToScreen((VECTOR3 *)&pos.x);
		syVectorAddPolarOffset(toFIXED(4), syRandom(), &pos.x);
		fdParticleInit(effect, SPRITE_SPARKLE, pos.x, pos.z);
		fdParticleSetCallback(effect, fdEffectUpdateHaltParticle);
		fdSpriteSetAnimationColorModesStub(effect->sprite, SPRITE_COLOR_WHITE);
		effect++;
	}

	pos.x = stopwatch->x.fixed;
	pos.y = stopwatch->y.fixed + toFIXED(8);
	pos.z = stopwatch->z.fixed;
	sySynch(8);
	fdActorReleaseStub(stopwatch);
	sySynch(4);
	sySynch(30);
	fdPsynergyFadeFromCastColor();
}

/* Per-tick update of a halted actor: gives its sprite one of eight colour
   modes (HaltFlickerColorModes) at random, so it flickers. */
void fdPsynergyUpdateHaltedActor(ACTOR *actor)
{
	void		*object = (void *)actor;
	const Sint8 *table = HaltFlickerColorModes;
	Sint32		 random = syRandom();
	fdActorSetSpriteColorModeStub(object, table[(Uint32)(random * 8) >> 16]);
}

/* Halts actorId, keeping its per-tick update and, if it is animated, its first
   animation's colour mode in MainRoutineStateWork. */
void fdPsynergyHaltActor(Sint32 actorId)
{
	ACTOR		  *actor;
	SPR_ANIMATION *entry;

	actor = fdActorLookup(actorId);
	if(actor != NULL) {
		MainRoutineStateWork.haltedUpdate = (Uint32)actor->update;
		MainRoutineStateWork.haltedColorMode = 0;
		if(actor->spriteStorageKind == ACTOR_SPRITE_ONE) {
			entry = actor->sprite->entries[0];
			if(entry != NULL) {
				MainRoutineStateWork.haltedColorMode = entry->colorMode;
			}
		}
		actor->update = fdPsynergyUpdateHaltedActor;
		actor->paused = 1;
		fdActorSetAnimationSpeedStub(actor, 0);
	}
}

/* Resumes actorId with the update and colour mode fdPsynergyHaltActor kept. */
void fdPsynergyResumeHaltedActor(Sint32 actorId)
{
	ACTOR *actor = fdActorLookup(actorId);
	if(actor != NULL) {
		if(actor->update == fdPsynergyUpdateHaltedActor) {
			MAIN_ROUTINE_STATE *mainState = &MainRoutineStateWork;
			actor->update = (void (*)(ACTOR *))mainState->haltedUpdate;
			mainState->haltedUpdate = 0;
			fdActorSetSpriteColorModeStub(actor, mainState->haltedColorMode);
		}
		actor->paused = 0;
		fdActorSetAnimationSpeedStub(actor, ANIM_STEP_NORMAL);
	}
}

/* Returns id, or -1 (no target) when Halt passes actor id's sprite over
   (SPEAKER_HALTED_LINES_NONE in SpeakerDefinitions): the talking monsters and
   the things, the 20 rows before the default. */
Sint32 fdPsynergyFilterHaltTarget(Sint32 id)
{
	if(fdSpeakerGetHaltedLines(id) == SPEAKER_HALTED_LINES_NONE)
		return -1;
	return id;
}

#undef HALT_SPARKLES
