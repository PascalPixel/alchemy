@ Import veneers after the OVERLAY_686 overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_686 overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	.global __divsi3
	.thumb_func
__divsi3:
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
	.global Runtime_BumpAllocate
	.thumb_func
Runtime_BumpAllocate:
	overlay_veneer Runtime_BumpAllocateFar
	.global Sys_Free
	.thumb_func
Sys_Free:
	overlay_veneer Sys_FreeFar
	.global VramBlock_LoadCached
	.thumb_func
VramBlock_LoadCached:
	overlay_veneer VramBlock_LoadCachedFar
	.global Resource_FindFreeEntry
	.thumb_func
Resource_FindFreeEntry:
	overlay_veneer Resource_FindFreeEntryFar
	.global Func_02002dac
	.thumb_func
Func_02002dac:
	overlay_veneer Func_080001e8
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
	.global Func_02002dcc
	.thumb_func
Func_02002dcc:
	overlay_veneer Func_08000418
	.global Func_02002dd4
	.thumb_func
Func_02002dd4:
	overlay_veneer Object_SetMode
	.global Func_02002ddc
	.thumb_func
Func_02002ddc:
	overlay_veneer Object_SetCallback
	.global Func_02002de4
	.thumb_func
Func_02002de4:
	overlay_veneer Func_080200c0
	.global Func_02002dec
	.thumb_func
Func_02002dec:
	overlay_veneer Func_08020120
	.global Func_02002df4
	.thumb_func
Func_02002df4:
	overlay_veneer Func_080201e0
	.global Func_02002dfc
	.thumb_func
Func_02002dfc:
	overlay_veneer Func_080201e8
	.global Func_02002e04
	.thumb_func
Func_02002e04:
	overlay_veneer Func_080201f0
	.global Func_02002e0c
	.thumb_func
Func_02002e0c:
	overlay_veneer Func_08020210
	.global ObjectDispatch_SetSingleChildField26
	.thumb_func
ObjectDispatch_SetSingleChildField26:
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	.global Animation_SetStateFlags
	.thumb_func
Animation_SetStateFlags:
	overlay_veneer Animation_SetStateFlagsFar
	.global Func_02002e24
	.thumb_func
Func_02002e24:
	overlay_veneer Func_08020228
	.global Func_02002e2c
	.thumb_func
Func_02002e2c:
	overlay_veneer Func_08020230
	.global Func_02002e34
	.thumb_func
Func_02002e34:
	overlay_veneer Func_08020290
	.global Battle_WaitMode0
	.thumb_func
Battle_WaitMode0:
	overlay_veneer Battle_WaitMode0Far
	.global Func_02002e44
	.thumb_func
Func_02002e44:
	overlay_veneer Func_080c8018
	.global Func_02002e4c
	.thumb_func
Func_02002e4c:
	overlay_veneer Func_080c8020
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer Object_GetByIdFar
	.global ObjectMotion_OffsetPositionAndResetMotion
	.thumb_func
ObjectMotion_OffsetPositionAndResetMotion:
	overlay_veneer ObjectMotion_OffsetPositionAndResetMotionFar
	.global ObjectMotion_CommitCurrentPositionAndActivate
	.thumb_func
ObjectMotion_CommitCurrentPositionAndActivate:
	overlay_veneer ObjectMotion_CommitCurrentPositionAndActivateFar
	.global Func_02002e6c
	.thumb_func
Func_02002e6c:
	overlay_veneer Func_080c80f8
	.global Object_SetModeById
	.thumb_func
Object_SetModeById:
	overlay_veneer Object_SetModeByIdFar
	.global Func_02002e7c
	.thumb_func
Func_02002e7c:
	overlay_veneer Func_080c8150
	.global Object_SetPartAttribute
	.thumb_func
Object_SetPartAttribute:
	overlay_veneer Object_SetPartAttributeFar
	.global ObjectMotion_SetActionVariant
	.thumb_func
ObjectMotion_SetActionVariant:
	overlay_veneer ObjectMotion_SetActionVariantFar
	.global Func_02002e94
	.thumb_func
Func_02002e94:
	overlay_veneer Func_080c8210
	.global Func_02002e9c
	.thumb_func
Func_02002e9c:
	overlay_veneer Func_080c8218
	.global Object_AttachWorkTargetToObject
	.thumb_func
Object_AttachWorkTargetToObject:
	overlay_veneer Object_AttachWorkTargetToObjectFar
	.global Motion_CamBounds
	.thumb_func
Motion_CamBounds:
	overlay_veneer Motion_CamBoundsFar
	.global Func_02002eb4
	.thumb_func
Func_02002eb4:
	overlay_veneer Func_080c8240
	.global Func_02002ebc
	.thumb_func
Func_02002ebc:
	overlay_veneer Func_080c8278
	.global Event_SetStatus1c6
	.thumb_func
Event_SetStatus1c6:
	overlay_veneer Event_SetStatus1c6Far
	.global Event_WaitValue1c8Frames
	.thumb_func
Event_WaitValue1c8Frames:
	overlay_veneer Event_WaitValue1c8FramesFar
	.global Func_02002ed4
	.thumb_func
Func_02002ed4:
	overlay_veneer Func_080c8480
	.global Func_02002edc
	.thumb_func
Func_02002edc:
	overlay_veneer Func_080c84e0
	.global Func_02002ee4
	.thumb_func
Func_02002ee4:
	overlay_veneer Func_080c8628
	.global Func_02002eec
	.thumb_func
Func_02002eec:
	overlay_veneer Audio_PlayCue
