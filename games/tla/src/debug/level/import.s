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
	.global Window_Create
	.thumb_func
Window_Create:
	overlay_veneer Window_CreateFar
	.global Window_Close
	.thumb_func
Window_Close:
	overlay_veneer Window_CloseFar
	.global Text_DrawMessage
	.thumb_func
Text_DrawMessage:
	overlay_veneer Text_DrawMessageFar
	.global Text_DrawCharacterAtOffset
	.thumb_func
Text_DrawCharacterAtOffset:
	overlay_veneer Text_DrawCharacterAtOffsetFar
	.global Text_DrawStringAtOffset
	.thumb_func
Text_DrawStringAtOffset:
	overlay_veneer Text_DrawStringAtOffsetFar
	.global Text_DrawStringInWindow
	.thumb_func
Text_DrawStringInWindow:
	overlay_veneer Text_DrawStringInWindowFar
	.global Text_DrawNumberAtOffset
	.thumb_func
Text_DrawNumberAtOffset:
	overlay_veneer Text_DrawNumberAtOffsetFar
	.global Text_DrawNumberInWindow
	.thumb_func
Text_DrawNumberInWindow:
	overlay_veneer Text_DrawNumberInWindowFar
	.global Func_0200068c
	.thumb_func
Func_0200068c:
	overlay_veneer MessageArgument_PushFar
	.global WindowSprite_RedrawSavedRect
	.thumb_func
WindowSprite_RedrawSavedRect:
	overlay_veneer WindowSprite_RedrawSavedRectFar
	.global WindowSprite_ClearList
	.thumb_func
WindowSprite_ClearList:
	overlay_veneer WindowSprite_ClearListFar
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
	overlay_veneer Event_StartFar
	.global Func_020006ec
	.thumb_func
Func_020006ec:
	overlay_veneer Event_EndFar
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
	overlay_veneer Event_SetMessageIdFar
	.global Func_0200071c
	.thumb_func
Func_0200071c:
	overlay_veneer Event_ShowMessageAndWaitFar
	.global Func_02000724
	.thumb_func
Func_02000724:
	overlay_veneer Event_SetExitRequestFar
	.global Func_0200072c
	.thumb_func
Func_0200072c:
	overlay_veneer Event_ResolvePendingActionsFar
	.global ItemMenu_DrawItemDetails
	.thumb_func
ItemMenu_DrawItemDetails:
	overlay_veneer ItemMenu_DrawItemDetailsFar
	.global Func_0200073c
	.thumb_func
Func_0200073c:
	overlay_veneer Sound_PlayFar
