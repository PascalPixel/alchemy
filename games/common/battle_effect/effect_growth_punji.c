/* btEffectRunGrowth and btEffectRunPunji, the same in both games. Both
   games' battle_effect/battle_effect_growth.c include it where the functions
   stand. */

/* Vines grow from the ground, flaring at each root.

   Abilities: Growth, Mad Growth, Wild Growth. */
void btEffectRunGrowth(BATTLE_EFFECT_PARM *args)
{
	btEffectRunGrowingColumns(args, GROWING_COLUMNS_VINES);
}

/* Green pillars grow from the ground, flaring at each root.

   Abilities: Punji, Punji Trap, Punji Strike. */
void btEffectRunPunji(BATTLE_EFFECT_PARM *args)
{
	btEffectRunGrowingColumns(args, GROWING_COLUMNS_PILLARS);
}
