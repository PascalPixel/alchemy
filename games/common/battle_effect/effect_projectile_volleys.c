/* btEffectRunSlash to btEffectRunWingBeat, the same in both games. Both
   games' battle_effect/battle_effect_projectiles.c include it where the
   functions stand. */

/* Six comets fly at each target, three ticks apart, and burst into a flash and
   sparks; the effect's variant picks one of three comets.

   Abilities: Slash, Wind Slash, Sonic Slash. */
void btEffectRunSlash(BATTLE_EFFECT_PARM *args)
{
	btEffectRunProjectileVolley(args, PROJECTILE_VOLLEY_SLASH);
}

/* Four spinning shurikens fly at each target, five ticks apart, and vanish
   once past it.

   Abilities: Shuriken. */
void btEffectRunShuriken(BATTLE_EFFECT_PARM *args)
{
	btEffectRunProjectileVolley(args, PROJECTILE_VOLLEY_SHURIKEN);
}

/* Ten musical notes fly slowly at each target and bounce off it, up or down at
   random.

   Abilities: Crazy Voice. */
void btEffectRunCrazyVoice(BATTLE_EFFECT_PARM *args)
{
	btEffectRunProjectileVolley(args, PROJECTILE_VOLLEY_CRAZY_VOICE);
}

/* Five yellow sparks fly level at each target, five ticks apart.

   Abilities: Wing Flutter. */
void btEffectRunWingFlutter(BATTLE_EFFECT_PARM *args)
{
	btEffectRunProjectileVolley(args, PROJECTILE_VOLLEY_WING_FLUTTER);
}

/* Ten large yellow sparks stream level at each target, one a tick, spread up
   and down.

   Abilities: Wing Stroke. */
void btEffectRunWingStroke(BATTLE_EFFECT_PARM *args)
{
	btEffectRunProjectileVolley(args, PROJECTILE_VOLLEY_WING_STROKE);
}

/* Ten small yellow sparks stream level at each target, one a tick.

   Abilities: Wing Beat. */
void btEffectRunWingBeat(BATTLE_EFFECT_PARM *args)
{
	btEffectRunProjectileVolley(args, PROJECTILE_VOLLEY_WING_BEAT);
}
