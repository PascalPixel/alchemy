/* mnPsynergyMenuIsActionRestricted, the same in both games. Both games'
   menu/menu_psynergy.c include it where the function stands. */

/* Returns 1 when actionEntry cannot be used from the menu: it has no field
   effect and is not usable both in the field and in battle. */
Bool mnPsynergyMenuIsActionRestricted(Sint32 actionEntry)
{
	ABILITY_DEFINITION *action =
		gmAbilityGetDirectStub(PSYNERGY_ABILITY_OF(actionEntry));

	if(action->fieldPsynergy != 0 ||
		(action->flg & (ABILITY_USABLE_IN_FIELD | ABILITY_USABLE_IN_BATTLE)) ==
			(ABILITY_USABLE_IN_FIELD | ABILITY_USABLE_IN_BATTLE))
		return FALSE;
	return TRUE;
}
