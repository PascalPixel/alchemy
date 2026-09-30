@ Import veneers after the OVERLAY_6AF overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_6AF overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	.global Engine_MathDivide
	.thumb_func
Engine_MathDivide:
	overlay_veneer IwramSignedDivide
	.global Scheduler_AddOrUpdateCallback
	.thumb_func
Scheduler_AddOrUpdateCallback:
	overlay_veneer Scheduler_AddOrUpdateCallbackFar
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
	.global Func_02000f7c
	.thumb_func
Func_02000f7c:
	overlay_veneer Object_SetMode
	.global Func_02000f84
	.thumb_func
Func_02000f84:
	overlay_veneer Object_SetCallback
	.global Func_02000f8c
	.thumb_func
Func_02000f8c:
	overlay_veneer Func_080200c0
	.global Func_02000f94
	.thumb_func
Func_02000f94:
	overlay_veneer Func_080201e8
	.global Func_02000f9c
	.thumb_func
Func_02000f9c:
	overlay_veneer Func_080201f0
	.global ObjectDispatch_SetSingleChildField26
	.thumb_func
ObjectDispatch_SetSingleChildField26:
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	.global Animation_SetStateFlags
	.thumb_func
Animation_SetStateFlags:
	overlay_veneer Animation_SetStateFlagsFar
	.global Func_02000fb4
	.thumb_func
Func_02000fb4:
	overlay_veneer Func_08020228
	.global Func_02000fbc
	.thumb_func
Func_02000fbc:
	overlay_veneer Func_08038210
	.global Battle_WaitMode0
	.thumb_func
Battle_WaitMode0:
	overlay_veneer Battle_WaitMode0Far
	.global Func_02000fcc
	.thumb_func
Func_02000fcc:
	overlay_veneer Func_080c8018
	.global Func_02000fd4
	.thumb_func
Func_02000fd4:
	overlay_veneer Func_080c8020
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer Object_GetByIdFar
	.global ObjectMotion_ResetAndSetPosition
	.thumb_func
ObjectMotion_ResetAndSetPosition:
	overlay_veneer ObjectMotion_ResetAndSetPositionFar
	.global Func_02000fec
	.thumb_func
Func_02000fec:
	overlay_veneer Func_080c80f8
	.global ObjectMotion_SetVariantCallback
	.thumb_func
ObjectMotion_SetVariantCallback:
	overlay_veneer ObjectMotion_SetVariantCallbackFar
	.global Object_SetPartAttribute
	.thumb_func
Object_SetPartAttribute:
	overlay_veneer Object_SetPartAttributeFar
	.global Func_02001004
	.thumb_func
Func_02001004:
	overlay_veneer Func_080c8230
	.global Motion_CamBounds
	.thumb_func
Motion_CamBounds:
	overlay_veneer Motion_CamBoundsFar
	.global Func_02001014
	.thumb_func
Func_02001014:
	overlay_veneer Func_080c8240
	.global Func_0200101c
	.thumb_func
Func_0200101c:
	overlay_veneer Func_080c8280
	.global Func_02001024
	.thumb_func
Func_02001024:
	overlay_veneer Func_080c8288
	.global Func_0200102c
	.thumb_func
Func_0200102c:
	overlay_veneer Func_080c82e0
	.global Func_02001034
	.thumb_func
Func_02001034:
	overlay_veneer Func_080c8378
	.global Func_0200103c
	.thumb_func
Func_0200103c:
	overlay_veneer Func_080c8380
	.global Func_02001044
	.thumb_func
Func_02001044:
	overlay_veneer Func_080c8390
	.global Func_0200104c
	.thumb_func
Func_0200104c:
	overlay_veneer Func_080c84e0
	.global Func_02001054
	.thumb_func
Func_02001054:
	overlay_veneer Func_080c8580
	.global Func_0200105c
	.thumb_func
Func_0200105c:
	overlay_veneer Func_080c86d0
	.global Func_02001064
	.thumb_func
Func_02001064:
	overlay_veneer Func_080c86d8
	.global Func_0200106c
	.thumb_func
Func_0200106c:
	overlay_veneer Audio_PlayCue
