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
	.global FieldEvent_WaitFrames
	.thumb_func
FieldEvent_WaitFrames:
	overlay_veneer FieldEvent_WaitFramesFar
	.global Func_020005c4
	.thumb_func
Func_020005c4:
	overlay_veneer Event_StartFar
	.global Func_020005cc
	.thumb_func
Func_020005cc:
	overlay_veneer Event_EndFar
	.global Event_AskYesNoWithActorAnimation
	.thumb_func
Event_AskYesNoWithActorAnimation:
	overlay_veneer Event_AskYesNoWithActorAnimationFar
	.global Actor_GetById
	.thumb_func
Actor_GetById:
	overlay_veneer Actor_GetByIdFar
	.global Func_020005e4
	.thumb_func
Func_020005e4:
	overlay_veneer Actor_SetHorizontalPositionWithTerrainFar
	.global Actor_SetModeById
	.thumb_func
Actor_SetModeById:
	overlay_veneer Actor_SetModeByIdFar
	.global Actor_SetAngleToward
	.thumb_func
Actor_SetAngleToward:
	overlay_veneer Actor_SetAngleTowardFar
	.global Actor_SetPartAttribute
	.thumb_func
Actor_SetPartAttribute:
	overlay_veneer Actor_SetPartAttributeFar
	.global Func_02000604
	.thumb_func
Func_02000604:
	overlay_veneer Event_SetMessageIdFar
	.global FieldMessage_ShowAtActor
	.thumb_func
FieldMessage_ShowAtActor:
	overlay_veneer FieldMessage_ShowAtActorFar
	.global Func_02000614
	.thumb_func
Func_02000614:
	overlay_veneer Event_ShowMessageAndWaitFar
	.global Actor_SetSpritePriority
	.thumb_func
Actor_SetSpritePriority:
	overlay_veneer Actor_SetSpritePriorityFar
	.global Func_02000624
	.thumb_func
Func_02000624:
	overlay_veneer Func_080c8210
	.global Func_0200062c
	.thumb_func
Func_0200062c:
	overlay_veneer Event_ResolvePendingActionsFar
	.global Func_02000634
	.thumb_func
Func_02000634:
	overlay_veneer Party_GetLeaderActorFar
	.global Func_0200063c
	.thumb_func
Func_0200063c:
	overlay_veneer Func_08108008
	.global Func_02000644
	.thumb_func
Func_02000644:
	overlay_veneer Func_08108010
	.global Func_0200064c
	.thumb_func
Func_0200064c:
	overlay_veneer Func_08108018
