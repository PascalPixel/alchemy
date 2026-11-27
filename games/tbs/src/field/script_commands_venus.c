/* A Venus Djinni joining the party. Part of script_commands.c. */

#include "script_commands_internal.h"

/* The motes that circle down onto the leader, evenly spread round a circle. */
#define VENUS_DESCENDING_MOTES 24

/* A Djinni sheet's animation 3 (venus_djinni.tsv): crouched, standing and
   stretched up in turn, a bounce. */
#define DJINNI_ANIMATION_BOUNCE 3

/* A Venus Djinni joins the party: on a green screen it hops twice, spins and
   leaps, then shrinks away; the leader turns to the camera and raises both arms
   as green particles rise and 24 motes circle down onto him, and a burst of
   sparks ends it. */
void fdDjinnRunVenusJoinEffect(Sint32 djinniId)
{
	ACTOR			   *leader;
	ACTOR			   *djinni;
	FIELD_EFFECT_STATE *effectState;
	FIELD_PARTICLE	   *effect;
	Sint32				i;
	FIXED				scale;
	VECTOR3				pos;

	leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	djinni = fdActorGetById(djinniId);
	if(djinni == NULL)
		return;

	fdEffectInitSlots();
	effectState = FieldEffectStateWork;
	/* Tint the screen green: PALETTE_TINT_CUSTOM | RGB(4, 16, 4), the form
	   script_commands_mercury.c spells out for its blue. */
	mnShopSaveAndTintFieldPaletteStub(PALETTE_TINT_CUSTOM | RGB(4, 16, 4));
	sySynch(30);
	djinni->paused = 0;
	sySoundPlayStub(SE_JUMP);
	fdActorJump(djinniId, 4, 15);
	sySoundPlayStub(SE_JUMP);
	fdActorJump(djinniId, 4, 15);
	/* Spin and leap, then shrink away while the screen shakes (1.3 pixels). */
	sySynch(30);
	djinni->update = fdEffectAdvanceSpinAngle;
	sySoundPlayStub(SE_HIGH_JUMP);
	fdActorJump(djinniId, 8, 22);
	sySoundPlayStub(SE_SPIN);
	fdMapGridSetScreenShakeStub(toFIXED(13) / 10, toFIXED(13) / 10, toFIXED(1));
	djinni->update = fdEffectShrinkActorAndDestroyFast;
	fdActorSelectAnimationStub(djinni, DJINNI_ANIMATION_BOUNCE);
	/* The leader turns to face the camera (FACING_SOUTH). */
	sySynch(90);
	fdActorFaceDirectionById(
		MainRoutineStateWork.leaderActorId, FACING_SOUTH, 0);
	sySynch(20);
	fdActorSelectAnimationStub(
		fdActorGetById(MainRoutineStateWork.leaderActorId),
		ANIM_LEADER_ARMS_RAISED);
	sySynch(30);
	fdMapGridSetScreenShakeStub(toFIXED(8) / 5, toFIXED(8) / 5, toFIXED(1));

	pos.x = djinni->x.fixed;
	pos.y = djinni->y.fixed;
	pos.z = djinni->z.fixed;
	fdCameraWorldToScreen(&pos);

	/* One rising particle a tick into each of the field effect sprites. */
	effect = effectState->particles;
	for(i = 0; i < PSYNERGY_EFFECT_SLOT_COUNT; i++) {
		fdParticleInit(effect, SPRITE_PARTICLE, pos.x, pos.z);
		fdParticleSetCallback(effect, fdEffectUpdateRadialSpread);
		fdParticleSetAnimation(effect, PARTICLE_ANIMATION_FLICKER);
		fdSpriteSetAnimationColorModesStub(
			effect->sprite, SPRITE_COLOR_GREEN_TINT);
		effect->scaleX = toFIXED(1) / 2;
		effect->scaleY = syRandom() + toFIXED(3) / 2;
		sySynch(1);
		effect++;
	}

	/* After 140 ticks every particle still in use stops rising. */
	sySynch(140);
	for(i = 0; i < PSYNERGY_EFFECT_SLOT_COUNT; i++) {
		if(effectState->particles[i].active)
			effectState->particles[i].state = VENUS_PARTICLE_LAST_RISE;
	}

	/* A shake of 1/65536 pixel: in effect the shake stops. */
	sySynch(20);
	fdMapGridSetScreenShakeStub(1, 1, 1);
	sySynch(30);

	/* The motes, 120 pixels above the leader; djinni, gone now, holds each mote
	   and then each spark. */
	for(i = 0; i < VENUS_DESCENDING_MOTES; i++) {
		pos.x = leader->x.fixed;
		pos.y = leader->y.fixed + toFIXED(120);
		pos.z = leader->z.fixed;
		djinni = fdActorSpawn(SPRITE_PARTICLE, pos.x, pos.y, pos.z);
		if(djinni != NULL) {
			scale = syRandom() / 3 + toFIXED(1);
			djinni->scaleY = scale;
			djinni->scaleX = scale;
			djinni->orbitFrame = 100;
			djinni->orbitAngle = i * TRIG_TURN / VENUS_DESCENDING_MOTES;
			djinni->update = fdEffectUpdateDescendingOrbitActor;
			djinni->motionFlags = 0;
			fdActorSelectAnimationStub(djinni, 7);
			fdActorSetSpriteColorModeStub(djinni, SPRITE_COLOR_GREEN_TINT);
		}
	}

	sySynch(100);
	sySoundPlayStub(SE_STOP_ACTION_SE);
	sySynch(1);
	sySoundPlayStub(SE_LIGHT_APPEAR);
	pos.x = leader->x.fixed;
	pos.y = leader->y.fixed + toFIXED(18);
	pos.z = leader->z.fixed;
	/* 8 sparks from 18 pixels above the leader, each thrown 32 pixels in a
	   random direction at 1.4 to 2.4 pixels a tick and 5 up (gravity is
	   ACTOR.velocityY), falling, and released after 8 ticks; stop at the first
	   that cannot be spawned. */
	for(i = 0; i <= 7; i++) {
		djinni = fdActorSpawn(SPRITE_PARTICLE, pos.x, pos.y, pos.z);
		if(djinni == NULL)
			break;
		djinni->scaleY = toFIXED(3) / 5;
		djinni->scaleX = toFIXED(3) / 5;
		djinni->motionFlags = ACTOR_FALLS;
		djinni->velocityY = toFIXED(5);
		djinni->groundHeight = djinni->y.fixed;
		djinni->speedLimit = syRandom() + toFIXED(7) / 5;
		fdActorSetTargetPosFromMagnitudeAngle(djinni, toFIXED(32), syRandom());
		fdActorSetSpriteColorModeStub(djinni, SPRITE_COLOR_GREEN_TINT);
		djinni->wait = 8;
		fdActorStartScriptStub(djinni, FieldEffectSharedParticleScript);
	}

	sySynch(15);
	mnShopRestoreFieldPaletteStub();
	fdEffectReleaseSlots();
}

