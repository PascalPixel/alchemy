// Reveal: the world turns grey but for a rippling window round the leader, which ends when the
// leader walks out of it or its time runs out. Part of script_commands.c.

#include "script_commands_internal.h"

// Draws every live actor whose sprite shows a rock (SPRITE_ROCK) in the colour mode value: Reveal
// greys the rocks with the backgrounds as it starts (SPRITE_COLOR_GREY) and gives them back their
// colours as it ends (SPRITE_COLOR_NORMAL).
void fdActorSetRevealSpriteColor(Sint32 value)
{
	ACTOR *actor = ActorsWork;
	Sint32 i;

	for (i = 0; i < ACTOR_COUNT; i++)
	{
		if (actor->script != NULL)
		{
			Uint8 kind = actor->spriteStorageKind;

			if (kind == ACTOR_SPRITE_ONE)
			{
				SPR           *animation = actor->sprite;
				SPR_ANIMATION *entry     = animation->entries[0];

				if (entry->animationId == SPRITE_ROCK)
				{
					entry->colorMode = value;
					animation->dirty = kind;
				}
			}
		}
		actor++;
	}
}

// How far the leader may walk from where Reveal was cast, in screen pixels up and down; sideways x
// counts at about 0.82 of itself, so the edge is an ellipse about 73 pixels to either side.
#define REVEAL_RADIUS 60

// Event of the timed Psynergy (FIELD_PSYNERGY_REVEAL): asks for it to end once the leader is
// REVEAL_RADIUS from its centre or its time has run out.
void fdPsynergyWatchLeaderDistance(void)
{
	FIELD_STATE *state;
	ACTOR       *actor;
	FIXED        distanceX;
	Sint32       distanceZ;

	state = FieldStateWork;
	actor = fdActorLookup(MainRoutineStateWork.leaderActorId);
	if (state->effectTimerRunning != 0)
	{
		if (state->revealFrame != 0)
			state->revealFrame--;
	}

	// In screen pixels: y is z less the height. x is scaled by 0.81649, about the square root of
	// 2/3.
	distanceX = syMulFX(state->revealCenterX - actor->x.fixed / toFIXED(1), toFIXED(0.81649));
	distanceZ = state->revealCenterY - (actor->z.fixed - actor->y.fixed) / toFIXED(1);
	if (distanceX * distanceX + distanceZ * distanceZ >= REVEAL_RADIUS * REVEAL_RADIUS
	    || state->revealFrame == 0)
	{
		// Raise Reveal as a Psynergy request that pays no PP, which runs its ending scripts.
		Uint16 *slot    = (Uint16 *)&state->psynergy;
		Uint32  request = ACT_PACKED | ABILITY_REVEAL;

		*slot           = request;
	}
}

// Reveal: the backgrounds turn grey and show only within a rippling ellipse round the leader; runs
// the scene's Reveal script if there is one, puts markers round the leader and watches how far the
// leader walks.
void fdPsynergyReveal(void)
{
	FIELD_STATE             *fieldState;
	SCREEN_TRANSITION_STATE *state;
	ACTOR                   *target;
	Sint32                   eventTarget;
	void                    *event;
	Sint32                   frame;

	fieldState = FieldStateWork;
	fdActorSetRevealSpriteColor(SPRITE_COLOR_GREY);
	fdScreenTransitionStartHeldWindow(WINDOW_TRANSITION_RIPPLE);
	state          = HEAP_SLOT_BLOCK(HEAP_SLOT_SCREEN_TRANSITION);
	target         = fdActorLookup(MainRoutineStateWork.leaderActorId);
	state->centerX = target->x.fixed;
	state->centerY = target->z.fixed - target->y.fixed;
	// The BG palettes turn grey in one tick.
	fdPaletteFadeSetSource(PALETTE_TINT_NONE, PALETTES_ALL);
	fdPaletteFadeSetTarget(PALETTE_TINT_GRAYSCALE, PALETTES_BG);
	fdPaletteFadeStartInterpolation(1);
	// Not in the English release.
#if !REVEAL_RECAST_STAYS_ACTIVE
	fieldState->revealActive = 0;
#endif
	sySynch(1);
	event =
		fdFindPsynergyCue(PSYNERGY_CUE(PSYNERGY_PHASE_ACTS), FIELD_PSYNERGY_REVEAL, &eventTarget);
	if (event != NULL)
		fdRunPsynergyScript(event, MainRoutineStateWork.leaderActorId, eventTarget);
	sySoundPlayStub(SE_PSYNERGY_SPARKLE);

	// Record the centre (in pixels) and the time in the field state, and put markers nearby.
	fieldState->revealActive       = 1;
	fieldState->revealCenterX      = state->centerX / toFIXED(1);
	fieldState->revealCenterY      = state->centerY / toFIXED(1);
	fieldState->revealFrame        = REVEAL_TICKS;
	fieldState->effectTimerRunning = 1;
	fdEffectSpawnRevealMarkers();
	for (frame = 0; frame <= 18; frame++)
	{
		sySynch(1);
		state->value = frame;
	}
	sySetEvent(fdPsynergyWatchLeaderDistance, EVENT_PASS_TICK);
}

// Ends Reveal if it is running: stops its watch, brings the colours back from black over 30 ticks
// and runs the scene's PSYNERGY_PHASE_FADES script.
void fdPsynergyEndReveal(void)
{
	FIELD_EFFECT_STATE      *effectState = FieldEffectStateWork;
	SCREEN_TRANSITION_STATE *transition  = ScreenTransitionStateWork;
	FIELD_STATE             *state       = FieldStateWork;
	Sint32                   eventTarget;
	void                    *event;

	if (state->revealActive != 0)
	{
		sySoundPlayStub(SE_MAP_TRANSITION);
		syCloseEvent(fdPsynergyWatchLeaderDistance);

		state->revealActive = 0;
		state->revealFrame  = 0;
		fdActorSetRevealSpriteColor(SPRITE_COLOR_NORMAL);

		fdPaletteFadeSetTarget(PALETTE_TINT_NONE, PALETTES_BG);
		fdPaletteFadeStartInterpolation(1);
		fdPaletteFadeSetSource(RGB(0, 0, 0), PALETTES_ALL);
		fdPaletteFadeSetTarget(PALETTE_TINT_NONE, PALETTES_ALL);
		fdPaletteFadeStartInterpolation(30);
		sySynch(1);

		event = fdFindPsynergyCue(
			PSYNERGY_CUE(PSYNERGY_PHASE_FADES), FIELD_PSYNERGY_REVEAL, &eventTarget);
		if (event != NULL)
			fdRunPsynergyScript(event, MainRoutineStateWork.leaderActorId, eventTarget);

		if (effectState->sceneVariant == 0)
		{
			transition->nextMode = 0;
			transition->duration = 1;
			transition->step     = 1;
			sySynch(10);
		}
	}
}
