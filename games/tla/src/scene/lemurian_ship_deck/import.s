.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.balign 4
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
	.global Vector_AddPolarOffsetFar
	.thumb_func
Vector_AddPolarOffsetFar:
	overlay_veneer Func_08000128
	.global HeapSlot_AllocFar
	.thumb_func
HeapSlot_AllocFar:
	overlay_veneer Func_08000140
	.global HeapSlot_Release
	.thumb_func
HeapSlot_Release:
	overlay_veneer HeapSlot_ReleaseFar
	.global VramCache_Load
	.thumb_func
VramCache_Load:
	overlay_veneer VramCache_LoadFar
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
	.global Func_02001efc
	.thumb_func
Func_02001efc:
	overlay_veneer Actor_SetMode
	.global Func_02001f04
	.thumb_func
Func_02001f04:
	overlay_veneer Actor_StartScriptFar
	.global Func_02001f0c
	.thumb_func
Func_02001f0c:
	overlay_veneer Func_080200c0
	.global Func_02001f14
	.thumb_func
Func_02001f14:
	overlay_veneer Actor_Destroy
	.global Func_02001f1c
	.thumb_func
Func_02001f1c:
	overlay_veneer Map_CenterOnFollowedPointFar
	.global Func_02001f24
	.thumb_func
Func_02001f24:
	overlay_veneer Actor_SetTargetPosition
	.global Func_02001f2c
	.thumb_func
Func_02001f2c:
	overlay_veneer Map_CopyCellPictureIndicesRectFar
	.global Func_02001f34
	.thumb_func
Func_02001f34:
	overlay_veneer Func_08020198
	.global Actor_SetSingleChildFlags
	.thumb_func
Actor_SetSingleChildFlags:
	overlay_veneer Actor_SetSingleChildFlagsFar
	.global FieldMessage_Show
	.thumb_func
FieldMessage_Show:
	overlay_veneer FieldMessage_ShowFar
	.global Func_02001f4c
	.thumb_func
Func_02001f4c:
	overlay_veneer Func_08038248
	.global FieldEvent_Wait
	.thumb_func
FieldEvent_Wait:
	overlay_veneer FieldEvent_WaitFar
	.global Func_02001f5c
	.thumb_func
Func_02001f5c:
	overlay_veneer Event_StartFar
	.global Func_02001f64
	.thumb_func
Func_02001f64:
	overlay_veneer Event_EndFar
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
	.global Actor_ResetAndSetPositionInMode2
	.thumb_func
Actor_ResetAndSetPositionInMode2:
	overlay_veneer Actor_ResetAndSetPositionInMode2Far
	.global Actor_SetPositionAndReset
	.thumb_func
Actor_SetPositionAndReset:
	overlay_veneer Actor_SetPositionAndResetFar
	.global Actor_CenterOnColumn
	.thumb_func
Actor_CenterOnColumn:
	overlay_veneer Actor_CenterOnColumnFar
	.global Actor_WaitForMoveById
	.thumb_func
Actor_WaitForMoveById:
	overlay_veneer Actor_WaitForMoveByIdFar
	.global Func_02001fc4
	.thumb_func
Func_02001fc4:
	overlay_veneer Actor_SetHorizontalPositionWithTerrainFar
	.global Func_02001fcc
	.thumb_func
Func_02001fcc:
	overlay_veneer Actor_SetHorizontalPositionAndAngleWithTerrainFar
	.global Func_02001fd4
	.thumb_func
Func_02001fd4:
	overlay_veneer Actor_MoveToObjectFar
	.global Actor_SetModeById
	.thumb_func
Actor_SetModeById:
	overlay_veneer Actor_SetModeByIdFar
	.global Motion_SetModeAndWaitAnimation
	.thumb_func
Motion_SetModeAndWaitAnimation:
	overlay_veneer Motion_SetModeAndWaitAnimationFar
	.global Actor_WaitForAnimationChange
	.thumb_func
Actor_WaitForAnimationChange:
	overlay_veneer Actor_WaitForAnimationChangeFar
	.global Actor_Jump
	.thumb_func
Actor_Jump:
	overlay_veneer Actor_JumpFar
	.global Actor_StartRepeatedMotion
	.thumb_func
Actor_StartRepeatedMotion:
	overlay_veneer Actor_StartRepeatedMotionFar
	.global Motion_SetVarCbAndRefresh
	.thumb_func
Motion_SetVarCbAndRefresh:
	overlay_veneer Motion_SetVarCbAndRefreshFar
	.global Actor_SetAngleToward
	.thumb_func
Actor_SetAngleToward:
	overlay_veneer Actor_SetAngleTowardFar
	.global Actor_TurnToFaceEachOther
	.thumb_func
Actor_TurnToFaceEachOther:
	overlay_veneer Actor_TurnToFaceEachOtherFar
	.global Func_0200201c
	.thumb_func
Func_0200201c:
	overlay_veneer Event_SetMessageIdFar
	.global FieldMessage_ShowAtActor
	.thumb_func
FieldMessage_ShowAtActor:
	overlay_veneer FieldMessage_ShowAtActorFar
	.global Func_0200202c
	.thumb_func
Func_0200202c:
	overlay_veneer Func_080c8198
	.global Func_02002034
	.thumb_func
Func_02002034:
	overlay_veneer Event_ShowMessageAndWaitFar
	.global Actor_ArmCallback
	.thumb_func
Actor_ArmCallback:
	overlay_veneer Actor_ArmCallbackFar
	.global Func_02002044
	.thumb_func
Func_02002044:
	overlay_veneer Actor_ArmCallbackAndRefreshFar
	.global Func_0200204c
	.thumb_func
Func_0200204c:
	overlay_veneer Func_080c8210
	.global Func_02002054
	.thumb_func
Func_02002054:
	overlay_veneer Func_080c8218
	.global Func_0200205c
	.thumb_func
Func_0200205c:
	overlay_veneer Camera_SetSpeedParametersFar
	.global Camera_MoveWithinMapBounds
	.thumb_func
Camera_MoveWithinMapBounds:
	overlay_veneer Camera_MoveWithinMapBoundsFar
	.global Func_0200206c
	.thumb_func
Func_0200206c:
	overlay_veneer Func_080c8240
	.global Func_02002074
	.thumb_func
Func_02002074:
	overlay_veneer Func_080c8258
	.global Func_0200207c
	.thumb_func
Func_0200207c:
	overlay_veneer Event_SetExitRequestFar
	.global Func_02002084
	.thumb_func
Func_02002084:
	overlay_veneer BattleEffect_ApplyColorToTargetBufferFar
	.global Func_0200208c
	.thumb_func
Func_0200208c:
	overlay_veneer BattleEffect_StartBufferInterpolationFar
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
	.global Func_020020ac
	.thumb_func
Func_020020ac:
	overlay_veneer Event_ResolvePendingActionsFar
	.global Func_020020b4
	.thumb_func
Func_020020b4:
	overlay_veneer Motion_LaunchFromFocusedObjectFar
	.global Actor_CommitPositionAndActivate
	.thumb_func
Actor_CommitPositionAndActivate:
	overlay_veneer Actor_CommitPositionAndActivateFar
	.global Func_020020c4
	.thumb_func
Func_020020c4:
	overlay_veneer Func_080c86f8
	.global Func_020020cc
	.thumb_func
Func_020020cc:
	overlay_veneer Func_080c8708
	.global Func_020020d4
	.thumb_func
Func_020020d4:
	overlay_veneer Sound_PlayFar
