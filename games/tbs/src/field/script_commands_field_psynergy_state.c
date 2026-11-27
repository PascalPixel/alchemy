/* A field Psynergy's state: setting it up, its caster and target, cleaning up.
   Part of script_commands.c. */

#include "script_commands_internal.h"

/* The ticks fdPsynergyEnd waits at most for the swelling arcs to end. */
#define END_ARC_WAIT_TICKS 30

/* The caster's casting animations on the party's sprite sheets:
   ANIM_CAST_PSYNERGY's first two pictures, held on the second, and from the
   second to its last, held on the last. */
#define ANIM_CAST_PSYNERGY_START 20
#define ANIM_CAST_PSYNERGY_END	 21

/* A PALETTE_FADE half's colours, the BG's and then the sprites', three channels
   each. */
#define FADE_HALF_COLORS   224
#define FADE_HALF_CHANNELS (FADE_HALF_COLORS * 3)

/* Event: updates the field Psynergy sprites. */
void fdEffectUpdateAllEffectSlots(void)
{
	fdEffectUpdateEffectSlots();
}

/* Prepares the state of the field Psynergy for abilityId
   (HEAP_SLOT_FIELD_EFFECT) and loads its spark tiles; mode is where it runs, an
   ACTOR_SYSTEM_MODE. */
void fdPsynergyLoadResources(Sint32 abilityId, Sint32 mode)
{
	FIELD_STATE			  *fieldState = FieldStateWork;
	MAP_GRID_SCROLL_STATE *grid = MapGridState[0];
	FIELD_EFFECT_STATE	  *effectState;
	Sint32				   running;

	running = fieldState->psynergyActing;
	if(running == 0) {
		effectState =
			syHeapSlotAllocate(HEAP_SLOT_FIELD_EFFECT, sizeof *effectState);
		DMA_FILL32(running, effectState, sizeof *effectState);
	} else {
		effectState = FieldEffectStateWork;
	}
	effectState->ability = abilityId;
	effectState->fieldPsynergy =
		gmAbilityGetDirectStub(abilityId)->fieldPsynergy;
	running = fieldState->psynergyActing;
	if(running != 0)
		return;
	effectState->freeBlocks = VRAM_BLOCK_COUNT - fdVRAMCacheCountFreeBlocks();
	effectState->mode = mode;
	effectState->flashCaster = 1;
	effectState->aimAhead = 1;
	effectState->announceName = 1;
	effectState->msgMode = 1;
	/* Keep the map's shake and its decay, to restore after the Psynergy. */
	effectState->savedShakeX = grid->shakeX;
	effectState->savedShakeY = grid->shakeY;
	effectState->savedShakeDecay = grid->shakeDecay;
	if(MainRoutineStateWork.mapId == SCENE_MERCURY_LIGHTHOUSE_ENTRANCE)
		effectState->lit = 1;
	if(MainRoutineStateWork.mapId == SCENE_MERCURY_LIGHTHOUSE_MIDDLE)
		effectState->lit = 1;
	fdPsynergySetupActorPair(MainRoutineStateWork.leaderActorId, -1);
	/* Every field Psynergy but Reveal itself pauses Reveal's countdown (running
	   is 0 here) until fdPsynergyEnd lets it run again. */
	if(effectState->fieldPsynergy != FIELD_PSYNERGY_REVEAL)
		fieldState->effectTimerRunning = running;
	effectState->tileSlot = syVRAMCacheFindFreeEntry();
	syVRAMCacheLoad(effectState->tileSlot, 256, FieldEffectSparkTiles);
	sySetEvent(fdEffectUpdateAllEffectSlots, EVENT_PASS_TICK);
}

/* Records the caster and the target of a field Psynergy, and where it takes
   place: on the target, or 16 pixels in front of the caster with none. */
void fdPsynergySetupActorPair(Sint32 userId, Sint32 targetId)
{
	ACTOR			   *user;
	ACTOR			   *target;
	Sint32				facing;
	FIELD_EFFECT_STATE *effectState;

	effectState = FieldEffectStateWork;
	effectState->casterId = userId;
	user = fdActorLookup((Sint16)userId);
	effectState->targetId = targetId;
	effectState->caster = user;
	target = fdActorLookup((Sint16)targetId);
	facing = FACING_NEAREST_CARDINAL(user->facing);
	effectState->target = target;
	effectState->angle = facing;
	if(target != NULL) {
		/* Keep the target's motion and colours, to restore them later. */
		effectState->savedCallback = target->update;
		effectState->savedScript = (const Sint32 *)target->script;
		{
			SPR	 *sprite = target->sprite;
			Uint8 colorMode = sprite->entries[0]->colorMode;

			effectState->savedPalette = colorMode;
		}
		effectState->x = target->x.fixed;
		effectState->z = target->z.fixed;
		effectState->y = target->y.fixed;
		return;
	}
	effectState->x = user->x.fixed;
	effectState->z = user->z.fixed;
	effectState->y = user->y.fixed;
	syVectorAddPolarOffset(toFIXED(16), facing, &effectState->x);
}

