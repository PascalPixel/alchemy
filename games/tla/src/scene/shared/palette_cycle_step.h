#include "palette_cycle_places.h"

/* Steps the palette cycle, unless a script, Reveal, the pause menu or any field
   Psynergy but Hover holds the field. */
void fdPaletteCycleStep(void)
{
	Uint16 cols[16];
	Sint32 phase = (PaletteCycleTick / CYCLE_STEP_TICKS) % CYCLE_PLACES;
	Sint32 i;

	if(PaletteCyclePausesInScripts != 0) {
		FIELD_STATE *fieldState = RamHeapSlots->fieldState;
		Sint16		*state = &fieldState->psynergy;

		if(*state != 0 && ABILITY_ID(*state) != ABILITY_HOVER)
			return;
		if(fieldState->revealActive != 0)
			return;
		if(fieldState->pauseMenu != 0)
			return;
	}
	for(i = 0; i <= CYCLE_PLACES - 1; i++)
		cols[(phase + i) % CYCLE_PLACES] =
			PaletteCycleColors[i + CYCLE_FIRST_COLOR];
	syDMAXCopy(cols, (void *)&BG_PLTT_COLOR(CYCLE_PALETTE, CYCLE_FIRST_COLOR),
		DMA_COPY_WORDS((CYCLE_PLACES - 1) / 2), REG_DMA3);
	PaletteCycleTick++;
}

#undef CYCLE_PALETTE
#undef CYCLE_FIRST_COLOR
#undef CYCLE_PLACES
#undef CYCLE_STEP_TICKS
