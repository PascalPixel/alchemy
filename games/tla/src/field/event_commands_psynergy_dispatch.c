/* @file */
/* Running a field Psynergy by its kind, and the priority and depth its */
/* sprites take to be drawn with an actor. */
/* Part of event_commands.c. */

/* Field Psynergy: the dispatchers that run each animation */

void fdPsynergyHaltOnTarget(s32 id);
void fdPsynergyForceOnTarget(s32 unused);
void fdPsynergyDouseOnTarget(s32 unused);
void fdPsynergyFrostOnTarget(s32 unused);
void fdPsynergyWhirlwindOnTarget(s32 unused);
void fdPsynergyCatchOnTarget(s32 targetId);
void fdPsynergySandOnTarget(s32 targetId);
void fdPsynergyBlazeOnTarget(s32 targetId);
void fdPsynergyCycloneOnTarget(s32 targetId);
void fdPsynergyScoopOnTarget(s32 targetId);
void fdPsynergyTremorOnTarget(s32 targetId);
void fdPsynergyGrindOnTarget(s32 targetId);
void fdPsynergyHoverOnTarget(s32 targetId);
void fdPsynergyTeleportOnTarget(s32 targetId);
void fdPsynergyParchOnTarget(s32 targetId);
void fdPsynergyBurstOnTarget(s32 targetId);
void fdPsynergyPoundOnTarget(s32 targetId);

/* Runs one step of the scene's field Psynergy: cast with no target, cast on */
/* the chosen target (secondActorId), Mind Read's end, or what a lasting */
/* Psynergy does when it ends. */
void fdPsynergyRunHandler(s32 kind) {
	if(kind == FIELD_PSYNERGY_HANDLER_UNTARGETED)
		fdPsynergyRun();
	else if(kind == FIELD_PSYNERGY_HANDLER_ON_TARGET)
		fdPsynergyDispatch();
	else if(kind == FIELD_PSYNERGY_HANDLER_FINISH)
		fdPsynergyFinishMindRead();
	else if(kind == FIELD_PSYNERGY_HANDLER_EXTRA)
		fdPsynergyRunExtraHandler();
}

/* Runs the field Psynergy animation the scene names, with no target chosen */
/* yet: each kind runs its effect to the end. */
void fdPsynergyRun(void) {
	FIELD_EFFECT_STATE *scene = Ram_HeapSlots->fieldEffState;
	FIELD_STATE *event = Ram_HeapSlots->fieldState;
	s32 animation = scene->fieldPsynergy;
	s32 targetId = scene->tgtId;
	s32 objectId;

	if(animation == FIELD_PSYNERGY_MOVE)
		fdPsynergyMove();
	else if(animation == FIELD_PSYNERGY_LIFT)
		fdPsynergyLift();
	else if(animation == FIELD_PSYNERGY_CARRY)
		fdPsynergyCarry();
	else if(animation == FIELD_PSYNERGY_FORCE)
		fdPsynergyForce();
	else if(animation == FIELD_PSYNERGY_DOUSE)
		fdPsynergyDouse();
	else if(animation == FIELD_PSYNERGY_WHIRLWIND)
		fdPsynergyWhirlwind();
	else if(animation == FIELD_PSYNERGY_FROST)
		fdPsynergyFrost();
	else if(animation == FIELD_PSYNERGY_PLY)
		fdPsynergyPly();
	else if(animation == FIELD_PSYNERGY_GROWTH)
		fdPsynergyGrowth();
	else if(animation == FIELD_PSYNERGY_CATCH)
		fdPsynergyCatch();
	else if(animation == FIELD_PSYNERGY_RETREAT)
		fdPsynergyRetreat();
	else if(animation == FIELD_PSYNERGY_AVOID)
		fdPsynergyAvoid();
	else if(animation == FIELD_PSYNERGY_CLOAK)
		fdPsynergyStartCloak();
	else if(animation == FIELD_PSYNERGY_REVEAL)
		fdPsynergyReveal();
	else if(animation == FIELD_PSYNERGY_LASH)
		fdPsynergyLash();
	else if(animation == FIELD_PSYNERGY_ARROW)
		fdPsynergyArrow();
	else if(animation == FIELD_PSYNERGY_CYCLONE)
		fdPsynergyCyclone();
	else if(animation == FIELD_PSYNERGY_BLAZE)
		fdPsynergyBlaze();
	else if(animation == FIELD_PSYNERGY_BURST)
		fdPsynergyBurst();
	else if(animation == FIELD_PSYNERGY_POUND)
		fdPsynergyPound();
	else if(animation == FIELD_PSYNERGY_SCOOP)
		fdPsynergyScoop();
	else if(animation == FIELD_PSYNERGY_TREMOR)
		fdPsynergyTremor();
	else if(animation == FIELD_PSYNERGY_GRIND)
		fdPsynergyGrind();
	else if(animation == FIELD_PSYNERGY_HOVER)
		fdPsynergyHover();
	else if(animation == FIELD_PSYNERGY_SAND)
		fdPsynergySand();
	else if(animation == FIELD_PSYNERGY_TELEPORT)
		fdPsynergyTeleport();
	else if(animation == FIELD_PSYNERGY_PARCH)
		fdPsynergyParch();
	else if(animation == FIELD_PSYNERGY_RING)
		fdPsynergyRing();
	else if(animation == FIELD_PSYNERGY_HALT) {
		/* Resume the actor stopped before, then stop the nearest one that has
		 * an */
		/* event to run; with none, the stopwatch is thrown at nothing. */
		MAIN_ROUTINE_STATE *party = &MainRoutineStateWork;
		s32 paused;

		paused = party->haltedActor;
		if(paused != -1) {
			fdPsynergyResumeHaltedActor(paused);
			party->haltedActor = -1;
		}
		objectId = fdFindNearbyPsynergyTarget(scene->casterId, animation);
		objectId = fdPsynergyFilterHaltTarget(objectId);
		if(scFindActorTalkTrigger(objectId) != 0) {
			fdPsynergySetupActorPair(scene->casterId, objectId);
			fdPsynergyHaltOnTarget(objectId);
			fdPsynergyHaltActor(objectId);
			party->haltedActor = objectId;
		}
		else {
			fdPsynergyHalt();
		}
	}
	else if(animation == FIELD_PSYNERGY_MIND_READ) {
		/* The wave and colour effect, stopping a running Reveal first. */
		if(event->revealActive != 0)
			fdPsynergyEndReveal();
		fdPsynergyStartMindReadWave(scene->casterId, targetId);
	}
}

