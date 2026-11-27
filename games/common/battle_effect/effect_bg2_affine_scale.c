/* btEffectResetBg2Pa and btEffectSetBg2AffineScaleHalf, the same in both
   games. The Broken Seal's battle_effect/battle_effect_neptune.c and The
   Lost Age's battle_effect/battle_effect_kirin.c include it where the
   functions stand. */

/* Sets BG2's horizontal scale back to one texel a pixel above the stretched
   lines. */
void btEffectResetBg2Pa(void)
{
	REG_BG2PA = Q_8_8(1);
}

/* Stretches BG2 to twice its width from the current line down. */
void btEffectSetBg2AffineScaleHalf(void)
{
	REG_BG2PA = Q_8_8(0.5);
}
