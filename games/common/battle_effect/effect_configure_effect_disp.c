/* btEffectConfigureEffectDisp, the same in both games. Both games'
   battle_effect/battle_effect_display.c include it where the function
   stands. */

/* Puts the battle screen back into the display state every effect starts from
   and ends with. */
void btEffectConfigureEffectDisp(void)
{
	REG_BLDCNT = 0;
	REG_BLDALPHA = EFFECT_BLEND_CANVAS(CANVAS_BLEND_LEVEL);
	EFFECT_SET_BATTLE_WINDOWS();
	REG_WININ = EFFECT_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_BG2 |
		WININ_WIN1_OBJ | WININ_WIN1_CLR;
	REG_WINOUT = EFFECT_WINOUT;
	IO_WRITE_QUEUE_PUSH(
		(Uint32)&REG_DISPCNT, EFFECT_DISPCNT, IO_WRITE_HALFWORD);
	sySynch(1);
}
