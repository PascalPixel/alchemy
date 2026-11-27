/* The display every effect starts from, the timed scroll of BG1 and BG2 the
   summons' skies use, and the line-by-line feed of window 0's edges. Part of
   battle_effect.c. */

#include "battle_effect/effect_configure_effect_disp.c"

/* Scrolls BG1 a step every few frames. */
void btEffectAdvanceScrollOnInterval(void)
{
	BATTLE_EFFECT_STATE *state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);

	state->scrollTmr++;
	if(state->scrollTmr == state->scrollInterval) {
		BgScrollWork[1].x += state->scrollStepX;
		BgScrollWork[1].y += state->scrollStepY;
		state->scrollTmr = 0;
	}
}

/* Slides the affine BG2 a step every few frames. */
void btEffectAdvanceBg2OnInterval(void)
{
	Sint32				 tmr;
	BATTLE_EFFECT_STATE *state;

	state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
	tmr = state->scrollTmr + 1;
	state->scrollTmr = tmr;
	if(tmr == state->scrollInterval) {
		REG_BG2X = state->bg2x;
		REG_BG2Y = state->bg2y;
		state->bg2x += state->scrollStepX;
		state->bg2y += state->scrollStepY;
		state->scrollTmr = 0;
	}
}

/* Restarts the DMA that feeds window 0's edges from the effect's per-line
   table. */
void btEffectArmWin0HBlankDMA(void)
{
	volatile Uint16 *channel = REG_DMA0;

	btDMAStop(channel);
	syDMAXCopy(PARTICLES, &REG_WIN0H, DMA_HBLANK_HALFWORD_PER_LINE,
		(volatile Uint32 *)channel);
}
