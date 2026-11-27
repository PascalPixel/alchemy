/* A Jupiter Djinni joining the party, and per-frame updates the field effects
   share: a pulse, a quickening spin, a slow shrink, and a Mars Djinni's sparks.
   Part of script_commands.c. */

/* A Jupiter Djinni joins the party: it bounces three times, spins, leaps out of
   sight and bursts into PSYNERGY_EFFECT_SLOT_COUNT lights, which circle and
   close in on the leader, who turns to the camera and raises its arms
   (ANIM_LEADER_ARMS_RAISED). */
void fdDjinnRunJupiterJoinEffect(Sint32 djinniId)
{
	VECTOR3				position;
	ACTOR			   *djinni;
	FIELD_EFFECT_STATE *effectState;
	FIELD_PARTICLE	   *effect;
	FIXED				scale;
	Sint32				i;

	djinni = fdActorGetById(djinniId);
	if(djinni == NULL)
		return;
	fdEffectInitSlots();
	effectState = Ram_HeapSlots->fieldEffState;
	/* Tint the screen yellow. */
	mnShopSaveAndTintFieldPaletteStub(PALETTE_TINT_CUSTOM | RGB(12, 12, 4));
	sySndPlayStub(SE_NOTE_LOW);
	fdActorRunRepeatedMotion(djinniId, 1);
	sySndPlayStub(SE_NOTE_MID);
	fdActorRunRepeatedMotion(djinniId, 1);
	sySndPlayStub(SE_NOTE_HIGH);
	fdActorRunRepeatedMotion(djinniId, 1);
	sySynch(20);

	/* Spin, then leap high. */
	sySndPlayStub(SE_SPIN);
	djinni->update = (OBJECT_UPDATE)fdDjinnAdvanceSpinAng;
	sySynch(40);
	sySndPlayStub(SE_HIGH_JUMP);
	fdActorJump(djinniId, 12, 22);

	/* A light particle in every slot the effect state has, from where the
	   Djinni is on screen. */
	position.x = djinni->x.fixed;
	position.y = djinni->y.fixed;
	position.z = djinni->z.fixed;
	fdCameraWorldToScreen(&position);
	fdActorReleaseStub(djinni);
	sySndPlayStub(SE_SCATTER);
	effect = effectState->particles;
	/* One a frame, each in a random colour mode from SPRITE_COLOR_NORMAL to
	   SPRITE_COLOR_GREY. */
	for(i = 0; i < (Sint32)ARRAY_COUNT(effectState->particles); i++) {
		fdParticleInit(effect, SPRITE_PARTICLE, position.x, position.z);
		fdParticleSetCallback(effect, fdEffectRunAngledApproachPhases);
		fdParticleSetAnimation(effect, ANIM_PARTICLE_TWINKLE);
		fdSprSetAnimationColModesStub(
			effect->spr, (syRandom() * (SPRITE_COLOR_GREY + 1)) >> 16);
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
	sySynch(40);
	sySndPlayStub(SE_SCATTER);
	sySynch(100);
	mnShopRestoreFieldPaletteStub();
	fdEffectReleaseSlots();
}

/* Per-frame update: makes an actor pulse, switching its scale between the two
   values of FieldEffect_PulseScales every 4 frames. */
void fdEffectPulseActorScale(ACTOR *actor)
{
	const Uint32 *table = FieldEffect_PulseScales;
	Uint32		  index = ((Uint32)VBlankCount >> 2) & 1;
	Uint32		  scale = table[index];

	actor->scaleX = scale;
	actor->scaleY = scale;
}

/* Per-frame update: spins an actor ever faster, from 1/16 turn a frame up to
   just over 1/8 (DEGtoANG(45) + 64), 80 more each frame for 52 frames. */
void fdEffectAdvanceAngWithRamp(ACTOR *actor)
{
	Uint32 step;

	step = actor->spinRampFrm * 80;
	actor->facing = actor->facing + step + FACING_STEP;
	if(step < FACING_STEP) {
		actor->spinRampFrm = actor->spinRampFrm + 1;
	}
}

/* Per-frame update: spins an actor 1/8 turn and shrinks it by 7/1024 a frame,
   releasing it once its scale falls below 3/16, 119 frames from full size. The
   Mars Djinni's joining shrinks the Djinni away with it. */
void fdEffectShrinkActorAndDestroySlow(void *obj)
{
	ACTOR *actor = obj;
	Sint32 step = -(toFIXED(7) / 1024);
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

/* Field effect sprite callback of a Mars Djinni's sparks (enum
   MARS_SPARK_STATE): the spark flies 120 pixels up the screen (give or take
   3/32 turn) from its origin at up to 5 pixels a frame and starts over, until
   it stops where it arrives, hidden; 40 frames later it swarms to points within
   4 pixels of one 20 pixels over the leader, until it is freed once it
   arrives. */
void fdEffectUpdateMarsSpark(FIELD_PARTICLE *effect)
{
	ACTOR  *owner;
	VECTOR3 position;
	Sint32	state;

	owner = fdActorGetById(MainRoutineStateWork.leaderActorId);
	state = effect->state;

	if(state == MARS_SPARK_AIM) {
		effect->x = effect->originX;
		effect->z = effect->originZ;
		position.x = effect->x;
		position.z = effect->z;
		syVectorAddPolarOffset(toFIXED(120),
			syRandomBelow(TRIG_TURN * 3 / 32) -
				syRandomBelow(TRIG_TURN * 3 / 32) + FACING_NORTH,
			&position);
		effect->tgtX = position.x;
		effect->tgtZ = position.z;
		effect->acceleration = toFIXED(5);
		effect->speedMax = toFIXED(5);
		effect->turnStepLimited = 0;
		effect->state++;

		/* At the leader's sprite priority, until MARS_SPARK_RISING brings it in
		   front after 3 frames. */
		((SPR *)effect->spr)->image.pri = (*(SPR **)&owner->spr)->image.pri;
		effect->flg = 0;
		effect->age = 0;
		if((VBlankCount & 1) != 0)
			sySndPlayStub(SE_HEAVY_IMPACT);
	} else if(state == MARS_SPARK_RISING) {
		if((Sint16)effect->age == 3) {
			((SPR *)effect->spr)->image.pri = 0;
			effect->flg = FIELD_PARTICLE_LIFTED_BY_Z;
		}
		if(fdParticleHasPendingTarget(effect) == 0)
			effect->state--;
	} else if(state == MARS_SPARK_STOPPING) {
		if(fdParticleHasPendingTarget(effect) == 0) {
			effect->originX = effect->x;
			effect->originZ = effect->z;
			((SPR *)effect->spr)->image.pri = 0;
			effect->flg = FIELD_PARTICLE_LIFTED_BY_Z;
			effect->render = 0;
			effect->state++;
			effect->funcDelay = 40;
		}
	} else if(state == MARS_SPARK_AIM_LEADER) {
		effect->render = 1;
		effect->x = effect->originX;
		effect->z = effect->originZ;
		SET_POSITION(&position, owner->x.fixed, owner->y.fixed + toFIXED(20),
			owner->z.fixed);
		fdCameraWorldToScreen(&position);
		syVectorAddPolarOffset(toFIXED(4), syRandom(), &position);
		effect->tgtX = position.x;
		effect->tgtZ = position.z;
		effect->state++;
		if((VBlankCount & 1) != 0)
			sySndPlayStub(SE_CRASH);
	} else if(state == MARS_SPARK_SWARMING) {
		if(fdParticleHasPendingTarget(effect) == 0)
			effect->state--;
	} else if(state == MARS_SPARK_LEAVING) {
		if(fdParticleHasPendingTarget(effect) == 0)
			fdEffectClearOwnedSlot(effect);
	}
}
