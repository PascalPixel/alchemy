@ Import veneers after the OVERLAY_6B3 overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_6B3 overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Func_080000c0
	overlay_veneer Func_080000d0
	overlay_veneer Func_08000168
	overlay_veneer Func_08000178
	overlay_veneer Func_080001a8
	overlay_veneer Func_080001c8
	overlay_veneer Func_080001d0
	overlay_veneer Func_080001d8
	overlay_veneer Func_080001f0
	overlay_veneer Func_08000290
	overlay_veneer Func_080003d8
	overlay_veneer Func_08038000
	overlay_veneer UiWork_FinalizeFar
	overlay_veneer Func_08038038
	overlay_veneer Func_08038048
	overlay_veneer Func_080380f8
	overlay_veneer Func_08038108
	overlay_veneer Func_080c8010
	overlay_veneer Func_080c8088
	overlay_veneer Func_080c8268
	overlay_veneer Func_080c85c8
	overlay_veneer Func_080c87c0
	overlay_veneer Func_080c87c8
	overlay_veneer Func_081a0008
	overlay_veneer Func_081a0010
	overlay_veneer Func_081a0018
	overlay_veneer Func_081a0020
	overlay_veneer Func_081a0028
	overlay_veneer Audio_PlayCue
