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
	.global Vector_AddPolarOffsetFar
	.thumb_func
Vector_AddPolarOffsetFar:
	overlay_veneer Func_08000128
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
	.global Func_02002bf0
	.thumb_func
Func_02002bf0:
	overlay_veneer Actor_SetMode
	.global Func_02002bf8
	.thumb_func
Func_02002bf8:
	overlay_veneer Actor_StartScriptFar
	.global Func_02002c00
	.thumb_func
Func_02002c00:
	overlay_veneer Func_080200c0
	.global Actor_SetPositionAndResetMotion
	.thumb_func
Actor_SetPositionAndResetMotion:
	overlay_veneer Actor_SetPositionAndResetMotionFar
	.global Func_02002c10
	.thumb_func
Func_02002c10:
	overlay_veneer Map_CenterOnFollowedPointFar
	.global Func_02002c18
	.thumb_func
Func_02002c18:
	overlay_veneer Actor_SetTargetPosition
	.global Func_02002c20
	.thumb_func
Func_02002c20:
	overlay_veneer Actor_CommitPosition
	.global Func_02002c28
	.thumb_func
Func_02002c28:
	overlay_veneer Map_PlayMapEditFar
	.global Func_02002c30
	.thumb_func
Func_02002c30:
	overlay_veneer Map_CopyCellPictureIndicesRectFar
	.global Func_02002c38
	.thumb_func
Func_02002c38:
	overlay_veneer Map_CopyCellAttributeRectFar
	.global Actor_SetSingleChildFlags
	.thumb_func
Actor_SetSingleChildFlags:
	overlay_veneer Actor_SetSingleChildFlagsFar
	.global Func_02002c48
	.thumb_func
Func_02002c48:
	overlay_veneer Map_SetScreenShakeFar
	.global FieldMessage_Show
	.thumb_func
FieldMessage_Show:
	overlay_veneer FieldMessage_ShowFar
	.global PartyInventory_Remove
	.thumb_func
PartyInventory_Remove:
	overlay_veneer PartyInventory_RemoveFar
	.global FieldEvent_Wait
	.thumb_func
FieldEvent_Wait:
	overlay_veneer FieldEvent_WaitFar
	.global Func_02002c68
	.thumb_func
Func_02002c68:
	overlay_veneer Event_StartFar
	.global Func_02002c70
	.thumb_func
Func_02002c70:
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
	.global Func_02002cd0
	.thumb_func
Func_02002cd0:
	overlay_veneer Actor_SetHorizontalPositionWithTerrainFar
	.global Func_02002cd8
	.thumb_func
Func_02002cd8:
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
	.global Actor_SetPartAttribute
	.thumb_func
Actor_SetPartAttribute:
	overlay_veneer Actor_SetPartAttributeFar
	.global Func_02002d18
	.thumb_func
Func_02002d18:
	overlay_veneer Event_SetMessageIdFar
	.global Func_02002d20
	.thumb_func
Func_02002d20:
	overlay_veneer Event_ShowMessageAndWaitFar
	.global Actor_ArmCallback
	.thumb_func
Actor_ArmCallback:
	overlay_veneer Actor_ArmCallbackFar
	.global Func_02002d30
	.thumb_func
Func_02002d30:
	overlay_veneer Actor_ArmCallbackAndRefreshFar
	.global Actor_SetSpritePriority
	.thumb_func
Actor_SetSpritePriority:
	overlay_veneer Actor_SetSpritePriorityFar
	.global Func_02002d40
	.thumb_func
Func_02002d40:
	overlay_veneer Func_080c8210
	.global Func_02002d48
	.thumb_func
Func_02002d48:
	overlay_veneer Func_080c8218
	.global Camera_FollowActor
	.thumb_func
Camera_FollowActor:
	overlay_veneer Camera_FollowActorFar
	.global Func_02002d58
	.thumb_func
Func_02002d58:
	overlay_veneer Camera_SetSpeedParametersFar
	.global Camera_MoveWithinMapBounds
	.thumb_func
Camera_MoveWithinMapBounds:
	overlay_veneer Camera_MoveWithinMapBoundsFar
	.global Func_02002d68
	.thumb_func
Func_02002d68:
	overlay_veneer Func_080c8260
	.global Func_02002d70
	.thumb_func
Func_02002d70:
	overlay_veneer Event_SetExitRequestFar
	.global Func_02002d78
	.thumb_func
Func_02002d78:
	overlay_veneer Field_RequestBattleFromEncounterTableFar
	.global Party_SetFields1eeAnd1f0
	.thumb_func
Party_SetFields1eeAnd1f0:
	overlay_veneer Party_SetFields1eeAnd1f0Far
	.global Func_02002d88
	.thumb_func
Func_02002d88:
	overlay_veneer BattleEffect_ApplyColorToTargetBufferFar
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
	.global Func_02002da8
	.thumb_func
Func_02002da8:
	overlay_veneer Event_ResolvePendingActionsFar
	.global Actor_WatchAndSetScript
	.thumb_func
Actor_WatchAndSetScript:
	overlay_veneer Actor_WatchAndSetScriptFar
	.global Func_02002db8
	.thumb_func
Func_02002db8:
	overlay_veneer Func_080c86f8
	.global Func_02002dc0
	.thumb_func
Func_02002dc0:
	overlay_veneer Func_080c8708
	.global Func_02002dc8
	.thumb_func
Func_02002dc8:
	overlay_veneer Func_080c8718
	.global Func_02002dd0
	.thumb_func
Func_02002dd0:
	overlay_veneer Func_080c8720
	.global Func_02002dd8
	.thumb_func
Func_02002dd8:
	overlay_veneer Func_080c8728
	.global Func_02002de0
	.thumb_func
Func_02002de0:
	overlay_veneer Sound_PlayFar
