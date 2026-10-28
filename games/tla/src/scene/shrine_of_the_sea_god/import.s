.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.balign 4
	.global __divsi3
	.thumb_func
__divsi3:
	overlay_veneer IwramSignedDivide
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
	.global Random16Far
	.thumb_func
Random16Far:
	overlay_veneer Func_080000f8
	.global Math_Sine
	.thumb_func
Math_Sine:
	overlay_veneer Math_SineFar
	.global Math_Cosine
	.thumb_func
Math_Cosine:
	overlay_veneer Math_CosineFar
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
	.global Func_02003968
	.thumb_func
Func_02003968:
	overlay_veneer Actor_SetMode
	.global Func_02003970
	.thumb_func
Func_02003970:
	overlay_veneer Actor_StartScriptFar
	.global Func_02003978
	.thumb_func
Func_02003978:
	overlay_veneer Func_080200c0
	.global Actor_SetPositionAndResetMotion
	.thumb_func
Actor_SetPositionAndResetMotion:
	overlay_veneer Actor_SetPositionAndResetMotionFar
	.global Func_02003988
	.thumb_func
Func_02003988:
	overlay_veneer Map_CenterOnFollowedPointFar
	.global Func_02003990
	.thumb_func
Func_02003990:
	overlay_veneer Actor_SetTargetPosition
	.global Func_02003998
	.thumb_func
Func_02003998:
	overlay_veneer Map_CopyCellPictureIndicesRectFar
	.global Func_020039a0
	.thumb_func
Func_020039a0:
	overlay_veneer DisplayBlend_EnableRunScriptFar
	.global Func_020039a8
	.thumb_func
Func_020039a8:
	overlay_veneer Map_CopyCellAttributeRectFar
	.global Actor_SetSingleChildFlags
	.thumb_func
Actor_SetSingleChildFlags:
	overlay_veneer Actor_SetSingleChildFlagsFar
	.global Func_020039b8
	.thumb_func
Func_020039b8:
	overlay_veneer Map_SetScreenShakeFar
	.global Func_020039c0
	.thumb_func
Func_020039c0:
	overlay_veneer Func_08020230
	.global FieldMessage_Show
	.thumb_func
FieldMessage_Show:
	overlay_veneer FieldMessage_ShowFar
	.global FieldEvent_Wait
	.thumb_func
FieldEvent_Wait:
	overlay_veneer FieldEvent_WaitFar
	.global Func_020039d8
	.thumb_func
Func_020039d8:
	overlay_veneer Event_StartFar
	.global Func_020039e0
	.thumb_func
Func_020039e0:
	overlay_veneer Event_EndFar
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
	.global Actor_SetPositionAndReset
	.thumb_func
Actor_SetPositionAndReset:
	overlay_veneer Actor_SetPositionAndResetFar
	.global Actor_WaitForMoveById
	.thumb_func
Actor_WaitForMoveById:
	overlay_veneer Actor_WaitForMoveByIdFar
	.global Func_02003a30
	.thumb_func
Func_02003a30:
	overlay_veneer Actor_SetHorizontalPositionWithTerrainFar
	.global Func_02003a38
	.thumb_func
Func_02003a38:
	overlay_veneer Actor_SetHorizontalPositionAndAngleWithTerrainFar
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
	.global Motion_SetVarCbAndRefresh
	.thumb_func
Motion_SetVarCbAndRefresh:
	overlay_veneer Motion_SetVarCbAndRefreshFar
	.global Actor_SetAngleToward
	.thumb_func
Actor_SetAngleToward:
	overlay_veneer Actor_SetAngleTowardFar
	.global Actor_SetPartAttribute
	.thumb_func
Actor_SetPartAttribute:
	overlay_veneer Actor_SetPartAttributeFar
	.global Func_02003a70
	.thumb_func
Func_02003a70:
	overlay_veneer Event_SetMessageIdFar
	.global Func_02003a78
	.thumb_func
Func_02003a78:
	overlay_veneer Func_080c8198
	.global Func_02003a80
	.thumb_func
Func_02003a80:
	overlay_veneer Event_ShowMessageAndWaitFar
	.global Actor_ArmCallback
	.thumb_func
Actor_ArmCallback:
	overlay_veneer Actor_ArmCallbackFar
	.global Func_02003a90
	.thumb_func
Func_02003a90:
	overlay_veneer Actor_ArmCallbackAndRefreshFar
	.global Actor_SetSpritePriority
	.thumb_func
Actor_SetSpritePriority:
	overlay_veneer Actor_SetSpritePriorityFar
	.global Func_02003aa0
	.thumb_func
Func_02003aa0:
	overlay_veneer Func_080c8208
	.global Func_02003aa8
	.thumb_func
Func_02003aa8:
	overlay_veneer Func_080c8210
	.global Func_02003ab0
	.thumb_func
Func_02003ab0:
	overlay_veneer Func_080c8218
	.global Func_02003ab8
	.thumb_func
Func_02003ab8:
	overlay_veneer Camera_SetSpeedParametersFar
	.global Camera_MoveWithinMapBounds
	.thumb_func
Camera_MoveWithinMapBounds:
	overlay_veneer Camera_MoveWithinMapBoundsFar
	.global Func_02003ac8
	.thumb_func
Func_02003ac8:
	overlay_veneer Func_080c8240
	.global Func_02003ad0
	.thumb_func
Func_02003ad0:
	overlay_veneer Func_080c8258
	.global Func_02003ad8
	.thumb_func
Func_02003ad8:
	overlay_veneer Event_SetExitRequestFar
	.global Func_02003ae0
	.thumb_func
Func_02003ae0:
	overlay_veneer Func_080c8288
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
	.global Func_02003b00
	.thumb_func
Func_02003b00:
	overlay_veneer Func_080c8480
	.global Func_02003b08
	.thumb_func
Func_02003b08:
	overlay_veneer Event_ResolvePendingActionsFar
	.global Actor_WatchAndSetScript
	.thumb_func
Actor_WatchAndSetScript:
	overlay_veneer Actor_WatchAndSetScriptFar
	.global Func_02003b18
	.thumb_func
Func_02003b18:
	overlay_veneer Func_080c86a8
	.global Func_02003b20
	.thumb_func
Func_02003b20:
	overlay_veneer Func_080c86e8
	.global Func_02003b28
	.thumb_func
Func_02003b28:
	overlay_veneer Func_080c8710
	.global Func_02003b30
	.thumb_func
Func_02003b30:
	overlay_veneer Func_080c8718
	.global Func_02003b38
	.thumb_func
Func_02003b38:
	overlay_veneer Func_080c8720
	.global Func_02003b40
	.thumb_func
Func_02003b40:
	overlay_veneer Func_080c8728
	.global Func_02003b48
	.thumb_func
Func_02003b48:
	overlay_veneer Sound_PlayFar
