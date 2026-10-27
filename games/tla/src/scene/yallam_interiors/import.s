.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.balign 4
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
	.global ObjectDispatch_SetSingleChildFlags
	.thumb_func
ObjectDispatch_SetSingleChildFlags:
	overlay_veneer ObjectDispatch_SetSingleChildFlagsFar
	.global Func_02000980
	.thumb_func
Func_02000980:
	overlay_veneer UiText_DrawQuantity
	.global Func_02000988
	.thumb_func
Func_02000988:
	overlay_veneer Item_Get
	.global PartyInventory_FindOwner
	.thumb_func
PartyInventory_FindOwner:
	overlay_veneer PartyInventory_FindOwnerFar
	.global Party_AdjustCoins
	.thumb_func
Party_AdjustCoins:
	overlay_veneer Party_AdjustCoinsFar
	.global Func_020009a0
	.thumb_func
Func_020009a0:
	overlay_veneer PartyInventory_FindRoomFar
	.global FieldEvent_WaitTicks
	.thumb_func
FieldEvent_WaitTicks:
	overlay_veneer FieldEvent_WaitTicksFar
	.global Func_020009b0
	.thumb_func
Func_020009b0:
	overlay_veneer EventRuntime_StartFar
	.global Func_020009b8
	.thumb_func
Func_020009b8:
	overlay_veneer EventRuntime_EndFar
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
	.global ObjectMotion_SetSpeedParameters
	.thumb_func
ObjectMotion_SetSpeedParameters:
	overlay_veneer ObjectMotion_SetSpeedParametersFar
	.global ObjectMotion_ResetAndSetPosition
	.thumb_func
ObjectMotion_ResetAndSetPosition:
	overlay_veneer ObjectMotion_ResetAndSetPositionFar
	.global ObjectMotion_ResetAndSetPositionInMode2
	.thumb_func
ObjectMotion_ResetAndSetPositionInMode2:
	overlay_veneer ObjectMotion_ResetAndSetPositionInMode2Far
	.global ObjectMotion_CommitCurrentPositionAndActivate
	.thumb_func
ObjectMotion_CommitCurrentPositionAndActivate:
	overlay_veneer ObjectMotion_CommitCurrentPositionAndActivateFar
	.global Func_020009f8
	.thumb_func
Func_020009f8:
	overlay_veneer ObjectMotion_SetHorizontalPositionWithTerrainFar
	.global Object_SetModeById
	.thumb_func
Object_SetModeById:
	overlay_veneer Object_SetModeByIdFar
	.global ObjectMotion_SetVariantCallback
	.thumb_func
ObjectMotion_SetVariantCallback:
	overlay_veneer ObjectMotion_SetVariantCallbackFar
	.global ObjectMotion_SetAngleToward
	.thumb_func
ObjectMotion_SetAngleToward:
	overlay_veneer ObjectMotion_SetAngleTowardFar
	.global Func_02000a18
	.thumb_func
Func_02000a18:
	overlay_veneer EventRuntime_SetMessageFar
	.global UiText_OpenMessageAtObject
	.thumb_func
UiText_OpenMessageAtObject:
	overlay_veneer UiText_OpenMessageAtObjectFar
	.global Func_02000a28
	.thumb_func
Func_02000a28:
	overlay_veneer EventRuntime_ShowMessageAndWaitFar
	.global ObjectMotion_ArmCallback
	.thumb_func
ObjectMotion_ArmCallback:
	overlay_veneer ObjectMotion_ArmCallbackFar
	.global ObjectMotion_SetActionVariant
	.thumb_func
ObjectMotion_SetActionVariant:
	overlay_veneer ObjectMotion_SetActionVariantFar
	.global Func_02000a40
	.thumb_func
Func_02000a40:
	overlay_veneer Func_080c83e0
	.global Func_02000a48
	.thumb_func
Func_02000a48:
	overlay_veneer Func_080c8480
	.global Func_02000a50
	.thumb_func
Func_02000a50:
	overlay_veneer EventRuntime_ResolvePendingActionsFar
	.global Func_02000a58
	.thumb_func
Func_02000a58:
	overlay_veneer EventRuntime_GetControlledOwnerFar
	.global Func_02000a60
	.thumb_func
Func_02000a60:
	overlay_veneer Func_080c87b0
	.global Func_02000a68
	.thumb_func
Func_02000a68:
	overlay_veneer Func_08108008
	.global Func_02000a70
	.thumb_func
Func_02000a70:
	overlay_veneer Func_08108010
	.global Func_02000a78
	.thumb_func
Func_02000a78:
	overlay_veneer Func_08108018
	.global Func_02000a80
	.thumb_func
Func_02000a80:
	overlay_veneer Func_08108020
	.global Func_02000a88
	.thumb_func
Func_02000a88:
	overlay_veneer Func_08108098
	.global Func_02000a90
	.thumb_func
Func_02000a90:
	overlay_veneer Func_081080a0
