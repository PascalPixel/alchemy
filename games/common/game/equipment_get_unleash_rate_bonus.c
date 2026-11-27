/* gmEquipmentGetUnleashRateBonus, the same in both games. Both games'
   game/game_battle_arithmetic.c include it where the function stands. */

/* Returns the unleash chance, in percent, that a unit's equipment adds
   (EQUIP_EFFECT_ADD_UNLEASH_CHANCE). */
Sint32 gmEquipmentGetUnleashRateBonus(UNIT *unit)
{
	Sint32				   chanceBonus;
	Sint32				   inventorySlot;
	Sint32				   effectIdx;
	const ITEM_DEFINITION *item;

	chanceBonus = 0;
	for(inventorySlot = 0; inventorySlot < INVENTORY_SLOTS; inventorySlot++) {
		if(unit->inventory[inventorySlot] & INVENTORY_EQUIPPED) {
			item = gmItemGetDirect(unit->inventory[inventorySlot]);
			for(effectIdx = 0; effectIdx < EQUIP_EFFECT_SLOTS; effectIdx++) {
				if(item->effs[effectIdx].kind ==
					EQUIP_EFFECT_ADD_UNLEASH_CHANCE)
					chanceBonus += item->effs[effectIdx].amount;
			}
		}
	}
	if(chanceBonus < 0)
		chanceBonus = 0;
	return chanceBonus;
}
