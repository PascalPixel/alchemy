@ Import veneers after the OVERLAY_66D overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_66D overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	.global GameFlag_Test
	.thumb_func
GameFlag_Test:
	overlay_veneer GameFlag_TestFar
	.global GameFlag_SetBit
	.thumb_func
GameFlag_SetBit:
	overlay_veneer GameFlag_SetBitFar
	.global Func_02000534
	.thumb_func
Func_02000534:
	overlay_veneer Func_08020170
	.global ObjectDispatch_SetSingleChildField26
	.thumb_func
ObjectDispatch_SetSingleChildField26:
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	.global Battle_WaitMode0
	.thumb_func
Battle_WaitMode0:
	overlay_veneer Battle_WaitMode0Far
	.global Func_0200054c
	.thumb_func
Func_0200054c:
	overlay_veneer Func_080c8018
	.global Func_02000554
	.thumb_func
Func_02000554:
	overlay_veneer Func_080c8020
	.global Inventory_PromptAndSetObjectMode
	.thumb_func
Inventory_PromptAndSetObjectMode:
	overlay_veneer Inventory_PromptAndSetObjectModeFar
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer Object_GetByIdFar
	.global ObjectMotion_SetSpeedParameters
	.thumb_func
ObjectMotion_SetSpeedParameters:
	overlay_veneer ObjectMotion_SetSpeedParametersFar
	.global Func_02000574
	.thumb_func
Func_02000574:
	overlay_veneer Func_080c80f8
	.global Object_SetModeById
	.thumb_func
Object_SetModeById:
	overlay_veneer Object_SetModeByIdFar
	.global Func_02000584
	.thumb_func
Func_02000584:
	overlay_veneer Func_080c8180
	.global UiText_OpenMessageAtObject
	.thumb_func
UiText_OpenMessageAtObject:
	overlay_veneer UiText_OpenMessageAtObjectFar
	.global Func_02000594
	.thumb_func
Func_02000594:
	overlay_veneer Func_080c81a0
	.global Func_0200059c
	.thumb_func
Func_0200059c:
	overlay_veneer Func_080c8208
	.global Func_020005a4
	.thumb_func
Func_020005a4:
	overlay_veneer Func_080c8278
	.global Event_ClearStatus1c6
	.thumb_func
Event_ClearStatus1c6:
	overlay_veneer Event_ClearStatus1c6Far
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
	.global Func_020005bc
	.thumb_func
Func_020005bc:
	overlay_veneer Func_080c8408
	.global Func_020005c4
	.thumb_func
Func_020005c4:
	overlay_veneer Func_080c84e0
	.global Func_020005cc
	.thumb_func
Func_020005cc:
	overlay_veneer Func_080c86a8
	.global Func_020005d4
	.thumb_func
Func_020005d4:
	overlay_veneer Func_080c86e8
	.global Func_020005dc
	.thumb_func
Func_020005dc:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8768
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8768
	.else
	overlay_veneer Func_080c8778
	.endif
	.endif
	.global Func_020005e4
	.thumb_func
Func_020005e4:
	overlay_veneer Func_08108008
	.global Func_020005ec
	.thumb_func
Func_020005ec:
	overlay_veneer Func_08108010
	.global Func_020005f4
	.thumb_func
Func_020005f4:
	overlay_veneer Audio_PlayCue
