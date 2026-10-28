.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.balign 4
	.global Task_Add
	.thumb_func
Task_Add:
	overlay_veneer Task_AddFar
	.global GameFlag_Test
	.thumb_func
GameFlag_Test:
	overlay_veneer GameFlag_TestFar
	.global GameFlag_ClearBit
	.thumb_func
GameFlag_ClearBit:
	overlay_veneer GameFlag_ClearBitFar
	.global Func_02000454
	.thumb_func
Func_02000454:
	overlay_veneer Map_PlayMapEditFar
	.global Func_0200045c
	.thumb_func
Func_0200045c:
	overlay_veneer Map_CopyCellPictureIndicesRectFar
	.global FieldEvent_WaitFrames
	.thumb_func
FieldEvent_WaitFrames:
	overlay_veneer FieldEvent_WaitFramesFar
	.global Func_0200046c
	.thumb_func
Func_0200046c:
	overlay_veneer Event_StartFar
	.global Func_02000474
	.thumb_func
Func_02000474:
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
	.global Func_020004bc
	.thumb_func
Func_020004bc:
	overlay_veneer Actor_SetHorizontalPositionWithTerrainFar
	.global Actor_SetModeById
	.thumb_func
Actor_SetModeById:
	overlay_veneer Actor_SetModeByIdFar
	.global Actor_Jump
	.thumb_func
Actor_Jump:
	overlay_veneer Actor_JumpFar
	.global Actor_SetAngleToward
	.thumb_func
Actor_SetAngleToward:
	overlay_veneer Actor_SetAngleTowardFar
	.global Func_020004dc
	.thumb_func
Func_020004dc:
	overlay_veneer Event_SetMessageIdFar
	.global FieldMessage_ShowAtActor
	.thumb_func
FieldMessage_ShowAtActor:
	overlay_veneer FieldMessage_ShowAtActorFar
	.global Func_020004ec
	.thumb_func
Func_020004ec:
	overlay_veneer Event_ShowMessageAndWaitFar
	.global Func_020004f4
	.thumb_func
Func_020004f4:
	overlay_veneer Actor_ArmCallbackAndRefreshFar
	.ifndef TLA_EDITION_JA
	.global Actor_SetSpritePriority
	.thumb_func
Actor_SetSpritePriority:
	overlay_veneer Actor_SetSpritePriorityFar
	.endif
	.global Func_02000504
	.thumb_func
Func_02000504:
	overlay_veneer Func_080c8210
	.global Func_0200050c
	.thumb_func
Func_0200050c:
	overlay_veneer Event_SetExitRequestFar
	.global Event_FinishScreenTransition
	.thumb_func
Event_FinishScreenTransition:
	overlay_veneer Event_FinishScreenTransitionFar
	.global Event_WaitTransitionFrames
	.thumb_func
Event_WaitTransitionFrames:
	overlay_veneer Event_WaitTransitionFramesFar
	.global Func_02000524
	.thumb_func
Func_02000524:
	overlay_veneer Event_ResolvePendingActionsFar
	.global Func_0200052c
	.thumb_func
Func_0200052c:
	overlay_veneer Motion_LaunchFromFocusedObjectFar
	.global Actor_CommitPositionAndActivate
	.thumb_func
Actor_CommitPositionAndActivate:
	overlay_veneer Actor_CommitPositionAndActivateFar
	.global Func_0200053c
	.thumb_func
Func_0200053c:
	overlay_veneer Party_GetLeaderActorFar
	.global Func_02000544
	.thumb_func
Func_02000544:
	overlay_veneer Sound_PlayFar
