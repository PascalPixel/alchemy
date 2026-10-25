.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global Resource_FarCall008
Resource_FarCall008:
	.global Func_08108000
	.thumb_func
Func_08108000:
	overlay_veneer Debug_ApplyPresetItemsAndFlags
	.global Func_08108008
	.thumb_func
Func_08108008:
	overlay_veneer Func_081082c4
	.global Func_08108010
	.thumb_func
Func_08108010:
	overlay_veneer Shop_ConfirmAct
	.global Func_08108018
	.thumb_func
Func_08108018:
	overlay_veneer Inn_CheckIn
	.global Func_08108020
	.thumb_func
Func_08108020:
	overlay_veneer Func_0810b5c4
	.global Func_08108028
	.thumb_func
Func_08108028:
	overlay_veneer Func_0810b3ec
	.global Func_08108030
	.thumb_func
Func_08108030:
	overlay_veneer ShopCursor_Advance
	.global Func_08108038
	.thumb_func
Func_08108038:
	overlay_veneer Func_081089e4
	.global Func_08108040
	.thumb_func
Func_08108040:
	overlay_veneer Shop_SetCursor
	.global Func_08108048
	.thumb_func
Func_08108048:
	overlay_veneer Func_08108aa8
	.global Shop_FillSelectorFar
	.thumb_func
Shop_FillSelectorFar:
	overlay_veneer Shop_FillSelector
	.global Shop_RestoreSceneTilesFar
	.thumb_func
Shop_RestoreSceneTilesFar:
	overlay_veneer Shop_RestoreSceneTiles
	.global Shop_InitializeEffectFar
	.thumb_func
Shop_InitializeEffectFar:
	overlay_veneer Shop_InitializeEffect
	.global Func_08108068
	.thumb_func
Func_08108068:
	overlay_veneer Func_0810b7a8
	.global Func_08108070
	.thumb_func
Func_08108070:
	overlay_veneer Func_0810b79c
	.global AudioCommand_WaitForStateByteClearFar
	.thumb_func
AudioCommand_WaitForStateByteClearFar:
	overlay_veneer AudioCommand_WaitForStateByteClear
	.global Inn_PlaySleepFar
	.thumb_func
Inn_PlaySleepFar:
	overlay_veneer Inn_PlaySleep
