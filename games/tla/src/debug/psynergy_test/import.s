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
	.global Math_Sine
	.thumb_func
Math_Sine:
	overlay_veneer Math_SineFar
	.global Resource_ResetEntry
	.thumb_func
Resource_ResetEntry:
	overlay_veneer Resource_ResetEntryFar
	.global VramCache_Load
	.thumb_func
VramCache_Load:
	overlay_veneer VramCache_LoadFar
	.global Resource_FindFreeEntry
	.thumb_func
Resource_FindFreeEntry:
	overlay_veneer Resource_FindFreeEntryFar
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
	.global Func_02000da4
	.thumb_func
Func_02000da4:
	overlay_veneer Func_080200c0
	.global Func_02000dac
	.thumb_func
Func_02000dac:
	overlay_veneer Map_CopyCellsRectFar
	.global Actor_SetSingleChildFlags
	.thumb_func
Actor_SetSingleChildFlags:
	overlay_veneer Actor_SetSingleChildFlagsFar
	.global Animation_ApplyChildValues
	.thumb_func
Animation_ApplyChildValues:
	overlay_veneer Animation_ApplyChildValuesFar
	.global Func_02000dc4
	.thumb_func
Func_02000dc4:
	overlay_veneer Func_08020300
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
	overlay_veneer Func_080380f8
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
	overlay_veneer Func_080382f8
	.global Func_02000e0c
	.thumb_func
Func_02000e0c:
	overlay_veneer Func_08038318
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
	.global Item_AdjustCounter
	.thumb_func
Item_AdjustCounter:
	overlay_veneer Item_AdjustCounterFar
	.global FieldEvent_Wait
	.thumb_func
FieldEvent_Wait:
	overlay_veneer FieldEvent_WaitFar
	.global Func_02000e5c
	.thumb_func
Func_02000e5c:
	overlay_veneer Func_080c8078
	.global Actor_GetById
	.thumb_func
Actor_GetById:
	overlay_veneer Actor_GetByIdFar
	.global Actor_SetPositionAndReset
	.thumb_func
Actor_SetPositionAndReset:
	overlay_veneer Actor_SetPositionAndResetFar
	.global Func_02000e74
	.thumb_func
Func_02000e74:
	overlay_veneer Actor_SetHorizontalPositionWithTerrainFar
	.global Actor_SetModeById
	.thumb_func
Actor_SetModeById:
	overlay_veneer Actor_SetModeByIdFar
	.global Actor_Jump
	.thumb_func
Actor_Jump:
	overlay_veneer Actor_JumpFar
	.global Actor_SetAngleToward
	.thumb_func
Actor_SetAngleToward:
	overlay_veneer Actor_SetAngleTowardFar
	.global Func_02000e94
	.thumb_func
Func_02000e94:
	overlay_veneer Event_SetMessageIdFar
	.global Func_02000e9c
	.thumb_func
Func_02000e9c:
	overlay_veneer Event_ShowMessageAndWaitFar
	.global Actor_ArmCallback
	.thumb_func
Actor_ArmCallback:
	overlay_veneer Actor_ArmCallbackFar
	.global Func_02000eac
	.thumb_func
Func_02000eac:
	overlay_veneer Func_080c8210
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
	overlay_veneer Func_080c87e8
	.global Func_02000efc
	.thumb_func
Func_02000efc:
	overlay_veneer Func_080c87f0
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
	overlay_veneer Func_08108008
	.global Func_02000f24
	.thumb_func
Func_02000f24:
	overlay_veneer Func_08108010
	.global Func_02000f2c
	.thumb_func
Func_02000f2c:
	overlay_veneer Func_08108018
	.global Func_02000f34
	.thumb_func
Func_02000f34:
	overlay_veneer Func_08108068
	.global Func_02000f3c
	.thumb_func
Func_02000f3c:
	overlay_veneer Sound_PlayFar
