/* @file */
/* A Mars Djinni joining the party, and the first of the Venus Djinni's */
/* functions, linked after it: its shrinking, its falling motes and its rising
 */
/* particles. */
/* Part of event_commands.c. */

#include "event_commands_internal.h"

/* A Mars Djinni joins the party: it turns to the camera, bounces three times,
 */
/* spins ever faster and shrinks away in a fountain of 24 sparks */
/* (fdEffectUpdateMarsSpark). The leader turns to the camera and raises */
/* its arms, the sparks swarm it while it pulses for 100 frames, and the */
/* sparks vanish. */
void fdDjinnRunMarsJoinEffect(s32 djinniId) {
	VECTOR3 position;
	FIXED scale;
	s32 i;
	ACTOR *djinni;
	FIELD_EFFECT_STATE *effectState;
	ACTOR *leader;
	FIELD_PARTICLE *effect;

	djinni = fdActorGetById(djinniId);
	leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	if(djinni == NULL)
		return;

	fdEffectInitSlots();
	effectState = FieldEffectStateWork;
	/* Tint the screen red. */
	mnShopSaveAndTintFieldPaletteStub(PALETTE_TINT_CUSTOM | RGB(16, 4, 4));
	sySynch(30);
	/* Turn to face the camera (FACING_SOUTH), then bounce three times, a rising
	 */
	/* note on each. */
	fdActorFaceDirectionById(djinniId, FACING_SOUTH, 0);
	sySynch(20);
	sySndPlayStub(SE_NOTE_LOW);
	fdActorRunRepeatedMotion(djinniId, 1);
	sySndPlayStub(SE_NOTE_MID);
	fdActorRunRepeatedMotion(djinniId, 1);
	sySndPlayStub(SE_NOTE_HIGH);
	fdActorRunRepeatedMotion(djinniId, 1);
	/* Spin up for 80 frames, then spin and shrink away. */
	sySynch(20);
	sySndPlayStub(SE_SPIN);
	djinni->update = fdEffectAdvanceAngWithRamp;
	djinni->tmr = 0;
	sySynch(80);
	djinni->update = fdEffectShrinkActorAndDestroySlow;
	fdActorSelectAnimationStub(djinni, 3);

	/* The sparks start where the Djinni is on screen. */
	position.x = djinni->x.fixed;
	position.y = djinni->y.fixed;
	position.z = djinni->z.fixed;
	fdCameraWorldToScr(&position);

	/* One spark a frame into each of the field effect sprites. */
	for(i = 0; i < PSYNERGY_EFFECT_SLOT_COUNT; i++) {
		effect = &effectState->particles[i];
		fdParticleInit(effect, SPRITE_PARTICLE, position.x, position.z);
		fdParticleSetCallback(effect, fdEffectUpdateMarsSpark);
		fdParticleSetAnimation(effect, PARTICLE_ANIMATION_FLICKER);
		fdSprSetAnimationColModesStub(effect->spr, SPRITE_COLOR_RED_TINT);
		scale = syRandom() / 3 + FIXED_ONE; /* 1.0 to 1.33 */
		effect->scaleY = scale;
		effect->scaleX = scale;
		sySynch(1);
	}

	sySynch(60);
	fdActorFaceDirectionById(MainRoutineStateWork.leaderActorId, FACING_SOUTH,
							 0);
	sySynch(20);
	fdActorSelectAnimationStub(
		fdActorGetById(MainRoutineStateWork.leaderActorId),
		ANIM_LEADER_ARMS_RAISED);

	/* Every spark still in use stops its fountain. */
	sySynch(20);
	for(i = 0; i < PSYNERGY_EFFECT_SLOT_COUNT; i++) {
		if(effectState->particles[i].active)
			effectState->particles[i].state = MARS_SPARK_STOPPING;
	}

	/* The leader pulses while the sparks swarm it. */
	sySynch(60);
	leader->update = fdEffectPulseActorScale;
	sySynch(100);

	/* Every spark still in use goes. */
	for(i = 0; i < PSYNERGY_EFFECT_SLOT_COUNT; i++) {
		if(effectState->particles[i].active)
			effectState->particles[i].state = MARS_SPARK_LEAVING;
	}

	/* The leader stops pulsing. */
	sySynch(10);
	leader->update = NULL;
	leader->scaleX = FIXED_ONE;
	leader->scaleY = FIXED_ONE;
	/* Fade the palette back from the tint and free the field effect sprites. */
	sySynch(30);
	mnShopRestoreFieldPaletteStub();
	fdEffectReleaseSlots();
}

