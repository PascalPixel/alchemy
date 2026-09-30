@ Import veneers after the OVERLAY_6B4 overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_6B4 overlay import veneers
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
	.global Func_0200141c
	.thumb_func
Func_0200141c:
	overlay_veneer Func_080000d0
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
	.global Func_02001444
	.thumb_func
Func_02001444:
	overlay_veneer Func_08000170
	.global Sys_Free
	.thumb_func
Sys_Free:
	overlay_veneer Sys_FreeFar
	.global Func_02001454
	.thumb_func
Func_02001454:
	overlay_veneer Func_080001f0
	.global Func_0200145c
	.thumb_func
Func_0200145c:
	overlay_veneer Object_SetMode
	.global Func_02001464
	.thumb_func
Func_02001464:
	overlay_veneer Object_SetCallback
	.global Func_0200146c
	.thumb_func
Func_0200146c:
	overlay_veneer Func_080200c0
	.global ObjectDispatch_SetSingleChildField26
	.thumb_func
ObjectDispatch_SetSingleChildField26:
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	.global Func_0200147c
	.thumb_func
Func_0200147c:
	overlay_veneer Func_08020228
	.global Func_02001484
	.thumb_func
Func_02001484:
	overlay_veneer Func_08038348
	.global Battle_WaitMode0
	.thumb_func
Battle_WaitMode0:
	overlay_veneer Battle_WaitMode0Far
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
	.global ObjectMotion_SetPositionAndReset
	.thumb_func
ObjectMotion_SetPositionAndReset:
	overlay_veneer ObjectMotion_SetPositionAndResetFar
	.global Func_020014b4
	.thumb_func
Func_020014b4:
	overlay_veneer Func_080c80f8
	.global Object_SetModeById
	.thumb_func
Object_SetModeById:
	overlay_veneer Object_SetModeByIdFar
	.global Object_SetActionById
	.thumb_func
Object_SetActionById:
	overlay_veneer Object_SetActionByIdFar
	.global Motion_SetVarCbAndRefresh
	.thumb_func
Motion_SetVarCbAndRefresh:
	overlay_veneer Motion_SetVarCbAndRefreshFar
	.global Func_020014d4
	.thumb_func
Func_020014d4:
	overlay_veneer Func_080c8168
	.global Func_020014dc
	.thumb_func
Func_020014dc:
	overlay_veneer Func_080c8170
	.global Func_020014e4
	.thumb_func
Func_020014e4:
	overlay_veneer Func_080c8180
	.global Func_020014ec
	.thumb_func
Func_020014ec:
	overlay_veneer Func_080c8198
	.global Func_020014f4
	.thumb_func
Func_020014f4:
	overlay_veneer Func_080c81a0
	.global Func_020014fc
	.thumb_func
Func_020014fc:
	overlay_veneer Func_080c81d0
	.global Func_02001504
	.thumb_func
Func_02001504:
	overlay_veneer Func_080c8200
	.global Func_0200150c
	.thumb_func
Func_0200150c:
	overlay_veneer Func_080c8210
	.global Func_02001514
	.thumb_func
Func_02001514:
	overlay_veneer Func_080c8218
	.global Func_0200151c
	.thumb_func
Func_0200151c:
	overlay_veneer Func_080c8230
	.global Func_02001524
	.thumb_func
Func_02001524:
	overlay_veneer Func_080c8238
	.global Func_0200152c
	.thumb_func
Func_0200152c:
	overlay_veneer Func_080c8240
	.global Func_02001534
	.thumb_func
Func_02001534:
	overlay_veneer Func_080c8268
	.global Func_0200153c
	.thumb_func
Func_0200153c:
	overlay_veneer Func_080c8378
	.global Func_02001544
	.thumb_func
Func_02001544:
	overlay_veneer Func_080c8390
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
	.global Func_02001564
	.thumb_func
Func_02001564:
	overlay_veneer Func_080c87f8
	.global Field_BeginPaletteTransition
	.thumb_func
Field_BeginPaletteTransition:
	overlay_veneer Field_BeginPaletteTransitionFar
	.global Func_02001574
	.thumb_func
Func_02001574:
	overlay_veneer Func_080c8808
	.global Func_0200157c
	.thumb_func
Func_0200157c:
	overlay_veneer Func_080c8810
	.global Func_02001584
	.thumb_func
Func_02001584:
	overlay_veneer Func_080c8818
	.global Func_0200158c
	.thumb_func
Func_0200158c:
	overlay_veneer Func_080c8820
	.global Func_02001594
	.thumb_func
Func_02001594:
	overlay_veneer Audio_PlayCue
