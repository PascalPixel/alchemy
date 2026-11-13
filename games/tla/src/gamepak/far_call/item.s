.syntax unified
	.thumb
	.include "games/tla/src/gamepak/far_call/veneer.s"
	.global Djinn_IsSetFar
	.thumb_func
Djinn_IsSetFar:
	overlay_veneer Djinn_IsSet
	.global Djinn_CanSetFar
	.thumb_func
Djinn_CanSetFar:
	overlay_veneer Djinn_CanSet
	.global Item_CanOwnerEquip
	.thumb_func
Item_CanOwnerEquip:
	overlay_veneer Item_CanUnitEquipDirect
	.global Item_IsCompatibleWithUnitFar
	.thumb_func
Item_IsCompatibleWithUnitFar:
	overlay_veneer Item_IsCompatibleWithUnit
	.global Inventory_FindEquippedFar
	.thumb_func
Inventory_FindEquippedFar:
	overlay_veneer Inventory_FindEquipped
	.global Party_AdjustCoinsFar
	.thumb_func
Party_AdjustCoinsFar:
	overlay_veneer Party_AdjustCoins
	.global Item_GetEquipmentGroupFar
	.thumb_func
Item_GetEquipmentGroupFar:
	overlay_veneer Item_GetEquipmentGroup
	.global Item_AdjustCounterFar
	.thumb_func
Item_AdjustCounterFar:
	overlay_veneer Item_AdjustCounter
	.global Inventory_CountFar
	.thumb_func
Inventory_CountFar:
	overlay_veneer Inventory_Count
	.global PartyInventory_HasSpaceFar
	.thumb_func
PartyInventory_HasSpaceFar:
	overlay_veneer PartyInventory_HasSpace
