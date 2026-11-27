/* The forge: its recipes, the materials the party holds and the roll for the
   result. Part of shop.c. */

/* The forge's recipe for an item, or NULL when it has none; the table ends at
   an item of -1. */
const FORGE_RECIPE *mnForgeFindRecipe(Sint32 itemId)
{
	const FORGE_RECIPE *recipe;

	itemId &= INVENTORY_ITEM_ID_MASK;
	for(recipe = ForgeRecipes; recipe->item != -1; recipe++) {
		if(recipe->item == itemId)
			return recipe;
	}
	return NULL;
}

/* Returns what the forge makes of an item, counted from MsgItemCondition: 0 for
   an item it has no recipe for, then FORGE_CONDITION plus one. */
Sint32 mnForgeGetItemCondition(Sint32 itemId)
{
	const FORGE_RECIPE *recipe = mnForgeFindRecipe(itemId);

	if(recipe != NULL)
		return recipe->condition + 1;
	return 0;
}

/* Rolls what the forge makes of a material: one of its recipe's results, each
   as likely as its weight; 0 for an item with no recipe. */
Sint32 mnForgeRollResult(Sint32 itemId)
{
	const FORGE_RECIPE *recipe = mnForgeFindRecipe(itemId);
	Sint32				total = 0;
	Sint32				result = 0;
	Sint32				i;
	Sint32				roll;

	if(recipe != NULL) {
		for(i = 0; i < 8 && recipe->results[i] != 0; i++)
			total += recipe->weights[i];
		roll = syRandomBelow(total);
		for(i = 0; i < 8 && recipe->results[i] != 0; i++) {
			roll -= recipe->weights[i];
			if(roll < 0)
				break;
		}
		if(i != 8)
			result = recipe->results[i];
	}
	return result;
}

/* Returns 1 when the party member carries anything the forge takes. */
Bool mnForgeUnitHasMaterial(Sint32 unitId)
{
	UNIT   *unit;
	Uint16 *inventory;
	Sint32	slot;

	unit = syUnitGet(unitId);
	slot = 0;
	inventory = unit->inventory;
	for(; slot < INVENTORY_SLOTS; slot++) {
		if(mnForgeGetItemCondition(*inventory++) != 0)
			return TRUE;
	}
	return FALSE;
}

/* Returns 1 when anyone in the party carries anything the forge takes. */
Bool mnForgePartyHasMaterial(void)
{
	Uint16 partyMemberIds[16];
	Sint32 count;
	Sint32 i;

	count = gmPartyListMembersStub(partyMemberIds);
	for(i = 0; i < count; i++) {
		if(mnForgeUnitHasMaterial(partyMemberIds[i]) != 0)
			return TRUE;
	}
	return FALSE;
}

/* Records the item the forge is making, for the smith to hand over later. */
void mnForgeSetItem(Sint32 itemId)
{
	MainRoutineStateWork.forgeItem = itemId & INVENTORY_ITEM_ID_MASK;
}

/* Returns the item the forge is making. */
Sint32 mnForgeGetItem(void)
{
	Uint32 itemId = MainRoutineStateWork.forgeItem;
	Uint32 mask;

	mask = INVENTORY_ITEM_ID_MASK;
	mask &= itemId;
	return mask;
}
