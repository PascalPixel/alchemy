.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global Djinn_IsActiveFar
	.thumb_func
Djinn_IsActiveFar:
	overlay_veneer Djinn_IsActive
	.global Djinn_IsReadyFar
	.thumb_func
Djinn_IsReadyFar:
	overlay_veneer Djinn_IsReady
	.global Item_CanOwnerEquip
	.thumb_func
Item_CanOwnerEquip:
	overlay_veneer Item_CanOwnerEquipDirect
	.global Item_IsCompatibleWithOwnerFar
	.thumb_func
Item_IsCompatibleWithOwnerFar:
	overlay_veneer Item_IsCompatibleWithOwner
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
