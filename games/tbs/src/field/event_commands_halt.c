/* @file */
/* Halt: a stopwatch flies out to the target, ticks and bursts into sparkles, */
/* and the target stands frozen, its colours flickering. */
/* Part of event_commands.c. */

#include "event_commands_internal.h"

/* The sparkles the stopwatch bursts into. */
#define HALT_SPARKLES 16

/* Field effect sprite callback of a Halt sparkle (OUT_AND_BACK_STATE): it */
/* heads for a point 40 to 70 pixels from where it appeared, then for a point 4
 */
/* pixels round 8 pixels above the Psynergy's position on screen, and is freed
 */
/* on arrival. */
void fdEffectUpdateHaltParticle(FIELD_PARTICLE *effect) {
	FIELD_EFFECT_STATE *effectState;
	VECTOR3 pos;
	s16 angle;
	s32 state;

	effectState = FieldEffectStateWork;
top:
	state = effect->state;
	if(state == OUT_AND_BACK_AIM_OUT) {
		pos.x = effect->originX;
		pos.z = effect->originZ;
		angle = syRandom();
		syVectorAddPolarOffset(syRandom() * 30 + toFIXED(40), (u16)angle,
							   (FIXED *)&pos);
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
		syVectorAddPolarOffset(toFIXED(4), syRandom(), (FIXED *)&pos);
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

/* Pauses the target (effectState->tgt) and runs fdPsynergyHalt. */
void fdPsynergyHaltOnTarget(s32 unused) {
	ACTOR *target = ((FIELD_EFFECT_STATE *)FieldEffectStateWork)->tgt;

	target->paused = 1;
	fdPsynergyHalt();
}

/* A stopwatch (SPRITE_STOPWATCH) flies from over the caster to the Psynergy's
 */
/* position over GROW_STEPS frames, growing, sounds three low tones and a high
 */
/* one, and bursts into HALT_SPARKLES sparkles that fly out and back. */
void fdPsynergyHalt(void) {
	FIELD_EFFECT_STATE *effectState = FieldEffectStateWork;
	ACTOR *caster = effectState->caster;
	ACTOR *stopwatch;
	FIELD_PARTICLE *effect;
	VECTOR3 pos;
	VECTOR3 from;
	VECTOR3 to;
	s32 step;
	s32 i;

	effectState->y = caster->y.fixed;
	stopwatch = fdActorSpawn(SPRITE_STOPWATCH, 0, 0, 0);
	step = 0;
	fdActorSelectAnimationStub(stopwatch, 0);
	if(stopwatch == NULL)
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
	/* It arrives on the last step, grown from a quarter to full size. */
	for(;;) {
		s32 steps = GROW_STEPS + 1;
		s32 scale;

		SET_ACTOR_POSITION(stopwatch,
						   from.x + (to.x - from.x) * step / GROW_STEPS,
						   from.y + (to.y - from.y) * step / GROW_STEPS,
						   from.z + (to.z - from.z) * step / GROW_STEPS);
		scale = fdEffectInterpolate(FIXED_ONE, FIXED_ONE / 4, step);
		stopwatch->scaleX = scale;
		stopwatch->scaleY = scale;
		sySynch(1);
		step++;
		if(step >= steps)
			break;
	}

	/* Three low tones and a high one, 10 frames apart, as the stopwatch changes
	 */
	/* to its second animation. */
	sySynch(5);
	fdActorSelectAnimationStub(stopwatch, 1);
	sySndPlayStub(SE_TRIPLE_TONE_LOW);
	sySynch(10);
	sySndPlayStub(SE_TRIPLE_TONE_LOW);
	sySynch(10);
	sySndPlayStub(SE_TRIPLE_TONE_LOW);
	sySynch(10);
	sySndPlayStub(SE_TRIPLE_TONE_HIGH);

	/* Sparkles 4 pixels round it fly out, then come back to the Psynergy's */
	/* position. */
	effect = &effectState->particles[0];
	for(i = 0; i < HALT_SPARKLES; i++) {
		pos.x = stopwatch->x.fixed;
		pos.y = stopwatch->y.fixed + toFIXED(8);
		pos.z = stopwatch->z.fixed;
		fdCameraWorldToScr((VECTOR3 *)&pos.x);
		syVectorAddPolarOffset(toFIXED(4), syRandom(), &pos.x);
		fdParticleInit(effect, SPRITE_SPARKLE, pos.x, pos.z);
		fdParticleSetCallback(effect, fdEffectUpdateHaltParticle);
		fdSprSetAnimationColModesStub(effect->spr, SPRITE_COLOR_WHITE);
		effect++;
	}

	pos.x = stopwatch->x.fixed;
	pos.y = stopwatch->y.fixed + toFIXED(8);
	pos.z = stopwatch->z.fixed;
	sySynch(8);
	fdActorReleaseStub(stopwatch);
	sySynch(4);
	sySynch(30);
	fdPsynergyFadeFromCastCol();
}

/* Per-frame update of a halted actor: gives its sprite one of eight colour */
/* modes (Halt_FlickerColorModes) at random, so it flickers. */
void fdPsynergyUpdateHaltedActor(ACTOR *actor) {
	void *object = (void *)actor;
	const s8 *table = Halt_FlickerColorModes;
	s32 random = syRandom();
	fdActorSetSprColModeStub(object, table[(u32)(random * 8) >> 16]);
}

/* Halts actorId, keeping its per-frame update and, if it is animated, its */
/* first animation's colour mode in MainRoutineStateWork. */
void fdPsynergyHaltActor(s32 actorId) {
	ACTOR *actor;
	SPR_ANIMATION *entry;

	actor = fdActorLookup(actorId);
	if(actor != NULL) {
		MainRoutineStateWork.haltedUpdate = (u32)actor->update;
		MainRoutineStateWork.haltedColMode = 0;
		if(actor->sprStorageKind == ACTOR_SPRITE_ONE) {
			entry = actor->spr->entries[0];
			if(entry != NULL) {
				MainRoutineStateWork.haltedColMode = entry->colMode;
			}
		}
		actor->update = fdPsynergyUpdateHaltedActor;
		actor->paused = 1;
		fdActorSetAnimationSpeedStub(actor, 0);
	}
}

/* Resumes actorId with the update and colour mode fdPsynergyHaltActor */
/* kept. */
void fdPsynergyResumeHaltedActor(s32 actorId) {
	ACTOR *actor = fdActorLookup(actorId);
	if(actor != NULL) {
		if(actor->update == fdPsynergyUpdateHaltedActor) {
			MAIN_ROUTINE_STATE *mainState = &MainRoutineStateWork;
			actor->update = (void (*)(ACTOR *))mainState->haltedUpdate;
			mainState->haltedUpdate = 0;
			fdActorSetSprColModeStub(actor, mainState->haltedColMode);
		}
		actor->paused = 0;
		fdActorSetAnimationSpeedStub(actor, ANIM_STEP_NORMAL);
	}
}

/* Returns id, or -1 (no target) when Halt passes actor id's sprite over */
/* (SPEAKER_HALTED_LINES_NONE in SpeakerDefinitions): the talking monsters and
 */
/* the things, the 20 rows before the default. */
s32 fdPsynergyFilterHaltTarget(s32 id) {
	if(fdSpeakerGetHaltedLines(id) == SPEAKER_HALTED_LINES_NONE)
		return -1;
	return id;
}

#undef HALT_SPARKLES
