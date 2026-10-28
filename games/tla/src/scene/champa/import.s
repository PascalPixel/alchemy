.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.balign 4
	.global WaitFrames
	.thumb_func
WaitFrames:
	overlay_veneer WaitFramesFar
	.global Task_Add
	.thumb_func
Task_Add:
	overlay_veneer Task_AddFar
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
	.global Func_020029dc
	.thumb_func
Func_020029dc:
	overlay_veneer Actor_SetMode
	.global Func_020029e4
	.thumb_func
Func_020029e4:
	overlay_veneer Actor_StartScriptFar
	.global Func_020029ec
	.thumb_func
Func_020029ec:
	overlay_veneer Func_080200c0
	.global Func_020029f4
	.thumb_func
Func_020029f4:
	overlay_veneer Func_08020198
	.global Func_020029fc
	.thumb_func
Func_020029fc:
	overlay_veneer Func_080201a0
	.global Func_02002a04
	.thumb_func
Func_02002a04:
	overlay_veneer Map_CopyCellAttributeRectFar
	.global Func_02002a0c
	.thumb_func
Func_02002a0c:
	overlay_veneer Func_080201f0
	.global Actor_SetSingleChildFlags
	.thumb_func
Actor_SetSingleChildFlags:
	overlay_veneer Actor_SetSingleChildFlagsFar
	.global Message_OpenWindow
	.thumb_func
Message_OpenWindow:
	overlay_veneer Message_OpenWindowFar
	.global Message_AllTyped
	.thumb_func
Message_AllTyped:
	overlay_veneer Message_AllTypedFar
	.global Func_02002a2c
	.thumb_func
Func_02002a2c:
	overlay_veneer Func_080380f8
	.global Window_CloseMatchingId
	.thumb_func
Window_CloseMatchingId:
	overlay_veneer Window_CloseMatchingIdFar
	.global FieldEvent_WaitFrames
	.thumb_func
FieldEvent_WaitFrames:
	overlay_veneer FieldEvent_WaitFramesFar
	.global Func_02002a44
	.thumb_func
Func_02002a44:
	overlay_veneer Event_StartFar
	.global Func_02002a4c
	.thumb_func
Func_02002a4c:
	overlay_veneer Event_EndFar
	.global Event_AskYesNoWithActorAnimation
	.thumb_func
Event_AskYesNoWithActorAnimation:
	overlay_veneer Event_AskYesNoWithActorAnimationFar
	.global Func_02002a5c
	.thumb_func
Func_02002a5c:
	overlay_veneer Func_080c8078
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
	.global Actor_SetPositionAndReset
	.thumb_func
Actor_SetPositionAndReset:
	overlay_veneer Actor_SetPositionAndResetFar
	.global Actor_CenterOnColumn
	.thumb_func
Actor_CenterOnColumn:
	overlay_veneer Actor_CenterOnColumnFar
	.global Actor_OffsetPositionAndResetMotion
	.thumb_func
Actor_OffsetPositionAndResetMotion:
	overlay_veneer Actor_OffsetPositionAndResetMotionFar
	.global Actor_WaitForMoveById
	.thumb_func
Actor_WaitForMoveById:
	overlay_veneer Actor_WaitForMoveByIdFar
	.global Func_02002a9c
	.thumb_func
Func_02002a9c:
	overlay_veneer Actor_SetHorizontalPositionWithTerrainFar
	.global Func_02002aa4
	.thumb_func
Func_02002aa4:
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
	.global Actor_StartRepeatedMotion
	.thumb_func
Actor_StartRepeatedMotion:
	overlay_veneer Actor_StartRepeatedMotionFar
	.global Motion_SetVarCbAndRefresh
	.thumb_func
Motion_SetVarCbAndRefresh:
	overlay_veneer Motion_SetVarCbAndRefreshFar
	.global Actor_TurnToFaceEachOther
	.thumb_func
Actor_TurnToFaceEachOther:
	overlay_veneer Actor_TurnToFaceEachOtherFar
	.global Func_02002adc
	.thumb_func
Func_02002adc:
	overlay_veneer Event_SetMessageIdFar
	.global FieldMessage_ShowAtActor
	.thumb_func
