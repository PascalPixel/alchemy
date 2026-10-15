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
	.global Party_AdjustShopCreditFar
	.thumb_func
Party_AdjustShopCreditFar:
	overlay_veneer Party_AdjustShopCredit
	.global Party_AdjustShopGiftsFar
	.thumb_func
Party_AdjustShopGiftsFar:
	overlay_veneer Party_AdjustShopGifts
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
	.global Battle_IsReviveEffectFar
	.thumb_func
Battle_IsReviveEffectFar:
	overlay_veneer Battle_IsReviveEffect
	.global Unit_RefreshPsynergyFar
	.thumb_func
Unit_RefreshPsynergyFar:
	overlay_veneer Unit_RefreshPsynergy
	.global Party_GetAverageLevelFar
	.thumb_func
Party_GetAverageLevelFar:
	overlay_veneer Party_GetAverageLevel
	.global Unit_UpdateClassFar
	.thumb_func
Unit_UpdateClassFar:
	overlay_veneer Unit_UpdateClass
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
