.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global Inventory_GetEquippedItemFar
	.thumb_func
Inventory_GetEquippedItemFar:
	overlay_veneer Inventory_GetEquippedItem
	.global BtlActGet
	.thumb_func
BtlActGet:
	overlay_veneer BattleAction_GetDirect
	.global OwnerAction_AddFar
	.thumb_func
OwnerAction_AddFar:
	overlay_veneer OwnerAction_Add
	.global Equipment_HasValueFar
	.thumb_func
Equipment_HasValueFar:
	overlay_veneer Equipment_HasValue
	.global Func_080ad090
	.thumb_func
Func_080ad090:
	overlay_veneer Func_080addf0
	.global Func_080ad098
	.thumb_func
Func_080ad098:
	overlay_veneer Func_080af794
	.global Func_080ad0a0
	.thumb_func
Func_080ad0a0:
	overlay_veneer Func_080af7ac
	.global Func_080ad0a8
	.thumb_func
Func_080ad0a8:
	overlay_veneer Func_080af8cc
