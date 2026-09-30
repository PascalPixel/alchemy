@ Import veneers after the OVERLAY_687 overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_687 overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	.global Engine_MathDivide
	.thumb_func
Engine_MathDivide:
	overlay_veneer IwramSignedDivide
	.global WaitFrames
	.thumb_func
WaitFrames:
	overlay_veneer WaitFramesFar
	.global Scheduler_AddOrUpdateCallback
	.thumb_func
Scheduler_AddOrUpdateCallback:
	overlay_veneer Scheduler_AddOrUpdateCallbackFar
	.global Scheduler_RemoveCallbackFar
	.thumb_func
Scheduler_RemoveCallbackFar:
	overlay_veneer Func_080000d8
	.global Random16Far
	.thumb_func
Random16Far:
	overlay_veneer Func_080000f8
	.global Math_Sine
	.thumb_func
Math_Sine:
	overlay_veneer Math_SineFar
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
	.global GameFlag_ClearBit
	.thumb_func
GameFlag_ClearBit:
	overlay_veneer GameFlag_ClearBitFar
	.global Func_02001f4c
	.thumb_func
Func_02001f4c:
	overlay_veneer Object_SetMode
	.global Func_02001f54
	.thumb_func
Func_02001f54:
	overlay_veneer Object_SetCallback
	.global Func_02001f5c
	.thumb_func
Func_02001f5c:
	overlay_veneer Func_080200c0
	.global Func_02001f64
	.thumb_func
Func_02001f64:
	overlay_veneer Func_080200c8
	.global Func_02001f6c
	.thumb_func
Func_02001f6c:
	overlay_veneer Object_SetPosition
	.global Func_02001f74
	.thumb_func
Func_02001f74:
	overlay_veneer Func_08020178
	.global ObjectDispatch_SetSingleChildField26
	.thumb_func
ObjectDispatch_SetSingleChildField26:
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	.global Animation_SetStateFlags
	.thumb_func
Animation_SetStateFlags:
	overlay_veneer Animation_SetStateFlagsFar
	.global Func_02001f8c
	.thumb_func
Func_02001f8c:
	overlay_veneer Func_08020228
	.global Func_02001f94
	.thumb_func
Func_02001f94:
	overlay_veneer Func_08020230
	.global Func_02001f9c
	.thumb_func
Func_02001f9c:
	overlay_veneer Func_080202f8
	.global Battle_WaitMode0
	.thumb_func
Battle_WaitMode0:
	overlay_veneer Battle_WaitMode0Far
	.global Func_02001fac
	.thumb_func
Func_02001fac:
	overlay_veneer Func_080c8018
	.global Func_02001fb4
	.thumb_func
Func_02001fb4:
	overlay_veneer Func_080c8020
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer Object_GetByIdFar
	.global ObjectMotion_SetSpeedParameters
	.thumb_func
ObjectMotion_SetSpeedParameters:
	overlay_veneer ObjectMotion_SetSpeedParametersFar
	.global ObjectMotion_ResetAndSetPositionInMode2
	.thumb_func
ObjectMotion_ResetAndSetPositionInMode2:
	overlay_veneer ObjectMotion_ResetAndSetPositionInMode2Far
	.global ObjectMotion_CommitCurrentPositionAndActivate
	.thumb_func
ObjectMotion_CommitCurrentPositionAndActivate:
	overlay_veneer ObjectMotion_CommitCurrentPositionAndActivateFar
	.global Func_02001fdc
	.thumb_func
Func_02001fdc:
	overlay_veneer Func_080c80f8
	.global Object_SetPartAttribute
	.thumb_func
Object_SetPartAttribute:
	overlay_veneer Object_SetPartAttributeFar
	.global Func_02001fec
	.thumb_func
Func_02001fec:
	overlay_veneer Func_080c8230
	.global Motion_CamBounds
	.thumb_func
Motion_CamBounds:
	overlay_veneer Motion_CamBoundsFar
	.global Func_02001ffc
	.thumb_func
Func_02001ffc:
	overlay_veneer Func_080c8240
	.global Func_02002004
	.thumb_func
Func_02002004:
	overlay_veneer Func_080c8278
	.global Func_0200200c
	.thumb_func
Func_0200200c:
	overlay_veneer Func_080c8280
	.global Party_SetFields1eeAnd1f0
	.thumb_func
Party_SetFields1eeAnd1f0:
	overlay_veneer Party_SetFields1eeAnd1f0Far
	.global Func_0200201c
	.thumb_func
Func_0200201c:
	overlay_veneer Func_080c8378
	.global Func_02002024
	.thumb_func
Func_02002024:
	overlay_veneer Func_080c8380
	.global Event_SetStatus1c6
	.thumb_func
Event_SetStatus1c6:
	overlay_veneer Event_SetStatus1c6Far
	.global Event_ClearStatus1c6
	.thumb_func
Event_ClearStatus1c6:
	overlay_veneer Event_ClearStatus1c6Far
	.global Event_WaitValue1c8Frames
	.thumb_func
Event_WaitValue1c8Frames:
	overlay_veneer Event_WaitValue1c8FramesFar
	.global Func_02002044
	.thumb_func
Func_02002044:
	overlay_veneer Func_080c84e0
	.global Func_0200204c
	.thumb_func
Func_0200204c:
	overlay_veneer Func_080c8570
	.global Func_02002054
	.thumb_func
Func_02002054:
	overlay_veneer Audio_PlayCue
