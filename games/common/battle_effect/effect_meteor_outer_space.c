/* btEffectRunMeteor and btEffectRunOuterSpace, the same in both games. The
   Broken Seal's battle_effect/battle_effect_starfield_fall.c and The Lost
   Age's battle_effect/battle_effect_particle_streams.c include it where the
   functions stand. */

/* The battle dissolves into a starfield and a meteor falls through it in flames
   among falling fireballs, then bursts in embers and flashes.

   Abilities: Meteor. */
void btEffectRunMeteor(BATTLE_EFFECT_PARM *args)
{
	btEffectRunStarfieldFall(args, STARFIELD_FALL_METEOR);
}

/* The Fusion Dragon leaps out of sight and falls back through a starfield in
   flames, then bursts in embers and flashes, all in the lightning bolt's
   colours.

   Abilities: Outer Space, the Fusion Dragon's. */
void btEffectRunOuterSpace(BATTLE_EFFECT_PARM *args)
{
	btEffectRunStarfieldFall(args, STARFIELD_FALL_OUTER_SPACE);
}
