/* A Mars Djinni joining the party, the fast shrink that ends a spin, and a
   Venus Djinni's motes and particles. Part of script_commands.c. */

/* A Mars Djinni joins the party: it turns to the camera, bounces three times,
   spins ever faster and shrinks away in a fountain of sparks
   (fdEffectUpdateMarsSpark). The leader turns to the camera and raises its
   arms, the sparks swarm it while it pulses for 100 frames, and the sparks
   vanish. */
void fdDjinnRunMarsJoinEffect(Sint32 djinniId)
{
	VECTOR3				position;
	FIXED				scale;
	Sint32				remaining;
	ACTOR			   *djinni;
	FIELD_EFFECT_STATE *effectState;
	ACTOR			   *leader;
	FIELD_PARTICLE	   *effect;

	djinni = fdActorGetById(djinniId);
	leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	if(djinni == NULL)
		return;

	fdEffectInitSlots();
	effectState = Ram_HeapSlots->fieldEffState;
	/* Tint the screen red. */
	mnShopSaveAndTintFieldPaletteStub(PALETTE_TINT_CUSTOM | RGB(16, 4, 4));
	sySynch(30);
	/* Turn to face the camera (FACING_SOUTH), then bounce three times, a rising
	   note on each. */
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
	djinni->update = (OBJECT_UPDATE)fdEffectAdvanceAngWithRamp;
	djinni->tmr = 0;
	sySynch(80);
	djinni->update = (OBJECT_UPDATE)fdEffectShrinkActorAndDestroySlow;
	fdActorSelectAnimationStub(djinni, DJINNI_ANIMATION_EXCITED);

	/* The sparks start where the Djinni is on screen. */
	position.x = djinni->x.fixed;
	position.y = djinni->y.fixed;
	position.z = djinni->z.fixed;
	fdCameraWorldToScreen(&position);

	/* One spark a frame into each field effect sprite. */
	effect = effectState->particles;
	for(remaining = 0; remaining < (Sint32)ARRAY_COUNT(effectState->particles);
		remaining++) {
		fdParticleInit(effect, SPRITE_PARTICLE, position.x, position.z);
		fdParticleSetCallback(effect, fdEffectUpdateMarsSpark);
		fdParticleSetAnimation(effect, ANIM_PARTICLE_TWINKLE);
		fdSprSetAnimationColModesStub(effect->spr, SPRITE_COLOR_RED_TINT);
		scale = syRandom() / 3 + toFIXED(1); /* 1.0 to 1.33 */
		effect->scaleY = scale;
		effect->scaleX = scale;
		sySynch(1);
		effect++;
	}

	sySynch(60);
	fdActorFaceDirectionById(
		MainRoutineStateWork.leaderActorId, FACING_SOUTH, 0);
	sySynch(20);
	fdActorSelectAnimationStub(
		fdActorGetById(MainRoutineStateWork.leaderActorId),
		ANIM_LEADER_ARMS_RAISED);

	/* Every spark still in use stops its fountain. */
	sySynch(20);
	for(remaining = 0; remaining < PSYNERGY_EFFECT_SLOT_COUNT; remaining++) {
		if(effectState->particles[remaining].active)
			effectState->particles[remaining].state = MARS_SPARK_STOPPING;
	}

	/* The leader pulses while the sparks swarm it. */
	sySynch(60);
	leader->update = (OBJECT_UPDATE)fdEffectPulseActorScale;
	sySynch(100);

	/* Every spark still in use goes. */
	for(remaining = 0; remaining < PSYNERGY_EFFECT_SLOT_COUNT; remaining++) {
		if(effectState->particles[remaining].active)
			effectState->particles[remaining].state = MARS_SPARK_LEAVING;
	}

	/* The leader stops pulsing. */
	sySynch(10);
	leader->update = NULL;
	leader->scaleX = toFIXED(1);
	leader->scaleY = toFIXED(1);
	/* Fade the palette back from the tint and free the field effect sprites. */
	sySynch(30);
	mnShopRestoreFieldPaletteStub();
	fdEffectReleaseSlots();
}

