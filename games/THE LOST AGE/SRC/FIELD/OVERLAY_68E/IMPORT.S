@ Import veneers after the OVERLAY_68E overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_68E overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer IwramSignedDivide
	overlay_veneer IwramSignedRemainder
	overlay_veneer Func_080000c0
	overlay_veneer Func_080000d0
	overlay_veneer Func_080000f8
	overlay_veneer Func_08000118
	overlay_veneer Func_08000120
	overlay_veneer Func_080003c8
	overlay_veneer Func_080003d0
	overlay_veneer Object_SetMode
	overlay_veneer ObjectDispatch_ApplyValueToChildrenFar
	overlay_veneer Func_08020178
	overlay_veneer Func_08038010
	overlay_veneer UiWork_FinalizeFar
	overlay_veneer Func_08038040
	overlay_veneer Func_08038080
	overlay_veneer Func_080380b0
	overlay_veneer Func_08038120
	overlay_veneer Func_08038140
	overlay_veneer Func_08038340
	overlay_veneer Func_08038380
	overlay_veneer Func_080ad040
	overlay_veneer Func_080ad1d8
	overlay_veneer Func_080ad208
	overlay_veneer Func_080ad2a8
	overlay_veneer Func_080ad2d0
	overlay_veneer Func_080ad2f8
	overlay_veneer Func_080c8010
	overlay_veneer Func_080c8018
	overlay_veneer Func_080c8020
	overlay_veneer Func_080c8060
	overlay_veneer Func_080c8070
	overlay_veneer Func_080c8088
	overlay_veneer Func_080c80d8
	overlay_veneer Func_080c80f8
	overlay_veneer Func_080c8110
	overlay_veneer Func_080c8118
	overlay_veneer Func_080c8180
	overlay_veneer Func_080c8188
	overlay_veneer Func_080c81a0
	overlay_veneer Func_080c81d0
	overlay_veneer Func_080c83a8
	overlay_veneer Func_080c83e0
	overlay_veneer Func_080c84e0
	overlay_veneer Func_080c8928
	overlay_veneer Audio_PlayCue
