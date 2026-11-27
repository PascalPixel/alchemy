/* Force: a fist flies out from the caster and knocks what it hits back a
   metatile. Part of script_commands.c. */

#include "script_commands_internal.h"

/* Runs Force cast on a target, through fdPsynergyForce. */
void fdPsynergyForceOnTarget(Sint32 unused)
{
	fdPsynergyForce();
}

/* The flashes that burst from over the caster's head, one every 2 ticks. */
#define FORCE_FLASHES 12

/* Writes the position 16 pixels above the actor. */
static __inline__ void fdEffectGetPosAboveActor(ACTOR *actor, VECTOR3 *pos)
{
	SET_POSITION(pos, actor->x.fixed, actor->y.fixed + PSYNERGY_EFFECT_HEIGHT,
		actor->z.fixed);
}

/* Flashes burst from over the caster's head, then a fist (SPRITE_MOVE_HAND)
   appears in front of it, grows and flies forward with three afterimages; what
   it hits (effectState->tgt) is pushed 16 pixels. */
void fdPsynergyForce(void)
{
	ACTOR			   *tgt;
	Sint32				eventTgt;
	ACTOR			   *copies[4];
	VECTOR3				pos;
	ACTOR			   *projectile;
	ACTOR			   *copy;
	ACTOR			   *caster;
	FIELD_EFFECT_STATE *effectState;
	FIELD_PARTICLE	   *effect;
	SPR				   *sharedSpr;
	void			   *event;
	Sint32				scale;
	Sint32				i;
	Uint8				resourceSlot;

	effectState = FieldEffectStateWork;
	tgt = effectState->tgt;
	fdPsynergyFadeToCastCol();
	sySndPlayStub(SE_PSYNERGY_START);
	effect = effectState->particles;

	for(i = 0; i < FORCE_FLASHES; i++) {
		caster = effectState->caster;
		fdEffectGetPosAboveActor(caster, &pos);
		fdCameraWorldToScreen(&pos);
		fdParticleInit(effect, SPRITE_PARTICLE, pos.x, pos.z);
		fdParticleSetCallback(effect, fdEffectUpdateRadialBurst);
		fdParticleSetAnimation(effect, PARTICLE_ANIMATION_FLICKER);
		fdSprSetAnimationColModesStub(effect->spr, SPRITE_COLOR_BLUE_TINT);
		effect->scaleY = toFIXED(7) / 10;
		effect->scaleX = toFIXED(7) / 10;
		sySynch(2);
		effect++;
	}

	/* The fist: 8 pixels in front of the caster, growing from 0.25 to full size
	   by 5/256 a tick (39 ticks). */
	caster = effectState->caster;
	pos.x = caster->x.fixed;
	pos.y = caster->y.fixed + PSYNERGY_EFFECT_HEIGHT;
	pos.z = caster->z.fixed;
	syVectorAddPolarOffset(toFIXED(8), effectState->ang, (FIXED *)&pos);
	projectile = fdActorSpawn(SPRITE_MOVE_HAND, pos.x, pos.y, pos.z);
	if(projectile == NULL) {
		fdPsynergyFadeFromCastCol();
		return;
	}
	projectile->scaleY = toFIXED(1) / 4;
	projectile->scaleX = toFIXED(1) / 4;
	projectile->facing = effectState->ang;
	projectile->speedLimit = toFIXED(4);
	projectile->acceleration = toFIXED(4);
	projectile->motionFlg = 0;
	fdActorSelectAnimationStub(projectile, MOVE_HAND_ANIMATION_FIST);
	fdActorSetSprColModeStub(projectile, SPRITE_COLOR_BGR);
	scale = projectile->scaleX;
	if(scale < toFIXED(1)) {
		do {
			scale += toFIXED(5) / 256;
			projectile->scaleY = scale;
			projectile->scaleX = scale;
			sySynch(1);
			scale = projectile->scaleX;
		} while(scale < toFIXED(1));
	}
	sySynch(3);

	/* Three afterimages, sharing one set of graphics. */
	sharedSpr = NULL;
	for(i = 2; i >= 0; i--) {
		copy = copies[i] = fdActorSpawn(SPRITE_MOVE_HAND, projectile->x.fixed,
			projectile->y.fixed, projectile->z.fixed);
		if(copy != NULL) {
			copy->scaleY = AFTERIMAGE_SCALE;
			copy->scaleX = AFTERIMAGE_SCALE;
			copy->facing = effectState->ang;
			copy->speedLimit = toFIXED(4);
			copy->acceleration = toFIXED(4);
			copy->motionFlg = 0;
			fdActorSelectAnimationStub(copy, MOVE_HAND_ANIMATION_FIST);
			fdActorSetSprColModeStub(copy, SPRITE_COLOR_RBG);
			sharedSpr = fdSprShareVRAMCacheEntry((SPR *)copy->spr, sharedSpr);
		}
	}
	resourceSlot = sharedSpr->slot;

	/* Destination: 56 pixels in front of the caster with effectState->aimAhead
	   set (no cue named the target), otherwise the Psynergy's position. */
	if(effectState->aimAhead != 0) {
		caster = effectState->caster;
		pos.x = caster->x.fixed;
		pos.y = caster->y.fixed + PSYNERGY_EFFECT_HEIGHT;
		pos.z = caster->z.fixed;
		syVectorAddPolarOffset(toFIXED(56), effectState->ang, (FIXED *)&pos);
	} else {
		pos.x = effectState->x;
		pos.y = effectState->y + PSYNERGY_EFFECT_HEIGHT;
		pos.z = effectState->z;
	}
	fdActorSetMoveTargetStub(projectile, pos.x, pos.y, pos.z);
	fdActorStartScriptStub(projectile, ForceFistScript);
	for(i = 0; i < 3; i++) {
		copy = copies[i];
		if(copy != NULL) {
			sySynch(3);
			fdActorSetMoveTargetStub(copy, pos.x, pos.y, pos.z);
			/* From the shared script's SC_SYNC: released once it arrives. */
			fdActorStartScriptStub(copy, FieldEffectSharedParticleScript + 1);
		}
	}

	/* Wait, at most 60 ticks, for the fist's script to end. */
	for(i = 0; i < 60 && projectile->script != NULL; i++)
		sySynch(1);

	/* Push what was hit 16 pixels, at 1 pixel a tick, if nothing is in the
	   way:
	   the ground there must be within a step (fdMapGridCheckCollision 0) and
	   free of actors. */
	if(tgt != NULL && effectState->leaveTgt == 0) {
		if(effectState->sceneVariant != 0)
			tgt->velY = toFIXED(8);
		pos.x = tgt->x.fixed;
		pos.y = tgt->y.fixed;
		pos.z = tgt->z.fixed;
		syVectorAddPolarOffset(toFIXED(16), effectState->ang, (FIXED *)&pos);
		if(fdMapGridCheckCollisionStub(tgt, (FIELD_POS *)&pos) ==
				COLLISION_NONE &&
			fdActorScriptCheckOverlapStub(tgt, (Sint32 *)&pos) == 0) {
			tgt->acceleration = toFIXED(1);
			tgt->speedLimit = toFIXED(1);
			fdActorSetMoveTargetStub(tgt, pos.x, pos.y, pos.z);
		}
	}

	event = fdFindPsynergyCue(
		PSYNERGY_CUE(PSYNERGY_PHASE_ACTS), FIELD_PSYNERGY_FORCE, &eventTgt);
	if(event != NULL)
		fdRunPsynergyScript(
			event, MainRoutineStateWork.leaderActorId, eventTgt);
	sySynch(10);
	fdPsynergyFadeFromCastCol();
	sySynch(20);
	/* VRAM_CACHE_ENTRY_COUNT means no entry to free. */
	if(resourceSlot != VRAM_CACHE_ENTRY_COUNT)
		syVRAMCacheFreeEntry(resourceSlot);
}

#undef FORCE_FLASHES
