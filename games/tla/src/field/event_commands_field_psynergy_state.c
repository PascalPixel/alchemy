/* @file */
/* The effect scene a field Psynergy runs in: fading to and from its cast */
/* colour, its effect slots and resources, the actor pair it works on, and */
/* Mind Read's wave and palettes. */
/* Part of event_commands.c. */

#include "battle/battle_system.h"
#include "text/msg_ids.h"

/* How long fdPsynergyEnd waits at most for the World Map's palette fade. */
#define END_FADE_WAIT_FRAMES 90

/* Fades the palettes back to the saved ones over the given frames. */
void fdBattleEffectFadeToSavedPalettes(s32 frames) {
	fdStartPaletteTransition(frames);
}

/* Fades the palettes back from a cast's colours to the saved ones over 8 */
/* frames; while Reveal is active the BG half fades to Reveal's grey */
/* instead (PALETTE_TINT_GRAYSCALE). */
void fdPsynergyFadeFromCastCol(void) { fdStartPaletteTransition(8); }

/* Converts a world position (16.16 pixels) to the screen: x and z receive the
 */
/* screen's x and y and y becomes 0. */
void fdCameraWorldToScr(VECTOR3 *position) {
	HEAP_SLOTS *heap = Ram_HeapSlots;

	if(((FIELD_STATE *)heap->fieldState)->mode == ACTOR_SYSTEM_WORLD_MAP) {
		s32 result[3];

		syConvert3Dto2D((const FIXED *)position, result);
		position->x = INTtoFIXED(result[0]);
		position->z = INTtoFIXED(result[1]);
		position->y = 0;
	}
	else {
		MAP_STATE *map = heap->mapGrid;
		s32 x = map->view[0] & FIELD_PIXEL_MASK;
		s32 z = map->view[1] & FIELD_PIXEL_MASK;

		/* Relative to the view's whole-pixel corner; height lifts a point up */
		/* the screen. */
		position->x -= x;
		position->z -= position->y;
		position->z -= z;
		position->y = 0;
	}
}

/* The same function as fdEffectUpdateAllSlots (event_commands_djinni.c), */
/* under another name. */
/* A task: updates the scene's 24 effect slots. */
void fdBattleEffectUpdateAllEffectSlots(void) {
	FIELD_EFFECT_STATE *scene;
	s32 i;

	scene = Ram_HeapSlots->fieldEffState;
	for(i = 0; i < (s32)ARRAY_COUNT(scene->particles); i++)
		fdParticleUpdate(&scene->particles[i]);
}

/* Prepares the state of the field Psynergy for ability (heap slot */
/* fieldEffState): its kind, the free VRAM it starts with, the map's shake */
/* to restore, and whether the place is lit. While another field Psynergy is */
/* resolving, only the ability and kind are set. */
void fdPsynergyLoadResources(s32 abilityId, s32 mode) {
	FIELD_STATE *event = Ram_HeapSlots->fieldState;
	MAP_STATE *map = Ram_HeapSlots->mapGrid;
	FIELD_EFFECT_STATE *state;
	s32 running;

	if(event->psynergyActing == 0) {
		state = syHeapSlotAlloc(HEAP_SLOT_FIELD_EFFECT, sizeof(*state));
		IWRAM_FILL_WORDS(state, sizeof(*state), 0);
	}
	else {
		state = Ram_HeapSlots->fieldEffState;
	}
	state->ability = abilityId;
	state->fieldPsynergy = btAbilityGet(abilityId)->fieldPsynergy;
	running = event->psynergyActing;
	if(running != 0)
		return;
	state->freeBlocks = VRAM_BLOCK_COUNT - fdResourceTableCountFreeBlocks();
	state->mode = mode;
	state->flashCaster = 1;
	state->aimAhead = 1;
	state->announceName = 1;
	state->msgMode = 1;
	state->msgShown = 0;
	state->stage = running;
	state->frm = -1;
	state->sprActive = running;
	state->savedShakeX = map->shakeX;
	state->savedShakeY = map->shakeY;
	state->savedShakeDecay = map->shakeDecay;
	if(MainRoutineStateWork.mapId == SCENE_GABOMBA_CATACOMBS_STREAM)
		state->lit = 1;
	if(MainRoutineStateWork.mapId == SCENE_TUNDARIA_TOWER_ENTRANCE)
		state->lit = 1;
	if(MainRoutineStateWork.mapId == SCENE_TUNDARIA_TOWER_CENTER_PRONG)
		state->lit = 1;
	if(MainRoutineStateWork.mapId == SCENE_TUNDARIA_TOWER_BURST_BROOCH)
		state->lit = 1;
	if(MainRoutineStateWork.mapId == SCENE_JUPITER_LIGHTHOUSE_AERIE)
		state->lit = 1;
	if(MainRoutineStateWork.mapId == (s32)&SceneId_JupiterLighthouseAerieBeacon)
		state->lit = 1;
	if(MainRoutineStateWork.mapId == SCENE_LOHO)
		state->lit = 1;
	fdPsynergySetupActorPair(MainRoutineStateWork.leaderActorId, -1);
	if(state->fieldPsynergy != FIELD_PSYNERGY_REVEAL)
		event->effTmrRunning = 0;
	if(state->fieldPsynergy != FIELD_PSYNERGY_RING)
		event->ringTmrRunning = 0;
	event->tgtX = 0;
	event->tgtY = 0;
	event->tgtZ = 0;
	event->blazeLongSpray = 0;
	event->psynergyHitObj = -1;
	fdBattleEffectStartArcEffect();
	sySetEvent(fdBattleEffectUpdateAllEffectSlots, TASK_PASS_FRAME);
}