/* Field effect sprite callback of a Mercury Djinni's droplets: each falls from
   100 pixels above (on screen) to a random point 2 to 8 pixels round a spot 15
   to 20 pixels over the leader, at 3 to 6 pixels a tick, and where it lands
   splashes 12 pixels in a random direction, at half size, with SE_CRACK. */
void fdEffectUpdateRandomTargetParticle(FIELD_PARTICLE *effect)
{
	ACTOR  *src;
	VECTOR3 pos;
	Sint32	state;

	src = fdActorGetById(MainRoutineStateWork.leaderActorId);
	state = effect->state;

	if(state == MERCURY_DROPLET_AIM) {
		pos.x = src->x.fixed;
		pos.y = src->y.fixed + syRandom() * 5 + toFIXED(15);
		pos.z = src->z.fixed;
		fdCameraWorldToScreen(&pos);
		syVectorAddPolarOffset(
			syRandom() * 6 + toFIXED(2), syRandom(), (FIXED *)&pos);

		effect->targetX = pos.x;
		effect->targetZ = pos.z;
		effect->x = pos.x;
		effect->z = pos.z - toFIXED(100);
		effect->acceleration = toFIXED(3);
		effect->speedMax = syRandom() * 3 + toFIXED(3);
		effect->scaleX = toFIXED(1);
		effect->scaleY = toFIXED(1);
		effect->turnStepLimited = state;
		effect->stopAtTarget = 1;
		effect->state++;
	} else if((Uint8)(effect->state - MERCURY_DROPLET_FALLING) <=
		MERCURY_DROPLET_LAST_FALL - MERCURY_DROPLET_FALLING) {
		if(fdParticleHasPendingTarget(effect) == 0) {
			pos.x = effect->x;
			pos.z = effect->z;
			syVectorAddPolarOffset(toFIXED(12), syRandom(), (FIXED *)&pos);
			effect->targetX = pos.x;
			effect->targetZ = pos.z;
			effect->stopAtTarget = 0;
			effect->speed = toFIXED(1);
			effect->acceleration = 0;
			effect->speedMax = syRandom() + toFIXED(11) / 5;
			effect->scaleX = toFIXED(1) / 2;
			effect->scaleY = toFIXED(1) / 2;
			sySoundPlayStub(SE_CRACK);

			if(effect->state == MERCURY_DROPLET_FALLING)
				effect->state--;
			else
				effect->state++;
			effect->callbackDelay = 6;
		}
	} else if(state == MERCURY_DROPLET_DONE) {
		fdEffectClearOwnedSlot(effect);
	}
}

/* Per-tick update of an afterimage: closes half the distance to the actor it
   watches. */
void fdEffectHalveDistanceToTarget(ACTOR *actor)
{
	fdEffectMoveAfterimageHalfway(actor);
}

#undef VENUS_DESCENDING_MOTES
#undef DJINNI_ANIMATION_BOUNCE