/* Runs the field Psynergy animation the scene names on the target the player */
/* chose (FIELD_EFFECT_STATE.tgtId). */
void fdPsynergyDispatch(void) {
	FIELD_EFFECT_STATE *scene = Ram_HeapSlots->fieldEffState;
	FIELD_STATE *event = Ram_HeapSlots->fieldState;
	s32 animation = scene->fieldPsynergy;
	s32 targetId = scene->tgtId;

	scene->aimAhead = 0;
	if(animation == FIELD_PSYNERGY_MOVE)
		fdPsynergySteerPushedTarget(targetId);
	else if(animation == FIELD_PSYNERGY_LIFT)
		fdPsynergyLiftOnTarget(targetId);
	else if(animation == FIELD_PSYNERGY_CARRY)
		fdPsynergySteerLiftedTarget(targetId);
	else if(animation == FIELD_PSYNERGY_FORCE)
		fdPsynergyForceOnTarget(targetId);
	else if(animation == FIELD_PSYNERGY_DOUSE)
		fdPsynergyDouseOnTarget(targetId);
	else if(animation == FIELD_PSYNERGY_FROST)
		fdPsynergyFrostOnTarget(targetId);
	else if(animation == FIELD_PSYNERGY_GROWTH)
		fdPsynergyGrowthOnTarget(targetId);
	else if(animation == FIELD_PSYNERGY_PLY)
		fdPsynergyPlyOnTarget(targetId);
	else if(animation == FIELD_PSYNERGY_WHIRLWIND)
		fdPsynergyWhirlwindOnTarget(targetId);
	else if(animation == FIELD_PSYNERGY_CATCH)
		fdPsynergyCatchOnTarget(targetId);
	else if(animation == FIELD_PSYNERGY_AVOID)
		fdPsynergyAvoid();
	else if(animation == FIELD_PSYNERGY_LASH)
		fdPsynergyLashOnTarget(targetId);
	else if(animation == FIELD_PSYNERGY_CLOAK)
		fdPsynergyStartCloak();
	else if(animation == FIELD_PSYNERGY_RETREAT)
		fdPsynergyRetreat();
	else if(animation == FIELD_PSYNERGY_REVEAL)
		fdPsynergyReveal();
	else if(animation == FIELD_PSYNERGY_SAND)
		fdPsynergySandOnTarget(targetId);
	else if(animation == FIELD_PSYNERGY_BLAZE)
		fdPsynergyBlazeOnTarget(targetId);
	else if(animation == FIELD_PSYNERGY_CYCLONE)
		fdPsynergyCycloneOnTarget(targetId);
	else if(animation == FIELD_PSYNERGY_SCOOP)
		fdPsynergyScoopOnTarget(targetId);
	else if(animation == FIELD_PSYNERGY_TREMOR)
		fdPsynergyTremorOnTarget(targetId);
	else if(animation == FIELD_PSYNERGY_GRIND)
		fdPsynergyGrindOnTarget(targetId);
	else if(animation == FIELD_PSYNERGY_HOVER)
		fdPsynergyHoverOnTarget(targetId);
	else if(animation == FIELD_PSYNERGY_TELEPORT)
		fdPsynergyTeleportOnTarget(targetId);
	else if(animation == FIELD_PSYNERGY_PARCH)
		fdPsynergyParchOnTarget(targetId);
	else if(animation == FIELD_PSYNERGY_BURST)
		fdPsynergyBurstOnTarget(targetId);
	else if(animation == FIELD_PSYNERGY_POUND)
		fdPsynergyPoundOnTarget(targetId);
	else if(animation == FIELD_PSYNERGY_RING)
		fdPsynergyRing();
	else if(animation == FIELD_PSYNERGY_MIND_READ) {
		/* The wave and colour effect, stopping a running Reveal first; a target
		 */
		/* other than the halted actor stands still while the wave runs. */
		MAIN_ROUTINE_STATE *party;

		if(event->revealActive != 0)
			fdPsynergyEndReveal();
		party = &MainRoutineStateWork;
		if(party->haltedActor != scene->tgtId)
			((ACTOR *)scene->tgt)->paused = 1;
		fdPsynergyStartMindReadWave(scene->casterId, targetId);
	}
	else if(animation == FIELD_PSYNERGY_HALT) {
		/* Resume the actor stopped before, then stop the target. */
		MAIN_ROUTINE_STATE *party = &MainRoutineStateWork;

		if(party->haltedActor != -1) {
			fdPsynergyResumeHaltedActor(party->haltedActor);
			party->haltedActor = -1;
		}
		fdPsynergyHaltActor(targetId);
		party->haltedActor = targetId;
		fdPsynergyHaltOnTarget(targetId);
	}
}