/* Spins an actor an eighth of a turn and shrinks it by 1/64 of its size a */
/* frame, then releases it: 53 frames from full size. The Venus Djinni's */
/* joining shrinks the Djinni away with it. */
DEFINE_SHRINK_AND_DESTROY(fdEffectShrinkActorAndDestroyFast, -(FIXED_ONE / 64))

/* Per-frame update of a Venus Djinni's falling motes: each circles the leader
 */
/* a 32nd of a turn a frame on a shrinking spiral (0.4 pixels of radius for */
/* each frame left) while it sinks a pixel a frame, and is released 20 pixels */
/* above the leader's feet. */
void fdEffectUpdateDescendingOrbitActor(ACTOR *mote) {
	VECTOR3 position;
	ACTOR *leader;
	s32 timer;
	s16 frames;
	FIXED y;

	leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	timer = mote->orbitFrm - 1;
	mote->orbitFrm = timer;
	frames = mote->orbitFrm;
	position.x = leader->x.fixed;
	position.z = leader->z.fixed;
	syVectorAddPolarOffset(frames * (toFIXED(2) / 5),
						   frames * (TRIG_TURN / 32) + mote->orbitAng,
						   (FIXED *)&position);
	mote->x.fixed = position.x;
	mote->z.fixed = position.z;
	y = mote->y.fixed - toFIXED(1);
	mote->y.fixed = y;
	if(y < leader->y.fixed + toFIXED(20))
		fdActorReleaseStub(mote);
}

/* Field effect sprite callback of a Venus Djinni's particles: each starts on */
/* screen at a random point 10 to 20 pixels round the leader and flies 120 */
/* pixels up the screen, over and over, until it is set to */
/* VENUS_PARTICLE_LAST_RISE and freed once it arrives. */
void fdEffectUpdateRadialSpread(FIELD_PARTICLE *effect) {
	VECTOR3 position;
	ACTOR *leader;
	s32 state;
	u32 random;

	leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	state = effect->state;

	if(state == VENUS_PARTICLE_AIM) {
		position.x = leader->x.fixed;
		position.y = leader->y.fixed;
		position.z = leader->z.fixed;

		random = syRandom() * 10 + toFIXED(10);
		syVectorAddPolarOffset(random, syRandom(), (FIXED *)&position);
		fdCameraWorldToScr(&position);

		effect->originX = position.x;
		effect->originZ = position.z;
		effect->x = position.x;
		effect->z = position.z;
		position.x = effect->x;
		position.z = effect->z;

		syVectorAddPolarOffset(toFIXED(120), FACING_NORTH, (FIXED *)&position);
		effect->tgtX = position.x;
		effect->tgtZ = position.z;
		effect->acceleration = FIXED_ONE;
		effect->speedMax = toFIXED(5);
		effect->turnStepLimited = state;
		effect->state++;

		if((VBlankCount & 1) != 0)
			sySndPlayStub(SE_CLATTER);
	}
	else if(state == VENUS_PARTICLE_RISING) {
		if(fdParticleHasPendingTarget(effect) == 0)
			effect->state--;
	}
	else if(state == VENUS_PARTICLE_LAST_RISE) {
		if(fdParticleHasPendingTarget(effect) == 0)
			fdEffectClearOwnedSlot(effect);
	}
}
