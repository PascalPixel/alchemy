@ Far-call stub table heading the shop and inn code at 0x080b0000: fixed 8-byte
@ veneers, each loading its target into r4 and branching, which the calling
@ convention permits. No compiler emits them; the shared overlay veneer macro
@ builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.global Func_080b0000
	.thumb_func
Func_080b0000:
	overlay_veneer Battle_ApplyPresetItemsAndFlags
	overlay_veneer Shop_Run
	overlay_veneer Shop_ConfirmAct
	overlay_veneer Inn_CheckIn
	.global ShopCursor_AdvanceFar
	.global Func_080b0020
	.thumb_func
ShopCursor_AdvanceFar:
Func_080b0020:
	overlay_veneer ShopCursor_Advance
	.global ShopCursor_MoveTowardTargetFar
	.thumb_func
ShopCursor_MoveTowardTargetFar:
	overlay_veneer ShopCursor_MoveTowardTarget
	.global Shop_SetCursorFar
	.thumb_func
Shop_SetCursorFar:
	overlay_veneer Shop_SetCursor
	.global Func_080b0038
	.thumb_func
Func_080b0038:
	.global ShopCursor_SetPositionImmediateFar
	.thumb_func
ShopCursor_SetPositionImmediateFar:
	overlay_veneer ShopCursor_SetPositionImmediate
	overlay_veneer Shop_FillSelector
	.global Unnamed_080b0840Far
	.thumb_func
Unnamed_080b0840Far:
	overlay_veneer Shop_RestoreSceneTiles
	.global Shop_InitEffectFar
	.thumb_func
Shop_InitEffectFar:
	overlay_veneer Shop_InitEffect
	overlay_veneer Shop_PickUnitItem
	overlay_veneer AudioCommand_WaitForStateByteClear
	overlay_veneer Inn_PlaySleep