/* Finishes the current field Psynergy when it is Mind Read: ends it and lets */
/* its target move again, unless the target is the actor kept stopped. */
void fdPsynergyFinishMindRead(void) {
	FIELD_EFFECT_STATE *effectState = Ram_HeapSlots->fieldEffState;

	if(effectState->fieldPsynergy == FIELD_PSYNERGY_MIND_READ) {
		fdPsynergyStopMindReadWave();
		if(MainRoutineStateWork.haltedActor != effectState->tgtId) {
			((ACTOR *)effectState->tgt)->paused = 0;
		}
	}
}

/* The extra handler of Reveal, Cloak, Avoid, Sand, Hover and the Ring: what */
/* each does when it ends. */
void fdPsynergyRunExtraHandler(void) {
	FIELD_EFFECT_STATE *scene = Ram_HeapSlots->fieldEffState;

	switch(scene->fieldPsynergy) {
	case FIELD_PSYNERGY_REVEAL:
		fdPsynergyEndReveal();
		break;
	case FIELD_PSYNERGY_CLOAK:
		fdPsynergyEndCloak();
		break;
	case FIELD_PSYNERGY_AVOID:
		fdPsynergyShowAvoidWoreOff();
		break;
	case FIELD_PSYNERGY_SAND:
		fdPsynergyEndSand();
		break;
	case FIELD_PSYNERGY_HOVER:
		fdPsynergyEndHover();
		break;
	case FIELD_PSYNERGY_RING:
		fdPsynergyEndRing();
		break;
	}
}

/* Per-frame update: sets every part of an actor's sprite to the next of the */
/* six orders of its colour channels (SPRITE_COLOR_NORMAL to SPRITE_COLOR_BRG).
 */
void fdActorCycleSprCols(ACTOR *actor) {
	SPR *sprite;
	s32 count;
	s32 i;

	if(actor->sprStorageKind == ACTOR_SPRITE_ONE) {
		sprite = actor->spr;
		if(sprite != 0 && sprite->retainCacheEntry == 0) {
			count = sprite->cnt;
			for(i = 0; i < count; i++)
				sprite->entries[i]->colMode =
					FrameCount % (SPRITE_COLOR_BRG + 1);
			sprite->dirty = 1;
		}
	}
}

/* Sends the actor toward the point magnitude away from it at angle. */
void fdActorSetTargetPosFromMagnitudeAng(ACTOR *actor, s32 magnitude,
										 s32 angle) {
	s32 values[3];

	if(actor != 0) {
		SET_VECTOR(values, actor->x.fixed, actor->y.fixed, actor->z.fixed);
		syVectorAddPolarOffset(magnitude, angle, values);
		fdActorSetMoveTargetStub(actor, values[0], values[1], values[2]);
	}
}

