/* @file */
/* The display every effect starts from, the timed scroll of BG1 and BG2 the */
/* summons' skies use, and the line-by-line feed of window 0's edges. */
/* Part of battle_effect.c. */

/* Puts the battle screen back into the display state every effect starts from
 */
/* and ends with. */
void btEffectConfigureEffectDisp(void) {
	REG_BLDCNT = 0;
	REG_BLDALPHA = EFFECT_BLEND_CANVAS(CANVAS_BLEND_LEVEL);
	EFFECT_SET_BATTLE_WINDOWS();
	REG_WININ = EFFECT_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_BG2 |
				WININ_WIN1_OBJ | WININ_WIN1_CLR;
	REG_WINOUT = EFFECT_WINOUT;
	IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, EFFECT_DISPCNT, IO_WRITE_HALFWORD);
	sySynch(1);
}

/* Scrolls BG1 a step every few VBlanks. */
void btEffectAdvanceScrollOnInterval(void) {
	BATTLE_EFFECT_STATE *state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);

	state->scrollTmr++;
	if(state->scrollTmr == state->scrollInterval) {
		BgScrollWork[1].x += state->scrollStepX;
		BgScrollWork[1].y += state->scrollStepY;
		state->scrollTmr = 0;
	}
}

/* Slides the affine BG2 a step every few VBlanks. */
void btEffectAdvanceBg2OnInterval(void) {
	s32 timer;
	BATTLE_EFFECT_STATE *state;

	state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
	timer = state->scrollTmr + 1;
	state->scrollTmr = timer;
	if(timer == state->scrollInterval) {
		REG_BG2X = state->bg2x;
		REG_BG2Y = state->bg2y;
		state->bg2x += state->scrollStepX;
		state->bg2y += state->scrollStepY;
		state->scrollTmr = 0;
	}
}

/* Restarts the DMA that feeds window 0's edges from the effect's per-line */
/* table. */
void btEffectArmWin0HBlankDma(void) {
	volatile u16 *channel = REG_DMA0;

	btDmaStop(channel);
	syDMAXCopy(PARTICLES, &REG_WIN0H, DMA_HBLANK_HALFWORD_PER_LINE,
			   (volatile u32 *)channel);
}