FieldMessage_ShowAtActor:
	overlay_veneer FieldMessage_ShowAtActorFar
	.global Func_02002aec
	.thumb_func
Func_02002aec:
	overlay_veneer Func_080c8198
	.global Func_02002af4
	.thumb_func
Func_02002af4:
	overlay_veneer Event_ShowMessageAndWaitFar
	.global Actor_ArmCallback
	.thumb_func
Actor_ArmCallback:
	overlay_veneer Actor_ArmCallbackFar
	.global Func_02002b04
	.thumb_func
Func_02002b04:
	overlay_veneer Actor_ArmCallbackAndRefreshFar
	.global Func_02002b0c
	.thumb_func
Func_02002b0c:
	overlay_veneer Battle_ShowPairedUnitWorkAndWaitFar
	.global Actor_SetSpritePriority
	.thumb_func
Actor_SetSpritePriority:
	overlay_veneer Actor_SetSpritePriorityFar
	.global Func_02002b1c
	.thumb_func
Func_02002b1c:
	overlay_veneer Func_080c8210
	.global Func_02002b24
	.thumb_func
Func_02002b24:
	overlay_veneer Func_080c8218
	.global Camera_FollowActor
	.thumb_func
Camera_FollowActor:
	overlay_veneer Camera_FollowActorFar
	.global Camera_MoveWithinMapBounds
	.thumb_func
Camera_MoveWithinMapBounds:
	overlay_veneer Camera_MoveWithinMapBoundsFar
	.global Func_02002b3c
	.thumb_func
Func_02002b3c:
	overlay_veneer Func_080c8240
	.global Func_02002b44
	.thumb_func
Func_02002b44:
	overlay_veneer Event_SetExitRequestFar
	.global Event_FinishScreenTransition
	.thumb_func
Event_FinishScreenTransition:
	overlay_veneer Event_FinishScreenTransitionFar
	.global Event_WaitTransitionFrames
	.thumb_func
Event_WaitTransitionFrames:
	overlay_veneer Event_WaitTransitionFramesFar
	.global Func_02002b5c
	.thumb_func
Func_02002b5c:
	overlay_veneer ObjectEvent_PlaceOnGroundFar
	.global Func_02002b64
	.thumb_func
Func_02002b64:
	overlay_veneer Event_ResolvePendingActionsFar
	.global BattleEffect_GetResourceId
	.thumb_func
BattleEffect_GetResourceId:
	overlay_veneer BattleEffect_GetResourceIdFar
	.global Func_02002b74
	.thumb_func
Func_02002b74:
	overlay_veneer Motion_LaunchFromFocusedObjectFar
	.global Actor_OffsetPositionAndReset
	.thumb_func
Actor_OffsetPositionAndReset:
	overlay_veneer Actor_OffsetPositionAndResetFar
	.global Actor_CommitPositionAndActivate
	.thumb_func
Actor_CommitPositionAndActivate:
	overlay_veneer Actor_CommitPositionAndActivateFar
	.global Actor_WatchAndSetScript
	.thumb_func
Actor_WatchAndSetScript:
	overlay_veneer Actor_WatchAndSetScriptFar
	.global ObjectTable_ReadActiveValue
	.thumb_func
ObjectTable_ReadActiveValue:
	overlay_veneer ObjectTable_ReadActiveValueFar
	.global Actor_SetActionCallback
	.thumb_func
Actor_SetActionCallback:
	overlay_veneer Actor_SetActionCallbackFar
	.global Func_02002ba4
	.thumb_func
Func_02002ba4:
	overlay_veneer Func_080c86d0
	.global Func_02002bac
	.thumb_func
Func_02002bac:
	overlay_veneer Func_080c86d8
	.global Func_02002bb4
	.thumb_func
Func_02002bb4:
	overlay_veneer Party_GetLeaderActorFar
	.global Func_02002bbc
	.thumb_func
Func_02002bbc:
	overlay_veneer Func_080c8860
	.global Func_02002bc4
	.thumb_func
Func_02002bc4:
	overlay_veneer Sound_PlayFar