/* The OBJ priority a Psynergy's canvas takes: the caster's own when it */
/* casts facing away (FACING_NORTH), else 0, in front of everything. */
s32 fdPsynergyGetCanvasPriority(ACTOR *actor, s32 facing) {
	if(facing == FACING_NORTH)
		return ((PLACED_SPR *)actor->spr)->pri;
	return 0;
}

/* Returns the OBJ priority of the actor's sprite. */
s32 fdActorGetSprPriority(ACTOR *actor) {
	return ((PLACED_SPR *)actor->spr)->pri;
}

/* On the world map a sprite's depth slot is one more for each two pixels its */
/* point is nearer the camera than 512, about the camera's distance from the */
/* ground it looks at, where the slot is 128, as fdRenderPlaceProjectedSpr */
/* sorts a projected sprite. */
#define WORLD_MAP_GROUND_DEPTH 512
#define WORLD_MAP_GROUND_SLOT  128

/* Returns the depth slot the actor's sprite takes, so that a sprite drawn */
/* with it sorts with it: from its projected depth on the world map, otherwise
 */
/* from its screen row. */
s32 fdActorGetSprDepth(ACTOR *actor) {
	FIXED point[3];
	s32 screen[3];
	s32 depth;

	if(FieldStateWork->mode == ACTOR_SYSTEM_WORLD_MAP) {
		point[0] = actor->x.fixed;
		point[1] = actor->y.fixed;
		point[2] = actor->z.fixed;
		syConvert3Dto2D(point, screen);
		depth =
			(WORLD_MAP_GROUND_DEPTH - screen[2]) / 2 + WORLD_MAP_GROUND_SLOT;
		if(depth <= 0)
			depth = 1;
	}
	else {
		MAP_STATE *map = Ram_HeapSlots->mapGrid;
		s32 bias = syMapStateView(map)[1] & FIELD_PIXEL_MASK;
		s32 base = map->origin[1];

		depth =
			FIXEDtoINT(actor->y.fixed - base + actor->z.fixed - base - bias) +
			PLACEMENT_IMAGE_DEPTH;
	}
	return depth;
}

/* Draws the Psynergy's canvas sprite (FIELD_EFFECT_STATE.spr) each frame. */
void fdPsynergyDrawCanvasSpr(void) {
	fdEffectSprDraw(&((FIELD_EFFECT_STATE *)Ram_HeapSlots->fieldEffState)->spr);
}

/* When the caster faces away (FACING_NORTH), starts the Psynergy's canvas */
/* sprite (FIELD_EFFECT_STATE.spr): a blank 32x32 sprite of its own VRAM
 * cache
 */
/* entry, twice as tall, 28 pixels above the caster in front of everything, */
/* drawn each frame by fdPsynergyDrawCanvasSpr until */
/* fdPsynergyReleaseSceneSpr. */
void fdPsynergyStartSceneSpr(void) {
	FIELD_EFFECT_STATE *effectState = Ram_HeapSlots->fieldEffState;
	ACTOR *mainActor;
	EFFECT_SPR *sprite;
	FIXED position[3];
	void *buffer;
	s32 slot;
	s32 tiles;

	mainActor = effectState->caster;
	if(effectState->ang == FACING_NORTH) {
		effectState->sprActive = 1;
		buffer = syHeapAlloc(512);
		IWRAM_FILL_WORDS(buffer, 512, 0);
		slot = syVramCacheFindFreeEntry();
		tiles = syVramCacheLoad(slot, 512, buffer);
		effectState->sprVramSlot = slot;
		syHeapRelease(buffer);
		sprite = &effectState->spr;
		fdEffectSprInit(sprite, 16, 31, OBJ_SIZE_32X32, tiles);
		sprite->depth = 0;
		sprite->oam.pri = 0;
		sprite->oam.fullCol = 0;
		sprite->oam.pal = 0;
		sprite->scaleY = FIXED_ONE * 2;
		position[0] = effectState->x;
		position[1] = mainActor->y.fixed + toFIXED(28);
		position[2] = mainActor->z.fixed;
		fdCameraWorldToScr(position);
		sprite->x = position[0];
		sprite->y = position[2];
		sySetEvent(fdPsynergyDrawCanvasSpr, TASK_PASS_FRAME);
	}
}
#undef WORLD_MAP_GROUND_DEPTH
#undef WORLD_MAP_GROUND_SLOT
