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
	.global ArcTan2
	.thumb_func
ArcTan2:
	overlay_veneer ArcTan2Far
	.global Math_Sine
	.thumb_func
Math_Sine:
	overlay_veneer Math_SineFar
	.global GameFlag_Test
	.thumb_func
GameFlag_Test:
	overlay_veneer GameFlag_TestFar
	.global GameFlag_SetBit
	.thumb_func
GameFlag_SetBit:
	overlay_veneer GameFlag_SetBitFar
	.global Func_02001b7c
	.thumb_func
Func_02001b7c:
	overlay_veneer Actor_SetMode
	.global Func_02001b84
	.thumb_func
Func_02001b84:
	overlay_veneer Map_PlayMapEditFar
	.global Func_02001b8c
	.thumb_func
Func_02001b8c:
	overlay_veneer Func_080201f0
	.global Actor_SetSingleChildFlags
	.thumb_func
Actor_SetSingleChildFlags:
	overlay_veneer Actor_SetSingleChildFlagsFar
	.global Animation_SetIndexAndInitializeObjects
	.thumb_func
Animation_SetIndexAndInitializeObjects:
	overlay_veneer Animation_SetIndexAndInitializeObjectsFar
	.global Func_02001ba4
	.thumb_func
Func_02001ba4:
	overlay_veneer Actor_SetSpritePriorityDirectFar
	.global FieldMessage_Show
	.thumb_func
FieldMessage_Show:
	overlay_veneer FieldMessage_ShowFar
	.global Func_02001bb4
	.thumb_func
Func_02001bb4:
	overlay_veneer Func_080ad208
	.global Func_02001bbc
	.thumb_func
Func_02001bbc:
	overlay_veneer Party_FindOwnerWithActionFar
	.global FieldEvent_Wait
	.thumb_func
FieldEvent_Wait:
	overlay_veneer FieldEvent_WaitFar
	.global Func_02001bcc
	.thumb_func
Func_02001bcc:
	overlay_veneer Event_StartFar
	.global Func_02001bd4
	.thumb_func
Func_02001bd4:
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
	.global Func_02001c2c
	.thumb_func
Func_02001c2c:
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
	.global Actor_TurnToFaceEachOther
	.thumb_func
Actor_TurnToFaceEachOther:
	overlay_veneer Actor_TurnToFaceEachOtherFar
	.global Func_02001c6c
	.thumb_func
Func_02001c6c:
	overlay_veneer Event_SetMessageIdFar
	.global FieldMessage_ShowAtActor
	.thumb_func
FieldMessage_ShowAtActor:
	overlay_veneer FieldMessage_ShowAtActorFar
	.global Func_02001c7c
	.thumb_func
Func_02001c7c:
	overlay_veneer Func_080c8198
	.global Func_02001c84
	.thumb_func
Func_02001c84:
	overlay_veneer Event_ShowMessageAndWaitFar
	.global Actor_ArmCallback
	.thumb_func
Actor_ArmCallback:
	overlay_veneer Actor_ArmCallbackFar
	.global Func_02001c94
	.thumb_func
Func_02001c94:
	overlay_veneer Actor_ArmCallbackAndRefreshFar
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
	.global Camera_MoveWithinMapBounds
	.thumb_func
Camera_MoveWithinMapBounds:
	overlay_veneer Camera_MoveWithinMapBoundsFar
	.global Func_02001cc4
	.thumb_func
Func_02001cc4:
	overlay_veneer Func_080c8240
	.global Func_02001ccc
	.thumb_func
Func_02001ccc:
	overlay_veneer Event_SetDestinationFar
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
	.global Actor_SetWideSprite
	.thumb_func
Actor_SetWideSprite:
	overlay_veneer Actor_SetWideSpriteFar
	.global Func_02001cf4
	.thumb_func
Func_02001cf4:
	overlay_veneer Actor_ReturnHomeFar
	.global Func_02001cfc
	.thumb_func
Func_02001cfc:
	overlay_veneer Func_080c8480
	.global Func_02001d04
	.thumb_func
Func_02001d04:
	overlay_veneer Event_ResolvePendingActionsFar
	.global Func_02001d0c
	.thumb_func
Func_02001d0c:
	overlay_veneer Motion_LaunchFromFocusedObjectFar
	.global Actor_CommitPositionAndActivate
	.thumb_func
Actor_CommitPositionAndActivate:
	overlay_veneer Actor_CommitPositionAndActivateFar
	.global Actor_WatchAndSetScript
	.thumb_func
Actor_WatchAndSetScript:
	overlay_veneer Actor_WatchAndSetScriptFar
	.global Func_02001d24
	.thumb_func
Func_02001d24:
	overlay_veneer Func_080c86a8
	.global Func_02001d2c
	.thumb_func
Func_02001d2c:
	overlay_veneer Func_080c86b8
	.global Func_02001d34
	.thumb_func
Func_02001d34:
	overlay_veneer Func_080c86c0
	.global Func_02001d3c
	.thumb_func
Func_02001d3c:
	overlay_veneer Func_080c86e8
	.global Func_02001d44
	.thumb_func
Func_02001d44:
	overlay_veneer Func_080c8760
	.global Func_02001d4c
	.thumb_func
Func_02001d4c:
	overlay_veneer Func_080c8780
	.global Func_02001d54
	.thumb_func
Func_02001d54:
	overlay_veneer Func_080c87e8
	.global Func_02001d5c
	.thumb_func
Func_02001d5c:
	overlay_veneer Func_080c8838
	.global Func_02001d64
	.thumb_func
Func_02001d64:
	overlay_veneer Func_080c8848
	.global Func_02001d6c
	.thumb_func
Func_02001d6c:
	overlay_veneer Func_080c8878
	.global Func_02001d74
	.thumb_func
Func_02001d74:
	overlay_veneer Func_080c88e0
	.global Func_02001d7c
	.thumb_func
Func_02001d7c:
	overlay_veneer Sound_PlayFar
