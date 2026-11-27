/* A Venus Djinni joining the party, and a Mercury Djinni's droplets. Part of
   script_commands.c. */

struct FIELD_EFFECT_STATE;

/* The motes that circle down onto the leader as a Venus Djinni joins. */
enum { VENUS_JOIN_MOTES = 24 };

/* A Venus Djinni joins the party: it hops twice, spins and leaps, then shrinks
   away; the leader turns to the camera and plays ANIM_LEADER_ARMS_RAISED while
   24 particles rise round it (fdEffectUpdateRadialSpread). */
void fdDjinnRunVenusJoinEffect(Sint32 djinniId)
{
	ACTOR			   *leader;
	ACTOR			   *djinni;
	FIELD_EFFECT_STATE *effectState;
	FIELD_PARTICLE	   *effect;
	Sint32				remaining;
	FIXED				scale;
	VECTOR3				position;

	leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	djinni = fdActorGetById(djinniId);
	if(djinni == NULL)
		return;

	fdEffectInitSlots();
	effectState = Ram_HeapSlots->fieldEffState;
	/* Tint the screen green: PALETTE_TINT_CUSTOM | RGB(4, 16, 4), the form
	   script_commands_mercury.c spells out for its blue. */
	mnShopSaveAndTintFieldPaletteStub(PALETTE_TINT_CUSTOM | RGB(4, 16, 4));
	sySynch(30);
	djinni->paused = 0;
	sySndPlayStub(SE_JUMP);
	fdActorJump(djinniId, 4, 15);
	sySndPlayStub(SE_JUMP);
	fdActorJump(djinniId, 4, 15);
	/* Spin and leap, then shrink away while the screen shakes (1.3 pixels). */
	sySynch(30);
	djinni->update = (OBJECT_UPDATE)fdDjinnAdvanceSpinAng;
	sySndPlayStub(SE_HIGH_JUMP);
	fdActorJump(djinniId, 8, 22);
	sySndPlayStub(SE_SPIN);
	fdMapGridSetScreenShakeStub(toFIXED(13) / 10, toFIXED(13) / 10, toFIXED(1));
	djinni->update = (OBJECT_UPDATE)fdEffectShrinkActorAndDestroyFast;
	fdActorSelectAnimationStub(djinni, DJINNI_ANIMATION_EXCITED);
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

	position.x = djinni->x.fixed;
	position.y = djinni->y.fixed;
	position.z = djinni->z.fixed;
	fdCameraWorldToScreen(&position);

	/* One rising particle a frame into each of the 24 field effect sprites. */
	effect = effectState->particles;
	for(remaining = 0; remaining < (Sint32)ARRAY_COUNT(effectState->particles);
		remaining++) {
		fdParticleInit(effect, SPRITE_PARTICLE, position.x, position.z);
		fdParticleSetCallback(effect, fdEffectUpdateRadialSpread);
		fdParticleSetAnimation(effect, ANIM_PARTICLE_TWINKLE);
		fdSprSetAnimationColModesStub(effect->spr, SPRITE_COLOR_GREEN_TINT);
		effect->scaleX = toFIXED(1) / 2;
		effect->scaleY = syRandom() + toFIXED(3) / 2;
		sySynch(1);
		effect++;
	}

	/* After 140 frames every particle still in use stops rising. */
	sySynch(140);
	for(remaining = 0; remaining < PSYNERGY_EFFECT_SLOT_COUNT; remaining++) {
		if(effectState->particles[remaining].active)
			effectState->particles[remaining].state = VENUS_PARTICLE_LAST_RISE;
	}

	/* A shake of 1/65536 pixel: in effect the shake stops. */
	sySynch(20);
	fdMapGridSetScreenShakeStub(1, 1, 1);
	sySynch(30);

	/* The motes, evenly spread round a circle, 120 pixels above the leader. */
	for(remaining = 0; remaining < VENUS_JOIN_MOTES; remaining++) {
		position.x = leader->x.fixed;
		position.y = leader->y.fixed + toFIXED(120);
		position.z = leader->z.fixed;
		djinni =
			fdActorSpawn(SPRITE_PARTICLE, position.x, position.y, position.z);
		if(djinni != NULL) {
			scale = syRandom() / 3 + toFIXED(1);
			djinni->scaleY = scale;
			djinni->scaleX = scale;
			djinni->orbitFrm = 100;
			djinni->orbitAng = remaining * TRIG_TURN / VENUS_JOIN_MOTES;
			djinni->update = (OBJECT_UPDATE)fdEffectUpdateDescendingOrbitActor;
			djinni->motionFlg = 0;
			fdActorSelectAnimationStub(djinni, ANIM_PARTICLE_TWINKLE);
			fdActorSetSprColModeStub(djinni, SPRITE_COLOR_GREEN_TINT);
		}
	}

	sySynch(100);
	sySndPlayStub(SE_STOP_EXIT_SE);
	sySynch(1);
	sySndPlayStub(SE_LIGHT_APPEAR);
	position.x = leader->x.fixed;
	position.y = leader->y.fixed + toFIXED(18);
	position.z = leader->z.fixed;
	/* 8 sparks from 18 pixels above the leader, each thrown 32 pixels in a
	   random direction at 1.4 to 2.4 pixels a frame and 5 up (gravity is
	   ACTOR.velY), falling, and released after 8 frames; stop at the first that
	   cannot be spawned. */
	for(remaining = 0; remaining <= 7; remaining++) {
		djinni =
			fdActorSpawn(SPRITE_PARTICLE, position.x, position.y, position.z);
		if(djinni == NULL)
			break;
		djinni->scaleY = toFIXED(3) / 5;
		djinni->scaleX = toFIXED(3) / 5;
		djinni->motionFlg = ACTOR_FALLS;
		djinni->velY = toFIXED(5);
		djinni->groundHeight = djinni->y.fixed;
		djinni->speedLimit = syRandom() + toFIXED(7) / 5;
		fdActorSetTargetPosFromMagnitudeAng(djinni, toFIXED(32), syRandom());
		fdActorSetSprColModeStub(djinni, SPRITE_COLOR_GREEN_TINT);
		djinni->wait = 8;
		fdActorStartScriptStub(djinni, FieldEffect_ReleaseScript);
	}

	sySynch(15);
	mnShopRestoreFieldPaletteStub();
	fdEffectReleaseSlots();
}

