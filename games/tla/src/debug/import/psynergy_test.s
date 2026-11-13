.syntax unified
	.thumb
	.include "games/tla/src/system/overlay/imports.inc"
	.balign 4
	.global __umodsi3
	.thumb_func
__umodsi3:
	overlay_veneer IwramUnsignedRemainder
	.global wait
	.thumb_func
wait:
	overlay_veneer waitFar
	.global Trig_Sin
	.thumb_func
Trig_Sin:
	overlay_veneer Trig_SinFar
	.global VramCache_FreeEntry
	.thumb_func
VramCache_FreeEntry:
	overlay_veneer VramCache_FreeEntryFar
	.global VramCache_Load
	.thumb_func
VramCache_Load:
	overlay_veneer VramCache_LoadFar
	.global VramCache_FindFreeEntry
	.thumb_func
VramCache_FindFreeEntry:
	overlay_veneer VramCache_FindFreeEntryFar
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
	.global GameFlag_Clear
	.thumb_func
GameFlag_Clear:
	overlay_veneer GameFlag_ClearFar
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
	.global Actor_SetSpriteColorMode
	.thumb_func
Actor_SetSpriteColorMode:
	overlay_veneer Actor_SetSpriteColorModeFar
	.global Actor_SetItemIcon
	.thumb_func
Actor_SetItemIcon:
	overlay_veneer Actor_SetItemIconFar
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
	.global NameEntry_EditMemberName
	.thumb_func
NameEntry_EditMemberName:
	overlay_veneer NameEntry_EditMemberNameFar
	.global DebugBrowser_Icons
	.thumb_func
DebugBrowser_Icons:
	overlay_veneer DebugBrowser_IconsFar
	.global Unit_Calculate
	.thumb_func
Unit_Calculate:
	overlay_veneer Unit_CalculateFar
	.global Inventory_AddItem
	.thumb_func
Inventory_AddItem:
	overlay_veneer Inventory_AddItemFar
	.global Unit_AddHp
	.thumb_func
Unit_AddHp:
	overlay_veneer Unit_AddHpFar
	.global Unit_AddPp
	.thumb_func
Unit_AddPp:
	overlay_veneer Unit_AddPpFar
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
	.global Item_AdjustCounter
	.thumb_func
Item_AdjustCounter:
	overlay_veneer Item_AdjustCounterFar
	.global Event_Wait
	.thumb_func
Event_Wait:
	overlay_veneer Event_WaitFar
	.global Event_PickUpItem
	.thumb_func
Event_PickUpItem:
	overlay_veneer Event_PickUpItemFar
	.global Actor_GetById
	.thumb_func
Actor_GetById:
	overlay_veneer Actor_GetByIdFar
	.global Actor_WalkToAndWaitById
	.thumb_func
Actor_WalkToAndWaitById:
	overlay_veneer Actor_WalkToAndWaitByIdFar
	.global Actor_SetHorizontalPositionWithTerrain
	.thumb_func
Actor_SetHorizontalPositionWithTerrain:
	overlay_veneer Actor_SetHorizontalPositionWithTerrainFar
	.global Actor_SetModeById
	.thumb_func
Actor_SetModeById:
	overlay_veneer Actor_SetModeByIdFar
	.global Actor_Jump
	.thumb_func
Actor_Jump:
	overlay_veneer Actor_JumpFar
	.global Actor_FaceActorById
	.thumb_func
Actor_FaceActorById:
	overlay_veneer Actor_FaceActorByIdFar
	.global Event_SetMessageId
	.thumb_func
Event_SetMessageId:
	overlay_veneer Event_SetMessageIdFar
	.global Event_ShowMessageAndWait
	.thumb_func
Event_ShowMessageAndWait:
	overlay_veneer Event_ShowMessageAndWaitFar
	.global Actor_ArmCallback
	.thumb_func
Actor_ArmCallback:
	overlay_veneer Actor_ArmCallbackFar
	.global Actor_ShowEmoteById
	.thumb_func
Actor_ShowEmoteById:
	overlay_veneer Actor_ShowEmoteByIdFar
	.global Event_SetDestination
	.thumb_func
Event_SetDestination:
	overlay_veneer Event_SetDestinationFar
	.global Field_RequestBattleFromEncounterTable
	.thumb_func
Field_RequestBattleFromEncounterTable:
	overlay_veneer Field_RequestBattleFromEncounterTableFar
	.global Camera_WorldToScreen
	.thumb_func
Camera_WorldToScreen:
	overlay_veneer Camera_WorldToScreenFar
	.global TileCopies_CopyTouchedCells
	.thumb_func
TileCopies_CopyTouchedCells:
	overlay_veneer TileCopies_CopyTouchedCellsFar
	.global Actor_FallOverById
	.thumb_func
Actor_FallOverById:
	overlay_veneer Actor_FallOverByIdFar
	.global Actor_FallOverWithAnimationById
	.thumb_func
Actor_FallOverWithAnimationById:
	overlay_veneer Actor_FallOverWithAnimationByIdFar
	.global Func_02000ee4
	.thumb_func
Func_02000ee4:
	overlay_veneer EffectSprite_InitFar
	.global Func_02000eec
	.thumb_func
Func_02000eec:
	overlay_veneer EffectSprite_DrawFar
	.global BattleEffect_GetObjectPriority
	.thumb_func
BattleEffect_GetObjectPriority:
	overlay_veneer BattleEffect_GetObjectPriorityFar
	.global BattleEffect_GetObjectDepth
	.thumb_func
BattleEffect_GetObjectDepth:
	overlay_veneer BattleEffect_GetObjectDepthFar
	.global Actor_DepartInRing
	.thumb_func
Actor_DepartInRing:
	overlay_veneer Actor_DepartInRingFar
	.global Actor_ArriveInRing
	.thumb_func
Actor_ArriveInRing:
	overlay_veneer Actor_ArriveInRingFar
	.global Func_02000f14
	.thumb_func
Func_02000f14:
	overlay_veneer FieldItem_JumpToLeaderFar
	.global Func_02000f1c
	.thumb_func
Func_02000f1c:
	overlay_veneer Shop_RunFar
	.global Sanctum_Run
	.thumb_func
Sanctum_Run:
	overlay_veneer Sanctum_RunFar
	.global Inn_CheckIn
	.thumb_func
Inn_CheckIn:
	overlay_veneer Inn_CheckInFar
	.global ItemPicker_RunToDrop
	.thumb_func
ItemPicker_RunToDrop:
	overlay_veneer ItemPicker_RunToDropFar
	.global Sound_Play
	.thumb_func
Sound_Play:
	overlay_veneer Sound_PlayFar
