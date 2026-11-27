/* Display, scroll and window HBlank DMA helpers. Part of battle_effect.c. */

#include "battle_effect/effect_configure_effect_disp.c"

/* Scrolls BG1 a step every few frames. */
void btEffectAdvanceScrollOnInterval(void)
{
	BATTLE_EFFECT_STATE *state;

	state = RamHeapSlots->battleEff;

	if(++state->scrollTmr == state->scrollInterval) {
		BgScrollWork[1].x += state->scrollStepX;
		BgScrollWork[1].y += state->scrollStepY;
		state->scrollTmr = 0;
	}
}

/* Slides the affine BG2 a step every few frames. */
void btEffectAdvanceBg2OnInterval(void)
{
	BATTLE_EFFECT_STATE *state;

	state = RamHeapSlots->battleEff;

	if(++state->scrollTmr == state->scrollInterval) {
		REG_BG2X = state->bg2x;
		REG_BG2Y = state->bg2y;
		state->bg2x += state->scrollStepX;
		state->bg2y += state->scrollStepY;
		state->scrollTmr = 0;
	}
}

/* Restarts the DMA that feeds window 0's edges, one a line, from the table the
   effect keeps in the map cell buffer. */
void btEffectArmWin0HBlankDMA(void)
{
	volatile Uint16 *channel = REG_DMA0;

	channel[DMA_CONTROL_HALF] &= (Uint16)~DMA_START_TIMING_DRQ_REPEAT;
	channel[DMA_CONTROL_HALF] &= (Uint16)~DMA_ENABLE;
	(void)channel[DMA_CONTROL_HALF];
	syDMAXCopy(RamMapMetatileBuffer, (void *)&REG_WIN0H,
		DMA_HBLANK_HALFWORD_PER_LINE, (volatile Uint32 *)channel);
}
