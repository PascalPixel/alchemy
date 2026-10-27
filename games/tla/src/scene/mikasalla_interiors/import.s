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
	.global FieldEvent_WaitTicks
	.thumb_func
FieldEvent_WaitTicks:
	overlay_veneer FieldEvent_WaitTicksFar
	.global Func_020005c4
	.thumb_func
Func_020005c4:
	overlay_veneer EventRuntime_StartFar
	.global Func_020005cc
	.thumb_func
Func_020005cc:
	overlay_veneer EventRuntime_EndFar
	.global Event_AskYesNoWithActorAnimation
	.thumb_func
Event_AskYesNoWithActorAnimation:
	overlay_veneer Event_AskYesNoWithActorAnimationFar
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer Object_GetByIdFar
	.global Func_020005e4
	.thumb_func
Func_020005e4:
	overlay_veneer ObjectMotion_SetHorizontalPositionWithTerrainFar
	.global Object_SetModeById
	.thumb_func
Object_SetModeById:
	overlay_veneer Object_SetModeByIdFar
	.global ObjectMotion_SetAngleToward
	.thumb_func
ObjectMotion_SetAngleToward:
	overlay_veneer ObjectMotion_SetAngleTowardFar
	.global Object_SetPartAttribute
	.thumb_func
Object_SetPartAttribute:
	overlay_veneer Object_SetPartAttributeFar
	.global Func_02000604
	.thumb_func
Func_02000604:
	overlay_veneer EventRuntime_SetMessageFar
	.global UiText_OpenMessageAtObject
	.thumb_func
UiText_OpenMessageAtObject:
	overlay_veneer UiText_OpenMessageAtObjectFar
	.global Func_02000614
	.thumb_func
Func_02000614:
	overlay_veneer EventRuntime_ShowMessageAndWaitFar
	.global ObjectMotion_SetActionVariant
	.thumb_func
ObjectMotion_SetActionVariant:
	overlay_veneer ObjectMotion_SetActionVariantFar
	.global Func_02000624
	.thumb_func
Func_02000624:
	overlay_veneer Func_080c8210
	.global Func_0200062c
	.thumb_func
Func_0200062c:
	overlay_veneer EventRuntime_ResolvePendingActionsFar
	.global Func_02000634
	.thumb_func
Func_02000634:
	overlay_veneer EventRuntime_GetControlledOwnerFar
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
