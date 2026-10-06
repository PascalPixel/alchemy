.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Func_08108030
	.thumb_func
Func_08108030:
	overlay_veneer Func_08108948
	.global Func_08108038
	.thumb_func
Func_08108038:
	overlay_veneer Func_081089e4
	.global Func_08108040
	.thumb_func
Func_08108040:
	overlay_veneer Func_08108a88
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
	.global Shop_InitEffectFar
	.thumb_func
Shop_InitEffectFar:
	overlay_veneer Shop_InitEffect
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
