@ Import veneers after the OVERLAY_6B4 overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_6B4 overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer IwramSignedDivide
	overlay_veneer Func_080000c0
	overlay_veneer Func_080000d0
	overlay_veneer Func_080000d8
	overlay_veneer Func_080000f8
	overlay_veneer Func_08000118
	overlay_veneer Func_08000120
	overlay_veneer Func_08000170
	overlay_veneer Func_08000178
	overlay_veneer Func_080001f0
	overlay_veneer Object_SetMode
	overlay_veneer Object_SetCallback
	overlay_veneer Func_080200c0
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	overlay_veneer Func_08020228
	overlay_veneer Func_08038348
	overlay_veneer Func_080c8010
	overlay_veneer Func_080c8088
	overlay_veneer Func_080c8098
	overlay_veneer Func_080c80d0
	overlay_veneer Func_080c80d8
	overlay_veneer Func_080c80f8
	overlay_veneer Func_080c8118
	overlay_veneer Func_080c8120
	overlay_veneer Func_080c8148
	overlay_veneer Func_080c8168
	overlay_veneer Func_080c8170
	overlay_veneer Func_080c8180
	overlay_veneer Func_080c8198
	overlay_veneer Func_080c81a0
	overlay_veneer Func_080c81d0
	overlay_veneer Func_080c8200
	overlay_veneer Func_080c8210
	overlay_veneer Func_080c8218
	overlay_veneer Func_080c8230
	overlay_veneer Func_080c8238
	overlay_veneer Func_080c8240
	overlay_veneer Func_080c8268
	overlay_veneer Func_080c8378
	overlay_veneer Func_080c8390
	overlay_veneer Func_080c83a8
	overlay_veneer Func_080c83b0
	overlay_veneer Func_080c83b8
	overlay_veneer Func_080c87f8
	overlay_veneer Func_080c8800
	overlay_veneer Func_080c8808
	overlay_veneer Func_080c8810
	overlay_veneer Func_080c8818
	overlay_veneer Func_080c8820
	overlay_veneer Audio_PlayCue
