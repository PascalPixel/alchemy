/* btEffectRunBruteForce, btEffectRunOnslaught and btEffectRunHeadbutt, the
   same in both games. The Broken Seal's
   battle_effect/battle_effect_rushing_strike.c and The Lost Age's
   battle_effect/battle_effect_star_mine.c include it where the functions
   stand. */

/* Rushes the target and scatters sparks in the ice-block palette.

   Abilities: Brute Force and, in The Lost Age, Troll. */
void btEffectRunBruteForce(BATTLE_EFFECT_PARM *args)
{
	btEffectRunRushingStrike(args, RUSHING_STRIKE_BRUTE_FORCE);
}

/* Rushes the target and scatters sparks in the pink-burst palette.

   Abilities: Onslaught. */
void btEffectRunOnslaught(BATTLE_EFFECT_PARM *args)
{
	btEffectRunRushingStrike(args, RUSHING_STRIKE_ONSLAUGHT);
}

/* Rushes the target and scatters sparks in the blast palette.

   Abilities: Headbutt. */
void btEffectRunHeadbutt(BATTLE_EFFECT_PARM *args)
{
	btEffectRunRushingStrike(args, RUSHING_STRIKE_HEADBUTT);
}
