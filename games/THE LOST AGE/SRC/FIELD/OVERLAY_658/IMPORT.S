@ Import veneers after the OVERLAY_658 overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_658 overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer IwramSignedDivide
	overlay_veneer Func_080003c8
	overlay_veneer Func_080003d0
	overlay_veneer Func_08020170
	overlay_veneer Func_080201e8
	overlay_veneer Func_080201f0
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	overlay_veneer Func_08038040
	overlay_veneer Func_08038120
	overlay_veneer Func_080ad2f0
	overlay_veneer Func_080c8010
	overlay_veneer Func_080c8018
	overlay_veneer Func_080c8020
	overlay_veneer Func_080c8060
	overlay_veneer Func_080c8070
	overlay_veneer Func_080c8088
	overlay_veneer Func_080c8098
	overlay_veneer Func_080c80c0
	overlay_veneer Func_080c80d0
	overlay_veneer Func_080c80d8
	overlay_veneer Func_080c80e0
	overlay_veneer Func_080c80e8
	overlay_veneer Func_080c80f0
	overlay_veneer Func_080c80f8
	overlay_veneer Func_080c8100
	overlay_veneer Func_080c8118
	overlay_veneer Func_080c8128
	overlay_veneer Func_080c8138
	overlay_veneer Func_080c8148
	overlay_veneer Func_080c8158
	overlay_veneer Func_080c8160
	overlay_veneer Func_080c8170
	overlay_veneer Func_080c8180
	overlay_veneer Func_080c8188
	overlay_veneer Func_080c8198
	overlay_veneer Func_080c81a0
	overlay_veneer Func_080c81d0
	overlay_veneer Func_080c81d8
	overlay_veneer Func_080c8200
	overlay_veneer Func_080c8210
	overlay_veneer Func_080c8218
	overlay_veneer Func_080c8228
	overlay_veneer Func_080c8238
	overlay_veneer Func_080c8240
	overlay_veneer Func_080c8278
	overlay_veneer Func_080c83a8
	overlay_veneer Func_080c83b0
	overlay_veneer Func_080c83b8
	overlay_veneer Func_080c83e0
	overlay_veneer Func_080c84e0
	overlay_veneer Func_080c8570
	overlay_veneer Func_080c85e8
	overlay_veneer Func_080c85f0
	overlay_veneer Func_080c85f8
	overlay_veneer Func_080c8778
	overlay_veneer Func_08108008
	overlay_veneer Audio_PlayCue
