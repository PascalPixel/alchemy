.syntax unified
	.thumb
	.include "games/tla/src/system/overlay/imports.inc"
	.balign 4
	.global __modsi3
	.thumb_func
__modsi3:
	overlay_veneer IwramSignedRemainder
	.global wait
	.thumb_func
wait:
	overlay_veneer waitFar
	.global Unit_Get
	.thumb_func
Unit_Get:
	overlay_veneer Unit_GetFar
	.global GameFlag_Get
	.thumb_func
GameFlag_Get:
	overlay_veneer GameFlag_GetFar
	.global GameFlag_Set
	.thumb_func
GameFlag_Set:
	overlay_veneer GameFlag_SetFar
	.global Import_ActorCreate
	.thumb_func
Import_ActorCreate:
	overlay_veneer Actor_CreateFar
	.global Map_CopyCellsRect
	.thumb_func
Map_CopyCellsRect:
	overlay_veneer Map_CopyCellsRectFar
	.global Actor_SetSpriteFlags
	.thumb_func
Actor_SetSpriteFlags:
	overlay_veneer Actor_SetSpriteFlagsFar
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
	.global Text_DrawMessageAsTiles
	.thumb_func
Text_DrawMessageAsTiles:
	overlay_veneer Text_DrawMessageAsTilesFar
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
	.global Portrait_OpenWindow
	.thumb_func
Portrait_OpenWindow:
	overlay_veneer Portrait_OpenWindowFar
	.global MessageArgument_Push
	.thumb_func
MessageArgument_Push:
	overlay_veneer MessageArgument_PushFar
	.global Message_CloseFinishedWindows
	.thumb_func
Message_CloseFinishedWindows:
	overlay_veneer Message_CloseFinishedWindowsFar
	.global WindowSprite_RedrawSavedRect
	.thumb_func
WindowSprite_RedrawSavedRect:
	overlay_veneer WindowSprite_RedrawSavedRectFar
	.global NameEntry_EditMemberName
	.thumb_func
NameEntry_EditMemberName:
	overlay_veneer NameEntry_EditMemberNameFar
	.global DebugBrowser_Icons
	.thumb_func
DebugBrowser_Icons:
	overlay_veneer DebugBrowser_IconsFar
	.global DebugBrowser_Portraits
	.thumb_func
DebugBrowser_Portraits:
	overlay_veneer DebugBrowser_PortraitsFar
	.global Summon_ShowLearnedMessage
	.thumb_func
Summon_ShowLearnedMessage:
	overlay_veneer Summon_ShowLearnedMessageFar
	.global Unit_Calculate
	.thumb_func
Unit_Calculate:
	overlay_veneer Unit_CalculateFar
	.global Func_02000d48
	.thumb_func
Func_02000d48:
	overlay_veneer Item_GetDirectFar
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
	.global Party_ListMembers
	.thumb_func
Party_ListMembers:
	overlay_veneer Party_ListMembersFar
	.global Party_RemoveMember
	.thumb_func
Party_RemoveMember:
	overlay_veneer Party_RemoveMemberFar
	.global Djinn_Give
	.thumb_func
Djinn_Give:
	overlay_veneer Djinn_GiveFar
	.global Djinn_Set
	.thumb_func
Djinn_Set:
	overlay_veneer Djinn_SetFar
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
	.global Party_AddIsaacsParty
	.thumb_func
Party_AddIsaacsParty:
	overlay_veneer Party_AddIsaacsPartyFar
	.ifndef TLA_EDITION_JA
	.global Djinn_GiveToParty
	.thumb_func
Djinn_GiveToParty:
	overlay_veneer Djinn_GiveToPartyFar
	.endif
	.global GameFlag_RefreshLureCap
	.thumb_func
GameFlag_RefreshLureCap:
	overlay_veneer GameFlag_RefreshLureCapFar
	.global PartyInventory_FindRoom
	.thumb_func
PartyInventory_FindRoom:
	overlay_veneer PartyInventory_FindRoomFar
	.global Class_GetSeries
	.thumb_func
Class_GetSeries:
	overlay_veneer Class_GetSeriesFar
	.global Event_Wait
	.thumb_func
Event_Wait:
	overlay_veneer Event_WaitFar
	.global PartyInventory_GiveItem
	.thumb_func
PartyInventory_GiveItem:
	overlay_veneer PartyInventory_GiveItemFar
	.global Event_AskYesNoWithActorAnimation
	.thumb_func
Event_AskYesNoWithActorAnimation:
	overlay_veneer Event_AskYesNoWithActorAnimationFar
	.global Actor_GetById
	.thumb_func
Actor_GetById:
	overlay_veneer Actor_GetByIdFar
	.global Actor_SetHorizontalPositionWithTerrain
	.thumb_func
Actor_SetHorizontalPositionWithTerrain:
	overlay_veneer Actor_SetHorizontalPositionWithTerrainFar
	.global Actor_SetModeById
	.thumb_func
Actor_SetModeById:
	overlay_veneer Actor_SetModeByIdFar
	.global Event_SetMessageId
	.thumb_func
Event_SetMessageId:
	overlay_veneer Event_SetMessageIdFar
	.global FieldMessage_ShowAtActor
	.thumb_func
FieldMessage_ShowAtActor:
	overlay_veneer FieldMessage_ShowAtActorFar
	.global Event_ShowMessageAndWait
	.thumb_func
Event_ShowMessageAndWait:
	overlay_veneer Event_ShowMessageAndWaitFar
	.global Field_RequestBattleFromEncounterTable
	.thumb_func
Field_RequestBattleFromEncounterTable:
	overlay_veneer Field_RequestBattleFromEncounterTableFar
	.global ItemDisplay_ShowItem
	.thumb_func
ItemDisplay_ShowItem:
	overlay_veneer ItemDisplay_ShowItemFar
	.global Party_GetLeaderActor
	.thumb_func
Party_GetLeaderActor:
	overlay_veneer Party_GetLeaderActorFar
	.global ItemMenu_DrawItemDetails
	.thumb_func
ItemMenu_DrawItemDetails:
	overlay_veneer ItemMenu_DrawItemDetailsFar
	.global Shop_Run
	.thumb_func
Shop_Run:
	overlay_veneer Shop_RunFar
	.global Sanctum_Run
	.thumb_func
Sanctum_Run:
	overlay_veneer Sanctum_RunFar
	.global Inn_CheckIn
	.thumb_func
Inn_CheckIn:
	overlay_veneer Inn_CheckInFar
	.global Forge_Run
	.thumb_func
Forge_Run:
	overlay_veneer Forge_RunFar
	.global ItemPicker_RunMode1
	.thumb_func
ItemPicker_RunMode1:
	overlay_veneer ItemPicker_RunMode1Far
	.global Forge_SetItem
	.thumb_func
Forge_SetItem:
	overlay_veneer Forge_SetItemFar
	.global Forge_GetItem
	.thumb_func
Forge_GetItem:
	overlay_veneer Forge_GetItemFar
	.global Sound_Play
	.thumb_func
Sound_Play:
	overlay_veneer Sound_PlayFar
