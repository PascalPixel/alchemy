/* btEffectRunNumbingSting to btEffectRunDrainFang, the same in both games.
   Both games' battle_effect/battle_effect_target_strike.c include it where
   the functions stand. */

/* One slash strikes the first target among glints, in lightning colours.

   Abilities: Numbing Sting and, in The Lost Age, Paralytail. */
void btEffectRunNumbingSting(BATTLE_EFFECT_PARM *args)
{
	btEffectRunTargetStrike(args, TARGET_STRIKE_STYLE_LIGHTNING_GLINT_SLASH);
}

/* Fangs bite the first target three times, in ember colours, while motes stream
   out of it and back into the user.

   Abilities: Cannibal Fang. */
void btEffectRunCannibalFang(BATTLE_EFFECT_PARM *args)
{
	btEffectRunTargetStrike(args, TARGET_STRIKE_STYLE_EMBER_DRAIN_BITES);
}

/* Fangs bite the first target once, in lime, with smoke.

   Abilities: Poisonous Bite; Poison Fang. */
void btEffectRunPoisonousBite(BATTLE_EFFECT_PARM *args)
{
	btEffectRunTargetStrike(args, TARGET_STRIKE_STYLE_LIME_SMOKE_BITE);
}

/* Fangs bite the first target once, with smoke, in Mars Djinn colours.

   Abilities: Rabid Fang. */
void btEffectRunRabidFang(BATTLE_EFFECT_PARM *args)
{
	btEffectRunTargetStrike(args, TARGET_STRIKE_STYLE_MARS_SMOKE_BITE);
}

/* Fangs bite the first target three times among glints, in lightning colours.

   Abilities: Electric Bite. */
void btEffectRunElectricBite(BATTLE_EFFECT_PARM *args)
{
	btEffectRunTargetStrike(args, TARGET_STRIKE_STYLE_LIGHTNING_GLINT_BITES);
}

/* One lime slash strikes the first target, with smoke.

   Abilities: Poison Tail; in The Lost Age, Poison Sting. */
void btEffectRunPoisonTail(BATTLE_EFFECT_PARM *args)
{
	btEffectRunTargetStrike(args, TARGET_STRIKE_STYLE_LIME_SMOKE_SLASH);
}

/* Fangs bite the first target three times, draining motes back into the user,
   in Mars Djinn colours.

   Abilities: Vampiric Fang. */
void btEffectRunVampiricFang(BATTLE_EFFECT_PARM *args)
{
	btEffectRunTargetStrike(args, TARGET_STRIKE_STYLE_MARS_DRAIN_BITES);
}

/* Fangs bite the first target twice, in blue, with sparks.

   Abilities: Double Fang. */
void btEffectRunDoubleFang(BATTLE_EFFECT_PARM *args)
{
	btEffectRunTargetStrike(args, TARGET_STRIKE_STYLE_BLUE_SPARK_BITES);
}

/* One heavy slash strikes the first target in Mars Djinn colours, throwing
   sparks.

   Abilities: Mortal Blow. */
void btEffectRunMortalBlow(BATTLE_EFFECT_PARM *args)
{
	btEffectRunTargetStrike(args, TARGET_STRIKE_STYLE_MARS_HEAVY_SLASH);
}

/* Two slashes strike the first target, throwing sparks, in lightning colours.

   Abilities: Twin Beaks and, in The Lost Age, Gryphon. */
void btEffectRunTwinBeaks(BATTLE_EFFECT_PARM *args)
{
	btEffectRunTargetStrike(args, TARGET_STRIKE_STYLE_LIGHTNING_SPARK_SLASHES);
}

/* Fangs bite the first target three times, in pink, with smoke.

   Abilities: Acid Bite. */
void btEffectRunAcidBite(BATTLE_EFFECT_PARM *args)
{
	btEffectRunTargetStrike(args, TARGET_STRIKE_STYLE_PINK_SMOKE_BITES);
}

/* Fangs bite the first target three times, added to what lies under them,
   draining large motes back into the user, in Mars Djinn colours.

   Abilities: Drain Fang. */
void btEffectRunDrainFang(BATTLE_EFFECT_PARM *args)
{
	btEffectRunTargetStrike(
		args, TARGET_STRIKE_STYLE_MARS_ADDITIVE_DRAIN_BITES);
}
