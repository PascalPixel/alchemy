.syntax unified
	.thumb
	.include "games/tla/src/system/overlay/imports.inc"
	.balign 4
	.global wait
	.thumb_func
wait:
	overlay_veneer waitFar
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
	.global Func_0200103c
	.thumb_func
Func_0200103c:
	overlay_veneer Actor_SetMode
	.global Func_02001044
	.thumb_func
Func_02001044:
	overlay_veneer Actor_StartScriptFar
	.global Func_0200104c
	.thumb_func
Func_0200104c:
	overlay_veneer Actor_ResetMotion
	.global Func_02001054
	.thumb_func
Func_02001054:
	overlay_veneer Actor_SetTargetPosition
	.global Func_0200105c
	.thumb_func
Func_0200105c:
	overlay_veneer Actor_CommitPosition
	.global Map_GetTerrainHeight
	.thumb_func
Map_GetTerrainHeight:
	overlay_veneer Map_GetTerrainHeightFar
	.global Func_0200106c
	.thumb_func
Func_0200106c:
	overlay_veneer Map_CopyCellAttributeRectFar
	.global Actor_SetSingleChildFlags
	.thumb_func
Actor_SetSingleChildFlags:
	overlay_veneer Actor_SetSingleChildFlagsFar
	.global FieldEvent_Wait
	.thumb_func
FieldEvent_Wait:
	overlay_veneer FieldEvent_WaitFar
	.global Func_02001084
	.thumb_func
Func_02001084:
	overlay_veneer Event_StartFar
	.global Func_0200108c
	.thumb_func
Func_0200108c:
	overlay_veneer Event_EndFar
	.global Actor_GetById
	.thumb_func
Actor_GetById:
	overlay_veneer Actor_GetByIdFar
	.global Actor_SetMovementTypeById
	.thumb_func
Actor_SetMovementTypeById:
	overlay_veneer Actor_SetMovementTypeByIdFar
	.global Actor_Stop
	.thumb_func
Actor_Stop:
	overlay_veneer Actor_StopFar
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
	.global Actor_WaitForMoveById
	.thumb_func
Actor_WaitForMoveById:
	overlay_veneer Actor_WaitForMoveByIdFar
	.global Func_020010d4
	.thumb_func
Func_020010d4:
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
	.global Motion_SetVarCbAndRefresh
	.thumb_func
Motion_SetVarCbAndRefresh:
	overlay_veneer Motion_SetVarCbAndRefreshFar
	.global Actor_SetAngleToward
	.thumb_func
Actor_SetAngleToward:
	overlay_veneer Actor_SetAngleTowardFar
	.global Func_02001104
	.thumb_func
Func_02001104:
	overlay_veneer Event_SetMessageIdFar
	.global Func_0200110c
	.thumb_func
Func_0200110c:
	overlay_veneer Event_ShowMessageAndWaitFar
	.global Actor_SetSpritePriority
	.thumb_func
Actor_SetSpritePriority:
	overlay_veneer Actor_SetSpritePriorityFar
	.global Func_0200111c
	.thumb_func
Func_0200111c:
	overlay_veneer Func_080c8210
	.global Func_02001124
	.thumb_func
Func_02001124:
	overlay_veneer Func_080c8218
	.global Camera_MoveWithinMapBounds
	.thumb_func
Camera_MoveWithinMapBounds:
	overlay_veneer Camera_MoveWithinMapBoundsFar
	.global Func_02001134
	.thumb_func
Func_02001134:
	overlay_veneer Func_080c8240
	.global Func_0200113c
	.thumb_func
Func_0200113c:
	overlay_veneer Event_SetDestinationFar
	.global Func_02001144
	.thumb_func
Func_02001144:
	overlay_veneer Func_080c8288
	.global Func_0200114c
	.thumb_func
Func_0200114c:
	overlay_veneer Func_080c82e0
	.global Event_StartScreenTransition
	.thumb_func
Event_StartScreenTransition:
	overlay_veneer Event_StartScreenTransitionFar
	.global Event_WaitTransitionFrames
	.thumb_func
Event_WaitTransitionFrames:
	overlay_veneer Event_WaitTransitionFramesFar
	.global Func_02001164
	.thumb_func
Func_02001164:
	overlay_veneer Event_ResolvePendingActionsFar
	.global Actor_WatchAndSetScript
	.thumb_func
Actor_WatchAndSetScript:
	overlay_veneer Actor_WatchAndSetScriptFar
	.global Func_02001174
	.thumb_func
Func_02001174:
	overlay_veneer Func_080c86a8
	.global Func_0200117c
	.thumb_func
Func_0200117c:
	overlay_veneer Func_080c86e8
	.global Func_02001184
	.thumb_func
Func_02001184:
	overlay_veneer Party_GetLeaderActorFar
	.global Func_0200118c
	.thumb_func
Func_0200118c:
	overlay_veneer Func_080c8860
	.global Actor_ResetTargetsAndVelocity
	.thumb_func
Actor_ResetTargetsAndVelocity:
	overlay_veneer Actor_ResetTargetsAndVelocityFar
	.global Func_0200119c
	.thumb_func
Func_0200119c:
	overlay_veneer Sound_PlayFar
