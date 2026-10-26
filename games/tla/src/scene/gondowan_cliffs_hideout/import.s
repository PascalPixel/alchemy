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
	.global ObjectDispatch_SetSingleChildFlags
	.thumb_func
ObjectDispatch_SetSingleChildFlags:
	overlay_veneer ObjectDispatch_SetSingleChildFlagsFar
	.global Func_020001b4
	.thumb_func
Func_020001b4:
	overlay_veneer EventRuntime_StartFar
	.global Func_020001bc
	.thumb_func
Func_020001bc:
	overlay_veneer EventRuntime_EndFar
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer Object_GetByIdFar
	.global Motion_SetModeAndWaitAnimation
	.thumb_func
Motion_SetModeAndWaitAnimation:
	overlay_veneer Motion_SetModeAndWaitAnimationFar
	.global ObjectMotion_Launch
	.thumb_func
ObjectMotion_Launch:
	overlay_veneer ObjectMotion_LaunchFar
	.global ObjectMotion_SetAngleToward
	.thumb_func
ObjectMotion_SetAngleToward:
	overlay_veneer ObjectMotion_SetAngleTowardFar
	.global Func_020001e4
	.thumb_func
Func_020001e4:
	overlay_veneer EventRuntime_SetMessageFar
	.global Func_020001ec
	.thumb_func
Func_020001ec:
	overlay_veneer EventRuntime_ShowMessageAndWaitFar
	.global Func_020001f4
	.thumb_func
Func_020001f4:
	overlay_veneer Func_080c8218
	.global Camera_FollowActor
	.thumb_func
Camera_FollowActor:
	overlay_veneer Camera_FollowActorFar
	.global Func_02000204
	.thumb_func
Func_02000204:
	overlay_veneer Camera_SetSpeedParametersFar
	.global Func_0200020c
	.thumb_func
Func_0200020c:
	overlay_veneer Func_080c8240
	.global Func_02000214
	.thumb_func
Func_02000214:
	overlay_veneer EventRuntime_ResolvePendingActionsFar
	.global Func_0200021c
	.thumb_func
Func_0200021c:
	overlay_veneer EventRuntime_GetControlledOwnerFar