/* Stops the caster's update function and restores its colours, then waits a
   tick. */
void fdPsynergyStopCasterUpdate(void)
{
	FIELD_EFFECT_STATE *effectState = FieldEffectStateWork;
	ACTOR			   *user = effectState->caster;

	user->update = NULL;
	fdActorSetSpriteColorModeStub(user, SPRITE_COLOR_NORMAL);
	sySynch(1);
}

/* Ends a field Psynergy: frees the field effect sprites still in use and,
   unless another is being resolved, waits (at most END_ARC_WAIT_TICKS) for the
   swelling arcs to end, stops the slot event, releases its tiles, restores the
   map's shake, plays the queued sound and releases its state. */
void fdPsynergyEnd(void)
{
	FIELD_EFFECT_STATE	  *effectState;
	FIELD_STATE			  *fieldState;
	MAP_GRID_SCROLL_STATE *grid;
	FIELD_PARTICLE		  *effectSlot;
	Sint32				   i;

	effectState = FieldEffectStateWork;
	fieldState = FieldStateWork;
	grid = MapGridState[0];
	for(i = 0; i < PSYNERGY_EFFECT_SLOT_COUNT; i++) {
		effectSlot = &effectState->particles[i];
		if(effectSlot->active != 0)
			fdEffectClearOwnedSlot(effectSlot);
	}

	/* While fdUsePsynergy resolves a field Psynergy (psynergyActing), a cleanup
	   from inside it frees only the slots; the rest waits for its own. */
	if(fieldState->psynergyActing == 0) {
		Sint32 waited = 0;
		void (*firstActiveUpdate)(ACTOR *) = fdEffectUpdateScaledArcActorB;
		void (*secondActiveUpdate)(ACTOR *) = fdEffectUpdateScaledArcActorA;
		Sint32 active;

		do {
			ACTOR *slot = ActorsWork;

			active = 0;
			i = 0;
			while(i < ACTOR_COUNT) {
				void (*update)(ACTOR *) = slot->update;

				if(update == firstActiveUpdate ||
					update == secondActiveUpdate) {
					active = 1;
					break;
				}
				i++;
				slot++;
			}

			if(active != 0) {
				waited++;
				sySynch(1);
			}
		} while(active != 0 && waited < END_ARC_WAIT_TICKS);

		fieldState->castStarted = 0;
		syCloseEvent(fdEffectUpdateAllEffectSlots);
		syVRAMCacheFreeEntry(effectState->tileSlot);
		grid->shakeX = effectState->savedShakeX;
		grid->shakeY = effectState->savedShakeY;
		grid->shakeDecay = effectState->savedShakeDecay;
		if(effectState->fieldPsynergy != FIELD_PSYNERGY_REVEAL)
			fieldState->effectTimerRunning = 1;
		fdPlayQueuedMusic();
		syHeapSlotRelease(HEAP_SLOT_FIELD_EFFECT);
	}
}

/* Starts casting: the caster raises its hand (ANIM_CAST_PSYNERGY_START) and
   stops; with FLAG_PLAY_PSYNERGY_CAST it goes on to ANIM_CAST_PSYNERGY_END. */
void fdPsynergyStartCast(void)
{
	Sint32				ability;
	ACTOR			   *caster;
	FIELD_EFFECT_STATE *effectState;
	void			   *fieldState;

	effectState = *(void **)syRamFieldEffectState;
	fieldState = *(void **)syRamFieldState;
	caster = effectState->caster;
	ability = (Sint16)effectState->ability;
	fdActorSelectAnimationStub(caster, ANIM_CAST_PSYNERGY_START);
	caster->targetX = caster->x.fixed;
	caster->targetY = caster->y.fixed;
	caster->targetZ = caster->z.fixed;
	SET_VELOCITY(caster, 0, 0, 0);
	if(effectState->flashCaster != 0) {
		sySoundPlayStub(SE_POWER_UP);
		caster->update = fdEffectFlickerActorAndSpawnArcs;
	}
	if(effectState->announceName != 0) {
		fdActorSetOthersPaused(caster, 1, 0);
		mnMessageArgumentPushStub(ability, MESSAGE_ARGUMENT_ABILITY);
		/* On the World Map and off it alike, the same announcement. */
		if((Sint8)effectState->mode != 0) {
			mnFieldMessageShowStub(MsgAbilityAnnounce, effectState->msgMode);
		} else {
			mnFieldMessageShowStub(MsgAbilityAnnounce, effectState->msgMode);
		}
		fdActorSetOthersPaused(caster, 0, ANIM_STEP_NORMAL);
	}
	if(gmFlagGetStub(FLAG_PLAY_PSYNERGY_CAST) != 0) {
		if(effectState->flashCaster != 0) {
			caster->update = fdEffectCycleActorAndSpawnArcs;
		}
		fdActorSelectAnimationStub(caster, ANIM_CAST_PSYNERGY_END);
	} else {
		fdPsynergyStopCasterUpdate();
	}
	((FIELD_STATE *)fieldState)->castStarted = 1;
}

