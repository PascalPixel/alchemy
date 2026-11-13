.syntax unified
	.thumb
	.include "games/tla/src/gamepak/far_call/veneer.s"
	.global Inventory_GetEquippedItemFar
	.thumb_func
Inventory_GetEquippedItemFar:
	overlay_veneer Inventory_GetEquippedItem
	.global Ability_Get
	.thumb_func
Ability_Get:
	overlay_veneer Ability_GetDirect
	.global Unit_LearnPsynergyFar
	.thumb_func
Unit_LearnPsynergyFar:
	overlay_veneer Unit_LearnPsynergy
	.global Unit_KnowsPsynergyFar
	.thumb_func
Unit_KnowsPsynergyFar:
	overlay_veneer Unit_KnowsPsynergy
	.global Func_080ad090
	.thumb_func
Func_080ad090:
	overlay_veneer Func_080addf0
	.global Func_080ad098
	.thumb_func
Func_080ad098:
	overlay_veneer Unit_CheckLevelUp
	.global Func_080ad0a0
	.thumb_func
Func_080ad0a0:
	overlay_veneer Party_InitializeCharacters
	.global Func_080ad0a8
	.thumb_func
Func_080ad0a8:
	overlay_veneer Unit_LevelNoOp
