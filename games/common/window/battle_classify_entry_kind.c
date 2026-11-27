/* mnBattleClassifyEntryKind, the same in both games. Both games'
   window/screens_battle_party_commands.c include it where the function
   stands. */

/* Classifies ability `act` for the target selection (the `kind` of
   mnBattleTargetRunSelection, BATTLE_TARGET_HINT), which shows what it would do
   on a party member and, for the curing and reviving hints, starts on one who
   needs it. */
Sint32 mnBattleClassifyEntryKind(const ABILITY_DEFINITION *act)
{
	Sint32 abilityKind;
	Sint32 tgtKind;

	tgtKind = TARGET_HINT_NONE;
	abilityKind = act->flg & ABILITY_FORMULA_MASK;
	if(abilityKind == ABILITY_FORMULA_HEAL_HP)
		tgtKind = TARGET_HINT_HP;
	if(abilityKind == ABILITY_FORMULA_HEAL_PP)
		tgtKind = TARGET_HINT_PP;
	if(act->eff == ABILITY_EFFECT_CURE_POISON)
		tgtKind = TARGET_HINT_CURE_POISON;
	if(act->eff == ABILITY_EFFECT_RESTORE)
		tgtKind = TARGET_HINT_RESTORE;
	if(act->eff == ABILITY_EFFECT_CURE_ALL)
		tgtKind = TARGET_HINT_CURE_ALL;
	if(gmBattleIsReviveEffectStub(act->eff) != 0)
		tgtKind = TARGET_HINT_REVIVE;
	return tgtKind;
}
