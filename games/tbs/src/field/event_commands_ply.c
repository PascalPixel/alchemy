/* @file */
/* Ply cast in the field: eight winged figures spiral into one, which flashes,
 */
/* sheds sparks and flies off. */
/* Part of event_commands.c. */

#include "event_commands_internal.h"

/* Per-frame update: sways 4 pixels either side of anchorX, once every 128 */
/* frames. */
void fdEffectSwayParticle(ACTOR *actor) {
	actor->x.fixed = actor->tgtX +
					 syMulFX(toFIXED(4), sySin(actor->tmr * (TRIG_TURN / 128)));
	actor->tmr++;
	actor->tmr = (actor->tmr + 128) % 128;
}

/* Per-frame update: spirals in on a point 10 pixels above the Psynergy's */
/* position, at a radius of as many pixels as frames are left; released when */
/* timer reaches 0. */
void fdEffectUpdateShrinkingOrbitActor(ACTOR *particle) {
	FIELD_EFFECT_STATE *effectState = FieldEffectStateWork;
	VECTOR3 pos;
	s16 remainingFrames;
	s32 decrementedTimer;

	if(particle != NULL) {
		decrementedTimer = (u16)particle->tmr - 1;
		particle->tmr = decrementedTimer;
		remainingFrames = decrementedTimer;
		if(remainingFrames != 0) {
			pos.x = effectState->x;
			pos.y = effectState->y + toFIXED(10);
			pos.z = effectState->z;
			syVectorAddPolarOffset(toFIXED(remainingFrames),
								   particle->userValue +
									   remainingFrames * (TRIG_TURN / 32),
								   (FIXED *)&pos);
			particle->x.fixed = pos.x;
			particle->y.fixed = pos.y;
			particle->z.fixed = pos.z;
		}
		else {
			fdActorStartScriptStub(particle, FieldEffect_SharedParticleScript);
		}
	}
}

/* Per-frame update: sways 6 pixels either side (a 64-frame cycle) 16 pixels */
/* above the Psynergy's position, and every 3 frames sheds a spark that sways */
/* as it slowly falls. */
void fdEffectRunSparkEmitter(ACTOR *emitter) {
	FIELD_EFFECT_STATE *effectState;
	ACTOR *spark;
	s32 pos[3];

	effectState = FieldEffectStateWork;
	if(emitter->tmr != -1) {
		SET_ACTOR_POSITION(
			emitter,
			effectState->x +
				syMulFX(toFIXED(6), sySin(emitter->tmr * (TRIG_TURN / 64))),
			effectState->y + PSYNERGY_EFFECT_HEIGHT, effectState->z);
		emitter->tmr++;
		emitter->tmr = (emitter->tmr + 64) % 64;
	}
	if(FrameCount % 3 == 0) {
		/* From 2 pixels up, 0 to 6 pixels in a random direction. */
		SET_VECTOR(pos, emitter->x.fixed, emitter->y.fixed + toFIXED(2),
				   emitter->z.fixed);
		syVectorAddPolarOffset(syRandom() * 6, syRandom(), pos);
		spark = fdActorSpawn(SPRITE_SPARKLE, pos[0], pos[1], pos[2]);
		/* Each falls, 0.007 pixels a frame faster each frame, and is released
		 */
		/* after 72 frames, its script's wait. */
		if(spark != 0) {
			spark->update = fdEffectSwayParticle;
			spark->scaleX = spark->scaleY = FIXED_ONE * 3 / 5;
			spark->motionFlg = ACTOR_FALLS;
			spark->gravity = toFIXED(0.007);
			spark->tmr = syRandom() >> 9;
			spark->tgtX = spark->x.fixed;
			fdActorSetSprColModeStub(spark, SPRITE_COLOR_BLUE_TINT);
			spark->wait = 72;
			fdActorStartScriptStub(spark, FieldEffect_SharedParticleScript);
		}
	}
}

/* Ply's flashing figure: its steps, of 2 frames each. */
#define PLY_FLASH_STEPS 30

/* Ply cast on a target, which stops the caster's update once it is done. */
void fdPsynergyPlyOnTarget(s32 unused) {
	fdPsynergyPly();
	fdPsynergyStopCasterUpdate();
}

/* Eight particles spiral into the centre, where one appears, sheds sparks and
 */
