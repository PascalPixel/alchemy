/* Running a field Psynergy by its kind, and the priority and depth its sprites
   take to be drawn with an actor. Part of script_commands.c. */

/* Field Psynergy: the dispatchers that run each animation */

void fdPsynergyHaltOnTarget(Sint32 id);
void fdPsynergyForceOnTarget(Sint32 unused);
void fdPsynergyDouseOnTarget(Sint32 unused);
void fdPsynergyFrostOnTarget(Sint32 unused);
void fdPsynergyWhirlwindOnTarget(Sint32 unused);
void fdPsynergyCatchOnTarget(Sint32 tgtId);
void fdPsynergySandOnTarget(Sint32 tgtId);
void fdPsynergyBlazeOnTarget(Sint32 tgtId);
void fdPsynergyCycloneOnTarget(Sint32 tgtId);
void fdPsynergyScoopOnTarget(Sint32 tgtId);
void fdPsynergyTremorOnTarget(Sint32 tgtId);
void fdPsynergyGrindOnTarget(Sint32 tgtId);
void fdPsynergyHoverOnTarget(Sint32 tgtId);
void fdPsynergyTeleportOnTarget(Sint32 tgtId);
void fdPsynergyParchOnTarget(Sint32 tgtId);
void fdPsynergyBurstOnTarget(Sint32 tgtId);
void fdPsynergyPoundOnTarget(Sint32 tgtId);

#include "field/psynergy_run_handler.c"

/* Runs the field Psynergy animation the scene names, with no target chosen yet:
   each kind runs its effect to the end. */
void fdPsynergyRun(void)
{
	FIELD_EFFECT_STATE *scene = RamHeapSlots->fieldEffState;
	FIELD_STATE		   *event = RamHeapSlots->fieldState;
	Sint32				anm = scene->fieldPsynergy;
	Sint32				tgtId = scene->tgtId;
	Sint32				objectId;

	if(anm == FIELD_PSYNERGY_MOVE)
		fdPsynergyMove();
	else if(anm == FIELD_PSYNERGY_LIFT)
		fdPsynergyLift();
	else if(anm == FIELD_PSYNERGY_CARRY)
		fdPsynergyCarry();
	else if(anm == FIELD_PSYNERGY_FORCE)
		fdPsynergyForce();
	else if(anm == FIELD_PSYNERGY_DOUSE)
		fdPsynergyDouse();
	else if(anm == FIELD_PSYNERGY_WHIRLWIND)
		fdPsynergyWhirlwind();
	else if(anm == FIELD_PSYNERGY_FROST)
		fdPsynergyFrost();
	else if(anm == FIELD_PSYNERGY_PLY)
		fdPsynergyPly();
	else if(anm == FIELD_PSYNERGY_GROWTH)
		fdPsynergyGrowth();
	else if(anm == FIELD_PSYNERGY_CATCH)
		fdPsynergyCatch();
	else if(anm == FIELD_PSYNERGY_RETREAT)
		fdPsynergyRetreat();
	else if(anm == FIELD_PSYNERGY_AVOID)
		fdPsynergyAvoid();
	else if(anm == FIELD_PSYNERGY_CLOAK)
		fdPsynergyStartCloak();
	else if(anm == FIELD_PSYNERGY_REVEAL)
		fdPsynergyReveal();
	else if(anm == FIELD_PSYNERGY_LASH)
		fdPsynergyLash();
	else if(anm == FIELD_PSYNERGY_ARROW)
		fdPsynergyArrow();
	else if(anm == FIELD_PSYNERGY_CYCLONE)
		fdPsynergyCyclone();
	else if(anm == FIELD_PSYNERGY_BLAZE)
		fdPsynergyBlaze();
	else if(anm == FIELD_PSYNERGY_BURST)
		fdPsynergyBurst();
	else if(anm == FIELD_PSYNERGY_POUND)
		fdPsynergyPound();
	else if(anm == FIELD_PSYNERGY_SCOOP)
		fdPsynergyScoop();
	else if(anm == FIELD_PSYNERGY_TREMOR)
		fdPsynergyTremor();
	else if(anm == FIELD_PSYNERGY_GRIND)
		fdPsynergyGrind();
	else if(anm == FIELD_PSYNERGY_HOVER)
		fdPsynergyHover();
	else if(anm == FIELD_PSYNERGY_SAND)
		fdPsynergySand();
	else if(anm == FIELD_PSYNERGY_TELEPORT)
		fdPsynergyTeleport();
	else if(anm == FIELD_PSYNERGY_PARCH)
		fdPsynergyParch();
	else if(anm == FIELD_PSYNERGY_RING)
		fdPsynergyRing();
	else if(anm == FIELD_PSYNERGY_HALT) {
		/* Resume the actor stopped before, then stop the nearest one that has
		   an script to run; with none, the stopwatch is thrown at nothing. */
		MAIN_ROUTINE_STATE *party = &MainRoutineStateWork;
		Sint32				paused;

		paused = party->haltedActor;
		if(paused != -1) {
			fdPsynergyResumeHaltedActor(paused);
			party->haltedActor = -1;
		}
		objectId = fdFindNearbyPsynergyTarget(scene->casterId, anm);
		objectId = fdPsynergyFilterHaltTarget(objectId);
		if(fdFindActorTalkCue(objectId) != 0) {
			fdPsynergySetupActorPair(scene->casterId, objectId);
			fdPsynergyHaltOnTarget(objectId);
			fdPsynergyHaltActor(objectId);
			party->haltedActor = objectId;
		} else {
			fdPsynergyHalt();
		}
	} else if(anm == FIELD_PSYNERGY_MIND_READ) {
		/* The wave and colour effect, stopping a running Reveal first. */
		if(event->revealActive != 0)
			fdPsynergyEndReveal();
		fdPsynergyStartMindReadWave(scene->casterId, tgtId);
	}
}

