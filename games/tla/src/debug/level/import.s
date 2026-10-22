.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.balign 4
	.global __modsi3
	.thumb_func
__modsi3:
	overlay_veneer IwramSignedRemainder
	.global WaitFrames
	.thumb_func
WaitFrames:
	overlay_veneer WaitFramesFar
	.global Unit_Get
	.thumb_func
Unit_Get:
	overlay_veneer Unit_GetFar
	.global UiWindow_Create
	.thumb_func
UiWindow_Create:
	overlay_veneer UiWindow_CreateFar
	.global UiWork_Finalize
	.thumb_func
UiWork_Finalize:
	overlay_veneer UiWork_FinalizeFar
	.global UiText_DrawResource
	.thumb_func
UiText_DrawResource:
	overlay_veneer UiText_DrawResourceFar
	.global UiText_DrawCharacterAtOffset
	.thumb_func
UiText_DrawCharacterAtOffset:
	overlay_veneer UiText_DrawCharacterAtOffsetFar
	.global UiText_DrawStringAtOffset
	.thumb_func
UiText_DrawStringAtOffset:
	overlay_veneer UiText_DrawStringAtOffsetFar
	.global UiText_DrawStringInWindow
	.thumb_func
UiText_DrawStringInWindow:
	overlay_veneer UiText_DrawStringInWindowFar
	.global UiText_DrawNumberAtOffset
	.thumb_func
UiText_DrawNumberAtOffset:
	overlay_veneer UiText_DrawNumberAtOffsetFar
	.global UiText_DrawNumberInWindow
	.thumb_func
UiText_DrawNumberInWindow:
	overlay_veneer UiText_DrawNumberInWindowFar
	.global Func_0200068c
	.thumb_func
Func_0200068c:
	overlay_veneer UiText_DrawQuantity
	.global RenderOutput_RedrawSavedRect
	.thumb_func
RenderOutput_RedrawSavedRect:
	overlay_veneer RenderOutput_RedrawSavedRectFar
	.global RenderOutput_ClearList
	.thumb_func
RenderOutput_ClearList:
	overlay_veneer RenderOutput_ClearListFar
	.global Func_020006a4
	.thumb_func
Func_020006a4:
	overlay_veneer Func_08038318
	.global Func_020006ac
	.thumb_func
Func_020006ac:
	overlay_veneer Func_08038320
	.global Unit_Calculate
	.thumb_func
Unit_Calculate:
	overlay_veneer Unit_CalculateFar
	.global Func_020006bc
	.thumb_func
Func_020006bc:
	overlay_veneer Item_Get
	.global PartyInventory_Add
	.thumb_func
PartyInventory_Add:
	overlay_veneer PartyInventory_AddFar
	.global Party_ListActiveOwners
	.thumb_func
Party_ListActiveOwners:
	overlay_veneer Party_ListActiveOwnersFar
	.global Unit_LevelUpTo
	.thumb_func
Unit_LevelUpTo:
	overlay_veneer Unit_LevelUpToFar
	.global PartyInventory_HasSpace
	.thumb_func
PartyInventory_HasSpace:
	overlay_veneer PartyInventory_HasSpaceFar
	.global Func_020006e4
	.thumb_func
Func_020006e4:
	overlay_veneer EventRuntime_BeginFar
	.global Func_020006ec
	.thumb_func
Func_020006ec:
	overlay_veneer EventRuntime_EndFar
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer Object_GetByIdFar
	.global Func_020006fc
	.thumb_func
Func_020006fc:
	overlay_veneer ObjectMotion_SetHorizontalPositionWithTerrainFar
	.global Object_SetModeById
	.thumb_func
Object_SetModeById:
	overlay_veneer Object_SetModeByIdFar
	.global ObjectMotion_WaitForAnimationChange
	.thumb_func
ObjectMotion_WaitForAnimationChange:
	overlay_veneer ObjectMotion_WaitForAnimationChangeFar
	.global Func_02000714
	.thumb_func
Func_02000714:
	overlay_veneer EventRuntime_SetMessageFar
	.global Func_0200071c
	.thumb_func
Func_0200071c:
	overlay_veneer EventRuntime_ShowMessageAndWaitFar
	.global Func_02000724
	.thumb_func
Func_02000724:
	overlay_veneer EventRuntime_SetCountdownFar
	.global Func_0200072c
	.thumb_func
Func_0200072c:
	.ifdef TLA_EDITION_ES
	overlay_veneer BattleEffect_ResolvePendingActionsFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer BattleEffect_ResolvePendingActionsFar
	.else
	overlay_veneer EventRuntime_ResolvePendingActionsFar
	.endif
	.endif
	.global ItemMenu_DrawItemDetails
	.thumb_func
ItemMenu_DrawItemDetails:
	overlay_veneer ItemMenu_DrawItemDetailsFar
	.global Func_0200073c
	.thumb_func
Func_0200073c:
	overlay_veneer Audio_PlayCue
