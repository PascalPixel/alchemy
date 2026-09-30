@ Far-call stub table at 0x080c8008: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Game_ResetForNewGameFar
	.thumb_func
Game_ResetForNewGameFar:
	overlay_veneer Game_ResetForNewGame
	.global Battle_WaitMode0Far
	.thumb_func
Battle_WaitMode0Far:
	overlay_veneer Battle_WaitMode0
	.global Func_080c8018
	.thumb_func
Func_080c8018:
	overlay_veneer Func_080d22a8
	.global Func_080c8020
	.thumb_func
Func_080c8020:
	overlay_veneer Func_080d2350
	overlay_veneer Event_RunObjectHookAndWait
	overlay_veneer Event_CallWithLastActiveObjectId
	overlay_veneer Scene_AssignViewFlags
	overlay_veneer Event_SetWorkWord10
	.global Event_PrepareObjectAndApplyValueFar
	.thumb_func
Event_PrepareObjectAndApplyValueFar:
	overlay_veneer Event_PrepareObjectAndApplyValue
	overlay_veneer Event_PrepareTwoObjectsAndApply
	.global Party_RemoveOwnerRestoredFar
	.thumb_func
Party_RemoveOwnerRestoredFar:
	overlay_veneer Party_RemoveOwnerRestored
	.global PartyInventory_GiveItemFar
	.thumb_func
PartyInventory_GiveItemFar:
	overlay_veneer PartyInventory_GiveItem
	overlay_veneer Inventory_TryAddAndReturnOwner
	.global Inventory_PromptAndSetObjectModeFar
	.thumb_func
Inventory_PromptAndSetObjectModeFar:
	overlay_veneer Inventory_PromptAndSetObjectMode
	.global Func_080c8078
	.thumb_func
Func_080c8078:
	overlay_veneer Func_080d296c
	.global UiText_DrawQuantityPairWithCueFar
	.thumb_func
UiText_DrawQuantityPairWithCueFar:
	overlay_veneer UiText_DrawQuantityPairWithCue
