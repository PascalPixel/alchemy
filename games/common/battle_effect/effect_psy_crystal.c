/* btEffectRunPsyCrystal, the same in both games. Both games'
   battle_effect/battle_effect_healing.c include it where the function
   stands. */

/* Healing motes gather around each target in orange, the Mars Djinni palette;
   with a prelude, a Mars Djinni appears first.

   Abilities: Psy Crystal, Adept Ring, Ember. */
void btEffectRunPsyCrystal(BATTLE_EFFECT_PARM *args)
{
	btEffectRunHealingMotes(args, HEALING_MODE_PSY_CRYSTAL);
}
