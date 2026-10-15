.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global Djinn_AddToLeastLoadedOwnerFar
	.thumb_func
Djinn_AddToLeastLoadedOwnerFar:
	overlay_veneer Djinn_AddToLeastLoadedOwner
	.global Party_SumDjinnCountsFar
	.thumb_func
Party_SumDjinnCountsFar:
	overlay_veneer Party_SumDjinnCounts
	.global Party_AdjustSixDigitCounterBFar
	.thumb_func
Party_AdjustSixDigitCounterBFar:
	overlay_veneer Party_AdjustSixDigitCounterB
	.global Party_AdjustCounterCappedAt28Far
	.thumb_func
Party_AdjustCounterCappedAt28Far:
	overlay_veneer Party_AdjustCounterCappedAt28
	.global Func_080ad268
	.thumb_func
Func_080ad268:
	overlay_veneer Inventory_CheckDiscard
	.global Func_080ad270
	.thumb_func
Func_080ad270:
	overlay_veneer Inventory_CheckUnequip
	.global Inventory_DiscardFar
	.thumb_func
Inventory_DiscardFar:
	overlay_veneer Inventory_Discard
	.global BattleEffectRevCheckFar
	.thumb_func
BattleEffectRevCheckFar:
	overlay_veneer BattleEffectRevCheck
	.global Owner_RefreshClassActionsFar
	.thumb_func
Owner_RefreshClassActionsFar:
	overlay_veneer Owner_RefreshClassActions
	.global Party_GetAverageLevelFar
	.thumb_func
Party_GetAverageLevelFar:
	overlay_veneer Party_GetAverageLevel
	.global Owner_RefreshDerivedDataFar
	.thumb_func
Owner_RefreshDerivedDataFar:
	overlay_veneer Owner_RefreshDerivedData
	.global Inventory_CountItemFar
	.thumb_func
Inventory_CountItemFar:
	overlay_veneer Inventory_CountItem
	.global PartyInventory_CountItemFar
	.thumb_func
PartyInventory_CountItemFar:
	overlay_veneer PartyInventory_CountItem
	.global PartyInventory_DiscardFar
	.thumb_func
PartyInventory_DiscardFar:
	overlay_veneer PartyInventory_Discard
