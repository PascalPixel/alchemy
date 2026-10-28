.syntax unified
	.thumb
	.include "games/tla/src/gamepak/far_call/veneer.s"
	.global PartyInventory_RemoveFar
	.thumb_func
PartyInventory_RemoveFar:
	overlay_veneer PartyInventory_Remove
	.global Inventory_EquipFar
	.thumb_func
Inventory_EquipFar:
	overlay_veneer Inventory_Equip
	.global Inventory_RemoveFar
	.thumb_func
Inventory_RemoveFar:
	overlay_veneer Inventory_Remove
	.global Inventory_BreakFar
	.thumb_func
Inventory_BreakFar:
	overlay_veneer Inventory_Break
	.global Inventory_RepairFar
	.thumb_func
Inventory_RepairFar:
	overlay_veneer Inventory_Repair
	.global Item_GetTargetModeFar
	.thumb_func
Item_GetTargetModeFar:
	overlay_veneer Item_GetTargetMode
