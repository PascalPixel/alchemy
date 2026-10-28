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
	.global Actor_SetSingleChildFlags
	.thumb_func
Actor_SetSingleChildFlags:
	overlay_veneer Actor_SetSingleChildFlagsFar
	.global Func_02000980
	.thumb_func
Func_02000980:
	overlay_veneer MessageArgument_PushFar
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
	.global FieldEvent_Wait
	.thumb_func
FieldEvent_Wait:
	overlay_veneer FieldEvent_WaitFar
	.global Func_020009b0
	.thumb_func
Func_020009b0:
	overlay_veneer Event_StartFar
	.global Func_020009b8
	.thumb_func
Func_020009b8:
	overlay_veneer Event_EndFar
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
	.global Actor_SetSpeedParameters
	.thumb_func
Actor_SetSpeedParameters:
	overlay_veneer Actor_SetSpeedParametersFar
	.global Actor_ResetAndSetPosition
	.thumb_func
Actor_ResetAndSetPosition:
	overlay_veneer Actor_ResetAndSetPositionFar
	.global Actor_ResetAndSetPositionInMode2
	.thumb_func
Actor_ResetAndSetPositionInMode2:
	overlay_veneer Actor_ResetAndSetPositionInMode2Far
	.global Actor_WaitForMoveById
	.thumb_func
Actor_WaitForMoveById:
	overlay_veneer Actor_WaitForMoveByIdFar
	.global Func_020009f8
	.thumb_func
Func_020009f8:
	overlay_veneer Actor_SetHorizontalPositionWithTerrainFar
	.global Actor_SetModeById
	.thumb_func
Actor_SetModeById:
	overlay_veneer Actor_SetModeByIdFar
	.global Actor_StartRepeatedMotion
	.thumb_func
Actor_StartRepeatedMotion:
	overlay_veneer Actor_StartRepeatedMotionFar
	.global Actor_SetAngleToward
	.thumb_func
Actor_SetAngleToward:
	overlay_veneer Actor_SetAngleTowardFar
	.global Func_02000a18
	.thumb_func
Func_02000a18:
	overlay_veneer Event_SetMessageIdFar
	.global FieldMessage_ShowAtActor
	.thumb_func
FieldMessage_ShowAtActor:
	overlay_veneer FieldMessage_ShowAtActorFar
	.global Func_02000a28
	.thumb_func
Func_02000a28:
	overlay_veneer Event_ShowMessageAndWaitFar
	.global Actor_ArmCallback
	.thumb_func
Actor_ArmCallback:
	overlay_veneer Actor_ArmCallbackFar
	.global Actor_SetSpritePriority
	.thumb_func
Actor_SetSpritePriority:
	overlay_veneer Actor_SetSpritePriorityFar
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
	overlay_veneer Event_ResolvePendingActionsFar
	.global Func_02000a58
	.thumb_func
Func_02000a58:
	overlay_veneer Party_GetLeaderActorFar
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
