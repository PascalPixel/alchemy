.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global Inventory_GetEquippedItemFar
	.thumb_func
Inventory_GetEquippedItemFar:
	overlay_veneer Inventory_GetEquippedItem
	.global BattleActGet
	.thumb_func
BattleActGet:
	overlay_veneer BattleAction_GetDirect
	.global Unit_LearnPsynergyFar
	.thumb_func
Unit_LearnPsynergyFar:
	overlay_veneer Unit_LearnPsynergy
	.global Unit_KnowsActionFar
	.thumb_func
Unit_KnowsActionFar:
	overlay_veneer Unit_KnowsAction
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