/* Runs the field Psynergy animation the scene names on the target the player
   chose (FIELD_EFFECT_STATE.tgtId). */
void fdPsynergyDispatch(void)
{
	FIELD_EFFECT_STATE *scene = RamHeapSlots->fieldEffState;
	FIELD_STATE		   *event = RamHeapSlots->fieldState;
	Sint32				anm = scene->fieldPsynergy;
	Sint32				tgtId = scene->tgtId;

	scene->aimAhead = 0;
	if(anm == FIELD_PSYNERGY_MOVE)
		fdPsynergySteerPushedTarget(tgtId);
	else if(anm == FIELD_PSYNERGY_LIFT)
		fdPsynergyLiftOnTarget(tgtId);
	else if(anm == FIELD_PSYNERGY_CARRY)
		fdPsynergySteerLiftedTarget(tgtId);
	else if(anm == FIELD_PSYNERGY_FORCE)
		fdPsynergyForceOnTarget(tgtId);
	else if(anm == FIELD_PSYNERGY_DOUSE)
		fdPsynergyDouseOnTarget(tgtId);
	else if(anm == FIELD_PSYNERGY_FROST)
		fdPsynergyFrostOnTarget(tgtId);
	else if(anm == FIELD_PSYNERGY_GROWTH)
		fdPsynergyGrowthOnTarget(tgtId);
	else if(anm == FIELD_PSYNERGY_PLY)
		fdPsynergyPlyOnTarget(tgtId);
	else if(anm == FIELD_PSYNERGY_WHIRLWIND)
		fdPsynergyWhirlwindOnTarget(tgtId);
	else if(anm == FIELD_PSYNERGY_CATCH)
		fdPsynergyCatchOnTarget(tgtId);
	else if(anm == FIELD_PSYNERGY_AVOID)
		fdPsynergyAvoid();
	else if(anm == FIELD_PSYNERGY_LASH)
		fdPsynergyLashOnTarget(tgtId);
	else if(anm == FIELD_PSYNERGY_CLOAK)
		fdPsynergyStartCloak();
	else if(anm == FIELD_PSYNERGY_RETREAT)
		fdPsynergyRetreat();
	else if(anm == FIELD_PSYNERGY_REVEAL)
		fdPsynergyReveal();
	else if(anm == FIELD_PSYNERGY_SAND)
		fdPsynergySandOnTarget(tgtId);
	else if(anm == FIELD_PSYNERGY_BLAZE)
		fdPsynergyBlazeOnTarget(tgtId);
	else if(anm == FIELD_PSYNERGY_CYCLONE)
		fdPsynergyCycloneOnTarget(tgtId);
	else if(anm == FIELD_PSYNERGY_SCOOP)
		fdPsynergyScoopOnTarget(tgtId);
	else if(anm == FIELD_PSYNERGY_TREMOR)
		fdPsynergyTremorOnTarget(tgtId);
	else if(anm == FIELD_PSYNERGY_GRIND)
		fdPsynergyGrindOnTarget(tgtId);
	else if(anm == FIELD_PSYNERGY_HOVER)
		fdPsynergyHoverOnTarget(tgtId);
	else if(anm == FIELD_PSYNERGY_TELEPORT)
		fdPsynergyTeleportOnTarget(tgtId);
	else if(anm == FIELD_PSYNERGY_PARCH)
		fdPsynergyParchOnTarget(tgtId);
	else if(anm == FIELD_PSYNERGY_BURST)
		fdPsynergyBurstOnTarget(tgtId);
	else if(anm == FIELD_PSYNERGY_POUND)
		fdPsynergyPoundOnTarget(tgtId);
	else if(anm == FIELD_PSYNERGY_RING)
		fdPsynergyRing();
	else if(anm == FIELD_PSYNERGY_MIND_READ) {
		/* The wave and colour effect, stopping a running Reveal first; a target
		   other than the halted actor stands still while the wave runs. */
		MAIN_ROUTINE_STATE *party;

		if(event->revealActive != 0)
			fdPsynergyEndReveal();
		party = &MainRoutineStateWork;
		if(party->haltedActor != scene->tgtId)
			((ACTOR *)scene->tgt)->paused = 1;
		fdPsynergyStartMindReadWave(scene->casterId, tgtId);
	} else if(anm == FIELD_PSYNERGY_HALT) {
		/* Resume the actor stopped before, then stop the target. */
		MAIN_ROUTINE_STATE *party = &MainRoutineStateWork;

		if(party->haltedActor != -1) {
			fdPsynergyResumeHaltedActor(party->haltedActor);
			party->haltedActor = -1;
		}
		fdPsynergyHaltActor(tgtId);
		party->haltedActor = tgtId;
		fdPsynergyHaltOnTarget(tgtId);
	}
}

