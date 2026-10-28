.syntax unified
	.thumb
	.include "games/tla/src/system/overlay/imports.inc"
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
	.global Func_020007bc
	.thumb_func
Func_020007bc:
	overlay_veneer Map_CenterOnFollowedPointFar
	.global Actor_SetSingleChildFlags
	.thumb_func
Actor_SetSingleChildFlags:
	overlay_veneer Actor_SetSingleChildFlagsFar
	.global Func_020007cc
	.thumb_func
Func_020007cc:
	overlay_veneer Func_08020240
	.global Func_020007d4
	.thumb_func
Func_020007d4:
	overlay_veneer Func_08020248
	.global Func_020007dc
	.thumb_func
Func_020007dc:
	overlay_veneer Func_080202a0
	.global Func_020007e4
	.thumb_func
Func_020007e4:
	overlay_veneer Func_08038210
	.global FieldEvent_Wait
	.thumb_func
FieldEvent_Wait:
	overlay_veneer FieldEvent_WaitFar
	.global Func_020007f4
	.thumb_func
Func_020007f4:
	overlay_veneer Event_StartFar
	.global Func_020007fc
	.thumb_func
Func_020007fc:
	overlay_veneer Event_EndFar
	.global Event_AskYesNoWithActorAnimation
	.thumb_func
Event_AskYesNoWithActorAnimation:
	overlay_veneer Event_AskYesNoWithActorAnimationFar
	.global Actor_GetById
	.thumb_func
Actor_GetById:
	overlay_veneer Actor_GetByIdFar
	.global Actor_SetMovementTypeById
	.thumb_func
Actor_SetMovementTypeById:
	overlay_veneer Actor_SetMovementTypeByIdFar
	.global Func_0200081c
	.thumb_func
Func_0200081c:
	overlay_veneer Actor_SetHorizontalPositionWithTerrainFar
	.global Func_02000824
	.thumb_func
Func_02000824:
	overlay_veneer Actor_SetHorizontalPositionAndAngleWithTerrainFar
	.global Motion_SetModeAndWaitAnimation
	.thumb_func
Motion_SetModeAndWaitAnimation:
	overlay_veneer Motion_SetModeAndWaitAnimationFar
	.global Motion_SetVarCbAndRefresh
	.thumb_func
Motion_SetVarCbAndRefresh:
	overlay_veneer Motion_SetVarCbAndRefreshFar
	.global Actor_SetAngleToward
	.thumb_func
Actor_SetAngleToward:
	overlay_veneer Actor_SetAngleTowardFar
	.global Func_02000844
	.thumb_func
Func_02000844:
	overlay_veneer Event_SetMessageIdFar
	.global FieldMessage_ShowAtActor
	.thumb_func
FieldMessage_ShowAtActor:
	overlay_veneer FieldMessage_ShowAtActorFar
	.global Func_02000854
	.thumb_func
Func_02000854:
	overlay_veneer Func_080c8198
	.global Func_0200085c
	.thumb_func
Func_0200085c:
	overlay_veneer Event_ShowMessageAndWaitFar
	.global Actor_ArmCallback
	.thumb_func
Actor_ArmCallback:
	overlay_veneer Actor_ArmCallbackFar
	.global Event_StartScreenTransition
	.thumb_func
Event_StartScreenTransition:
	overlay_veneer Event_StartScreenTransitionFar
	.global Event_WaitTransitionFrames
	.thumb_func
Event_WaitTransitionFrames:
	overlay_veneer Event_WaitTransitionFramesFar
	.global Func_0200087c
	.thumb_func
Func_0200087c:
	overlay_veneer Func_080c8480
	.global Func_02000884
	.thumb_func
Func_02000884:
	overlay_veneer Event_ResolvePendingActionsFar
	.global Func_0200088c
	.thumb_func
Func_0200088c:
	overlay_veneer Party_GetLeaderActorFar
	.global Func_02000894
	.thumb_func
Func_02000894:
	overlay_veneer Func_080c87a8
	.global Func_0200089c
	.thumb_func
Func_0200089c:
	overlay_veneer Func_08108008
	.global Func_020008a4
	.thumb_func
Func_020008a4:
	overlay_veneer Func_08108010
	.global Func_020008ac
	.thumb_func
Func_020008ac:
	overlay_veneer Func_08108018
	.global Func_020008b4
	.thumb_func
Func_020008b4:
	overlay_veneer Sound_PlayFar
