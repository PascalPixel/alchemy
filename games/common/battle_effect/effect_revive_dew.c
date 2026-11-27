/* btEffectRunRevive and btEffectRunDew, the same in both games. Both games'
   battle_effect/battle_effect_djinn_beams.c include it where the functions
   stand. */

/* Motes drift down over each target and a column of light falls on it, in the
   Venus Djinni's colours; with a prelude, a Venus Djinni appears first.

   Abilities: Revive, Water of Life and, in The Lost Age, Lich; Quartz. */
void btEffectRunRevive(BATTLE_EFFECT_PARM *args)
{
	btEffectRunDjinnBeams(args, ELEMENT_VENUS);
}

/* Dew plays Revive's beams of light in the Mercury Djinni's colours, and with a
   prelude a Mercury Djinni appears first.

   Abilities: Dew. */
void btEffectRunDew(BATTLE_EFFECT_PARM *args)
{
	btEffectRunDjinnBeams(args, ELEMENT_MERCURY);
}