/* Finishes the current field Psynergy when it is Mind Read: ends it and lets
   its target move again, unless the target is the actor kept stopped. */
void fdPsynergyFinishMindRead(void)
{
	FIELD_EFFECT_STATE *effectState = RamHeapSlots->fieldEffState;

	if(effectState->fieldPsynergy == FIELD_PSYNERGY_MIND_READ) {
		fdPsynergyStopMindReadWave();
		if(MainRoutineStateWork.haltedActor != effectState->tgtId) {
			((ACTOR *)effectState->tgt)->paused = 0;
		}
	}
}

/* The extra handler of Reveal, Cloak, Avoid, Sand, Hover and the Ring: what
   each does when it ends. */
void fdPsynergyRunExtraHandler(void)
{
	FIELD_EFFECT_STATE *scene = RamHeapSlots->fieldEffState;

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

/* Per-tick update: sets every part of an actor's sprite to the next of the six
   orders of its colour channels (SPRITE_COLOR_NORMAL to SPRITE_COLOR_BRG). */
void fdActorCycleSprCols(ACTOR *actor)
{
	SPR	  *spr;
	Sint32 cnt;
	Sint32 i;

	if(actor->sprStorageKind == ACTOR_SPRITE_ONE) {
		spr = actor->spr;
		if(spr != 0 && spr->retainCacheEntry == 0) {
			cnt = spr->cnt;
			for(i = 0; i < cnt; i++)
				spr->entries[i]->colMode = TickCount % (SPRITE_COLOR_BRG + 1);
			spr->dirty = 1;
		}
	}
}

/* Sends the actor toward the point magnitude away from it at angle. */
void fdActorSetTargetPosFromMagnitudeAng(
	ACTOR *actor, Sint32 magnitude, Sint32 ang)
{
	Sint32 values[3];

	if(actor != 0) {
		SET_VECTOR(values, actor->x.fixed, actor->y.fixed, actor->z.fixed);
		syVectorAddPolarOffset(magnitude, ang, values);
		fdActorSetMoveTargetStub(actor, values[0], values[1], values[2]);
	}
}

/* The OBJ priority a Psynergy's canvas takes: the caster's own when it casts
   facing away (FACING_NORTH), else 0, in front of everything. */
Sint32 fdPsynergyGetCanvasPriority(ACTOR *actor, Sint32 facing)
{
	if(facing == FACING_NORTH)
		return ((PLACED_SPR *)actor->spr)->pri;
	return 0;
}

/* Returns the OBJ priority of the actor's sprite. */
Sint32 fdActorGetSprPriority(ACTOR *actor)
{
	return ((PLACED_SPR *)actor->spr)->pri;
}

/* On the world map a sprite's depth slot is one more for each two pixels its
   point is nearer the camera than 512, about the camera's distance from the
   ground it looks at, where the slot is 128, as fdRenderPlaceProjectedSpr sorts
   a projected sprite. */
#define WORLD_MAP_GROUND_DEPTH 512
#define WORLD_MAP_GROUND_SLOT  128

/* Returns the depth slot the actor's sprite takes, so that a sprite drawn with
   it sorts with it: from its projected depth on the world map, otherwise from
   its screen row. */
Sint32 fdActorGetSprDepth(ACTOR *actor)
{
	FIXED  point[3];
	Sint32 screen[3];
	Sint32 depth;

	if(FieldStateWork->mode == ACTOR_SYSTEM_WORLD_MAP) {
		point[0] = actor->x.fixed;
		point[1] = actor->y.fixed;
		point[2] = actor->z.fixed;
		syConvert3Dto2D(point, screen);
		depth =
			(WORLD_MAP_GROUND_DEPTH - screen[2]) / 2 + WORLD_MAP_GROUND_SLOT;
		if(depth <= 0)
			depth = 1;
	} else {
		MAP_STATE *map = RamHeapSlots->mapGrid;
		Sint32	   bias = syMapStateView(map)[1] & FIELD_PIXEL_MASK;
		Sint32	   base = map->origin[1];

		depth =
			FIXEDtoINT(actor->y.fixed - base + actor->z.fixed - base - bias) +
			PLACEMENT_IMAGE_DEPTH;
	}
	return depth;
}

/* Draws the Psynergy's canvas sprite (FIELD_EFFECT_STATE.spr) each tick. */
void fdPsynergyDrawCanvasSpr(void)
{
	fdEffectSprDraw(&((FIELD_EFFECT_STATE *)RamHeapSlots->fieldEffState)->spr);
}

/* When the caster faces away (FACING_NORTH), starts the Psynergy's canvas
   sprite (FIELD_EFFECT_STATE.spr): a blank 32x32 sprite of its own VRAM cache
   entry, twice as tall, 28 pixels above the caster in front of everything,
   drawn each tick by fdPsynergyDrawCanvasSpr until
   fdPsynergyReleaseSceneSpr. */
void fdPsynergyStartSceneSpr(void)
{
	FIELD_EFFECT_STATE *effectState = RamHeapSlots->fieldEffState;
	ACTOR			   *mainActor;
	EFFECT_SPR		   *spr;
	FIXED				pos[3];
	void			   *buf;
	Sint32				slot;
	Sint32				tiles;

	mainActor = effectState->caster;
	if(effectState->ang == FACING_NORTH) {
		effectState->sprActive = 1;
		buf = syHeapAlloc(512);
		IWRAM_FILL_WORDS(buf, 512, 0);
		slot = syVRAMCacheFindFreeEntry();
		tiles = syVRAMCacheLoad(slot, 512, buf);
		effectState->sprVramSlot = slot;
		syHeapRelease(buf);
		spr = &effectState->spr;
		fdEffectSprInit(spr, 16, 31, OBJ_SIZE_32X32, tiles);
		spr->depth = 0;
		spr->oam.pri = 0;
		spr->oam.fullCol = 0;
		spr->oam.pal = 0;
		spr->scaleY = toFIXED(2);
		pos[0] = effectState->x;
		pos[1] = mainActor->y.fixed + toFIXED(28);
		pos[2] = mainActor->z.fixed;
		fdCameraWorldToScreen(pos);
		spr->x = pos[0];
		spr->y = pos[2];
		sySetEvent(fdPsynergyDrawCanvasSpr, EVENT_PASS_TICK);
	}
}

#undef WORLD_MAP_GROUND_DEPTH
#undef WORLD_MAP_GROUND_SLOT
