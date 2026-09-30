@ Import veneers after the OVERLAY_64C overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_64C overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer IwramSignedDivide
	overlay_veneer IwramSignedRemainder
	overlay_veneer IwramUnsignedRemainder
	overlay_veneer Func_080000c0
	overlay_veneer Func_080000d0
	overlay_veneer Func_080000d8
	overlay_veneer Func_080000f8
	overlay_veneer Func_08000118
	overlay_veneer Func_08000120
	overlay_veneer Func_08000128
	overlay_veneer Func_08000170
	overlay_veneer Func_08000178
	overlay_veneer Func_080001a8
	overlay_veneer Func_080001b8
	overlay_veneer Func_080001c8
	overlay_veneer Func_080001d0
	overlay_veneer Func_080001d8
	overlay_veneer Func_080001e8
	overlay_veneer Func_08000268
	overlay_veneer Func_08000298
	overlay_veneer Func_08000318
	overlay_veneer Func_080003c8
	overlay_veneer Func_080003d0
	overlay_veneer Func_080003d8
	overlay_veneer Func_08020030
	overlay_veneer Object_SetMode
	overlay_veneer Object_SetCallback
	overlay_veneer Func_080200c0
	overlay_veneer Func_080200c8
	overlay_veneer Func_08020128
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	overlay_veneer Func_08020220
	overlay_veneer Func_08020228
	overlay_veneer Func_08020278
	overlay_veneer Func_08020350
	overlay_veneer Func_080ad310
	overlay_veneer Func_080ad318
	overlay_veneer Func_080c8010
	overlay_veneer Func_080c8018
	overlay_veneer Func_080c8088
	overlay_veneer Func_080c80f8
	overlay_veneer Func_080c8118
	overlay_veneer Func_080c8230
	overlay_veneer Func_080c8238
	overlay_veneer Func_080c8268
	overlay_veneer Func_080c83a8
	overlay_veneer Func_080c83b0
	overlay_veneer Func_080c83b8
	overlay_veneer Func_080c8430
	overlay_veneer Func_080c8438
	overlay_veneer Func_080c84e0
	overlay_veneer Func_080c8638
	overlay_veneer Func_080c86f0
	overlay_veneer Func_080c8778
	overlay_veneer Func_080c8960
	overlay_veneer Func_081a0000
	overlay_veneer Audio_PlayCue