/* Records the caster and the target of a field Psynergy. */
void fdPsynergySetupActorPair(s32 userId, s32 targetId) {
	ACTOR *user;
	ACTOR *target;
	FIELD_EFFECT_STATE *scene;

	scene = Ram_HeapSlots->fieldEffState;
	scene->casterId = userId;
	user = fdActorLookup((s16)userId);
	scene->tgtId = targetId;
	scene->caster = user;
	target = fdActorLookup((s16)targetId);
	/* The facing rounded to four directions, and to eight. */
	scene->ang = (user->facing + DEGtoANG(45)) & TRIG_TURN * 3 / 4;
	scene->ang8 = (user->facing + FACING_STEP) & TRIG_TURN * 7 / 8;
	scene->tgt = target;
	if(user != NULL) {
		u8 priorityFlags;

		priorityFlags = user->priFlg;

		scene->savedPriFlg = priorityFlags;
		scene->savedSprPri = ((SPR *)user->spr)->image.pri;
	}
	if(target != NULL) {
		/* Save target motion and colors for later restoration. */
		scene->savedFunc = target->update;
		scene->savedScript = target->script;
		scene->savedPal = ((SPR *)target->spr)->entries[0]->colMode;
		scene->x = target->x.fixed;
		scene->z = target->z.fixed;
		scene->y = target->y.fixed;
	}
	else {
		scene->x = user->x.fixed;
		scene->z = user->z.fixed;
		scene->y = user->y.fixed;
		syVectorAddPolarOffset(toFIXED(16), scene->ang, &scene->x);
	}
}

