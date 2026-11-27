/* btEffectRunSmokeBomb and btEffectRunBlast, the same in both games. Both
   games' battle_effect/battle_effect_eruption.c include it where the
   functions stand. */

/* A fountain of grey motes and puffs of smoke bursts up beside the targets and
   falls back, flashing each target.

   Abilities: Smoke Bomb, Sleep Bomb. */
void btEffectRunSmokeBomb(BATTLE_EFFECT_PARM *args)
{
	btEffectRunEruption(args, ERUPTION_SMOKE_BOMB);
}

/* A fountain of fire-coloured motes and puffs of smoke bursts up beside the
   targets and falls back, flashing each target and making it recoil.

   Abilities: Blast, Mad Blast, Fiery Blast. */
void btEffectRunBlast(BATTLE_EFFECT_PARM *args)
{
	btEffectRunEruption(args, ERUPTION_BLAST);
}