/* Field effect sprite callback of a Mercury Djinni's droplets: each falls from
   100 pixels above (on screen) to a random point 2 to 8 pixels round a spot 15
   to 20 pixels over the leader, at 3 to 6 pixels a frame, and where it lands
   splashes 12 pixels in a random direction, at half size, with SE_CRACK. It
   then falls again 6 frames later, until fdDjinnRunMercuryJoinEffect sets its
   state to MERCURY_DROPLET_LAST_FALL: its next splash is its last. */
void fdEffectUpdateRandomTargetParticle(FIELD_PARTICLE *effect)
{
	ACTOR  *source;
	VECTOR3 position;
	Sint32	state;

	source = fdActorGetById(MainRoutineStateWork.leaderActorId);
	state = effect->state;

	if(state == MERCURY_DROPLET_AIM) {
		position.x = source->x.fixed;
		position.y = source->y.fixed + syRandom() * 5 + toFIXED(15);
		position.z = source->z.fixed;
		fdCameraWorldToScreen(&position);
		syVectorAddPolarOffset(
			syRandom() * 6 + toFIXED(2), syRandom(), &position);

		effect->tgtX = position.x;
		effect->tgtZ = position.z;
		effect->x = position.x;
		effect->z = position.z - toFIXED(100);
		effect->acceleration = toFIXED(3);
		effect->speedMax = syRandom() * 3 + toFIXED(3);
		effect->scaleX = toFIXED(1);
		effect->scaleY = toFIXED(1);
		effect->turnStepLimited = state;
		effect->stopAtTgt = 1;
		effect->state++;
	} else if((Uint8)(effect->state - MERCURY_DROPLET_FALLING) <=
		MERCURY_DROPLET_LAST_FALL - MERCURY_DROPLET_FALLING) {
		if(fdParticleHasPendingTarget(effect) == 0) {
			position.x = effect->x;
			position.z = effect->z;
			syVectorAddPolarOffset(toFIXED(12), syRandom(), &position);
			effect->tgtX = position.x;
			effect->tgtZ = position.z;
			effect->stopAtTgt = 0;
			effect->speed = toFIXED(1);
			effect->acceleration = 0;
			effect->speedMax = syRandom() + toFIXED(11) / 5;
			effect->scaleX = toFIXED(1) / 2;
			effect->scaleY = toFIXED(1) / 2;
			sySndPlayStub(SE_CRACK);

			if(effect->state == MERCURY_DROPLET_FALLING)
				effect->state--;
			else
				effect->state++;
			effect->funcDelay = 6;
		}
	} else if(state == MERCURY_DROPLET_DONE) {
		fdEffectClearOwnedSlot(effect);
	}
}