/* Starts casting: the caster raises its hand (ANIM_CAST_PSYNERGY_START) and */
/* stops, flashes with a sound, and the ability's name is announced; with */
/* FLAG_PLAY_PSYNERGY_CAST set the caster holds out its hand */
/* (ANIM_CAST_PSYNERGY_END) and keeps its glow. */
void fdPsynergyStartCast(void) {
	FIELD_EFFECT_STATE *state = Ram_HeapSlots->fieldEffState;
	FIELD_STATE *event = Ram_HeapSlots->fieldState;
	ACTOR *actor = state->caster;
	s32 ability = state->ability;

	fdActorSelectAnimationStub(actor, ANIM_CAST_PSYNERGY_START);
	actor->tgtX = actor->x.fixed;
	actor->velX = 0;
	actor->tgtY = actor->y.fixed;
	actor->velY = 0;
	actor->tgtZ = actor->z.fixed;
	actor->velZ = 0;
	if(state->flashCaster != 0) {
		sySndPlayStub(SE_POWER_UP);
		fdBattleEffectSetObjectUpdateMode(actor, CASTER_LOOK_FLASHING_WHITE);
	}
	if(state->announceName != 0) {
		state->msgShown = 1;
		fdActorSetOthersPaused(actor, 1, 0);
		mnMessageArgumentPushStub(ability, MESSAGE_ARGUMENT_ABILITY);
		fdMessageShowStub((s32)&MsgCastAbilityName, state->msgMode);
		fdActorSetOthersPaused(actor, 0, ANIM_STEP_NORMAL);
		state->msgShown = 0;
	}
	if(syGameFlagGet(FLAG_PLAY_PSYNERGY_CAST)) {
		if(state->flashCaster != 0)
			fdBattleEffectSetObjectUpdateMode(actor,
											  CASTER_LOOK_CYCLING_COLORS);
		fdActorSelectAnimationStub(actor, ANIM_CAST_PSYNERGY_END);
	}
	else {
		fdBattleEffectStopMainActor();
	}
	event->castStarted = 1;
}

/* Takes the caster out of its casting update and waits a frame. */
void fdBattleEffectStopMainActor(void) {
	FIELD_EFFECT_STATE *state = Ram_HeapSlots->fieldEffState;

	fdBattleEffectSetObjectUpdateMode(state->caster, CASTER_LOOK_NORMAL);
	sySynch(1);
}

/* Ends a field Psynergy: frees the field effect sprites still in use and, */
/* unless another is being resolved, waits for the swelling arcs to end, */
/* restores the map's shake and releases the state. */
void fdPsynergyEnd(void) {
	FIELD_EFFECT_STATE *state = Ram_HeapSlots->fieldEffState;
	FIELD_STATE *event = Ram_HeapSlots->fieldState;
	PALETTE_FADE *fade = Ram_HeapSlots->palFade;
	MAP_STATE *map = Ram_HeapSlots->mapGrid;
	ACTOR *owner = state->caster;
	s32 i;
	s16 first;

	for(i = 0; i < (s32)ARRAY_COUNT(state->particles); i++) {
		FIELD_PARTICLE *slot = &state->particles[i];
		if(slot->active)
			fdEffectClearOwnedSlot(slot);
	}
	first = FIELD_PSYNERGY_LASH;
	if(state->fieldPsynergy >= first &&
	   (state->fieldPsynergy <= FIELD_PSYNERGY_GRIND ||
		state->fieldPsynergy == FIELD_PSYNERGY_TELEPORT))
		fdPsynergyMarkWorldMapTilesUsed();
	if(event->psynergyActing != 0)
		return;
	fdBattleEffectWaitForArcGroups();
	fdBattleEffectEndArcEffect();
	if(event->mode == ACTOR_SYSTEM_WORLD_MAP && fade != NULL) {
		i = 0;
		if(fade->duration != 0) {
			do {
				sySynch(1);
				if(++i >= END_FADE_WAIT_FRAMES)
					break;
			} while(fade->duration != 0);
		}
	}
	event->castStarted = 0;
	fdPsynergyReleaseSceneSpr();
	syCloseEvent(fdBattleEffectUpdateAllEffectSlots);
	map->shakeX = state->savedShakeX;
	map->shakeY = state->savedShakeY;
	map->shakeDecay = state->savedShakeDecay;
	if(owner != NULL) {
		owner->priFlg = state->savedPriFlg;
		((SPR *)owner->spr)->image.pri = state->savedSprPri;
	}
	if(state->fieldPsynergy != FIELD_PSYNERGY_REVEAL)
		event->effTmrRunning = 1;
	if(state->fieldPsynergy != FIELD_PSYNERGY_RING)
		event->ringTmrRunning = 1;
	fdEventNoOp();
	syHeapSlotRelease(HEAP_SLOT_FIELD_EFFECT);
}

/* On the World Map, notes that the field Psynergy has used the second half of
 */
