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
	.global Func_02001bc4
	.thumb_func
Func_02001bc4:
	overlay_veneer Map_PlayMapEditFar
	.global Func_02001bcc
	.thumb_func
Func_02001bcc:
	overlay_veneer Map_CopyCellAttributeRectFar
	.global PartyInventory_Remove
	.thumb_func
PartyInventory_Remove:
	overlay_veneer PartyInventory_RemoveFar
	.global FieldEvent_Wait
	.thumb_func
FieldEvent_Wait:
	overlay_veneer FieldEvent_WaitFar
	.global Func_02001be4
	.thumb_func
Func_02001be4:
	overlay_veneer Event_StartFar
	.global Func_02001bec
	.thumb_func
Func_02001bec:
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
	.global Actor_RefreshSelectorById
	.thumb_func
Actor_RefreshSelectorById:
	overlay_veneer Actor_RefreshSelectorByIdFar
	.global Actor_Stop
	.thumb_func
Actor_Stop:
	overlay_veneer Actor_StopFar
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
	.global Func_02001c3c
	.thumb_func
Func_02001c3c:
	overlay_veneer Actor_SetHorizontalPositionWithTerrainFar
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
	.global Func_02001c74
	.thumb_func
Func_02001c74:
	overlay_veneer Event_SetMessageIdFar
	.global FieldMessage_ShowAtActor
	.thumb_func
FieldMessage_ShowAtActor:
	overlay_veneer FieldMessage_ShowAtActorFar
	.global Func_02001c84
	.thumb_func
Func_02001c84:
	overlay_veneer Func_080c8198
	.global Func_02001c8c
	.thumb_func
Func_02001c8c:
	overlay_veneer Event_ShowMessageAndWaitFar
	.global Actor_ArmCallback
	.thumb_func
Actor_ArmCallback:
	overlay_veneer Actor_ArmCallbackFar
	.global Actor_SetSpritePriority
	.thumb_func
Actor_SetSpritePriority:
	overlay_veneer Actor_SetSpritePriorityFar
	.global Func_02001ca4
	.thumb_func
Func_02001ca4:
	overlay_veneer Func_080c8210
	.global Func_02001cac
	.thumb_func
Func_02001cac:
	overlay_veneer Func_080c8218
	.global Camera_FollowActor
	.thumb_func
Camera_FollowActor:
	overlay_veneer Camera_FollowActorFar
	.global Func_02001cbc
	.thumb_func
Func_02001cbc:
	overlay_veneer Camera_SetSpeedParametersFar
	.global Camera_MoveWithinMapBounds
	.thumb_func
Camera_MoveWithinMapBounds:
	overlay_veneer Camera_MoveWithinMapBoundsFar
	.global Func_02001ccc
	.thumb_func
Func_02001ccc:
	overlay_veneer Func_080c8240
	.global Func_02001cd4
	.thumb_func
Func_02001cd4:
	overlay_veneer Event_SetExitRequestFar
	.global Event_FinishScreenTransition
	.thumb_func
Event_FinishScreenTransition:
	overlay_veneer Event_FinishScreenTransitionFar
	.global Event_WaitTransitionFrames
	.thumb_func
Event_WaitTransitionFrames:
	overlay_veneer Event_WaitTransitionFramesFar
	.global Func_02001cec
	.thumb_func
Func_02001cec:
	overlay_veneer Func_080c83e0
	.global Func_02001cf4
	.thumb_func
Func_02001cf4:
	overlay_veneer ObjectEvent_PlaceOnGroundFar
	.global Func_02001cfc
	.thumb_func
Func_02001cfc:
	overlay_veneer Event_ResolvePendingActionsFar
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
	.global Actor_SetActionCallback
	.thumb_func
Actor_SetActionCallback:
	overlay_veneer Actor_SetActionCallbackFar
	.global Func_02001d24
	.thumb_func
Func_02001d24:
	overlay_veneer Func_080c8700
	.global Func_02001d2c
	.thumb_func
Func_02001d2c:
	overlay_veneer Func_080c8708
	.global Func_02001d34
	.thumb_func
Func_02001d34:
	overlay_veneer Party_GetLeaderActorFar
	.global Func_02001d3c
	.thumb_func
Func_02001d3c:
	overlay_veneer Func_080c8878
	.global Func_02001d44
	.thumb_func
Func_02001d44:
	overlay_veneer Func_08108008
	.global Func_02001d4c
	.thumb_func
Func_02001d4c:
	overlay_veneer Sound_PlayFar
