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
	.global GameFlag_Test
	.thumb_func
GameFlag_Test:
	overlay_veneer GameFlag_TestFar
	.global GameFlag_SetBit
	.thumb_func
GameFlag_SetBit:
	overlay_veneer GameFlag_SetBitFar
	.global Func_02000c88
	.thumb_func
Func_02000c88:
	overlay_veneer Func_080200c0
	.global Func_02000c90
	.thumb_func
Func_02000c90:
	overlay_veneer Map_CopyCellsRectFar
	.global ObjectDispatch_SetSingleChildFlags
	.thumb_func
ObjectDispatch_SetSingleChildFlags:
	overlay_veneer ObjectDispatch_SetSingleChildFlagsFar
	.global Window_Create
	.thumb_func
Window_Create:
	overlay_veneer Window_CreateFar
	.global Window_Close
	.thumb_func
Window_Close:
	overlay_veneer Window_CloseFar
	.global Message_OpenWindow
	.thumb_func
Message_OpenWindow:
	overlay_veneer Message_OpenWindowFar
	.global FieldMessage_Show
	.thumb_func
FieldMessage_Show:
	overlay_veneer FieldMessage_ShowFar
	.global Message_AllTyped
	.thumb_func
Message_AllTyped:
	overlay_veneer Message_AllTypedFar
	.global WindowSprite_PrepareForRedraw
	.thumb_func
WindowSprite_PrepareForRedraw:
	overlay_veneer WindowSprite_PrepareForRedrawFar
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
	.global Func_02000d00
	.thumb_func
Func_02000d00:
	overlay_veneer Func_080380f8
	.global Func_02000d08
	.thumb_func
Func_02000d08:
	overlay_veneer MessageArgument_PushFar
	.global Message_CloseFinishedWindows
	.thumb_func
Message_CloseFinishedWindows:
	overlay_veneer Message_CloseFinishedWindowsFar
	.global WindowSprite_RedrawSavedRect
	.thumb_func
WindowSprite_RedrawSavedRect:
	overlay_veneer WindowSprite_RedrawSavedRectFar
	.global Func_02000d20
	.thumb_func
Func_02000d20:
	overlay_veneer Func_080382f8
	.global Func_02000d28
	.thumb_func
Func_02000d28:
	overlay_veneer Func_08038318
	.global Func_02000d30
	.thumb_func
Func_02000d30:
	overlay_veneer Func_08038320
	.global Func_02000d38
	.thumb_func
Func_02000d38:
	overlay_veneer Func_08038340
	.global Unit_Calculate
	.thumb_func
Unit_Calculate:
	overlay_veneer Unit_CalculateFar
	.global Func_02000d48
	.thumb_func
Func_02000d48:
	overlay_veneer Item_Get
	.global Inventory_AddItem
	.thumb_func
Inventory_AddItem:
	overlay_veneer Inventory_AddItemFar
	.global PartyInventory_Add
	.thumb_func
PartyInventory_Add:
	overlay_veneer PartyInventory_AddFar
	.global Inventory_Break
	.thumb_func
Inventory_Break:
	overlay_veneer Inventory_BreakFar
	.global Unit_AddHp
	.thumb_func
Unit_AddHp:
	overlay_veneer Unit_AddHpFar
	.global Unit_AddPp
	.thumb_func
Unit_AddPp:
	overlay_veneer Unit_AddPpFar
	.global Party_ListActiveOwners
	.thumb_func
Party_ListActiveOwners:
	overlay_veneer Party_ListActiveOwnersFar
	.global Party_RemoveActiveOwner
	.thumb_func
Party_RemoveActiveOwner:
	overlay_veneer Party_RemoveActiveOwnerFar
	.global Djinn_AddToOwner
	.thumb_func
Djinn_AddToOwner:
	overlay_veneer Djinn_AddToOwnerFar
	.global Djinn_Activate
	.thumb_func
