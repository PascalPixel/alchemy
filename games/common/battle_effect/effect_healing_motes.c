/* btEffectRunPly, btEffectRunWish and btEffectRunCure, the same in both
   games. Both games' battle_effect/battle_effect_healing.c include it where
   the functions stand. */

/* Sparkles of light swirl around each target and close in on it, in the ice
   shard's or ice block's colours, with the canvas shown at its own width and
   centred on the first target.

   Abilities: Ply, Ply Well, Pure Ply, Herb, Nut, Vial, Potion, Soothing Water
   and, in The Lost Age, Pixie, Faery, Weird Nymph, Succubus, Earnest Ply;
   Fizz. */
void btEffectRunPly(BATTLE_EFFECT_PARM *args)
{
	btEffectRunHealingMotes(args, HEALING_MODE_PLY);
}

/* Healing motes tumble in on each target from all around it.

   Abilities: Wish, Wish Well, Pure Wish and, in The Lost Age, Mist Potion,
   Elder Wood, Estre Wood; Regenerate; Reflect; Spritz. */
void btEffectRunWish(BATTLE_EFFECT_PARM *args)
{
	btEffectRunHealingMotes(args, HEALING_MODE_WISH);
}

/* A ring of healing motes below each target closes in on it, in the blast
   sprite sheet's colours.

   Abilities: Cure, Cure Well, Potent Cure, Corn and, in The Lost Age, Flower;
   Worms. */
void btEffectRunCure(BATTLE_EFFECT_PARM *args)
{
	btEffectRunHealingMotes(args, HEALING_MODE_CURE);
}
