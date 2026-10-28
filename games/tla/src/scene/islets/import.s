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
	.global Task_Add
	.thumb_func
Task_Add:
	overlay_veneer Task_AddFar
	.global Task_RemoveFar
	.thumb_func
Task_RemoveFar:
	overlay_veneer Func_080000d8
	.global Math_Sine
	.thumb_func
Math_Sine:
	overlay_veneer Math_SineFar
	.global Vector_AddPolarOffsetFar
	.thumb_func
Vector_AddPolarOffsetFar:
	overlay_veneer Func_08000128
	.global HeapSlot_AllocEwram
	.thumb_func
HeapSlot_AllocEwram:
	overlay_veneer HeapSlot_AllocEwramFar
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
	.global Func_02003de4
	.thumb_func
Func_02003de4:
	overlay_veneer Actor_SetMode
	.global Func_02003dec
	.thumb_func
Func_02003dec:
	overlay_veneer Actor_StartScriptFar
	.global Func_02003df4
	.thumb_func
Func_02003df4:
	overlay_veneer Func_080200c0
	.global Func_02003dfc
	.thumb_func
Func_02003dfc:
	overlay_veneer Actor_Destroy
	.global Actor_SetPositionAndResetMotion
	.thumb_func
Actor_SetPositionAndResetMotion:
	overlay_veneer Actor_SetPositionAndResetMotionFar
	.global Func_02003e0c
	.thumb_func
Func_02003e0c:
	overlay_veneer Actor_SetTargetPosition
	.global Func_02003e14
	.thumb_func
Func_02003e14:
	overlay_veneer Actor_CommitPosition
	.global Func_02003e1c
	.thumb_func
Func_02003e1c:
	overlay_veneer Map_PlayMapEditFar
	.global Func_02003e24
	.thumb_func
Func_02003e24:
	overlay_veneer Map_CopyCellAttributeRectFar
	.global Func_02003e2c
	.thumb_func
Func_02003e2c:
	overlay_veneer Func_08020210
	.global Actor_SetSingleChildFlags
	.thumb_func
Actor_SetSingleChildFlags:
	overlay_veneer Actor_SetSingleChildFlagsFar
	.global Func_02003e3c
	.thumb_func
Func_02003e3c:
	overlay_veneer Func_08020300
	.global FieldMessage_Show
	.thumb_func
FieldMessage_Show:
	overlay_veneer FieldMessage_ShowFar
	.global PartyInventory_FindOwner
	.thumb_func
PartyInventory_FindOwner:
	overlay_veneer PartyInventory_FindOwnerFar
	.global PartyInventory_Remove
	.thumb_func
PartyInventory_Remove:
	overlay_veneer PartyInventory_RemoveFar
	.global FieldEvent_Wait
	.thumb_func
FieldEvent_Wait:
	overlay_veneer FieldEvent_WaitFar
	.global Func_02003e64
	.thumb_func
Func_02003e64:
	overlay_veneer Event_StartFar
	.global Func_02003e6c
	.thumb_func
Func_02003e6c:
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
	.global Actor_SetMovementTypeById
	.thumb_func
Actor_SetMovementTypeById:
	overlay_veneer Actor_SetMovementTypeByIdFar
	.global Actor_Stop
	.thumb_func
Actor_Stop:
	overlay_veneer Actor_StopFar
	.global Actor_SetMovementTypeAndWaitById
	.thumb_func
Actor_SetMovementTypeAndWaitById:
	overlay_veneer Actor_SetMovementTypeAndWaitByIdFar
	.global Actor_ResetAndSetPosition
	.thumb_func
Actor_ResetAndSetPosition:
	overlay_veneer Actor_ResetAndSetPositionFar
	.global Actor_SetPositionAndCommit
	.thumb_func
Actor_SetPositionAndCommit:
	overlay_veneer Actor_SetPositionAndCommitFar
	.global Actor_ResetAndSetPositionInMode2
	.thumb_func
Actor_ResetAndSetPositionInMode2:
	overlay_veneer Actor_ResetAndSetPositionInMode2Far
	.global Actor_SetPositionAndReset
	.thumb_func
Actor_SetPositionAndReset:
	overlay_veneer Actor_SetPositionAndResetFar
	.global Actor_OffsetPositionAndResetMotion
	.thumb_func
Actor_OffsetPositionAndResetMotion:
	overlay_veneer Actor_OffsetPositionAndResetMotionFar
	.global Actor_WaitForMoveById
	.thumb_func
Actor_WaitForMoveById:
	overlay_veneer Actor_WaitForMoveByIdFar
	.global Func_02003edc
	.thumb_func
Func_02003edc:
	overlay_veneer Actor_SetHorizontalPositionWithTerrainFar
	.global Func_02003ee4
	.thumb_func
