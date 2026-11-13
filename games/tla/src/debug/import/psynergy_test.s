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
	.global GameFlag_Test
	.thumb_func
GameFlag_Test:
	overlay_veneer GameFlag_TestFar
	.global GameFlag_SetBit
	.thumb_func
GameFlag_SetBit:
	overlay_veneer GameFlag_SetBitFar
	.global GameFlag_ClearBit
	.thumb_func
GameFlag_ClearBit:
	overlay_veneer GameFlag_ClearBitFar
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
	.global Func_02000dec
	.thumb_func
Func_02000dec:
	overlay_veneer Portrait_OpenWindowFar
	.global Func_02000df4
	.thumb_func
Func_02000df4:
	overlay_veneer MessageArgument_PushFar
	.global Message_CloseFinishedWindows
	.thumb_func
Message_CloseFinishedWindows:
	overlay_veneer Message_CloseFinishedWindowsFar
	.global Func_02000e04
	.thumb_func
Func_02000e04:
	overlay_veneer NameEntry_EditMemberNameFar
	.global Func_02000e0c
	.thumb_func
Func_02000e0c:
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
	.global Func_02000e5c
	.thumb_func
Func_02000e5c:
	overlay_veneer Func_080c8078
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
	.global Func_02000eb4
	.thumb_func
Func_02000eb4:
	overlay_veneer Event_SetDestinationFar
	.global Func_02000ebc
	.thumb_func
Func_02000ebc:
	overlay_veneer Field_RequestBattleFromEncounterTableFar
	.global Func_02000ec4
	.thumb_func
Func_02000ec4:
	overlay_veneer Camera_WorldToScreenFar
	.global Func_02000ecc
	.thumb_func
Func_02000ecc:
	overlay_veneer Func_080c8760
	.global Func_02000ed4
	.thumb_func
Func_02000ed4:
	overlay_veneer Func_080c87b0
	.global Func_02000edc
	.thumb_func
Func_02000edc:
	overlay_veneer Func_080c87b8
	.global Func_02000ee4
	.thumb_func
Func_02000ee4:
	overlay_veneer Func_080c87c0
	.global Func_02000eec
	.thumb_func
Func_02000eec:
	overlay_veneer Func_080c87c8
	.global Func_02000ef4
	.thumb_func
Func_02000ef4:
	overlay_veneer BattleEffect_GetObjectPriorityFar
	.global Func_02000efc
	.thumb_func
Func_02000efc:
	overlay_veneer BattleEffect_GetObjectDepthFar
	.global Func_02000f04
	.thumb_func
Func_02000f04:
	overlay_veneer Func_080c8880
	.global Func_02000f0c
	.thumb_func
Func_02000f0c:
	overlay_veneer Func_080c8888
	.global Func_02000f14
	.thumb_func
Func_02000f14:
	overlay_veneer Func_080c8948
	.global Func_02000f1c
	.thumb_func
Func_02000f1c:
	overlay_veneer Shop_RunFar
	.global Func_02000f24
	.thumb_func
Func_02000f24:
	overlay_veneer Sanctum_RunFar
	.global Func_02000f2c
	.thumb_func
Func_02000f2c:
	overlay_veneer Inn_CheckInFar
	.global Func_02000f34
	.thumb_func
Func_02000f34:
	overlay_veneer ItemPicker_RunToDropFar
	.global Sound_Play
	.thumb_func
Sound_Play:
	overlay_veneer Sound_PlayFar
