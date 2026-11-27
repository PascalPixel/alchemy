/* The Teleport circles' shimmer, which every scene with a circle keeps a copy
   of (Contigo, the Anemos Inner Sanctum, the Islet Cave, the Yampi Desert's
   cave and three of Mars Lighthouse's rooms): the circle's colours run round
   it, a palette cycle, as Teleport is cast and as the party arrives on a
   circle. */

/* Writes each listed background colour from its red, green and blue, keeps the
   lists for the shimmer, and saves the palettes as the fade's source; arriving
   by Teleport, the party then arrives on the circle. */
void scPaletteSetColorsFade(
	Uint16 *slots, Uint16 *colours, Uint16 *cycleColors, Uint16 *cycleSlots)
{
	PALETTE_FADE *fade = RamHeapSlots->paletteFade;

	PaletteSlots = slots;
	PaletteColors = colours;
	PaletteCycleColors = cycleColors;
	PaletteCycleSlots = cycleSlots;
	while(*slots != PALETTE_LIST_END && *colours != PALETTE_LIST_END) {
		int index = *slots++;
		int red = *colours++;
		int green = *colours++;
		int blue = *colours++;

		SET_RGB(BG_PLTT[index], red, green, blue);
	}
	syDMAXCopy((const void *)BG_PLTT, fade->palette,
		DMA_COPY_WORDS(PALETTE_FADE_BG_COLORS / 2), REG_DMA3);
	syDMAXCopy((const void *)OBJ_PLTT, &fade->palette[PALETTE_FADE_BG_COLORS],
		DMA_COPY_WORDS(
			(ARRAY_COUNT(fade->palette) - PALETTE_FADE_BG_COLORS) / 2),
		REG_DMA3);
	fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
	if(MainRoutineStateWork.teleportState == TELEPORT_ARRIVING)
		scTeleportArriveWhirling();
}

/* Starts the shimmer at its slow pace, as Teleport is cast: the first step
   after 16 ticks, each after a tick less, down to two. */
void scTeleportStartShimmer(void)
{
	PaletteCycleIndex = 0;
	PaletteCycleLength = syListCountEntries(PaletteSlots);
	PaletteCycleDelay = 15;
	PaletteCycleTimer = 16;
	PaletteCycleBright = 1;
	sySetTickEvent(fdPaletteCycleFade, EVENT_PASS_TICK);
}

/* Stops the shimmer, leaving the next transition a backdrop fade. */
void scTeleportStopShimmer(void)
{
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	syFieldCloseEvent(fdPaletteCycleFade);
}

/* Starts the shimmer at a step every two ticks, as the party arrives. */
void scTeleportStartFastShimmer(void)
{
	PaletteCycleIndex = 0;
	PaletteCycleLength = syListCountEntries(PaletteSlots);
	PaletteCycleDelay = 2;
	PaletteCycleTimer = 2;
	PaletteCycleBright = 1;
	sySetTickEvent(fdPaletteCycleFade, EVENT_PASS_TICK);
}

#include "list_count_entries.h"
#include "field/field_mode.h"

#include "palette_cycle_fade.h"

/* The party arrives by Teleport on the circle: the room fades in grey while the
   circle shimmers and the whirl plays, then its colours come back. */
void scTeleportArriveWhirling(void)
{
	FIELD_STATE *state = RamHeapSlots->fieldState;
	Sint32		 saved;

	saved = state->transition;
	fdPaletteFadeApplyTarget(PALETTE_TINT_CUSTOM | RGB(8, 8, 8), PALETTES_BG);
	fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_OBJECTS);
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	scTeleportStartFastShimmer();
	fdTeleportPlayWhirl();
	syWait(40);
	scTeleportStopShimmer();
	fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_BG);
	fdPaletteFadeInterpolate(16);
	syWait(16);
	{
		MAIN_ROUTINE_STATE *party = &MainRoutineStateWork;

		party->teleportState = TELEPORT_NONE;
	}
	state->transition = saved;
}
