@ Import veneers after the SUHARA_GATE overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — SUHARA_GATE overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Func_080000c0
	overlay_veneer Func_080003c8
	overlay_veneer Func_080003d0
	overlay_veneer Func_08020178
	overlay_veneer Func_080201e8
	overlay_veneer Func_080c8010
	overlay_veneer Func_080c8018
	overlay_veneer Func_080c8020
	overlay_veneer Func_080c8088
	overlay_veneer Func_080c80c0
	overlay_veneer Func_080c80c8
	overlay_veneer Func_080c80d0
	overlay_veneer Func_080c80d8
	overlay_veneer Func_080c80e0
	overlay_veneer Func_080c80f0
	overlay_veneer Func_080c80f8
	overlay_veneer Func_080c8118
	overlay_veneer Func_080c8128
	overlay_veneer Func_080c8138
	overlay_veneer Func_080c8148
	overlay_veneer Func_080c8158
	overlay_veneer Func_080c8180
	overlay_veneer Func_080c8198
	overlay_veneer Func_080c81d0
	overlay_veneer Func_080c8210
	overlay_veneer Func_080c8228
	overlay_veneer Func_080c8238
	overlay_veneer Func_080c8240
	overlay_veneer Func_080c8278
	overlay_veneer Func_080c8280
	overlay_veneer Func_080c8288
	overlay_veneer Func_080c8290
	overlay_veneer Func_080c8298
	overlay_veneer Func_080c83a8
	overlay_veneer Func_080c83b8
	overlay_veneer Func_080c84e0
	overlay_veneer Func_080c85e8
	overlay_veneer Func_080c85f8
	overlay_veneer Func_080c8680
	overlay_veneer Func_080c87b0
	overlay_veneer Func_080c87b8
	overlay_veneer Audio_PlayCue
