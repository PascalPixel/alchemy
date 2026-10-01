@ Import veneers after the OVERLAY_680 overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_680 overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	.global WaitFrames
	.thumb_func
WaitFrames:
	overlay_veneer WaitFramesFar
	.global Scheduler_AddOrUpdateCallback
	.thumb_func
Scheduler_AddOrUpdateCallback:
	overlay_veneer Scheduler_AddOrUpdateCallbackFar
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
	.global Vector_AddPolarOffsetFar
	.thumb_func
Vector_AddPolarOffsetFar:
	overlay_veneer Func_08000128
	.global GameFlag_Test
	.thumb_func
GameFlag_Test:
	overlay_veneer GameFlag_TestFar
	.global GameFlag_SetBit
	.thumb_func
GameFlag_SetBit:
	overlay_veneer GameFlag_SetBitFar
	.global Func_02000e24
	.thumb_func
Func_02000e24:
	overlay_veneer Object_SetMode
	.global Func_02000e2c
	.thumb_func
Func_02000e2c:
	overlay_veneer Object_SetCallback
	.global Func_02000e34
	.thumb_func
Func_02000e34:
	overlay_veneer Func_080200c0
	.global Func_02000e3c
	.thumb_func
Func_02000e3c:
	overlay_veneer Object_SetPosition
	.global Func_02000e44
	.thumb_func
Func_02000e44:
	.ifdef TLA_EDITION_DE
	overlay_veneer Func_080201b8
	.else
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080201b8
	.else
	.ifdef TLA_EDITION_FR
	overlay_veneer Func_080201b8
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080201b8
	.else
	overlay_veneer Func_080201e8
	.endif
	.endif
	.endif
	.endif
	.global ObjectDispatch_SetSingleChildField26
	.thumb_func
ObjectDispatch_SetSingleChildField26:
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	.global Func_02000e54
	.thumb_func
Func_02000e54:
	overlay_veneer Func_08020228
	.global Func_02000e5c
	.thumb_func
Func_02000e5c:
	overlay_veneer Func_08020290
	.global UiText_ShowPositionedMessageAndWait
	.thumb_func
UiText_ShowPositionedMessageAndWait:
	overlay_veneer UiText_ShowPositionedMessageAndWaitFar
	.global Battle_WaitMode0
	.thumb_func
Battle_WaitMode0:
	overlay_veneer Battle_WaitMode0Far
	.global Func_02000e74
	.thumb_func
Func_02000e74:
	overlay_veneer Func_080c8018
	.global Func_02000e7c
	.thumb_func
Func_02000e7c:
	overlay_veneer Func_080c8020
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer Object_GetByIdFar
	.global Func_02000e8c
	.thumb_func
Func_02000e8c:
	overlay_veneer Func_080c80f8
	.global Object_SetModeById
	.thumb_func
Object_SetModeById:
	overlay_veneer Object_SetModeByIdFar
	.global ObjectMotion_SetVariantCallback
	.thumb_func
ObjectMotion_SetVariantCallback:
	overlay_veneer ObjectMotion_SetVariantCallbackFar
	.global Func_02000ea4
	.thumb_func
Func_02000ea4:
	overlay_veneer Func_080c8180
	.global Func_02000eac
	.thumb_func
Func_02000eac:
	overlay_veneer Func_080c81a0
	.global Func_02000eb4
	.thumb_func
Func_02000eb4:
	overlay_veneer Func_080c81a8
	.global Func_02000ebc
	.thumb_func
Func_02000ebc:
	overlay_veneer Func_080c8218
	.global Motion_CamBounds
	.thumb_func
Motion_CamBounds:
	overlay_veneer Motion_CamBoundsFar
	.global Func_02000ecc
	.thumb_func
Func_02000ecc:
	overlay_veneer Func_080c8240
	.global Func_02000ed4
	.thumb_func
Func_02000ed4:
	overlay_veneer Func_080c8248
	.global Func_02000edc
	.thumb_func
Func_02000edc:
	overlay_veneer Func_080c8368
	.global Event_SetStatus1c6
	.thumb_func
Event_SetStatus1c6:
	overlay_veneer Event_SetStatus1c6Far
	.global Event_WaitValue1c8Frames
	.thumb_func
Event_WaitValue1c8Frames:
	.ifdef TLA_EDITION_ES
	overlay_veneer Event_SetStatus1c6Far
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Event_SetStatus1c6Far
	.else
	overlay_veneer Event_WaitValue1c8FramesFar
	.endif
	.endif
	.global Func_02000ef4
	.thumb_func
Func_02000ef4:
	overlay_veneer Func_080c84e0
	.global Object_Spawn
	.thumb_func
Object_Spawn:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8748
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8748
	.else
	overlay_veneer Object_SpawnFar
	.endif
	.endif
	.global Func_02000f04
	.thumb_func
Func_02000f04:
	overlay_veneer Func_080c8760
	.global Func_02000f0c
	.thumb_func
Func_02000f0c:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8768
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8768
	.else
	overlay_veneer Func_080c8778
	.endif
	.endif
	.global Func_02000f14
	.thumb_func
Func_02000f14:
	overlay_veneer Func_080c8780
	.global Func_02000f1c
	.thumb_func
Func_02000f1c:
	overlay_veneer Func_080c88c8
	.global Func_02000f24
	.thumb_func
Func_02000f24:
	overlay_veneer Audio_PlayCue
