/* @file */
/* Reveal: the world turns grey but for a rippling window round the leader, */
/* which ends when the leader walks out of it or its time runs out. */
/* Part of event_commands.c. */

#include "event_commands_internal.h"

/* Draws every live actor whose sprite shows a rock (SPRITE_ROCK) in the colour
 */
/* mode value: Reveal greys the rocks with the backgrounds as it starts */
/* (SPRITE_COLOR_GREY) and gives them back their colours as it ends */
/* (SPRITE_COLOR_NORMAL). */
void fdActorSetRevealSprCol(s32 value) {
	ACTOR *actor = ActorsWork;
	s32 i;

	for(i = 0; i < ACTOR_COUNT; i++) {
		if(actor->script != NULL) {
			u8 kind = actor->sprStorageKind;

			if(kind == ACTOR_SPRITE_ONE) {
				SPR *animation = actor->spr;
				SPR_ANIMATION *entry = animation->entries[0];

				if(entry->anmId == SPRITE_ROCK) {
					entry->colMode = value;
					animation->dirty = kind;
				}
			}
		}
		actor++;
	}
}

/* How far the leader may walk from where Reveal was cast, in screen pixels up
 */
/* and down; sideways x counts at about 0.82 of itself, so the edge is an */
/* ellipse about 73 pixels to either side. */
#define REVEAL_RADIUS 60

/* Task of the timed Psynergy (FIELD_PSYNERGY_REVEAL): asks for it to end once
 */
/* the leader is REVEAL_RADIUS from its centre or its time has run out. */
void fdPsynergyWatchLeaderDistance(void) {
	FIELD_STATE *state;
	ACTOR *actor;
	FIXED distanceX;
	s32 distanceZ;

	state = FieldStateWork;
	actor = fdActorLookup(MainRoutineStateWork.leaderActorId);
	if(state->effTmrRunning != 0) {
		if(state->revealFrm != 0)
			state->revealFrm--;
	}

	/* In screen pixels: y is z less the height. x is scaled by 0.81649, about
	 */
	/* the square root of 2/3. */
	distanceX = syMulFX(state->revealCenterX - actor->x.fixed / FIXED_ONE,
						toFIXED(0.81649));
	distanceZ =
		state->revealCenterY - (actor->z.fixed - actor->y.fixed) / FIXED_ONE;
	if(distanceX * distanceX + distanceZ * distanceZ >=
		   REVEAL_RADIUS * REVEAL_RADIUS ||
	   state->revealFrm == 0) {
		/* Raise Reveal as a Psynergy request that pays no PP, which runs its */
		/* ending events. */
		u16 *slot = (u16 *)&state->psynergy;
		u32 request = ACT_PACKED | ABILITY_REVEAL;

		*slot = request;
	}
}

/* Reveal: the backgrounds turn grey and show only within a rippling ellipse */
/* round the leader; runs the scene's Reveal event if there is one, puts */
/* markers round the leader and watches how far the leader walks. */
void fdPsynergyReveal(void) {
	FIELD_STATE *fieldState;
	SCR_TRANSITION_STATE *state;
	ACTOR *target;
	s32 eventTarget;
	void *event;
	s32 frame;

	fieldState = FieldStateWork;
	fdActorSetRevealSprCol(SPRITE_COLOR_GREY);
	fdScrTransitionStartHeldWindow(WINDOW_TRANSITION_RIPPLE);
	state = HEAP_SLOT_BLOCK(HEAP_SLOT_SCREEN_TRANSITION);
	target = fdActorLookup(MainRoutineStateWork.leaderActorId);
	state->centerX = target->x.fixed;
	state->centerY = target->z.fixed - target->y.fixed;
	/* The BG palettes turn grey in one frame. */
	fdPaletteFadeSetSource(PALETTE_TINT_NONE, PALETTES_ALL);
	fdPaletteFadeSetTarget(PALETTE_TINT_GRAYSCALE, PALETTES_BG);
	fdPaletteFadeStartInterpolation(1);
	/* Not in the English release. */
#if !(TBS_PATCH_1 && !TBS_PATCH_2)
	fieldState->revealActive = 0;
#endif
	sySynch(1);
	event = scFindPsynergyTrigger(PSYNERGY_TRIGGER(PSYNERGY_PHASE_ACTS),
								  FIELD_PSYNERGY_REVEAL, &eventTarget);
	if(event != NULL)
		fdRunPsynergyEvent(event, MainRoutineStateWork.leaderActorId,
						   eventTarget);
	sySndPlayStub(SE_PSYNERGY_SPARKLE);

	/* Record the centre (in pixels) and the time in the field state, and put */
	/* markers nearby. */
	fieldState->revealActive = 1;
	fieldState->revealCenterX = state->centerX / FIXED_ONE;
	fieldState->revealCenterY = state->centerY / FIXED_ONE;
	fieldState->revealFrm = REVEAL_FRAMES;
	fieldState->effTmrRunning = 1;
	fdEffectSpawnRevealMarkers();
	for(frame = 0; frame <= 18; frame++) {
		sySynch(1);
		state->value = frame;
	}
	sySetEvent(fdPsynergyWatchLeaderDistance, TASK_PASS_FRAME);
}

/* Ends Reveal if it is running: stops its watch, brings the colours back from
 */
/* black over 30 frames and runs the scene's PSYNERGY_PHASE_FADES event. */
void fdPsynergyEndReveal(void) {
	FIELD_EFFECT_STATE *effectState = FieldEffectStateWork;
	SCR_TRANSITION_STATE *transition = ScreenTransitionStateWork;
	FIELD_STATE *state = FieldStateWork;
	s32 eventTarget;
	void *event;

	if(state->revealActive != 0) {
		sySndPlayStub(SE_MAP_TRANSITION);
		syCloseEvent(fdPsynergyWatchLeaderDistance);

		state->revealActive = 0;
		state->revealFrm = 0;
		fdActorSetRevealSprCol(SPRITE_COLOR_NORMAL);

		fdPaletteFadeSetTarget(PALETTE_TINT_NONE, PALETTES_BG);
		fdPaletteFadeStartInterpolation(1);
		fdPaletteFadeSetSource(RGB(0, 0, 0), PALETTES_ALL);
		fdPaletteFadeSetTarget(PALETTE_TINT_NONE, PALETTES_ALL);
		fdPaletteFadeStartInterpolation(30);
		sySynch(1);

		event = scFindPsynergyTrigger(PSYNERGY_TRIGGER(PSYNERGY_PHASE_FADES),
									  FIELD_PSYNERGY_REVEAL, &eventTarget);
		if(event != NULL)
			fdRunPsynergyEvent(event, MainRoutineStateWork.leaderActorId,
							   eventTarget);

		if(effectState->sceneVariant == 0) {
			transition->nextMode = 0;
			transition->duration = 1;
			transition->step = 1;
			sySynch(10);
		}
	}
}
