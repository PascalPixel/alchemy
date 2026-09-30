@ Import veneers after the OVERLAY_67F overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_67F overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer IwramSignedDivide
	overlay_veneer WaitFramesFar
	overlay_veneer Func_080000f8
	overlay_veneer Math_SineFar
	overlay_veneer Math_CosineFar
	overlay_veneer Func_080003c8
	overlay_veneer Func_080003d0
	overlay_veneer Object_SetMode
	overlay_veneer Object_SetCallback
	overlay_veneer Func_080200c0
	overlay_veneer Object_SetPositionAndResetMotionFar
	overlay_veneer Func_08020120
	overlay_veneer Func_08020178
	overlay_veneer Func_080201c0
	overlay_veneer Func_080201e8
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	overlay_veneer Func_08020228
	overlay_veneer Func_080202f0
	overlay_veneer Func_08020360
	overlay_veneer Func_080c8018
	overlay_veneer Func_080c8020
	overlay_veneer Object_GetByIdFar
	overlay_veneer Func_080c80f8
	overlay_veneer Func_080c8110
	overlay_veneer Object_SetModeByIdFar
	overlay_veneer Func_080c8170
	overlay_veneer Func_080c8208
	overlay_veneer Func_080c8220
	overlay_veneer Func_080c8228
	overlay_veneer Func_080c82e0
	overlay_veneer Func_080c8480
	overlay_veneer Func_080c84e0
	overlay_veneer Func_080c8518
	overlay_veneer Func_080c86f8
	overlay_veneer Func_080c8708
	overlay_veneer Func_080c8710
	overlay_veneer Func_080c8718
	overlay_veneer Func_080c8720
	overlay_veneer Func_080c8728
	overlay_veneer Func_080c8730
	overlay_veneer Func_080c8738
	overlay_veneer Audio_PlayCue
