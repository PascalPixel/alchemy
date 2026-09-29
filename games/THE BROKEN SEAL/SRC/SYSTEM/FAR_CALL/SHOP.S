@ Far-call stub table heading the shop and inn code at 0x080b0000: fixed 8-byte
@ veneers, each loading its target into r4 and branching, which the calling
@ convention permits. No compiler emits them; the shared overlay veneer macro
@ builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.global FarCall_ShopTable
FarCall_ShopTable:
	.global Func_080b0000
	.thumb_func
Func_080b0000:
	.global Battle_ApplyPresetItemsAndFlagsFar
	.thumb_func
Battle_ApplyPresetItemsAndFlagsFar:
	overlay_veneer Battle_ApplyPresetItemsAndFlags
	.global Shop_RunFar
	.thumb_func
Shop_RunFar:
	overlay_veneer Shop_Run
	.global Shop_ConfirmActFar
	.thumb_func
Shop_ConfirmActFar:
	overlay_veneer Shop_ConfirmAct
	.global Inn_CheckInFar
	.thumb_func
Inn_CheckInFar:
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
	.global Shop_PickUnitItemFar
	.thumb_func
Shop_PickUnitItemFar:
	overlay_veneer Shop_PickUnitItem
	.global AudioCommand_WaitForStateByteClearFar
	.thumb_func
AudioCommand_WaitForStateByteClearFar:
	overlay_veneer AudioCommand_WaitForStateByteClear
	.global Func_080b0068
	.thumb_func
Func_080b0068:
	overlay_veneer Inn_PlaySleep