/* flies far away. */
void fdPsynergyPly(void) {
	FIELD_EFFECT_STATE *effectState = FieldEffectStateWork;
	ACTOR *target = effectState->caster;
	ACTOR *actor;
	SPR *last;
	u8 resourceSlot;
	s32 i;

	fdPsynergyFadeToCastCol();

	/* The spiralling particles: one a frame, each 1/8 turn on from the last. */
	last = 0;
	for(i = 0; i < 8; i++) {
		actor = fdActorSpawn(SPRITE_WINGED_FIGURE, target->x.fixed,
							 target->y.fixed + toFIXED(32), target->z.fixed);
		if(actor != 0) {
			actor->scaleY = FIXED_ONE * 7 / 10;
			actor->scaleX = FIXED_ONE * 7 / 10;
			actor->update = fdEffectUpdateShrinkingOrbitActor;
			actor->tmr = 120;
			actor->userValue = i * (TRIG_TURN / 8);
			actor->motionFlg = ACTOR_FLOATS;
			last = fdSprShareVramCacheEntry(actor->spr, last);
		}
		sySynch(1);
	}

	/* The particles share one sprite cache; keep its slot to free at the end.
	 */
	/* last is read with no check that any particle spawned. */
	resourceSlot = last->slot;
	sySndPlayStub(SE_PSYNERGY_START);
	/* The last particle spawned 7 frames ago with 120 frames to live: wait */
	/* until the spiral is nearly closed. */
	sySynch(110);

	/* The one in the centre. */
	target = fdActorSpawn(SPRITE_WINGED_FIGURE, 0, 0, 0);
	/* FAKEMATCH(temporary): actor, a copy of target, the figure read by both
	 * names. */
	actor = target;
	if(target != 0) {
		target->scaleY = FIXED_ONE * 7 / 10;
		target->scaleX = FIXED_ONE * 7 / 10;
		target->x.fixed = effectState->x;
		target->y.fixed = effectState->y + toFIXED(16);
		target->z.fixed = effectState->z;
		target->motionFlg = ACTOR_FLOATS;
		fdActorSetSprColModeStub(target, SPRITE_COLOR_WHITE);
	}

	sySndPlayStub(SE_PSYNERGY_SPARKLE);
	sySynch(12);
	if(target != 0) {
		/* Blue every 2 frames, red one time in 4. */
		for(i = 0; i < PLY_FLASH_STEPS; i++) {
			/* FAKEMATCH(dead): target set again to actor. */
			target = actor;
			if(i & 3)
				fdActorSetSprColModeStub(actor, SPRITE_COLOR_BLUE_TINT);
			else
				fdActorSetSprColModeStub(actor, SPRITE_COLOR_RED_TINT);
			sySynch(2);
		}
	}

	/* Sparks. */
	fdActorSetSprColModeStub(actor, SPRITE_COLOR_NORMAL);
	sySndPlayStub(MUS_POISON_CURED);
	if(actor != 0) {
		target->update = fdEffectRunSparkEmitter;
		target->tmr = 0;
		/* Cast on a target (enabled cleared by fdPsynergyDispatch), */
		/* the sparks fall for 192 frames instead of 128. */
		if(effectState->aimAhead != 0)
			sySynch(128);
		else
			sySynch(192);
	}

	/* Fly away: 192 pixels at angle TRIG_TURN * 29 / 32, and release it once it
	 */
	/* arrives. */
	if(target != 0) {
		target->tmr = -1;
		target->speedLimit = toFIXED(5);
		target->acceleration = toFIXED(2) / 5;
		target->steeringFlg = 0;
		fdActorSetTargetPosFromMagnitudeAng(target, toFIXED(192),
											TRIG_TURN * 29 / 32);
		fdActorWaitForMoveStub(target);
		fdActorReleaseStub(target);
	}
	/* syVramCacheFindFreeEntry returns VRAM_CACHE_ENTRY_COUNT for no entry. */
	if(resourceSlot != VRAM_CACHE_ENTRY_COUNT)
		syVramCacheFreeEntry(resourceSlot);
	if(effectState->finishFunc != 0)
		effectState->finishFunc();
	fdPsynergyFadeFromCastCol();
}

#undef PLY_FLASH_STEPS