Func_02003ee4:
	overlay_veneer Actor_SetHorizontalPositionAndAngleWithTerrainFar
	.global Func_02003eec
	.thumb_func
Func_02003eec:
	overlay_veneer Actor_MoveToObjectFar
	.global Actor_SetModeById
	.thumb_func
Actor_SetModeById:
	overlay_veneer Actor_SetModeByIdFar
	.global Motion_SetModeAndWaitAnimation
	.thumb_func
Motion_SetModeAndWaitAnimation:
	overlay_veneer Motion_SetModeAndWaitAnimationFar
	.global Actor_Jump
	.thumb_func
Actor_Jump:
	overlay_veneer Actor_JumpFar
	.global Func_02003f0c
	.thumb_func
Func_02003f0c:
	overlay_veneer Actor_JumpDownLedgeFar
	.global Actor_SetAngleToward
	.thumb_func
Actor_SetAngleToward:
	overlay_veneer Actor_SetAngleTowardFar
	.global Actor_SetPartAttribute
	.thumb_func
Actor_SetPartAttribute:
	overlay_veneer Actor_SetPartAttributeFar
	.global Func_02003f24
	.thumb_func
Func_02003f24:
	overlay_veneer Event_SetMessageIdFar
	.global Func_02003f2c
	.thumb_func
Func_02003f2c:
	overlay_veneer Event_ShowMessageAndWaitFar
	.global Actor_ArmCallback
	.thumb_func
Actor_ArmCallback:
	overlay_veneer Actor_ArmCallbackFar
	.global Func_02003f3c
	.thumb_func
Func_02003f3c:
	overlay_veneer Actor_ArmCallbackAndRefreshFar
	.global Actor_SetSpritePriority
	.thumb_func
Actor_SetSpritePriority:
	overlay_veneer Actor_SetSpritePriorityFar
	.global Func_02003f4c
	.thumb_func
Func_02003f4c:
	overlay_veneer Func_080c8208
	.global Func_02003f54
	.thumb_func
Func_02003f54:
	overlay_veneer Func_080c8210
	.global Func_02003f5c
	.thumb_func
Func_02003f5c:
	overlay_veneer Func_080c8218
	.global Func_02003f64
	.thumb_func
Func_02003f64:
	overlay_veneer Camera_SetSpeedParametersFar
	.global Camera_MoveWithinMapBounds
	.thumb_func
Camera_MoveWithinMapBounds:
	overlay_veneer Camera_MoveWithinMapBoundsFar
	.global Func_02003f74
	.thumb_func
Func_02003f74:
	overlay_veneer Func_080c8240
	.global Func_02003f7c
	.thumb_func
Func_02003f7c:
	overlay_veneer Event_SetExitRequestFar
	.global Func_02003f84
	.thumb_func
Func_02003f84:
	overlay_veneer Func_080c82e0
	.global Event_StartScreenTransition
	.thumb_func
Event_StartScreenTransition:
	overlay_veneer Event_StartScreenTransitionFar
	.global Event_FinishScreenTransition
	.thumb_func
Event_FinishScreenTransition:
	overlay_veneer Event_FinishScreenTransitionFar
	.global Event_WaitTransitionFrames
	.thumb_func
Event_WaitTransitionFrames:
	overlay_veneer Event_WaitTransitionFramesFar
	.global Func_02003fa4
	.thumb_func
Func_02003fa4:
	overlay_veneer Event_ResolvePendingActionsFar
	.global Func_02003fac
	.thumb_func
Func_02003fac:
	overlay_veneer Func_080c84e8
	.global Func_02003fb4
	.thumb_func
Func_02003fb4:
	overlay_veneer Func_080c84f0
	.global Func_02003fbc
	.thumb_func
Func_02003fbc:
	overlay_veneer Func_080c84f8
	.global Func_02003fc4
	.thumb_func
Func_02003fc4:
	overlay_veneer Func_080c8500
	.global Actor_WatchAndSetScript
	.thumb_func
Actor_WatchAndSetScript:
	overlay_veneer Actor_WatchAndSetScriptFar
	.global Func_02003fd4
	.thumb_func
Func_02003fd4:
	overlay_veneer Event_NoOpFar
	.global Func_02003fdc
	.thumb_func
Func_02003fdc:
	overlay_veneer Func_080c8738
	.global Func_02003fe4
	.thumb_func
Func_02003fe4:
	overlay_veneer Func_080c8748
	.ifndef TLA_EDITION_JA
	.global Func_02003fec
	.thumb_func
Func_02003fec:
	overlay_veneer Func_080c8760
	.endif
	.global Func_02003ff4
	.thumb_func
Func_02003ff4:
	overlay_veneer Func_08108010
	.global Func_02003ffc
	.thumb_func
Func_02003ffc:
	overlay_veneer Sound_PlayFar