/* the BG tile buffer, so the next still image decodes it again. */
void fdPsynergyMarkWorldMapTilesUsed(void) {
	HEAP_SLOTS *heap = Ram_HeapSlots;
	s32 mode = ((FIELD_STATE *)heap->fieldState)->mode;

	if(mode == ACTOR_SYSTEM_WORLD_MAP)
		((WORLD_MAP_VIEW *)heap->mapGrid)->tileBufUsed = mode;
}

/* Starts Mind Read's wave between the caster and the one it reads */
/* (HEAP_SLOT_MIND_READ): a hue taken from the frame count, the field faded */
/* toward it, the message window greyed for thoughts, and */
/* fdPsynergyUpdateMindReadWave run each frame. */
void fdPsynergyStartMindReadWave(s32 casterId, s32 targetId) {
	MIND_READ_STATE *state;

	state = syHeapSlotAllocEwram(HEAP_SLOT_MIND_READ, sizeof(*state));
	btDmaFill32(0, state, sizeof(*state));
	state->hue = (FrameCount * 2) % HUE_DEGREES;
	state->srcId = casterId;
	state->tgtId = targetId;
	state->delay = 8;
	state->cnt = 0;
	fdPsynergyFadeToCastCol();
	fdMindReadAdvanceTint();
	fdPaletteFadeSetTarget(RGB(state->red, state->green, state->blue) |
							   PALETTE_TINT_CUSTOM,
						   PALETTES_BG);
	fdPaletteFadeStartInterpolation(8);
	fdPsynergyGreyMindReadPalette();
	sySetEvent((TASK_FUNC)fdPsynergyUpdateMindReadWave, TASK_PASS_FRAME);
}

/* Stops the wave effect: restores the caster's colours and the palettes and */
/* releases the Psynergy's state. */
void fdPsynergyStopMindReadWave(void) {
	MIND_READ_STATE *state = Ram_HeapSlots->mindRead;

	fdPsynergyRestoreMindReadPalette();
	syCloseEvent(fdPsynergyUpdateMindReadWave);
	fdActorSetLayersColMode(fdActorGetById(state->srcId),
							SPRITE_COLOR_NORMAL + 1);
	fdPsynergyFadeFromCastCol();
	syHeapSlotRelease(HEAP_SLOT_MIND_READ);
}

#define MIND_READ_GREY RGB(28, 28, 28) /* a thought window's light grey */

/* Greys the message window's white and most of its other colours */
/* (MIND_READ_GREY), so messages read as thoughts, and starts the BG0 line */
/* scroll (fdMindReadStartRipple) at each VBlank. */
void fdPsynergyGreyMindReadPalette(void) {
	volatile u16 *p;

	((WIN_STATE *)Ram_HeapSlots->winState)->mindRead = 1;
	p = &BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_COLOR_WHITE);
	*p = MIND_READ_GREY;
	p += 2;
	*p = MIND_READ_GREY;
	p += 8;
	*p = MIND_READ_GREY;
	p += 1;
	*p = MIND_READ_GREY;
	p -= 8;
	*p = MIND_READ_GREY;
	p += 1;
	*p = MIND_READ_GREY;
	p += 1;
	*p = MIND_READ_GREY;
	p += 1;
	*p = MIND_READ_GREY;
	p += 1;
	*p = MIND_READ_GREY;
	p += 1;
	*p = MIND_READ_GREY;
	p += 1;
	*p = MIND_READ_GREY;
	sySetEvent((TASK_FUNC)fdMindReadStartRipple, TASK_PASS_VBLANK - 20);
}

/* Stops the BG0 line scroll and restores WINDOW_PALETTE. */
void fdPsynergyRestoreMindReadPalette(void) {
	WIN_STATE *state = (WIN_STATE *)Ram_HeapSlots->winState;

	syCloseEvent((TASK_FUNC)fdMindReadStartRipple);
	BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_COLOR_WHITE) = RGB_WHITE;
	BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_COLOR_BLACK) = RGB(0, 0, 0);
	BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_COLOR_DARK_GREY) = RGB(10, 10, 10);
	BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_COLOR_LIGHT_GREY) = RGB(20, 20, 20);
	mnWindowColApplyStub(MainRoutineStateWork.winHue,
						 MainRoutineStateWork.winBrightness);
	state->mindRead = 0;
}