/* Saves the palettes for the Psynergy and fades them toward one of the eight
   cast colours (FieldPsynergyCastColors), picked by the tick counter or
   forced by FLAG_PSYNERGY_TINT(0) to (7), over 8 ticks. */
void fdPsynergyFadeToCastColor(void)
{
	FIELD_STATE	 *fieldState;
	PALETTE_FADE *fade;
	Sint32		  tint;

	/* Save the colours the buffer moves toward into the fieldState fade: the
	   sprite half and, unless Reveal is active, the BG half. */
	fieldState = FieldStateWork;
	fade = PaletteFadeState;
	syDMAXCopy(&fade->target[FADE_HALF_CHANNELS],
		&fieldState->savedFadeTarget[FADE_HALF_CHANNELS],
		DMA_COPY_WORDS(FADE_HALF_CHANNELS / 2), REG_DMA3);
	if(fieldState->revealActive == 0)
		syDMAXCopy(fade->target, fieldState->savedFadeTarget,
			DMA_COPY_WORDS(FADE_HALF_CHANNELS / 2), REG_DMA3);
	syDMAXCopy(fade->target, fade->current,
		DMA_COPY_WORDS(sizeof fade->current / 4), REG_DMA3);

	/* Of the forcing flags, the highest one set wins. */
	tint = TickCount & 7;
	if(gmFlagGetStub(FLAG_PSYNERGY_TINT(0)))
		tint = 0;
	if(gmFlagGetStub(FLAG_PSYNERGY_TINT(1)))
		tint = 1;
	if(gmFlagGetStub(FLAG_PSYNERGY_TINT(2)))
		tint = 2;
	if(gmFlagGetStub(FLAG_PSYNERGY_TINT(3)))
		tint = 3;
	if(gmFlagGetStub(FLAG_PSYNERGY_TINT(4)))
		tint = 4;
	if(gmFlagGetStub(FLAG_PSYNERGY_TINT(5)))
		tint = 5;
	if(gmFlagGetStub(FLAG_PSYNERGY_TINT(6)))
		tint = 6;
	if(gmFlagGetStub(FLAG_PSYNERGY_TINT(7)))
		tint = 7;
	fdPaletteFadeSetTarget(FieldPsynergyCastColors[tint], PALETTES_BG);
	fdPaletteFadeStartInterpolation(8);
}

/* Fades the palettes back from a cast's colours to the saved ones over 8
   ticks; while Reveal is active the BG half fades to Reveal's grey instead
   (PALETTE_TINT_GRAYSCALE). */
void fdPsynergyFadeFromCastColor(void)
{
	FIELD_STATE *state = FieldStateWork;
	Sint32		 savedPalettes = (Sint32)state->savedFadeTarget;

	fdPaletteFadeSetTarget(savedPalettes, PALETTES_SPRITES);
	if(state->revealActive != 0) {
		fdPaletteFadeSetTarget(PALETTE_TINT_GRAYSCALE, PALETTES_BG);
	} else {
		fdPaletteFadeSetTarget(savedPalettes, PALETTES_BG);
	}
	fdPaletteFadeStartInterpolation(8);
}

/* Converts a world position (16.16 pixels) to the screen: x and z receive the
   screen's x and y and y becomes 0. */
void fdCameraWorldToScreen(VECTOR3 *pos)
{
#if FIX_POINTER_OUTSIDE_OBJECT
	FIELD_STATE *fieldState = FieldStateWork;

	if(fieldState->mode == ACTOR_SYSTEM_WORLD_MAP) {
#else
	FIELD_STATE **fieldStateSlot = (FIELD_STATE **)&FieldStateWork;

	if((*fieldStateSlot)->mode == ACTOR_SYSTEM_WORLD_MAP) {
#endif
		Sint32 result[3];

		syConvert3Dto2D((const FIXED *)pos, result);
		pos->x = toFIXED(result[0]);
		pos->z = toFIXED(result[1]);
		pos->y = 0;
	} else {
#if FIX_POINTER_OUTSIDE_OBJECT
		MAP_GRID_SCROLL_STATE *state = MapGridState[0];
#else
		/* UB: MapGridState's heap slot, reached by stepping back from
		   FieldStateWork's. */
		MAP_GRID_SCROLL_STATE *state =
			*(MAP_GRID_SCROLL_STATE **)((Uint8 *)fieldStateSlot -
				(HEAP_SLOT_FIELD - HEAP_SLOT_MAP_GRID) * sizeof(void *));
#endif
		Sint32 x = state->view[0] & FIELD_PIXEL_MASK;
		Sint32 z = state->view[1] & FIELD_PIXEL_MASK;

		/* Relative to the view's whole-pixel corner; height lifts a point up
		   the screen. */
		pos->x -= x;
		pos->z -= pos->y;
		pos->z -= z;
		pos->y = 0;
	}
}

#undef END_ARC_WAIT_TICKS
#undef ANIM_CAST_PSYNERGY_START
#undef ANIM_CAST_PSYNERGY_END
#undef FADE_HALF_COLORS
#undef FADE_HALF_CHANNELS