/* Spins an actor 1/8 turn and shrinks it by 1/64 a frame, releasing it once its
   scale falls below 3/16: 53 frames from full size. */
void fdEffectShrinkActorAndDestroyFast(void *obj)
{
	ACTOR *actor = obj;
	Sint32 step = -(toFIXED(1) / 64);
	Sint32 scaleY;
	Sint32 scale;
	Sint32 angle;

	scaleY = actor->scaleY + step;
	scale = actor->scaleX;
	actor->scaleY = scaleY;
	angle = actor->facing;
	scale += step;
	actor->facing = angle + DEGtoANG(45);
	actor->scaleX = scale;
	if(scale < toFIXED(3) / 16)
		fdActorReleaseStub(actor);
}

/* Per-frame update of a Venus Djinni's falling motes: each circles the leader
   on a shrinking spiral (radius timer * 0.4 pixels) while it sinks a pixel a
   frame, and is released 20 pixels above the leader's feet. Set by
   fdDjinnRunVenusJoinEffect, the Venus Djinni's effect. */
void fdEffectUpdateDescendingOrbitActor(ACTOR *mote)
{
	VECTOR3 position;
	ACTOR  *leader;
	Sint32	timer;
	Sint16	frames;
	FIXED	y;

	leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	timer = mote->orbitFrm - 1;
	mote->orbitFrm = timer;
	frames = mote->orbitFrm;
	position.x = leader->x.fixed;
	position.z = leader->z.fixed;
	syVectorAddPolarOffset(frames * (toFIXED(2) / 5),
		frames * (TRIG_TURN / 32) + mote->orbitAng, &position);
	mote->x.fixed = position.x;
	mote->z.fixed = position.z;
	y = mote->y.fixed - toFIXED(1);
	mote->y.fixed = y;
	if(y < leader->y.fixed + toFIXED(20))
		fdActorReleaseStub(mote);
}

/* Field effect sprite callback of a Venus Djinni's particles: each starts on
   screen at a random point 10 to 20 pixels round the leader and flies 120
   pixels up the screen, over and over, until it is set to
   VENUS_PARTICLE_LAST_RISE and freed once it arrives. */
void fdEffectUpdateRadialSpread(FIELD_PARTICLE *effect)
{
	VECTOR3 position;
	ACTOR  *source;
	Sint32	state;
	Uint32	random;

	source = fdActorGetById(MainRoutineStateWork.leaderActorId);
	state = effect->state;

	if(state == VENUS_PARTICLE_AIM) {
		position.x = source->x.fixed;
		position.y = source->y.fixed;
		position.z = source->z.fixed;

		random = syRandom() * 10 + toFIXED(10);
		syVectorAddPolarOffset(random, syRandom(), &position);
		fdCameraWorldToScreen(&position);

		effect->originX = position.x;
		effect->originZ = position.z;
		effect->x = position.x;
		effect->z = position.z;
		position.x = effect->x;
		position.z = effect->z;

		syVectorAddPolarOffset(toFIXED(120), FACING_NORTH, &position);
		effect->tgtX = position.x;
		effect->tgtZ = position.z;
		effect->acceleration = toFIXED(1);
		effect->speedMax = toFIXED(5);
		effect->turnStepLimited = state;
		effect->state++;

		if((VBlankCount & 1) != 0)
			sySndPlayStub(SE_CLATTER);
	} else if(state == VENUS_PARTICLE_RISING) {
		if(fdParticleHasPendingTarget(effect) == 0)
			effect->state--;
	} else if(state == VENUS_PARTICLE_LAST_RISE) {
		if(fdParticleHasPendingTarget(effect) == 0)
			fdEffectClearOwnedSlot(effect);
	}
}
