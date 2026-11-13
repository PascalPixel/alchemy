.syntax unified
	.thumb
	.include "games/tla/src/gamepak/far_call/veneer.s"
	.global Resource_FarCall008
Resource_FarCall008:
	.global Func_08108000
	.thumb_func
Func_08108000:
	overlay_veneer Shop_OpenDebugShop
	.global Func_08108008
	.thumb_func
Func_08108008:
	overlay_veneer Shop_Run
	.global Func_08108010
	.thumb_func
Func_08108010:
	overlay_veneer Sanctum_Run
	.global Func_08108018
	.thumb_func
Func_08108018:
	overlay_veneer Inn_CheckIn
	.global Func_08108020
	.thumb_func
Func_08108020:
	overlay_veneer Forge_Run
	.global Func_08108028
	.thumb_func
Func_08108028:
	overlay_veneer Inn_QuoteRoomPrice
	.global Func_08108030
	.thumb_func
Func_08108030:
	overlay_veneer MenuCursor_Advance
	.global Func_08108038
	.thumb_func
Func_08108038:
	overlay_veneer MenuCursor_MoveTowardTarget
	.global Func_08108040
	.thumb_func
Func_08108040:
	overlay_veneer MenuCursor_Set
	.global Func_08108048
	.thumb_func
Func_08108048:
	overlay_veneer MenuCursor_SetPositionImmediate
	.global Shop_FillSelectorFar
	.thumb_func
Shop_FillSelectorFar:
	overlay_veneer QuantityBar_Fill
	.global Shop_RestoreSceneTilesFar
	.thumb_func
Shop_RestoreSceneTilesFar:
	overlay_veneer Shop_SaveAndTintFieldPalette
	.global Shop_InitializeEffectFar
	.thumb_func
Shop_InitializeEffectFar:
	overlay_veneer Shop_RestoreFieldPalette
	.global Func_08108068
	.thumb_func
Func_08108068:
	overlay_veneer ItemPicker_RunToDrop
	.global Func_08108070
	.thumb_func
Func_08108070:
	overlay_veneer Func_0810b79c
	.global Sound_WaitForJingleFar
	.thumb_func
Sound_WaitForJingleFar:
	overlay_veneer Sound_WaitForJingle
	.global Inn_PlaySleepFar
	.thumb_func
Inn_PlaySleepFar:
	overlay_veneer Inn_PlaySleep
