/* @file */
/* A Mercury Djinni joining the party. */
/* Part of event_commands.c. */

#define MERCURY_AFTERIMAGE_MAX 8  /* afterimages of the rising Djinni */
#define MERCURY_RISE_FRAMES	   15 /* frames the Djinni rises, 8 pixels each */

/* Moves the actor halfway to the actor it watches; a copy of */
/* fdActorMoveHalfwayTowardTarget. */
void fdActorMoveHalfwayTowardTargetCopy(ACTOR *actor) {
	ACTOR *target = (ACTOR *)actor->watchedActor;

	actor->x.fixed += (target->x.fixed - actor->x.fixed) / 2;
	actor->y.fixed += (target->y.fixed - actor->y.fixed) / 2;
	actor->z.fixed += (target->z.fixed - actor->z.fixed) / 2;
}

/* A Mercury Djinni joins the party: the screen is tinted blue, the Djinni */
/* bounces twice and hops three times ever higher, then rises 120 pixels out of
 */
/* sight trailing eight afterimages; the leader turns to the camera and raises
 */
/* its arms, and droplets fall about it. */
void fdDjinnRunMercuryJoinEffect(s32 djinniId) {
	ACTOR *djinni;
	ACTOR *copy;
	ACTOR *previous;
	SPR *sharedSprite;
	FIELD_EFFECT_STATE *effectState;
	ACTOR **cursor;
	ACTOR **list;
	FIELD_PARTICLE *effect;
	s32 sprite;
	s32 i;
	s32 sharedEntry;
	ACTOR *copies[MERCURY_AFTERIMAGE_MAX];
	VECTOR3 position;

	djinni = fdActorGetById(djinniId);
	if(djinni == NULL)
		return;

	fdEffectInitSlots();
	effectState = Ram_HeapSlots->fieldEffState;
	mnShopSaveAndTintFieldPaletteStub(PALETTE_TINT_CUSTOM | RGB(4, 4, 16));
	sySynch(30);
	/* Unpause the Djinni, so its script and motion run. */
	djinni->paused = 0;
	sySndPlayStub(SE_NOTE_LOW);
	fdActorRunRepeatedMotion(djinniId, 1);
	sySndPlayStub(SE_NOTE_HIGH);
	fdActorRunRepeatedMotion(djinniId, 1);
	sySynch(20);

	/* Three hops, at 3, 5 and 7 pixels a frame. */
	sySndPlayStub(SE_JUMP);
	fdActorJump(djinniId, 3, 14);
	sySndPlayStub(SE_JUMP);
	fdActorJump(djinniId, 5, 16);
	sySndPlayStub(SE_JUMP);
	fdActorJump(djinniId, 7, 18);
	sySynch(20);

	/* MERCURY_AFTERIMAGE_MAX afterimages of the same sprite at 15/16 size, each
	 */
	/* closing half the distance to the one before it every frame, sharing one
	 */
	/* VRAM cache entry, as in fdDjinnRunBattleStartHop. */
	sprite = ((SPR *)djinni->spr)->entries[0]->anmId;
	sharedSprite = NULL;
	previous = djinni;
	cursor = copies;
	for(i = 0; i < MERCURY_AFTERIMAGE_MAX; i++) {
		copy = fdActorCreateStub(sprite, djinni->x.fixed, djinni->y.fixed,
								 djinni->z.fixed);
		*cursor++ = copy;
		if(copy != NULL) {
			copy->scaleY = AFTERIMAGE_SCALE;
			copy->scaleX = AFTERIMAGE_SCALE;
			copy->motionFlg = 0;
			copy->priFlg = ACTOR_PRIORITY_UNDERFOOT;
			copy->steeringFlg |= ACTOR_FACES_MOTION;
			copy->update = (OBJECT_UPDATE)fdActorMoveHalfwayTowardTargetCopy;
			copy->facing = djinni->facing;
			fdActorSetSprColModeStub(copy, SPRITE_COLOR_BLUE_TINT);
			fdActorSelectAnimationStub(copy, ANIM_STILL);
			fdActorSetSprFlagsStub(copy, 0);
			sharedSprite = fdSprShareVramCacheEntry(copy->spr, sharedSprite);
			copy->watchedActor = (union FIELD_OBJECT *)previous;
			previous = copy;
		}
	}

	sharedEntry = sharedSprite->slot;
	sySndPlayStub(SE_HIGH_JUMP);
	djinni->motionFlg = 0;
	for(i = 0; i < MERCURY_RISE_FRAMES; i++) {
		djinni->y.fixed += toFIXED(8);
		sySynch(1);
	}
	fdActorReleaseStub(djinni);
	list = copies;
	for(i = 0; i < MERCURY_AFTERIMAGE_MAX; i++)
		fdActorReleaseStub((*list++));
	if(sharedEntry != VRAM_CACHE_ENTRY_COUNT)
		syVramCacheFreeEntry(sharedEntry);
	sySynch(10);
	fdActorFaceDirectionById(MainRoutineStateWork.leaderActorId, FACING_SOUTH,
							 0);
	sySynch(20);
	fdActorSelectAnimationStub(
		fdActorGetById(MainRoutineStateWork.leaderActorId),
		ANIM_LEADER_ARMS_RAISED);
	sySynch(20);

	/* The droplets (SPRITE_DROPLET), one a frame, from where the Djinni was on
	 */
	/* screen (fdEffectUpdateRandomTargetParticle moves them). */
	SET_POSITION(&position, djinni->x.fixed, djinni->y.fixed, djinni->z.fixed);
	fdCameraWorldToScr(&position);

	effect = effectState->particles;
	for(i = 0; i < (s32)ARRAY_COUNT(effectState->particles); i++) {
		fdParticleInit(effect, SPRITE_DROPLET, position.x, position.z);
		fdParticleSetCallback(effect,
							  (void *)fdEffectUpdateRandomTargetParticle);
		/* SPRITE_DROPLET has two animations and no twinkle, so the droplet
		 * keeps */
		/* its first (fdSprStartAnimation passes over a number past the
		 * sheet's). */
		fdParticleSetAnimation(effect, ANIM_PARTICLE_TWINKLE);
		fdSprSetAnimationColModesStub(effect->spr, SPRITE_COLOR_BLUE_TINT);
		sySynch(1);
		effect++;
	}

	/* After 120 frames every droplet still in use takes its last fall. */
	sySynch(120);
	for(i = 0; i < PSYNERGY_EFFECT_SLOT_COUNT; i++) {
		if(effectState->particles[i].active)
			effectState->particles[i].state = MERCURY_DROPLET_LAST_FALL;
	}

	sySynch(50);
	mnShopRestoreFieldPaletteStub();
	fdEffectReleaseSlots();
}

#undef MERCURY_AFTERIMAGE_MAX
#undef MERCURY_RISE_FRAMES
