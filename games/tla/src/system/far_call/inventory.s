.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global Resource_FarCall005
Resource_FarCall005:
	.global Trade_GetOfferStateFar
	.thumb_func
Trade_GetOfferStateFar:
	overlay_veneer Trade_GetOfferState
	.global UnitCalcFar
	.thumb_func
UnitCalcFar:
	.global BattleUnit_Recalculate
	.thumb_func
BattleUnit_Recalculate:
	overlay_veneer UnitCalc
	.global Item_Get
	.thumb_func
Item_Get:
	overlay_veneer Item_GetDirect
	.global Inventory_GetQuantityFar
	.thumb_func
Inventory_GetQuantityFar:
	overlay_veneer Inventory_GetQuantity
	.global Inventory_AddItemFar
	.thumb_func
Inventory_AddItemFar:
	overlay_veneer Inventory_AddItem
	.global PartyInventory_AddFar
	.thumb_func
PartyInventory_AddFar:
	overlay_veneer PartyInventory_Add
	.global Inventory_FindFar
	.thumb_func
Inventory_FindFar:
	overlay_veneer Inventory_Find
	.global PartyInventory_FindOwnerFar
	.thumb_func
PartyInventory_FindOwnerFar:
	overlay_veneer PartyInventory_FindOwner
