.syntax unified
	.thumb
	.include "games/tbs/src/system/overlay.inc"
	.global FarCall_ShopTable
FarCall_ShopTable:
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
	.thumb_func
ShopCursor_AdvanceFar:
	overlay_veneer ShopCursor_Advance
	.global ShopCursor_MoveTowardTargetFar
	.thumb_func
ShopCursor_MoveTowardTargetFar:
	overlay_veneer ShopCursor_MoveTowardTarget
	.global Shop_SetCursorFar
	.thumb_func
Shop_SetCursorFar:
	overlay_veneer Shop_SetCursor
	.global ShopCursor_SetPositionImmediateFar
	.thumb_func
ShopCursor_SetPositionImmediateFar:
	overlay_veneer ShopCursor_SetPositionImmediate
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
	.global Shop_PickUnitItemFar
	.thumb_func
Shop_PickUnitItemFar:
	overlay_veneer Shop_PickUnitItem
	.global AudioCommand_WaitForStateByteClearFar
	.thumb_func
AudioCommand_WaitForStateByteClearFar:
	overlay_veneer AudioCommand_WaitForStateByteClear
	.global Inn_PlaySleepFar
	.thumb_func
Inn_PlaySleepFar:
	overlay_veneer Inn_PlaySleep
