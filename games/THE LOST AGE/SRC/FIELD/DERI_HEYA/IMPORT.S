@ Import veneers after the DERI_HEYA overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — DERI_HEYA overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Func_080003c0
	overlay_veneer Func_080003c8
	overlay_veneer Func_080003d0
	overlay_veneer Func_080003d8
	overlay_veneer Object_ResetMotion
	overlay_veneer Func_080201a0
	overlay_veneer Func_080201e0
	overlay_veneer Func_08020218
	overlay_veneer Func_08020300
	overlay_veneer Func_08038040
	overlay_veneer Func_08038120
	overlay_veneer Func_08038140
	overlay_veneer Func_080ad008
	overlay_veneer Func_080ad010
	overlay_veneer Func_080ad018
	overlay_veneer Func_080ad028
	overlay_veneer Func_080ad278
	overlay_veneer Func_080ad288
	overlay_veneer Func_080ad2a0
	overlay_veneer Func_080ad2f0
	overlay_veneer Func_080c8010
	overlay_veneer Func_080c8018
	overlay_veneer Func_080c8020
	overlay_veneer Func_080c8070
	overlay_veneer Func_080c8088
	overlay_veneer Func_080c80d8
	overlay_veneer Func_080c80f8
	overlay_veneer Func_080c8118
	overlay_veneer Func_080c8138
	overlay_veneer Func_080c8158
	overlay_veneer Func_080c8180
	overlay_veneer Func_080c8188
	overlay_veneer Func_080c81a0
	overlay_veneer Func_080c81d0
	overlay_veneer Func_080c8200
	overlay_veneer Func_080c8210
	overlay_veneer Func_080c82e0
	overlay_veneer Func_080c83d8
	overlay_veneer Func_080c8480
	overlay_veneer Func_080c84e0
	overlay_veneer Func_080c8558
	overlay_veneer Func_080c86f8
	overlay_veneer Func_080c8708
	overlay_veneer Func_080c8778
	overlay_veneer Func_08108008
	overlay_veneer Func_08108010
	overlay_veneer Func_08108018
	overlay_veneer Func_08108068
	overlay_veneer Audio_PlayCue