Djinn_Activate:
	overlay_veneer Djinn_ActivateFar
	.global Unit_LevelUpTo
	.thumb_func
Unit_LevelUpTo:
	overlay_veneer Unit_LevelUpToFar
	.global Party_AdjustCoins
	.thumb_func
Party_AdjustCoins:
	overlay_veneer Party_AdjustCoinsFar
	.global PartyInventory_HasSpace
	.thumb_func
PartyInventory_HasSpace:
	overlay_veneer PartyInventory_HasSpaceFar
	.global Func_02000db0
	.thumb_func
Func_02000db0:
	overlay_veneer Func_080ad240
	.ifndef TLA_EDITION_JA
	.global Djinn_AddToLeastLoadedOwner
	.thumb_func
Djinn_AddToLeastLoadedOwner:
	overlay_veneer Djinn_AddToLeastLoadedOwnerFar
	.endif
	.global Func_02000dc0
	.thumb_func
Func_02000dc0:
	overlay_veneer GameFlag_RefreshLureCapFar
	.global Func_02000dc8
	.thumb_func
Func_02000dc8:
	overlay_veneer PartyInventory_FindRoomFar
	.global Func_02000dd0
	.thumb_func
Func_02000dd0:
	overlay_veneer Func_080ad2f8
	.global FieldEvent_WaitFrames
	.thumb_func
FieldEvent_WaitFrames:
	overlay_veneer FieldEvent_WaitFramesFar
	.global PartyInventory_GiveItem
	.thumb_func
PartyInventory_GiveItem:
	overlay_veneer PartyInventory_GiveItemFar
	.global Event_AskYesNoWithActorAnimation
	.thumb_func
Event_AskYesNoWithActorAnimation:
	overlay_veneer Event_AskYesNoWithActorAnimationFar
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer Object_GetByIdFar
	.global Func_02000df8
	.thumb_func
Func_02000df8:
	overlay_veneer ObjectMotion_SetHorizontalPositionWithTerrainFar
	.global Object_SetModeById
	.thumb_func
Object_SetModeById:
	overlay_veneer Object_SetModeByIdFar
	.global Func_02000e08
	.thumb_func
Func_02000e08:
	overlay_veneer EventRuntime_SetMessageFar
	.global FieldMessage_ShowAtActor
	.thumb_func
FieldMessage_ShowAtActor:
	overlay_veneer FieldMessage_ShowAtActorFar
	.global Func_02000e18
	.thumb_func
Func_02000e18:
	overlay_veneer EventRuntime_ShowMessageAndWaitFar
	.global Func_02000e20
	.thumb_func
Func_02000e20:
	overlay_veneer Field_RequestBattleFromEncounterTableFar
	.global Func_02000e28
	.thumb_func
Func_02000e28:
	overlay_veneer Func_080c83e0
	.global Func_02000e30
	.thumb_func
Func_02000e30:
	overlay_veneer EventRuntime_GetControlledOwnerFar
	.global ItemMenu_DrawItemDetails
	.thumb_func
ItemMenu_DrawItemDetails:
	overlay_veneer ItemMenu_DrawItemDetailsFar
	.global Func_02000e40
	.thumb_func
Func_02000e40:
	overlay_veneer Func_08108008
	.global Func_02000e48
	.thumb_func
Func_02000e48:
	overlay_veneer Func_08108010
	.global Func_02000e50
	.thumb_func
Func_02000e50:
	overlay_veneer Func_08108018
	.global Func_02000e58
	.thumb_func
Func_02000e58:
	overlay_veneer Func_08108020
	.global Func_02000e60
	.thumb_func
Func_02000e60:
	overlay_veneer Func_08108070
	.global Func_02000e68
	.thumb_func
Func_02000e68:
	overlay_veneer Func_08108098
	.global Func_02000e70
	.thumb_func
Func_02000e70:
	overlay_veneer Func_081080a0
	.global Func_02000e78
	.thumb_func
Func_02000e78:
	overlay_veneer Sound_PlayFar
