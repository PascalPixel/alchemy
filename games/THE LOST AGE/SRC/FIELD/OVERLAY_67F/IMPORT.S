@ Import veneers after the OVERLAY_67F overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_67F overlay import veneers
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
	.global Func_02001418
	.thumb_func
Func_02001418:
	overlay_veneer Func_080003c8
	.global Func_02001420
	.thumb_func
Func_02001420:
	overlay_veneer Func_080003d0
	.global Func_02001428
	.thumb_func
Func_02001428:
	overlay_veneer Object_SetMode
	.global Func_02001430
	.thumb_func
Func_02001430:
	overlay_veneer Object_SetCallback
	.global Func_02001438
	.thumb_func
Func_02001438:
	overlay_veneer Func_080200c0
	.global Object_SetPositionAndResetMotion
	.thumb_func
Object_SetPositionAndResetMotion:
	overlay_veneer Object_SetPositionAndResetMotionFar
	.global Func_02001448
	.thumb_func
Func_02001448:
	overlay_veneer Func_08020120
	.global Func_02001450
	.thumb_func
Func_02001450:
	overlay_veneer Func_08020178
	.global Map_GetTerrainHeight
	.thumb_func
Map_GetTerrainHeight:
	overlay_veneer Map_GetTerrainHeightFar
	.global Func_02001460
	.thumb_func
Func_02001460:
	overlay_veneer Func_080201e8
	.global ObjectDispatch_SetSingleChildField26
	.thumb_func
ObjectDispatch_SetSingleChildField26:
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	.global Func_02001470
	.thumb_func
Func_02001470:
	overlay_veneer Func_08020228
	.global Func_02001478
	.thumb_func
Func_02001478:
	overlay_veneer Func_080202f0
	.global Func_02001480
	.thumb_func
Func_02001480:
	overlay_veneer Func_08020360
	.global Func_02001488
	.thumb_func
Func_02001488:
	overlay_veneer Func_080c8018
	.global Func_02001490
	.thumb_func
Func_02001490:
	overlay_veneer Func_080c8020
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer Object_GetByIdFar
	.global Func_020014a0
	.thumb_func
Func_020014a0:
	overlay_veneer Func_080c80f8
	.global Func_020014a8
	.thumb_func
Func_020014a8:
	overlay_veneer Func_080c8110
	.global Object_SetModeById
	.thumb_func
Object_SetModeById:
	overlay_veneer Object_SetModeByIdFar
	.global Func_020014b8
	.thumb_func
Func_020014b8:
	overlay_veneer Func_080c8170
	.global Func_020014c0
	.thumb_func
Func_020014c0:
	overlay_veneer Func_080c8208
	.global Func_020014c8
	.thumb_func
Func_020014c8:
	overlay_veneer Func_080c8220
	.global Func_020014d0
	.thumb_func
Func_020014d0:
	overlay_veneer Func_080c8228
	.global Func_020014d8
	.thumb_func
Func_020014d8:
	overlay_veneer Func_080c82e0
	.global Func_020014e0
	.thumb_func
Func_020014e0:
	overlay_veneer Func_080c8480
	.global Func_020014e8
	.thumb_func
Func_020014e8:
	overlay_veneer Func_080c84e0
	.global Func_020014f0
	.thumb_func
Func_020014f0:
	overlay_veneer Func_080c8518
	.global Func_020014f8
	.thumb_func
Func_020014f8:
	overlay_veneer Func_080c86f8
	.global Func_02001500
	.thumb_func
Func_02001500:
	overlay_veneer Func_080c8708
	.global Func_02001508
	.thumb_func
Func_02001508:
	overlay_veneer Func_080c8710
	.global Func_02001510
	.thumb_func
Func_02001510:
	overlay_veneer Func_080c8718
	.global Func_02001518
	.thumb_func
Func_02001518:
	overlay_veneer Func_080c8720
	.global Func_02001520
	.thumb_func
Func_02001520:
	overlay_veneer Func_080c8728
	.global Func_02001528
	.thumb_func
Func_02001528:
	overlay_veneer Func_080c8730
	.global Func_02001530
	.thumb_func
Func_02001530:
	overlay_veneer Func_080c8738
	.global Func_02001538
	.thumb_func
Func_02001538:
	overlay_veneer Audio_PlayCue
